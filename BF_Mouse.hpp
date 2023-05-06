#pragma once
#include <Windows.h>
#include <stdexcept>

/*
鼠标键           BMS_MouseEvent
取鼠标位置       BMS_GetMousePosition
鼠标移动         BMS_MoveTo
鼠标相对移动     BMS_MoveR
安装鼠标钩子     BMS_InstallMouseHook
消息循环         BMS_LoopMessage
卸载钩子         BMS_UnInstallMouseHook
钩子回调         BMS_MouseHookProc
*/

/*
// ------------------------------------鼠标钩子wParam参数---------------------------------------------
wParam：表示消息类型，它的值可以是以下常量之一：

WM_MOUSEMOVE：表示鼠标移动消息。
WM_LBUTTONDOWN：表示鼠标左键按下消息。
WM_LBUTTONUP：表示鼠标左键弹起消息。
WM_RBUTTONDOWN：表示鼠标右键按下消息。
WM_RBUTTONUP：表示鼠标右键弹起消息。
WM_MBUTTONDOWN：表示鼠标中键按下消息。
WM_MBUTTONUP：表示鼠标中键弹起消息。
WM_MOUSEWHEEL：表示鼠标滚轮消息。
WM_MOUSEHWHEEL：表示鼠标横向滚轮消息。
*/
// ----------------------------------------------------------------------------------------------------

// ------------------------------------MouseEvent---------------------------------------------
#define MOUSE_LEFT_CLICK            MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP                                 // 左键单击
#define MOUSE_LEFT_DOUBLE_CLICK     MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP|MOUSEEVENTF_DOUBLECLICK         // 左键双击
#define MOUSE_LEFT_DOWN             MOUSEEVENTF_LEFTDOWN                                                    // 左键按下
#define MOUSE_LEFT_UP               MOUSEEVENTF_LEFTUP                                                      // 左键弹起

#define MOUSE_RIGHT_CLICK           MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP                               // 右键单击
#define MOUSE_RIGHT_DOUBLE_CLICK    MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP|MOUSE_RIGHT_DOUBLE_CLICK      // 右键双击
#define MOUSE_RIGHT_DOWN            MOUSEEVENTF_RIGHTDOWN                                                   // 右键按下
#define MOUSE_RIGHT_UP              MOUSEEVENTF_RIGHTUP                                                     // 右键弹起

#define MOUSE_MIDDLE_CLICK          MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP                             // 中键单击
#define MOUSE_MIDDLE_DOUBLE_CLICK   MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP|MOUSE_MIDDLE_DOUBLE_CLICK   // 中键双击
#define MOUSE_MIDDLE_DOWN           MOUSEEVENTF_MIDDLEDOWN                                                  // 中键按下
#define MOUSE_MIDDLE_UP             MOUSEEVENTF_MIDDLEUP                                                    // 中键弹起
// -------------------------------------------------------------------------------------------

// -----------------------------------------声明函数-------------------------------------------
void    BMS_MouseEvent(DWORD dwFlags, int delay);      // 鼠标键 // dwFlags键标志  delay延时时间
POINT   BMS_GetMousePosition();                       // 取鼠标位置
BOOL    BMS_MoveTo(int x, int y);                      // 鼠标移动
BOOL    BMS_MoveR(int dx, int dy);                     // 鼠标相对移动

BOOL    BMS_InstallMouseHook(HOOKPROC MouseHookProc);                                  // 安装鼠标钩子
void    BMS_LoopMessage();                                                             // 消息循环
BOOL    BMS_UnInstallMouseHook();                                                      // 卸载钩子
// LRESULT CALLBACK MS_MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);     // 钩子回调
// --------------------------------------------------------------------------------------------

// ------------------------------------------实现函数------------------------------------------
void    BMS_MouseEvent(DWORD dwFlags, int delay)
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = dwFlags;
    SendInput(1, &input, sizeof(INPUT));
    Sleep(delay);
}
POINT   BMS_GetMousePosition()
{
    POINT p;
    if (GetCursorPos(&p))
    {
        return p;
    }
    return { -1, -1 };
}
BOOL    BMS_MoveTo(int x, int y)
{
    BOOL ret = SetCursorPos(x, y);
    return ret;
}
BOOL    BMS_MoveR(int dx, int dy)
{
    POINT currentPos;
    GetCursorPos(&currentPos);
    BOOL ret = SetCursorPos(currentPos.x + dx, currentPos.y + dy);
    return ret;
}

HHOOK   g_mouseHook = NULL;// 全局鼠标钩子句柄
// 鼠标钩子回调函数 -- 自写回调函数
/*
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // 如果鼠标事件被允许 && 鼠标发生 WM_ 事件
    if (nCode > 0 && wParam == WM_ )
    {
        // 获取鼠标坐标
        MSLLHOOKSTRUCT* mouseInfo = (MSLLHOOKSTRUCT*)lParam;
        int x = mouseInfo->pt.x; // 获取鼠标事件的屏幕坐标 x 坐标
        int y = mouseInfo->pt.y; // 获取鼠标事件的屏幕坐标 y 坐标
        // TODO: 在这里编写鼠标钩子的处理代码
    }
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

// 消息循环分发消息
LoopMessage();
*/
BOOL    BMS_InstallMouseHook(HOOKPROC MouseHookProc)
{
    if (g_mouseHook != NULL)
    {
        // 钩子已存在
        return false;
    }
    // 获取当前模块句柄
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule == NULL) {
        return false;
    }

    // 安装鼠标钩子
    g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hModule, 0);
    if (g_mouseHook == NULL) {
        // 安装失败
        return false;
    }
    return true;

}
void    BMS_LoopMessage()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
BOOL    BMS_UnInstallMouseHook()
{
    // 卸载鼠标钩子
    BOOL ret = UnhookWindowsHookEx(g_mouseHook);
    return ret;
}
// --------------------------------------------------------------------------------------------
