#pragma once

#include <algorithm>
#include <string>
#include <functional> 
#include <cctype>

/// Try to find in the Haystack the Needle - ignore case
inline bool StringSearchIgnoreCase(const std::string & strHaystack, const std::string & strNeedle)
{
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
	);
	return (it != strHaystack.end());
}

inline bool CharArrayFromFormatV(char* out, int outsize, const char* format, va_list args)
{
	int writtenCount = _vsnprintf_s(out, outsize, outsize, format, args);
	if (writtenCount > 0 && writtenCount < outsize)
	{
		out[writtenCount] = '\0';
		return true;
	}
	else
	{
		out[outsize - 1] = '\0';
		return false;
	}
}

inline bool CharArrayFromFormatV(wchar_t* out, int outsize, const wchar_t* format, va_list args)
{
    int writtenCount = _vsnwprintf_s(out, outsize, outsize, format, args);
    if (writtenCount > 0 && writtenCount < outsize)
    {
        out[writtenCount] = '\0';
        return true;
    }
    else
    {
        out[outsize - 1] = '\0';
        return false;
    }
}

template<size_t Count>
inline void CharArrayFromFormat(char(&out)[Count], const char* format, ...)
{
	va_list args;
	va_start(args, format);
	CharArrayFromFormatV(out, Count, format, args);
	va_end(args);
}

template<size_t Count>
inline void CharArrayFromFormat(wchar_t(&out)[Count], const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    CharArrayFromFormatV(out, Count, format, args);
    va_end(args);
}

inline std::string StringFromFormatV(const char* format, va_list args)
{
	int required = _vscprintf(format, args);
	std::unique_ptr<char[]> buf(new char[required + 1]);
	CharArrayFromFormatV(buf.get(), required + 1, format, args);

	std::string temp = buf.get();
	return std::move(temp);
}

inline std::wstring StringFromFormatV(const wchar_t* format, va_list args)
{
    int required = _vscwprintf(format, args);
    std::unique_ptr<wchar_t[]> buf(new wchar_t[required + 1]);
    CharArrayFromFormatV(buf.get(), required + 1, format, args);

    std::wstring temp = buf.get();
    return std::move(temp);
}

inline std::string StringFromFormat(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::string res = StringFromFormatV(format, args);
	va_end(args);
	return std::move(res);
}

inline std::wstring StringFromFormat(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring res = StringFromFormatV(format, args);
    va_end(args);
    return std::move(res);
}

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}
