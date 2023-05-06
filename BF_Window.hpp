#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// -----------------------------------------------------------------------------------------------
/*
暴力枚举窗口句柄					ListWindowName_vector  ListWindowName_char  ListWindowName_string
取行暴力枚举的窗口数组行列数		GetWindowRows  GetWindowClos
取窗口句柄 -- 模糊窗口名			FindVagueWindow
取窗口高度		getWindowHeight
取窗口宽度		getWindowWidth

置顶窗口		SetWindowTopmost_ByHwnd
置窗口可见		IsVisibleWindow
置窗口宽高		SetWindowSize
置窗口标题		SetWindowText_EX
置窗口透明度	SetWindowTransparency
置窗口鼠标穿透	SetWindowVirtual

移动窗口		MoveWindow
销毁窗口		DestroyWindow_EX
移除窗口边框	RemoveBorder

不建议使用：
创建窗口		CreateNewWindow
消息循环		LoopMessage
*/
// -----------------------------------------------------------------------------------------------


class BF_Window
{
public:
	// 创建空白窗口 不建议在本类中创建窗口 -- Windows平台时无控制台
	// BUG: 拖拉边框变黑、鼠标碰到黑色区域箭头改不回来
	HWND CreateNewWindow(LPCTSTR lpWindowName, int nWidth, int nHeight);	// lpWindowName窗口名 nWidth宽 nHeight高 isConsole是否关闭控制台
	// 消息循环
	int LoopMessage();						// 获取消息分发消息


public:
	// 暴力枚举窗口
	vector<char> ListWindowName_vector();	// 容器类型
	char** ListWindowName_char();			// 数组类型
	string* ListWindowName_string();		// 字符串类型
	// 取行暴力枚举的窗口数组行列数
	int GetWindowRows();
	int GetWindowClos();
	// 取窗口句柄 -- 模糊窗口名				☆☆☆☆☆
	HWND FindVagueWindow(const char* vagueTitle); // substring模糊窗口名
	// 取窗口高度
	int getWindowHeight(HWND hWnd);
	// 取窗口宽度
	int getWindowWidth(HWND hWnd);


public:
	// 置窗口置顶
	BOOL SetWindowTopmost_ByHwnd(HWND hwnd);
	// 置窗口可视 --真，可视  假不可视
	BOOL IsVisibleWindow(HWND hWnd, bool judge);			// judge是否可视
	// 置窗口宽高
	BOOL SetWindowSize(HWND hWnd, int width, int height);
	// 置窗口标题
	BOOL SetWindowText_EX(HWND hwnd, LPCSTR title);
	// 置窗口透明度
	BOOL SetWindowTransparency(HWND hWnd, BYTE alpha);
	// 置窗口鼠标穿透 -- 鼠标不再能实际点到窗口 true穿透 false不穿透
	BOOL SetWindowVirtual(HWND hWnd, bool judge);			// judge是否可穿透   -- 穿透后没能还原
	// 移动窗口
	BOOL MoveWindow(HWND hWnd, int x, int y);
	// 销毁窗口
	BOOL DestroyWindow_EX(HWND hWnd);
	// 移除窗口边框
	BOOL RemoveBorder(HWND hWnd);

public:
	// 构造函数
	BF_Window();
	// 析构函数
	~BF_Window();
private:
	// windows窗口总数
	int windowNum;
	int rows;
	int cols;

	char** m_listPtr;
	string* m_StringStr;
};

// 构造函数
BF_Window::BF_Window()
{
	windowNum = 0;
	rows = 0;
	cols = 0;
	m_listPtr = nullptr;
	m_StringStr = nullptr;
}

