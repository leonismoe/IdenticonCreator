#ifndef _TEXT_INPUT_H
#define _TEXT_INPUT_H

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include "ElementsManager.h"
#include "Callbacks.h"


#define PADDING(top, right, bottom, left)  (TextInput::Padding { top, right, bottom, left })

class TextInput {

	friend LRESULT CALLBACK TextInputCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam);

public:
	struct Padding {
		int top;
		int right;
		int bottom;
		int left;
	};

	const static enum TEXTINPUT_STATE { NORMAL, HOVER, FOCUS };

	const static COLORREF COLOR_DEFAULT_BORDER_NORMAL;
	const static COLORREF COLOR_DEFAULT_BORDER_HOVER;
	const static COLORREF COLOR_DEFAULT_BORDER_FOCUS;

	const static LPCWSTR DEFAULT_FONT;
	const static int DEFAULT_WIDTH;
	const static int DEFAULT_HEIGHT;
	const static int DEFAULT_FONTSIZE;
	const static Padding DEFAULT_PADDING;

	TextInput();
	~TextInput();
	TextInput(HWND pwnd, int x, int y, WNDPROC callback,
		int width, int height,
		Padding padding = DEFAULT_PADDING, int border = 1,
		LPCWSTR font = DEFAULT_FONT, int fontsize = DEFAULT_FONTSIZE);
	void initialize(HWND pwnd, int x, int y, WNDPROC callback,
		int width, int height,
		Padding padding = DEFAULT_PADDING, int border = 1,
		LPCWSTR font = DEFAULT_FONT, int fontsize = DEFAULT_FONTSIZE);
	static int isTextInput(HWND &hWnd);
	static const TextInput* const TextInput::getInstance(HWND &hWnd);
	static const TextInput* const getInstance(int index);
	static void handleEvents(HWND &hWnd, UINT uMsg, WPARAM wParam, LONG lParam, int index = -1);
	static LRESULT CALLBACK InputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam);

protected:
	static std::vector<TextInput*> InputInstances;
	static std::vector<HWND> InputHWNDs;

	int inputID;
	int x;
	int y;
	int width = DEFAULT_WIDTH;
	int height = DEFAULT_HEIGHT;
	int fontsize = DEFAULT_FONTSIZE;
	int border = 1;
	int radius = 6;
	Padding padding;
	LPCWSTR font = DEFAULT_FONT;
	HWND hWnd;
	TEXTINPUT_STATE state = NORMAL;
	WNDPROC oldWndProc;
	WNDPROC callback;

	COLORREF normalBorderColor = COLOR_DEFAULT_BORDER_NORMAL;
	COLORREF hoverBorderColor  = COLOR_DEFAULT_BORDER_HOVER;
	COLORREF focusBorderColor  = COLOR_DEFAULT_BORDER_FOCUS;

	int ctrlx;
	int ctrly;
	int ctrlwidth;
	int ctrlheight;


};

#endif
