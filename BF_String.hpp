#pragma once
#include <string>
#include <Windows.h>
#include <cctype>		// std::tolower
#include <algorithm>	// std::search
#include <iterator>
#include <vector>

// --------------------------------------
// 以std::string类作为字符串操作
// 
// 取文本左边		GetStrLeft
// 取文本右边		GetStrRight
// 取文本长度		GetStrLength
// 取文本中间		GetStrMiddle
// 取字节数组		GetStrBytes
// 寻找文本			FindString
// 到小写			GetStrLower
// 到大写			GetStrHigher
// 到文本			ToString
// 删首空			DeleteFrontSpaces
// 删尾空			DeleteLastSpaces
// 分割文本			SplitStr
// 文本替换			ReplaceSubString
// 文本比较			CompareStr
// --------------------------------------

#define 取文本左边	GetStrLeft
#define 取文本右边	GetStrRight
#define 取文本长度	GetStrLength
#define 取文本中间	GetStrMiddle
#define 取字节数组	GetStrBytes
#define 寻找文本		GetStrBytes
#define 到小写		GetStrLower
#define 到大写		GetStrHigher
#define 到文本		ToString
#define 删首空		DeleteFrontSpaces
#define 删尾空		DeleteLastSpaces
#define 分割文本		SplitStr
#define 文本替换		ReplaceSubString
#define 文本比较		CompareStr

