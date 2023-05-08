#pragma once
#include <Windows.h>
#include <string>
#include <string>

//判断加载成功否		LoadSuccessMessageEx();
//创建窗口			ImGuiCreate_WindowEx(int GlobalLeft, int GlobalTop, int GlobalWidth, int GlobalHeight, void (*callProc)(), HWND hWnd);
//开始绘制			ImGuiStart_ConsoleEx(std::string title, int consoleWidth, int consoleHeight);
//结束绘制			ImGuiEnd_ConsoleEx();
//置全局颜色			GlobalColorEx(int color);
//消息循环			LoopMessageEx();
//画矩形				DrawRectEx(int rectLeft, int rectTop, int rectWidth, int rectHeight, int rectPixel);
//画圆				DrawCircleEx(int circleLeft, int circleTop, int radius, int curvature, int ciclePixel);
//画准星				DrawSightEx(int left, int top, int type);
//画线				DrawLineEx(int x1, int y1, int x2, int y2, int px);
//写文字				DrawStringEx(std::string text, int left, int top, int fontSize);
//多选框				CheckBoxEx(std::string title, bool decided);
//不换行				DontNewLineEx();

typedef void (*MyFunction_LoadSuccessMessage)();
typedef void (*MyFunction_ImGuiCreate_Window)(int GlobalLeft, int GlobalTop, int GlobalWidth, int GlobalHeight, void (*callProc)(), HWND hWnd);
typedef void (*MyFunction_DrawConsole)();
typedef void (*MyFunction_ImGuiStart_Console)(std::string title, int consoleWidth, int consoleHeight);
typedef void (*MyFunction_ImGuiEnd_Console)();
typedef void (*MyFunction_GlobalColor)(int color);
typedef void (*MyFunction_ColorToRGBA)();
typedef void (*MyFunction_LoopMessage)();
typedef void (*MyFunction_DrawRect)(int rectLeft, int rectTop, int rectWidth, int rectHeight, int rectPixel);
typedef void (*MyFunction_DrawCircle)(int circleLeft, int circleTop, int radius, int curvature, int ciclePixel);
typedef void (*MyFunction_DrawSight)(int left, int top, int type);
typedef void (*MyFunction_DrawLine)(int x1, int y1, int x2, int y2, int px);
typedef void (*MyFunction_DrawString)(std::string text, int left, int top, int fontSize);
typedef void (*MyFunction_CheckBox)(std::string title, bool decided);
typedef void (*MyFunction_DontNewLine)();

typedef float (*MyFunctionFloat)();
typedef bool (*MyFunctionBool)();

class BF_ImGui
{
public:
	// 初始化imgui类  32位调试环境
	// dir为dll的 路径+名称 例: C:\\Users\\Administrator\\Desktop\\imgui.dll
	BOOL Init();
	void LoadSuccessMessageEx();
	void ImGuiCreate_WindowEx(int GlobalLeft, int GlobalTop, int GlobalWidth, int GlobalHeight, void (*callProc)(), HWND hWnd);
	void DrawConsoleEx();
	void ImGuiStart_ConsoleEx(std::string title, int consoleWidth, int consoleHeight);
	void ImGuiEnd_ConsoleEx();
	void GlobalColorEx(int color);
	void LoopMessageEx();
	void DrawRectEx(int rectLeft, int rectTop, int rectWidth, int rectHeight, int rectPixel);
	void DrawCircleEx(int circleLeft, int circleTop, int radius, int curvature, int ciclePixel);
	void DrawSightEx(int left, int top, int type);
	void DrawLineEx(int x1, int y1, int x2, int y2, int px);
	void DrawStringEx(std::string text, int left, int top, int fontSize);
	void CheckBoxEx(std::string title, bool decided);
	void DontNewLineEx();

public:
	BF_ImGui();
	~BF_ImGui();

private:
	HMODULE dllHandle;

	MyFunction_LoadSuccessMessage       LoadSuccessMessage;
	MyFunction_ImGuiCreate_Window       ImGuiCreate_Window;
	MyFunction_DrawConsole              DrawConsole;
	MyFunction_ImGuiStart_Console       ImGuiStart_Console;
	MyFunction_ImGuiEnd_Console         ImGuiEnd_Console;
	MyFunction_GlobalColor              GlobalColor;
	MyFunction_ColorToRGBA              ColorToRGBA;
	MyFunction_LoopMessage              LoopMessage;
	MyFunction_DrawRect                 DrawRect;
	MyFunction_DrawCircle               DrawCircle;
	MyFunction_DrawSight                DrawSight;
	MyFunction_DrawLine                 DrawLine;
	MyFunction_DrawString               DrawString;
	MyFunction_CheckBox                 CheckBox;
	MyFunction_DontNewLine              DontNewLine;

	MyFunctionBool Button;
	MyFunctionFloat GetFPS;

};

BF_ImGui::BF_ImGui()
{
	dllHandle = NULL;
}

BF_ImGui::~BF_ImGui()
{
	if (dllHandle != NULL)
		// 卸载DLL
		FreeLibrary(dllHandle);
}

