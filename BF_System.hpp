#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

// ---------------------------------------------------------------
// 使用控制台创建或释放时，必须定义为全局对象
//
// 创建控制台	AllocConsole
// 释放控制台	FreeConsole
// 关闭控制台	CloseConsole
// 运行			RunExe
// 取剪辑板文本	Getclipboard
// 置剪辑板文本	Setclipboard
// 清除剪辑板	ClearClipboard
// 鸣叫			Beep
// 取启动时间	clock
// 延时			Sleep
// 取当前目录	GetNowDirectory
// 取执行文件名	GetNowProcessName
// ---------------------------------------------------------------

#define 创建控制台	AllocConsole
#define 释放控制台	FreeConsole
#define 关闭控制台	CloseConsole
#define 运行			RunExe
#define 取剪辑板文本	Getclipboard
#define 置剪辑板文本	Setclipboard
#define 清除剪辑板	ClearClipboard
#define 鸣叫			Beep
#define 取启动时间	clock
#define 延时			Sleep
#define 取当前目录	GetNowDirectory
#define 取执行文件名	GetNowProcessName

class BF_System
{
public:
	BOOL AllocConsole();	// 创建控制台(MFC中)
	BOOL FreeConsole();		// 释放控制台，停止输出
	BOOL CloseConsole();	// 释放控制台 + 关闭控制台窗口

	// 运行
	// 本命令运行指定的可执行文件或者外部命令。如果成功，返回真，否则返回假。本命令为初级命令。
	// 参数<1>的名称为“欲运行的命令行”，类型为“文本型（text）”。
	static bool RunExe(std::string directory/*目录*/);

	// 取剪辑板文本
	// 返回存放于当前 Windows 系统剪辑板中的文本。如果当前剪辑板中无文本数据，将返回空文本。本命令为初级命令。
	static std::string Getclipboard(); 

	// 置剪辑板文本
	// 将指定文本存放到当前 Windows 系统剪辑板中去，剪辑板中的原有内容被覆盖。成功返回真，失败返回假。本命令为初级命令。
	// 参数<1>的名称为“准备置入剪辑板的文本”，类型为“文本型（text）”。
	static bool Setclipboard(const std::string& text); 

	// 清除剪辑板
	// 清除当前 Windows 系统剪辑板中的所有数据。本命令为初级命令。
	static bool ClearClipboard();		

	// 鸣叫--可直接调用标准库 Beep(int, int);
	// 通过计算机媒体设备或者喇叭发出一个声音。本命令为初级命令。
	static BOOL Beep(DWORD frequent, DWORD time);		

	// 取启动时间	-- 调用标准库clock(); // 计算程序某段代码的执行时间
	// clock();		
	
	// 延时			-- 调用标准库Sleep();
	// Sleep();		
	
	// 取当前目录
	// 取当前被执行的C程序文件所处的目录。本命令为初级命令。
	static std::string GetNowDirectory();

	// 取执行文件名
	// 取当前被执行的易程序文件的名称。本命令为初级命令。
	static std::string GetNowProcessName();		

public:
	BF_System();
	~BF_System();
private:
	bool console_exist;
	FILE* fp;
	HWND console_Hwnd;
	DWORD console_dwProcessId;
	HANDLE console_hProcess;
};

BF_System::BF_System()
{
	console_exist = false; // 保证第一次可以申请成功
	fp = NULL;
	console_Hwnd = NULL;
	console_dwProcessId = NULL;
	console_hProcess = NULL;
}

BF_System::~BF_System()
{
	if (fp != nullptr)
	{
		FreeConsole();
	}
}

BOOL BF_System::AllocConsole()
{
	// 保存原始的标准输出流  -- 未打开流，执行后original_stdout仍为空
	// freopen_s(&original_stdout, "CONOUT$", "w", stdout);
	if (console_exist == true)
	{
		return false;
	}
	BOOL ret = ::AllocConsole();
	if (ret == false)
	{
		return false;
	}
	console_exist = true;
	// 获取控制台窗口句柄
	console_Hwnd = GetConsoleWindow();
	//// 获取控制台窗口的进程ID
	//GetWindowThreadProcessId(console_Hwnd, &console_dwProcessId);
	//// 获取控制台窗口的进程句柄
	//console_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, console_dwProcessId);
	// 若以上操作失败
	if (console_Hwnd == NULL)
	{
		FreeConsole();
		return FALSE;
	}

	// 重定向
	freopen_s(&fp, "CONOUT$", "w", stdout);
	return true;
}

BOOL BF_System::FreeConsole()
{
	// 关闭控制台窗口
	BOOL ret = ::FreeConsole();
	if (ret == true)
	{
		console_exist = false;
	}
	// 恢复标准输出流 -- 未打开文件流无需恢复
	//freopen_s(&fp, "CONOUT$", "w", original_stdout);
	return ret;
}

BOOL BF_System::CloseConsole()
{
	auto ret = FreeConsole();
	Sleep(10);
	auto ret2 = PostMessage(console_Hwnd, WM_CLOSE, 0, 0);
	return ret2;
}

bool BF_System::RunExe(std::string commandLine)
{
	// 这俩变量，弃用的两个参数，没有用了
	bool waitForCompletion = false;
	int windowShowStyle = SW_SHOWNORMAL;

	STARTUPINFO startupInfo = { sizeof(startupInfo) };
	PROCESS_INFORMATION processInfo;
	bool success = false;

	if (CreateProcess(NULL, &commandLine[0], NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		if (waitForCompletion) {
			WaitForSingleObject(processInfo.hProcess, INFINITE);
		}
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		success = true;
	}
	return success;
}

std::string BF_System::Getclipboard() {
	std::string clipboardText = "";

	if (OpenClipboard(NULL)) {
		HANDLE clipboardData = GetClipboardData(CF_TEXT);
		if (clipboardData != NULL) {
			char* clipboardTextPtr = (char*)GlobalLock(clipboardData);
			if (clipboardTextPtr != NULL) {
				clipboardText = clipboardTextPtr;
				GlobalUnlock(clipboardData);
			}
		}
		CloseClipboard();
	}

	return clipboardText;
}

bool BF_System::Setclipboard(const std::string& text) {
	bool result = false;

	if (OpenClipboard(NULL)) {
		EmptyClipboard();
		HGLOBAL clipboardData = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
		if (clipboardData != NULL) {
			char* clipboardDataPtr = (char*)GlobalLock(clipboardData);
			if (clipboardDataPtr != NULL) {
				memcpy(clipboardDataPtr, text.c_str(), text.size() + 1);
				GlobalUnlock(clipboardData);
				SetClipboardData(CF_TEXT, clipboardData);
				result = true;
			}
		}
		CloseClipboard();
	}

	return result;
}

bool BF_System::ClearClipboard()
{
	if (!OpenClipboard(NULL))
	{
		return false;
	}

	if (!EmptyClipboard())
	{
		CloseClipboard();
		return false;
	}

	CloseClipboard();
	return true;
}

BOOL BF_System::Beep(DWORD frequent, DWORD time)
{
	auto ret = ::Beep(frequent, time);
	return ret;
}

std::string BF_System::GetNowDirectory()
{
	char buffer[MAX_PATH];
	::GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string path(buffer);
	size_t pos = path.find_last_of("\\/");
	return (pos == std::string::npos) ? "" : path.substr(0, pos);
}

std::string BF_System::GetNowProcessName() {
	char filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);
	return std::string(filename);
}

