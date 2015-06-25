#include "TextInput.h"

const COLORREF TextInput::COLOR_DEFAULT_BORDER_NORMAL = RGB(0xCA, 0xCA, 0xCA);
const COLORREF TextInput::COLOR_DEFAULT_BORDER_HOVER  = RGB(0x9A, 0x9A, 0x9A);
const COLORREF TextInput::COLOR_DEFAULT_BORDER_FOCUS  = RGB(0x4E, 0xA6, 0xEA);

const LPCWSTR TextInput::DEFAULT_FONT = L"Î¢ÈíÑÅºÚ";
const int TextInput::DEFAULT_WIDTH = 295;
const int TextInput::DEFAULT_HEIGHT = 43;
const int TextInput::DEFAULT_FONTSIZE = 24;
const TextInput::Padding TextInput::DEFAULT_PADDING = { 0, 6, 0, 6 };

std::vector<TextInput*> TextInput::InputInstances;
std::vector<HWND> TextInput::InputHWNDs;

TextInput::TextInput() {}
TextInput::~TextInput() {}
TextInput::TextInput(HWND pWnd, int x, int y, WNDPROC callback, int width, int height, Padding padding, int border, LPCWSTR font, int fontsize) {
	initialize(pWnd, x, y, callback, width, height, padding, border, font, fontsize);
}

void TextInput::initialize(HWND pWnd, int x, int y, WNDPROC callback, int width, int height, Padding padding, int border, LPCWSTR font, int fontsize) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->callback = callback;
	this->font = font;
	this->fontsize = fontsize;
	this->padding = padding;
	this->border = border;

	this->ctrlx = x + border + padding.left;
	this->ctrly = y + (height - fontsize) / 2 + padding.top;
	this->ctrlwidth = width - 2 * border - padding.right - padding.left;
	this->ctrlheight = fontsize;

	inputID = ElementsManager::createElement(ElementsManager::ETYPES::TEXTINPUT);
	hWnd = CreateWindowEx(NULL, L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER,
		ctrlx, ctrly, ctrlwidth, ctrlheight, pWnd, (HMENU)inputID, 0, 0);
	HFONT hFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, font);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);
	oldWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)InputWndProc);
	InputHWNDs.push_back(hWnd);
	InputInstances.push_back(this);
}

int TextInput::isTextInput(HWND &hWnd) {
	std::vector<HWND>::iterator hwnd_iterator;
	if (hWnd && (hwnd_iterator = std::find(InputHWNDs.begin(), InputHWNDs.end(), hWnd)) != InputHWNDs.end()) {
		int index = hwnd_iterator - InputHWNDs.begin();
		return index;
	}
	return -1;
}

const TextInput* const TextInput::getInstance(HWND &hWnd) {
	std::vector<TextInput*>::iterator it;
	if (!hWnd) return nullptr;
	for (it = InputInstances.begin(); it != InputInstances.end(); ++it) {
		if ((*it)->hWnd == hWnd) {
			return *it;
		}
	}
	return nullptr;
}

const TextInput* const TextInput::getInstance(int index) {
	if (index >= 0 && index < (int)InputInstances.size()) {
		return InputInstances[index];
	}
	return nullptr;
}

void TextInput::handleEvents(HWND &hWnd, UINT uMsg, WPARAM wParam, LONG lParam, int index) {
	;
}

LRESULT CALLBACK TextInput::InputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam) {
	int index = isTextInput(hWnd);
	if (index < 0) return DefWindowProc(hWnd, uMsg, wParam, lParam);
	TextInput *instance = InputInstances[index];

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		HWND winobj = (HWND)lParam;
		instance->callback(hWnd, uMsg, wParam, lParam);
		break;
	}

	case WM_PAINT:
	case WM_NCPAINT:
	{
		PAINTSTRUCT Ps;
		HDC hDC = BeginPaint(hWnd, &Ps);
		COLORREF borderColor;
		switch (instance->state) {
		case NORMAL:
			borderColor = instance->normalBorderColor;
			break;
		case HOVER:
			borderColor = instance->hoverBorderColor;
			break;
		case FOCUS:
			borderColor = instance->focusBorderColor;
			break;
		}
		SelectObject(hDC, CreatePen(PS_SOLID, instance->border, borderColor));
		SelectObject(hDC, GetStockObject(NULL_BRUSH));
		int x = instance->x - instance->ctrlx;
		int y = instance->y - instance->ctrly;
		RoundRect(hDC, x, y, x + instance->width, y + instance->height, instance->radius, instance->radius);

		EndPaint(hWnd, &Ps);
		break;
	}

	case WM_KILLFOCUS:
	{
		if (instance->state != NORMAL) {
			instance->state = NORMAL;
			CallWindowProc(InputInstances[index]->oldWndProc, hWnd, uMsg, wParam, lParam);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return TRUE;
		}
		break;
	}

	case WM_SETFOCUS:
	{
		if (instance->state != FOCUS) {
			instance->state = FOCUS;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	}

	case WM_CHAR:
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_RETURN:
			SetFocus(GetParent(hWnd));
			return 0;
		}
		break;
	}
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
	{
		if (instance->state != FOCUS) {
			instance->state = NORMAL;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	}
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
	case WM_NCMOUSEHOVER:
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		TrackMouseEvent(&tme);

		if (instance->state == FOCUS) {
			break;
		} else if (instance->state != HOVER) {
			instance->state = HOVER;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		}
		break;
	}

	}

	return CallWindowProc(InputInstances[index]->oldWndProc, hWnd, uMsg, wParam, lParam);
}
