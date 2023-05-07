#pragma once
#include <Windows.h>
#include <conio.h>

// ----------------------------------------------------------------
// 等待按键 "_getch" 为c++标准库函数，返回int，无需类对象调用
// 
// 按键							KeyPress
// 按键消息						KeyPressToWindow
// 等待按键						_getch
// 判断按键状态					GetKeystatus
// 窗口_监视热键_注册				InstallKeyboardHook
// 窗口_监视热键_卸载				UninstallKeyboardHook
// 消息循环						LoopMessage
// ----------------------------------------------------------------

#define 按键					KeyPress
#define 按键消息				KeyPressToWindow
#define 等待按键				_getch
#define 判断按键状态			GetKeystatus
#define 窗口_监视热键_注册	InstallKeyboardHook
#define 窗口_监视热键_卸载	UninstallKeyboardHook
#define 消息循环				LoopMessage

#define 按下		256
#define 放开		257
#define ALT按下	260
#define 键代码	kbStruct->vkCode

// 等待按键
// C++标准库函数，按下某个键后返回键代码
int __cdecl _getch(void);

class BF_Keyboard
{
public:
	// 按键
	// keyCode: 字母填ASCII码，功能键填宏，例VK_F1
	// type: 1按键 2按下 3放开
	static void KeyPress(int keyCode, int type);

	// 指定窗口窗口发送模拟按键消息
	// hWnd:	窗口句柄
	// keyCode: 字母填ASCII码，功能键填宏，例VK_F1
	// type:	1按键 2按下 3放开
	static void KeyPressToWindow(HWND hWnd, int keyCode, int type);

	// 判断按键状态
	// 按下返回真，弹起返回假; 该函数需要处在死循环中才能起到检测作用
	// keyCode 键代码， speed 检测速度(可忽略，越低速度越快，cpu越高，默认为10)
	// keyCode 是字母时，用大写ASCII码；是其他键时，用标准宏（例如：F1: VK_F1)
	static BOOL GetKeystatus(int& keyCode, int speed = 10);

	// 窗口_监视热键_注册
	// callBack 为回调函数指针
	bool InstallKeyboardHook(HOOKPROC callBack);

	// 窗口_监视热键_卸载
	bool UninstallKeyboardHook();

	// 消息循环
	void LoopMessage();

private:
	static void SendKeyMessage(int& keyCode, bool type);
	static void SendKeyMessageToWindow1(HWND& hwnd, int& keyCode, bool keyDown);
	static void SendKeyMessageToWindow2(HWND& hwnd, int& keyCode, bool keyDown);

public:
	BF_Keyboard();
	~BF_Keyboard();

private:
	HHOOK hook;
	MSG msg;
};

BF_Keyboard::BF_Keyboard() { hook = NULL; }

BF_Keyboard::~BF_Keyboard() { if (hook != NULL) UninstallKeyboardHook(); }

void BF_Keyboard::KeyPress(int keyCode, int type)
{
	// 1 #按键_ 2 #按下_ 3 #放开_

	if (type == 1)
	{
		SendKeyMessage(keyCode, true);
		SendKeyMessage(keyCode, false);
	}
	if (type == 2)
	{
		SendKeyMessage(keyCode, true);
	}
	if (type == 3)
	{
		SendKeyMessage(keyCode, false);
	}
}

void BF_Keyboard::SendKeyMessage(int& keyCode, bool type)
{
	INPUT input;

	// 1.设置结构体
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	// 2.设置键代码
	input.ki.wVk = keyCode;

	// 3.判断键类型
	if (type)
	{
		input.ki.dwFlags = 0;  // 按下
	}
	else
	{
		input.ki.dwFlags = KEYEVENTF_KEYUP;  // 弹起
	}

	// 4.发送消息
	SendInput(1, &input, sizeof(INPUT));
}

void BF_Keyboard::SendKeyMessageToWindow1(HWND& hwnd, int& keyCode, bool keyDown)
{
	// 构造键盘按键消息
	LPARAM lParam = 0;
	if (keyDown)
	{
		lParam |= (1 << 31);  // 第 31 位表示按键是否为按下状态
	}
	lParam |= (MapVirtualKey(keyCode, MAPVK_VK_TO_VSC) << 16);  // 第 16-23 位表示扫描码
	if (keyDown == true)
	{

	}
	PostMessage(hwnd, WM_KEYDOWN, keyCode, lParam);  // 发送按下消息
	PostMessage(hwnd, WM_KEYUP, keyCode, lParam | (1 << 30));  // 发送弹起消息
}

void BF_Keyboard::KeyPressToWindow(HWND hWnd, int keyCode, int type)
{
	switch (type)
	{
	case 1: SendKeyMessageToWindow1(hWnd, keyCode, true); break;	// 按键
	case 2: SendKeyMessageToWindow2(hWnd, keyCode, true); break;	// 按下
	case 3: SendKeyMessageToWindow2(hWnd, keyCode, false); break;	// 弹起
	}

}

void BF_Keyboard::SendKeyMessageToWindow2(HWND& hwnd, int& keyCode, bool keyDown)
{
	LPARAM lParam = 0;
	if (keyDown)
	{
		lParam |= (1 << 31); // 第 31 位表示按键是否为按下状态
	}
	lParam |= (MapVirtualKey(keyCode, MAPVK_VK_TO_VSC) << 16); // 第 16-23 位表示扫描码

	UINT message = keyDown ? WM_KEYDOWN : WM_KEYUP; // 根据 keyDown 参数判断是按下还是弹起操作
	SendMessage(hwnd, message, keyCode, lParam);
}

// 判断按键状态
// 按下返回真，弹起返回假; 该函数需要处在死循环中才能起到检测作用
// keyCode 键代码， speed 检测速度(可忽略，越低速度越快，cpu越高，默认为10)
// keyCode 是字母时，用大写ASCII码；是其他键时，用标准宏（例如：F1: VK_F1)
BOOL BF_Keyboard::GetKeystatus(int& keyCode, int speed)
{
	SHORT ret = GetAsyncKeyState(keyCode) & 0x8000;
	if (ret < 0)
	{
		if (speed == 0)
		{
			return true;
		}
		Sleep(speed);
		return true;
	}
	else
	{
		if (speed == 0)
		{
			return false;
		}
		Sleep(speed);
		return false;
	}
}

// 全局钩子回调函数示例
/*
LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
	// 个人代码写在下面

	if (wParam == ALT按下 || 按下 || 放开)	// 举例
	{
		cout << "键代码" << 键代码 << endl;
	}

	// 个人代码写在上面
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BF_Keyboard::LoopMessage();
*/
bool BF_Keyboard::InstallKeyboardHook(HOOKPROC callBack)
{
	this->hook = SetWindowsHookEx(WH_KEYBOARD_LL, callBack, NULL, 0);
	return (hook != NULL);
}

bool BF_Keyboard::UninstallKeyboardHook()
{
	BOOL result = UnhookWindowsHookEx(this->hook);
	hook = NULL;
	return (result != FALSE);
}

void BF_Keyboard::LoopMessage()
{
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


// 钩子安装案例
//
//LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
//	// 个人代码写在下面
//
//	if (wParam == ALT按下 || 按下 || 放开)	// 举例
//	{
//		cout << "键代码" << 键代码 << endl;
//	}
//
//	// 个人代码写在上面
//	return CallNextHookEx(NULL, nCode, wParam, lParam);
//}
//int main()
//{
//	BF_Keyboard key;
//	key.InstallKeyboardHook(KeyboardHookCallback);
//	key.LoopMessage();
//	return 0;
//}