class BF_String
{
public:
	// 取文本左边
	// 返回一个文本，其中包含指定文本中从左边算起指定数量的字符。本命令为初级命令。
	// 参数<1>的名称为“欲取其部分的文本”，类型为“文本型（text）”。
	// 参数<2>的名称为“欲取出字符的数目”，类型为“整数型（int）”。
	std::string GetStrLeft(std::string source, int length);
	// 取文本右边
	// 返回一个文本，其中包含指定文本中从右边算起指定数量的字符。本命令为初级命令。
	// 参数<1>的名称为“欲取其部分的文本”，类型为“文本型（text）”。
	// 参数<2>的名称为“欲取出字符的数目”，类型为“整数型（int）”。
	std::string GetStrRight(std::string source, int length);
	// 取文本长度
	// 参数<1>的名称为“文本数据”，类型为“文本型（text）”。参数值指定欲检查其长度的文本数据。
	int GetStrLength(std::string source);
	// 取文本中间
	// 返回一个文本，其中包含指定文本中从指定位置算起指定数量的字符。本命令为初级命令。
	// 参数<1>的名称为“欲取其部分的文本”，类型为“文本型（text）”。
	// 参数<2>的名称为“起始取出位置”，类型为“整数型（int）”。1为首位置，2为第2个位置，如此类推。
	// 参数<3>的名称为“欲取出字符的数目”，类型为“整数型（int）”。
	std::string GetStrMiddle(std::string source, int start, int length);
	// 取文本字节数组
	// 返回char[]型字节数组
	const char* GetStrBytes(std::string& source);
	// 寻找文本
	// 返回一个整数值，指定一文本在另一文本中最先出现的位置，位置值从  开始。如果未找到，返回 - 1。本命令为初级命令。
	// 参数<1>的名称为“被搜寻的文本”，类型为“文本型（text）”。
	// 参数<2>的名称为“欲寻找的文本”，类型为“文本型（text）”。
	// 参数<3>的名称为“是否不区分大小写”，类型为“逻辑型（bool）”，初始值为“假”。为真不区分大小写，为假区分。
	std::size_t FindString(const std::string& source, const std::string& subStr, bool judge);	// judge为true忽略大小写 false不忽略  // 失败返回4294967295
	// 到小写
	// 参数<1>的名称为“欲变换的文本”，类型为“文本型（text）”。
	// 参数<1>的名称为“欲变换的文本”，类型为“文本型（text）”。
	std::string GetStrLower(std::string source);
	// 到大写
	// 将文本中的小写英文字母变换为大写，返回变换后的结果文本。本命令为初级命令。
	// 参数<1>的名称为“欲变换的文本”，类型为“文本型（text）”。
	std::string GetStrHigher(std::string source);
	// 到文本 支持 整数、长整数、短整数、浮点数、逻辑型、字节型、字节集、双精度浮点、无符号整数、短整数、长整数（日期） -- 日期型还没有做，后续完善
	// 返回一个文本，代表指定数值、逻辑值或日期时间被转换后的结果。如果为文本数据，将被直接返回。本命令为初级命令。
	// 参数<1>的名称为“待转换的数据”，类型为“通用型（all）”，提供参数数据时可以同时提供数组或非数组数据。参数值只能为数值、逻辑值、字节集、日期时间或数值型数组。
	std::string ToString(int source);
	std::string ToString(long long source);
	std::string ToString(short source);
	std::string ToString(float source);
	std::string ToString(bool source);
	std::string ToString(char source);
	std::string ToString(char& source);
	std::string ToString(long double source);
	std::string ToString(unsigned int source);
	std::string ToString(unsigned long long source);
	// 删首空
	// 返回一个文本，其中包含被删除了首部全角或半角空格的指定文本。本命令为初级命令。
	// 参数<1>的名称为“欲删除空格的文本”，类型为“文本型（text）”。
	std::string DeleteFrontSpaces(std::string source);
	// 删尾空
	// 返回一个文本，其中包含被删除了尾部全角或半角空格的指定文本。本命令为初级命令。
	// 参数<1>的名称为“欲删除空格的文本”，类型为“文本型（text）”。
	std::string DeleteLastSpaces(std::string str);
	// 分割文本
	//将指定文本进行分割，返回分割后的一维文本数组。本命令为初级命令。
	// 参数<1>的名称为“待分割文本”，类型为“文本型（text）”。如果参数值是一个长度为零的文本，则返回一个空数组，即没有任何成员的数组。
	// 参数<2>的名称为“用作分割的文本”，类型为“文本型（text）”，可以被省略。参数值用于标识子文本边界。如果被省略，则默认使用半角逗号字符作为分隔符。如果是一个长度为零的文本，则返回的数组仅包含一个成员，即完整的“待分割文本”。
	// 参数<3>的名称为“要返回的子文本数目”，类型为“整数型（int）”，可以被省略。如果被省略，则默认返回所有的子文本。
	std::vector<std::string> SplitStr(const std::string& source, const std::string& splitStr);
	// 文本替换
	// 将指定文本的某一部分用其它的文本替换。本命令为初级命令。
	// 参数<1>的名称为“欲被替换的文本”，类型为“文本型（text）”。
	// 参数<2>的名称为“起始替换位置”，类型为“整数型（int）”。替换的起始位置，1为首位置，2为第2个位置，如此类推。
	// 参数<3>的名称为“替换长度”，类型为“整数型（int）”。
	// 参数<4>的名称为“用作替换的文本”，类型为“文本型（text）”，可以被省略。如果本参数被省略，则删除文本中的指定部分。
	std::string ReplaceSubString(std::string source, int start, int length, std::string subString);
	// 文本比较
	//如果返回值小于0，表示文本一小于文本二；如果等于0，表示文本一等于文本二；如果大于0，表示文本一大于文本二。如果比较时区分大小写，也可以使用比较运算符进行同样的操作。本命令为初级命令。
	// 参数<1>的名称为“待比较文本一”，类型为“文本型（text）”。
	// 参数<2>的名称为“待比较文本二”，类型为“文本型（text）”。
	// 参数<3>的名称为“是否区分大小写”，类型为“逻辑型（bool）”，初始值为“真”。为真区分大小写，为假不区分。
	bool CompareStr(std::string string1, std::string string2, bool judge);

public:
	BF_String();
	~BF_String();
};

BF_String::BF_String() {}
BF_String::~BF_String() {}

std::string BF_String::GetStrLeft(std::string source, int length)
{
	std::string retStr = source.substr(0, length);
	return retStr;
}

std::string BF_String::GetStrRight(std::string source, int length)
{
	int sourceLength = source.length();
	std::string retStr = source.substr(sourceLength - length, length);
	return retStr;
}

int BF_String::GetStrLength(std::string source)
{
	int length = source.length();
	return length;
}

std::string BF_String::GetStrMiddle(std::string source, int start, int length)
{
	std::string ret = source.substr(start, length);
	return ret;
}

const char* BF_String::GetStrBytes(std::string& source)
{
	const char* ret = source.data();
	return ret;
}

