#ifndef char_hpp
#define char_hpp

#include <cuchar>
#include <cwchar>
#include <cstring>

namespace fmt
{
	struct mb : std::mbstate_t
	{
		mb()
		{
			std::memset(this, 0, sizeof(mb));
		}

		operator bool() const
		{
			return 0 != std::mbsinit(this);
		}

		std::ptrdiff_t len(char const* s, std::size_t n = MB_CUR_MAX)
		{
			return std::mbrlen(s, n, this);
		}

		std::ptrdiff_t to(wchar_t* w, char const* s, std::size_t n = MB_CUR_MAX)
		{
			return std::mbrtowc(w, s, n, this);
		}

		std::ptrdiff_t to(char* s, wchar_t w)
		{
			return std::wcrtomb(s, w, this);
		}

		std::ptrdiff_t to(wchar_t* w, char const** s, std::size_t n)
		{
			return std::mbsrtowcs(w, s, n, this);
		}

		std::ptrdiff_t to(char* s, wchar_t const** w, std::size_t n)
		{
			return std::wcsrtombs(s, w, n, this);
		}
	};
}

#endif // file