BOOL BF_ImGui::Init()
{
	// 加载DLL
	dllHandle = LoadLibraryA("imgui.dll");
	if (dllHandle == NULL) {
		// 加载失败
		return false;
	}
	// 取函数地址
	// 1
	LoadSuccessMessage = (MyFunction_LoadSuccessMessage)GetProcAddress(dllHandle, "LoadSuccessMessage");
	if (LoadSuccessMessage == NULL) {
		// 取地址失败
		return false;
	}

	// 2
	ImGuiCreate_Window = (MyFunction_ImGuiCreate_Window)GetProcAddress(dllHandle, "ImGuiCreate_Window");
	if (ImGuiCreate_Window == NULL) {
		return false;
	}

	// 3
	DrawConsole = (MyFunction_DrawConsole)GetProcAddress(dllHandle, "DrawConsole");
	if (DrawConsole == NULL) {
		return false;
	}

	ImGuiStart_Console = (MyFunction_ImGuiStart_Console)GetProcAddress(dllHandle, "ImGuiStart_Console");
	if (ImGuiStart_Console == NULL) {
		return false;
	}

	ImGuiEnd_Console = (MyFunction_ImGuiEnd_Console)GetProcAddress(dllHandle, "ImGuiEnd_Console");
	if (ImGuiEnd_Console == NULL) {
		return false;
	}

	GlobalColor = (MyFunction_GlobalColor)GetProcAddress(dllHandle, "GlobalColor");
	if (GlobalColor == NULL) {
		return false;
	}

	ColorToRGBA = (MyFunction_ColorToRGBA)GetProcAddress(dllHandle, "ColorToRGBA");
	if (ColorToRGBA == NULL) {
		return false;
	}

	LoopMessage = (MyFunction_LoopMessage)GetProcAddress(dllHandle, "LoopMessage");
	if (LoopMessage == NULL) {
		return false;
	}

	DrawRect = (MyFunction_DrawRect)GetProcAddress(dllHandle, "DrawRect");
	if (DrawRect == NULL) {
		return false;
	}

	DrawCircle = (MyFunction_DrawCircle)GetProcAddress(dllHandle, "DrawCircle");
	if (DrawCircle == NULL) {
		return false;
	}

	DrawSight = (MyFunction_DrawSight)GetProcAddress(dllHandle, "DrawSight");
	if (DrawSight == NULL) {
		return false;
	}

	DrawLine = (MyFunction_DrawLine)GetProcAddress(dllHandle, "DrawLine");
	if (DrawLine == NULL) {
		return false;
	}

	DrawString = (MyFunction_DrawString)GetProcAddress(dllHandle, "DrawString");
	if (DrawString == NULL) {
		return false;
	}

	CheckBox = (MyFunction_CheckBox)GetProcAddress(dllHandle, "CheckBox");
	if (CheckBox == NULL) {
		return false;
	}

	DontNewLine = (MyFunction_DontNewLine)GetProcAddress(dllHandle, "DontNewLine");
	if (DontNewLine == NULL) {
		return false;
	}

	GetFPS = (MyFunctionFloat)GetProcAddress(dllHandle, "GetFPS");
	if (GetFPS == NULL) {
		return false;
	}

	Button = (MyFunctionBool)GetProcAddress(dllHandle, "Button");
	if (Button == NULL) {
		return false;
	}

}

void BF_ImGui::LoadSuccessMessageEx()
{
	LoadSuccessMessage();
}

void BF_ImGui::ImGuiCreate_WindowEx(int GlobalLeft, int GlobalTop, int GlobalWidth, int GlobalHeight, void (*callProc)(), HWND hWnd)
{
	ImGuiCreate_Window(GlobalLeft, GlobalTop, GlobalWidth, GlobalHeight, callProc, 0);
}

void BF_ImGui::DrawConsoleEx()
{
	DrawConsole();
}

void BF_ImGui::ImGuiStart_ConsoleEx(std::string title, int consoleWidth, int consoleHeight)
{
	ImGuiStart_Console(title, consoleWidth, consoleHeight);
}

void BF_ImGui::ImGuiEnd_ConsoleEx()
{
	ImGuiEnd_Console();
}

inline void BF_ImGui::GlobalColorEx(int color)
{
	GlobalColor(color);
}

void BF_ImGui::LoopMessageEx()
{
	LoopMessage();
}

void BF_ImGui::DrawRectEx(int rectLeft, int rectTop, int rectWidth, int rectHeight, int rectPixel)
{
	DrawRect(rectLeft, rectTop, rectWidth, rectHeight, rectPixel);
}

void BF_ImGui::DrawCircleEx(int circleLeft, int circleTop, int radius, int curvature, int ciclePixel)
{
	DrawCircle(circleLeft, circleTop, radius, curvature, ciclePixel);
}

void BF_ImGui::DrawSightEx(int left, int top, int type)
{
	DrawSight(left, top, type);
}

void BF_ImGui::DrawLineEx(int x1, int y1, int x2, int y2, int px)
{
	DrawLineEx(x1, y1, x2, y2, px);
}

void BF_ImGui::DrawStringEx(std::string text, int left, int top, int fontSize)
{
	DrawString(text, left, top, fontSize);
}

void BF_ImGui::CheckBoxEx(std::string title, bool decided)
{
	// text穿进去必有错误！！！
	CheckBox(title, decided);
}

void BF_ImGui::DontNewLineEx()
{
	DontNewLine();
}