std::size_t BF_String::FindString(const std::string& source, const std::string& subStr, bool judge)
{
	if (judge == true)
	{
		std::string sourceLower, subStrLower;
		// 两个串都转为小写
		std::transform(source.begin(), source.end(), back_inserter(sourceLower), [](char c) { return std::tolower(c); });
		std::transform(subStr.begin(), subStr.end(), back_inserter(subStrLower), [](char c) { return std::tolower(c); });

		// 搜索字串 不区分大小写
		auto it = std::search(sourceLower.begin(), sourceLower.end(), subStrLower.begin(), subStrLower.end());

		if (it != sourceLower.end()) {
			return std::distance(sourceLower.begin(), it);
		}
		else {
			return std::string::npos;
		}
	}
	else
	{
		// 搜索字串 区分大小写
		auto it = std::search(source.begin(), source.end(), subStr.begin(), subStr.end());

		if (it != source.end()) {
			return std::distance(source.begin(), it);
		}
		else {
			return std::string::npos;
		}
	}
}

std::string BF_String::GetStrLower(std::string source)
{
	std::string sourceLower;
	// 转为小写
	std::transform(source.begin(), source.end(), back_inserter(sourceLower), [](char c) { return std::tolower(c); });
	return sourceLower;
}

std::string BF_String::GetStrHigher(std::string source)
{
	std::string sourceHigher;
	// 转为小写
	std::transform(source.begin(), source.end(), back_inserter(sourceHigher), [](char c) { return std::toupper(c); });
	return sourceHigher;
}

std::string BF_String::ToString(int source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(long long source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(short source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(float source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(bool source)
{
	std::string ret;
	if (source == true)
	{
		ret = "true";
	}
	else
	{
		ret = "false";
	}
	return ret;
}

std::string BF_String::ToString(char source)
{
	char arr[2] = { source, 0 };
	std::string ret(arr);
	return ret;
}

std::string BF_String::ToString(char& source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(long double source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(unsigned int source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::ToString(unsigned long long source)
{
	std::string ret = std::to_string(source);
	return ret;
}

std::string BF_String::DeleteFrontSpaces(std::string source)
{
	// 查找第一个非空格字符的位置
	size_t pos = source.find_first_not_of(' ');

	if (pos == std::string::npos) {
		// 如果没有非空格字符，则返回空字符串
		return "";
	}
	else {
		// 删除前面的空格
		source.erase(0, pos);
		return source;
	}
}

std::string BF_String::DeleteLastSpaces(std::string str) {
	// 找到最后一个非空格字符的位置
	std::size_t pos = str.find_last_not_of(' ');
	if (pos != std::string::npos) {
		// 删除这个位置之后的所有字符
		str.erase(pos + 1);
	}
	else {
		// 如果字符串全是空格，则删除整个字符串
		str.clear();
	}
	return str;
}

std::vector<std::string> BF_String::SplitStr(const std::string& source, const std::string& splitStr) {
	std::vector<std::string> result;
	size_t pos = 0, last_pos = 0;
	while ((pos = source.find(splitStr, last_pos)) != std::string::npos) {
		if (pos != last_pos) {
			result.push_back(source.substr(last_pos, pos - last_pos));
		}
		last_pos = pos + splitStr.length();
	}
	if (last_pos < source.length()) {
		result.push_back(source.substr(last_pos));
	}
	return result;
}

std::string BF_String::ReplaceSubString(std::string source, int start, int length, std::string subString)
{
	// 判断替换长度是否小于等于替换文本的长度
	if (length <= subString.length()) {
		// 使用替换文本的前 b 个字符作为替换文本
		subString = subString.substr(0, length);
	}
	else {
		// 使用空格填充多出的长度
		subString.append(length - subString.length(), ' ');
	}

	// 替换文本
	source.replace(start, length, subString);

	return source;
}

bool BF_String::CompareStr(std::string string1, std::string string2, bool judge) {
	if (judge) {
		// 忽略大小写
		std::transform(string1.begin(), string1.end(), string1.begin(), ::tolower);
		std::transform(string2.begin(), string2.end(), string2.begin(), ::tolower);
	}
	// 比较串是否相等
	return string1.compare(string2) == 0;
}
