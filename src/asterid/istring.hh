#pragma once

#include <string>
#include <string_view>

namespace asterid {
	
	struct insensitive_char_traits : public std::char_traits<char> {
		
		static bool eq(char c1, char c2) {
			return std::toupper(c1) == std::toupper(c2);
		}
		static bool lt(char c1, char c2) {
			return std::toupper(c1) <  std::toupper(c2);
		}
		static int compare(const char* s1, const char* s2, size_t n) {
			while ( n-- != 0 ) {
				if ( std::toupper(*s1) < std::toupper(*s2) ) return -1;
				if ( std::toupper(*s1) > std::toupper(*s2) ) return 1;
				++s1; ++s2;
			}
			return 0;
		}
		static char const * find(const char* s, int n, char a) {
			auto const ua (std::toupper(a));
			while ( n-- != 0 ) 
			{
				if (std::toupper(*s) == ua)
					return s;
				s++;
			}
			return nullptr;
		}
	};

	typedef std::basic_string<char, insensitive_char_traits> istring;
	typedef std::basic_string_view<istring> istring_view;
	
	template <typename T>
	struct istring_convertible {
		
		istring_convertible() = delete;
		inline istring_convertible(istring && str) : istr(std::forward<istring>(str)) {}
		inline istring_convertible(T const & str) : istr(str.begin(), str.end()) {}
		
		inline istring && get() { return std::move(istr); }
		
	private:
		istring istr;
	};
}

namespace std {
	
	template <> struct hash<asterid::istring> {
		size_t operator() (asterid::istring const & str) const {
			size_t h = 14695981039346656037UL;
			for (char c : str) {
				h ^= std::toupper(c);
				h *= 1099511628211UL;
			}
			return h;
		}
	};
	
}
