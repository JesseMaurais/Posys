#ifndef pipe_hpp
#define pipe_hpp "Communication Pipes"

#include "fmt.hpp"
#include "ptr.hpp"
#include "file.hpp"
#include "mode.hpp"

namespace env::file
{
	struct descriptor : fwd::unique, stream
	{
		ssize_t read(void *buf, size_t sz) const override;
		ssize_t write(const void *buf, size_t sz) const override;
		bool open(fmt::string::view path, mode = rw, permit = owner(rw));
		bool close();

		explicit descriptor(fmt::string::view path, mode am = rw, permit pm = owner(rw))
		{
			(void) open(path, am, pm);
		}

		explicit descriptor(int fd = invalid)
		{
			(void) set(fd);
		}

		~descriptor()
		{
			if (not fail(fd))
			{
				(void) close();
			}
		}

		int set(int value = invalid)
		{
			int const tmp = fd;
			fd = value;
			return tmp;
		}

		int get() const
		{
			return fd;
		}

	protected:

		int fd;
	};

	struct pipe : fwd::unique, stream
	{
		explicit pipe();

		explicit pipe(int fd[2])
		{
			set(fd);
		}

		void set(int fd[2] = nullptr)
		{
			for (auto n : { 0, 1 })
			{
				fds[n].set(fd ? fd[n] : invalid);
			}
		}

		void get(int fd[2]) const
		{
			for (auto n : { 0, 1 })
			{
				fd[n] = fds[n].get();
			}
		}

		ssize_t read(void* buf, size_t sz) const override
		{
			return fds[0].read(buf, sz);
		}

		ssize_t write(const void* buf, size_t sz) const override
		{
			return fds[1].write(buf, sz);
		}

		const descriptor& operator[](size_t n) const
		{
			return fds[n];
		}

		descriptor& operator[](size_t n)
		{
			return fds[n];
		}

	protected:

		descriptor fds[2];
	};

	struct process : fwd::unique, stream
	{
		bool start(fmt::string::view::init args);
		bool start(fmt::string::view::span args);
		bool start(size_t argc, char const **argv);
		bool quit();
		int wait();

		explicit process(fmt::string::view::init args)
		{
			(void) start(args);
		}

		explicit process(fmt::string::view::span args)
		{
			(void) start(args);
		}

		explicit process(size_t argc, char const** argv)
		{
			(void) start(argc, argv);
		}

		explicit process(int pid = invalid, int fd[3] = nullptr)
		{
			(void) set(pid, fd);
		}

		ssize_t error(void *buf, size_t sz) const
		{
			return fd[2].read(buf, sz);
		}

		ssize_t read(void *buf, size_t sz) const override
		{
			return fd[1].read(buf, sz);
		}

		ssize_t write(const void *buf, size_t sz) const override
		{
			return fd[0].write(buf, sz);
		}

		void close(int n = invalid)
		{
			if (fail(n)) 
			{
				for (auto m : { 0, 1, 2 })
				{
					close(m);
				}
			}
			else fd[n].close();
		}

		int set(int p = invalid, int f[3] = nullptr)
		{
			for (auto n : { 0, 1, 2 })
			{
				if (nullptr != f)
				{
					f[n] = fd[n].set(f[n]);
				}
				else
				{
					fd[n].set();
				}
			}
			auto tmp = p;
			pid = p;
			return tmp;
		}

		int get(int f[3] = nullptr) const
		{
			if (nullptr != f)
			{
				for (auto n : { 0, 1, 2 })
				{
					f[n] = fd[n].get();
				}
			}
			return pid;
		}

	protected:

		int pid = invalid;
		descriptor fd[3];
	};
}

#endif // file
