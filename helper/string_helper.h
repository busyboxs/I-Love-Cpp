#pragma once

#include <string>
#include <vector>

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
namespace string_helper
{
	inline std::string ws2s(const std::wstring& wide, UINT CodePage = CP_ACP)
	{
		int wide_length = static_cast<int>(wide.length());
		if (wide_length == 0)
			return std::string();

		int charcount = WideCharToMultiByte(CodePage, 0, wide.data(), wide_length, NULL, 0, NULL, NULL);
		if (charcount == 0)
			return std::string();

		std::string mb;
		mb.resize(charcount);
		WideCharToMultiByte(CodePage, 0, wide.data(), wide_length, &mb[0], charcount, NULL, NULL);

		return mb;
	}

	inline std::wstring s2ws(const std::string& mb, UINT CodePage = CP_ACP)
	{
		if (mb.empty())
			return std::wstring();

		int mb_length = static_cast<int>(mb.length());

		int charcount = MultiByteToWideChar(CodePage, 0, mb.data(), mb_length, NULL, 0);
		if (charcount == 0)
			return std::wstring();

		std::wstring wide;
		wide.resize(charcount);
		MultiByteToWideChar(CodePage, 0, mb.data(), mb_length, &wide[0], charcount);

		return wide;
	}
}

#endif // WIN32


namespace string_helper
{
	/**
	 * @brief 移除字符串中的空格，并返回修改后的字符串
	 * @param str 要被移除空格，换行符等符号（‘ ’，‘\f’, '\n', '\r', '\t', '\v'）的字符串
	 * @return 修改后的字符串
	 */
	inline std::string RemoveSpace(std::string str)
	{
		str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
		return str;
	}

	/**
	 * @brief 移除字符串中的空格，并返回修改后的字符串
	 * @param str 要被移除空格，换行符等符号（‘ ’，‘\f’, '\n', '\r', '\t', '\v'）的字符串
	 * @return 修改后的字符串
	 */
	inline std::wstring RemoveSpace(std::wstring str)
	{
		str.erase(std::remove_if(str.begin(), str.end(), ::iswspace), str.end());
		return str;
	}

	/**
	 * @brief 替换字符串中旧字符串为新字符串
	 * @param str 待替换的字符串
	 * @param old_str 要被替换的旧字符串
	 * @param new_str 要替换的新字符串
	 * @return 修改后的字符串
	 */
	inline std::string ReplaceStr(std::string str, const std::string& old_str, const std::string& new_str)
	{
		size_t pos = 0;
		while ((pos = str.find(old_str, pos)) != std::string::npos)
		{
			str.replace(pos, old_str.length(), new_str);
			pos += new_str.length();
		}
		return str;
	}

	/**
	 * @brief 将字符串按分隔符进行分割，分割结果保存到 vector 中
	 * @param [in] s 输入字符串
	 * @param [out] tokens 分割结果列表
	 * @param delimiters 分割符
	*/
	inline void Split(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = ";")
	{
		tokens.clear();
		auto lastPos = s.find_first_not_of(delimiters, 0);
		auto pos = s.find_first_of(delimiters, lastPos);
		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			tokens.push_back(s.substr(lastPos, pos - lastPos));
			lastPos = s.find_first_not_of(delimiters, pos);
			pos = s.find_first_of(delimiters, lastPos);
		}
	}

	/**
	 * @brief 将字符串按分隔符进行分割，可以设置是否跳过空字符串
	 * @param str 待分割的字符串
	 * @param delimiter 分割符字符串
	 * @param skip_empty 是否跳过空字符串，若跳过，则返回结果中不包括空
	 * @return 分割结果列表
	*/
	inline std::vector<std::string> Split(const std::string& str, const std::string& delimiter = ";", bool skip_empty = true)
	{
		if (str.empty())
		{
			return {};
		}

		std::vector<std::string> strings;
		auto delim_len = delimiter.length();

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			auto sub = str.substr(prev, pos - prev);
			if (!(skip_empty && sub.empty()))
			{
				strings.push_back(sub);
			}
			prev = pos + delim_len;
		}

		// To get the last substring (or only, if delimiter is not found)
		auto sub = str.substr(prev);
		if (!(skip_empty && sub.empty()))
		{
			strings.push_back(sub);
		}

		return strings;
	}

	/**
	 * @brief 用拼接符拼接 vector 中字符串
	 * @param inputs 待拼接的字符串列表
	 * @param delimiter 拼接符
	 * @return 返回拼接后的字符串
	*/
	inline std::string Join(const std::vector<std::string>& inputs, const std::string& delimiter = ";")
	{
		if (inputs.empty())
			return std::string();

		std::string join_str;
		size_t i = 0;
		auto len = inputs.size();

		for (i = 0; i < len - 1; ++i)
		{
			join_str += inputs[i] + delimiter;
		}

		if (i < len)
		{
			join_str += inputs[i];
		}

		return join_str;
	}
}
