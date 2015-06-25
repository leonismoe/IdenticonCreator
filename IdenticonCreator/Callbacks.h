#ifndef _BUTTON_CALLBACK_H
#define _BUTTON_CALLBACK_H

#include <Windows.h>

#define CLOSEBTN_NORMAL_FG   RGB(0xCC, 0xCC, 0xCC)
#define CLOSEBTN_NORMAL_BG   RGB(0xFF, 0xFF, 0xFF)
#define CLOSEBTN_HOVERED_FG  RGB(0xFF, 0xFF, 0xFF)
#define CLOSEBTN_HOVERED_BG  RGB(0xE0, 0x43, 0x43)
#define CLOSEBTN_PUSHED_FG   RGB(0xFF, 0xFF, 0xFF)
#define CLOSEBTN_PUSHED_BG   RGB(0x99, 0x3D, 0x3D)


LRESULT CALLBACK CloseBtnCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam);
LRESULT CALLBACK MinimizeBtnCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam);
LRESULT CALLBACK TextInputCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam);

#endif
