#pragma once

#include <string>

namespace Utils {
	static void ConvertStrToWstr(std::wstring& ws, std::string& orig)
	{
		wchar_t* buf = new wchar_t[orig.size() + 1];
		size_t num_chars = mbstowcs(buf, orig.c_str(), orig.size() + 1);
		ws = buf;
		delete[] buf;
	}
}