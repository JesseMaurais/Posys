#include "dev.hpp"
#include "ps.hpp"

namespace env::dev
{
	shell::line dev::sym(view path)
	{
		fmt::ipstream sub
		#ifdef _WIN32
			{ "dumpbin", "-nologo", "-imports", path };
		#else
			{ "objdump", "-t", path };
		#endif
		return get(sub);
	}

	shell::line dev::dyn(view path)
	{
		fmt::ipstream sub
		#ifdef _WIN32
			{ "dumpbin", "-nologo", "-exports", path };
		#else
			{ "objdump", "-T", path };
		#endif
		return get(sub);
	}
};
