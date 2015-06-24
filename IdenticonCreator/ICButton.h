#ifndef _IC_BUTTON_H
#define _IC_BUTTON_H

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include "ElementsManager.h"

class ICButton {
	
public:
	const static enum ICBUTTON_STATE { NORMAL, HOVERED, PUSHED };

	const static COLORREF COLOR_DEFAULT_NORMAL_FOREGROUND;
	const static COLORREF COLOR_DEFAULT_NORMAL_BACKGROUND;
	const static COLORREF COLOR_DEFAULT_HOVERED_FOREGROUND;
	const static COLORREF COLOR_DEFAULT_HOVERED_BACKGROUND;
	const static COLORREF COLOR_DEFAULT_PUSHED_FOREGROUND;
	const static COLORREF COLOR_DEFAULT_PUSHED_BACKGROUND;

	const static LPCWSTR BUTTONID;
	const static LPCWSTR DEFAULT_FONT;
	const static int DEFAULT_WIDTH;
	const static int DEFAULT_HEIGHT;
	const static int DEFAULT_FONTSIZE;

	// <summary>创建 ICButton 实例</summary>
	// <param name="pwnd">父窗口句柄</param>
	// <param name="label">按钮文字</param>
	// <param name="x">按钮横坐标</param>
	// <param name="y">按钮纵坐标</param>
	// <param name="width">按钮宽度</param>
	// <param name="height">按钮高度</param>
	ICButton();
	ICButton(HWND pwnd, LPCWSTR label, int x, int y, WNDPROC callback,
		int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
		int fontsize = DEFAULT_FONTSIZE, LPCWSTR font = DEFAULT_FONT,
		COLORREF fgNormal = COLOR_DEFAULT_NORMAL_FOREGROUND, COLORREF bgNormal = COLOR_DEFAULT_NORMAL_BACKGROUND,
		COLORREF fgHovered = COLOR_DEFAULT_HOVERED_FOREGROUND, COLORREF bgHovered = COLOR_DEFAULT_HOVERED_BACKGROUND,
		COLORREF fgPushed = COLOR_DEFAULT_PUSHED_FOREGROUND, COLORREF bgPushed = COLOR_DEFAULT_PUSHED_BACKGROUND);
	~ICButton();
	void initialize(HWND pwnd, LPCWSTR label, int x, int y, WNDPROC callback,
		int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
		int fontsize = DEFAULT_FONTSIZE, LPCWSTR font = DEFAULT_FONT,
		COLORREF fgNormal = COLOR_DEFAULT_NORMAL_FOREGROUND, COLORREF bgNormal = COLOR_DEFAULT_NORMAL_BACKGROUND,
		COLORREF fgHovered = COLOR_DEFAULT_HOVERED_FOREGROUND, COLORREF bgHovered = COLOR_DEFAULT_HOVERED_BACKGROUND,
		COLORREF fgPushed = COLOR_DEFAULT_PUSHED_FOREGROUND, COLORREF bgPushed = COLOR_DEFAULT_PUSHED_BACKGROUND);
	bool isCursorOnButton(int xPos, int yPos);

	static int isICButton(HWND &hwnd);
	static void drawButton(LPDRAWITEMSTRUCT &pDIS);
	static void ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LONG lParam, int index = -1);
	static LRESULT CALLBACK ICBWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam);
	static void mouseLeave(ICButton *instance = NULL, ICButton *except = NULL);
	//static void mouseHover(ICButton *instance = NULL);
	//static void mouseDown(ICButton *instance = NULL);
	//static void mouseUp(ICButton *instance = NULL);

protected:
	static std::vector<ICButton*> ButtonInstances;
	static std::vector<HWND> ButtonHWNDs;
	int btnID;
	int x;
	int y;
	int width;
	int height;
	int fontsize;
	LPCWSTR font;
	COLORREF fgNormalColor = COLOR_DEFAULT_NORMAL_FOREGROUND;
	COLORREF bgNormalColor = COLOR_DEFAULT_NORMAL_BACKGROUND;
	COLORREF fgHoveredColor = COLOR_DEFAULT_HOVERED_FOREGROUND;
	COLORREF bgHoveredColor = COLOR_DEFAULT_HOVERED_BACKGROUND;
	COLORREF fgPushedColor = COLOR_DEFAULT_PUSHED_FOREGROUND;
	COLORREF bgPushedColor = COLOR_DEFAULT_PUSHED_BACKGROUND;
	HWND instance;
	ICBUTTON_STATE state = NORMAL;
	WNDPROC oldWndProc;
	WNDPROC callback;
};
/*
enum ICBUTTON_STATE { NORMAL, HOVER, PUSHED };

void ICButton_Register();
void ICButton_Unregister();
void ICButton_Paint(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ICButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
*/
#endif
