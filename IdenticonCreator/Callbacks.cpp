#include "ICButton.h"
#include "TextInput.h"
#include "Canvas.h"
#include "Identicon.h"
#include "Callbacks.h"

LRESULT CALLBACK CloseBtnCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam) {
	DestroyWindow(GetParent(hWnd));
	return TRUE;
}

LRESULT CALLBACK MinimizeBtnCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam) {
	ShowWindow(GetParent(hWnd), SW_MINIMIZE);
	return TRUE;
}

LRESULT CALLBACK TextInputCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam) {
	int index = TextInput::isTextInput(hWnd);
	if (index < 0) return FALSE;
	TextInput *instance = TextInput::InputInstances[index];
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		HWND winobj = (HWND)lParam;
		if (HIWORD(wParam) == EN_CHANGE) {
			int length = Edit_GetTextLength(hWnd) + 1;
			LPWSTR buffer = new WCHAR[length];
			Edit_GetText(hWnd, buffer, length);
			std::wstring text = buffer;
			delete[] buffer;
			buffer = nullptr;
			Identicon::create(text);
			break;
		}
		break;
	}
	}
	return TRUE;
}