// 窗口消息回调
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) 
{
	vector<char>* windowTitles = reinterpret_cast<vector<char>*>(lParam);
	const int TITLE_SIZE = 1024;
	char windowTitle[TITLE_SIZE];
	GetWindowText(hwnd, windowTitle, TITLE_SIZE);
	if (strlen(windowTitle) > 0)
	{
		windowTitles->insert(windowTitles->end(), windowTitle, windowTitle + strlen(windowTitle) + 1);
		/*char split[3] = {'|', '|', 0};
		windowTitles->insert(windowTitles->end(), split, split + strlen(split) + 1);*/
	}
	return TRUE;
}

HWND BF_Window::CreateNewWindow(LPCTSTR lpWindowName, int nWidth, int nHeight)
{
	// 创建窗口类
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "MyWindowClass";
	RegisterClass(&wc);

	// 创建窗口
	HWND hWnd = CreateWindow(
		"MyWindowClass",
		lpWindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nWidth,
		nHeight,
		GetConsoleWindow(),
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	if (!hWnd)
	{
		//return -1;
		return nullptr;
	}

	// 显示窗口
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);


	return hWnd;
}

int BF_Window::LoopMessage()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

vector<char> BF_Window::ListWindowName_vector()
{
	vector<char> windowTitles;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowTitles));
	rows = 0;

	// 获取最大长度标题 + 获取窗口数量
	int recordLength = 0;
	int MaxLength = 0;
	for (const auto& title : windowTitles)
	{
		if (title != 0)
		{
			recordLength++;
		}
		else
		{
			if (MaxLength <= recordLength)
			{
				MaxLength = recordLength;
			}
			recordLength = 0;

			// 顺便记录窗口数量
			rows++;
		}
	}
	windowNum = rows;

	return windowTitles;
}

char** BF_Window::ListWindowName_char()
{
	if (m_listPtr != nullptr)
	{
		return m_listPtr;
	}
	vector<char> windowTitles;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowTitles));
	this->rows = 0;

	// 获取最大长度标题 + 获取窗口数量
	int recordLength = 0;
	int MaxLength = 0;
	for (const auto& title : windowTitles)
	{
		if (title != 0)
		{
			recordLength++;
		}
		else
		{
			if (MaxLength <= recordLength)
			{
				MaxLength = recordLength;
			}
			recordLength = 0;

			// 顺便记录窗口数量
			rows++;
		}
	}
	windowNum = rows;
	this->cols = MaxLength;

	// 根据最大长度标题建立二维数组
	char** listPtr = new char*[rows];
	for (int j = 0; j < rows; j++)
	{
		listPtr[j] = new char[MaxLength];
	}

	// 向二位数组中抄入容器数据
	int sum = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (windowTitles[sum] != 0)
			{
				listPtr[i][j] = windowTitles[sum];
				sum++;
				continue;
			}
			// listPtr[i][j] = windowTitles[sum];
			listPtr[i][j] = 0;
			sum++;
			break;
		}
	}

	// 调试输出二维数组
	/*for (int i = 0; i < rows; i++)
	{
		printf("%s\n", listPtr[i]);
	}*/
	m_listPtr = listPtr;
	return listPtr;
}

string* BF_Window::ListWindowName_string()
{
	if (m_StringStr != nullptr)
	{
		return m_StringStr;
	}
	char** listPtr = this->ListWindowName_char();

	string* StringStr = new string[rows];
	for (int i = 0; i < rows; i++)
	{
		StringStr[i] = listPtr[i];
	}
	
	// 留着析构用
	m_StringStr = StringStr;
	return StringStr;
}

int BF_Window::GetWindowRows()
{
	return this->rows;
}

int BF_Window::GetWindowClos()
{
	return this->cols;
}

HWND hWnd;
BOOL CALLBACK EnumWindowsProcFindHwnd(HWND hwnd, LPARAM lParam) // lParam传入的参数
{
	// lParam为传入的子串地址
	char* substr = (char*)(lParam);// ret存储传入的子串地址

	// 获取窗口标题到 windowTitle
	const int TITLE_SIZE = 1024;
	char str[TITLE_SIZE];
	GetWindowText(hwnd, str, TITLE_SIZE);

	// 子串匹配
	int strLen = strlen(str);
	int substrLen = strlen(substr);

	if (substrLen > strLen)
	{
		return -1;
	}

	for (int i = 0; i <= strLen - substrLen; i++)
	{
		bool match = true;

		for (int j = 0; j < substrLen; j++) 
		{
			if (str[i + j] != substr[j])
			{
				match = false;
				break;
			}
		}

		if (match) 
		{
			// 使用substr将值返回
			hWnd = hwnd;
			// 返回FALSE停止枚举
			return FALSE;
		}
	}

	// 返回TRUE继续枚举
	return TRUE;
}

HWND BF_Window::FindVagueWindow(const char* vagueTitle)
{
	const char* SubString = vagueTitle;

	// 匹配子串
	EnumWindows(EnumWindowsProcFindHwnd, (LPARAM)(SubString)); // SubString 传入参数
	
	return hWnd;
}

int BF_Window::getWindowHeight(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect.bottom - rect.top;
}

int BF_Window::getWindowWidth(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect.right - rect.left;
}

BOOL BF_Window::SetWindowTopmost_ByHwnd(HWND hwnd)
{
	BOOL ret = SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return ret;
}

BOOL BF_Window::IsVisibleWindow(HWND hWnd, bool judge)
{
	if (judge == true) {
		BOOL ret = ShowWindow(hWnd, SW_SHOW);
		return ret;
	}
	if (judge == false) {
		BOOL ret = ShowWindow(hWnd, SW_HIDE);
		return ret;
	}
	return FALSE;
}

BOOL BF_Window::SetWindowSize(HWND hWnd, int width, int height)
{
	BOOL ret = SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	return ret;
}

BOOL BF_Window::SetWindowText_EX(HWND hwnd, LPCSTR title)
{
	BOOL ret = SetWindowText(hWnd, title);
	return ret;
}

BOOL BF_Window::SetWindowTransparency(HWND hWnd, BYTE alpha)
{
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	BOOL ret = SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return ret;
}

BOOL BF_Window::SetWindowVirtual(HWND hWnd, bool judge)
{
	// 获取窗口样式和扩展风格
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	// 将扩展风格设置为 WS_EX_TRANSPARENT
	// true为穿透，false不穿透
	if (judge == true) {
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_TRANSPARENT);
	}
	if (judge == false) {
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwExStyle & ~WS_EX_TRANSPARENT);
	}

	// 将样式设置为 WS_POPUP
	// true为穿透，false不穿透
	if (judge == true) {
		LONG ret = SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SYSMENU & WS_POPUP);
		return ret;
	}
	if (judge == false) {
		LONG ret = SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
		return ret;
	}
	
}

BOOL BF_Window::MoveWindow(HWND hWnd, int x, int y)
{
	// 获取窗口宽度和高度
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 设置窗口位置
	BOOL ret = SetWindowPos(hWnd, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
	return ret;
}

BOOL BF_Window::DestroyWindow_EX(HWND hWnd)
{
	BOOL ret = PostMessage(hWnd, WM_CLOSE, 0, 0);
	return ret;
}

BOOL BF_Window::RemoveBorder(HWND hWnd)
{
	// 获取窗口样式
	LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);

	// 将样式设置为无边框样式
	SetWindowLongPtr(hWnd, GWL_STYLE, style & ~WS_BORDER);

	// 调整窗口大小和位置以适应新样式
	BOOL ret = SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	return ret;
}

BF_Window::~BF_Window()
{
	if (m_listPtr != nullptr)
	{
		// 释放内存
		for (int i = 0; i < rows; ++i) {
			delete[] m_listPtr[i];
		}
		delete[] m_listPtr;
	}

	if (m_StringStr != nullptr)
	{
		// 释放内存
		delete[] m_StringStr;
	}

}
