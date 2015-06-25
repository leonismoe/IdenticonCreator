#include "ICButton.h"

const COLORREF ICButton::COLOR_DEFAULT_NORMAL_FOREGROUND = RGB(0xCC, 0xCC, 0xCC);
const COLORREF ICButton::COLOR_DEFAULT_NORMAL_BACKGROUND = RGB(0xFF, 0xFF, 0xFF);
const COLORREF ICButton::COLOR_DEFAULT_HOVERED_FOREGROUND = RGB(0x33, 0x33, 0x33);
const COLORREF ICButton::COLOR_DEFAULT_HOVERED_BACKGROUND = RGB(0xE7, 0xE7, 0xE7);
const COLORREF ICButton::COLOR_DEFAULT_PUSHED_FOREGROUND = RGB(0xFF, 0xFF, 0xFF);
const COLORREF ICButton::COLOR_DEFAULT_PUSHED_BACKGROUND = RGB(0x4E, 0xA6, 0xEA);

const LPCWSTR ICButton::BUTTONID = L"BUTTON";
const LPCWSTR ICButton::DEFAULT_FONT = L"Î¢ÈíÑÅºÚ";
const int ICButton::DEFAULT_WIDTH  = 30;
const int ICButton::DEFAULT_HEIGHT = 26;
const int ICButton::DEFAULT_FONTSIZE = 20;

std::vector<ICButton*> ICButton::ButtonInstances;
std::vector<HWND> ICButton::ButtonHWNDs;

ICButton::ICButton() {}
ICButton::~ICButton() {}

ICButton::ICButton(HWND pwnd, LPCWSTR label, int x, int y, WNDPROC callback, int width, int height, int fontsize, LPCWSTR font, COLORREF fgNormal, COLORREF bgNormal, COLORREF fgHovered, COLORREF bgHovered, COLORREF fgPushed, COLORREF bgPushed) {
	initialize(pwnd, label, x, y, callback, width, height, fontsize, font, fgNormal, bgNormal, fgHovered, bgHovered, fgPushed, bgPushed);
}

void ICButton::initialize(HWND pwnd, LPCWSTR label, int x, int y, WNDPROC callback, int width, int height, int fontsize, LPCWSTR font, COLORREF fgNormal, COLORREF bgNormal, COLORREF fgHovered, COLORREF bgHovered, COLORREF fgPushed, COLORREF bgPushed) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->callback = callback;
	this->fontsize = fontsize;
	this->font = font;
	this->fgNormalColor = fgNormal;
	this->bgNormalColor = bgNormal;
	this->fgHoveredColor = fgHovered;
	this->bgHoveredColor = bgHovered;
	this->fgPushedColor = fgPushed;
	this->bgPushedColor = bgPushed;
	btnID = ElementsManager::createElement(ElementsManager::ETYPES::BUTTON);
	instance = CreateWindowEx(0, BUTTONID, label,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		x, y, width, height, pwnd, (HMENU)btnID, 0, 0);
	oldWndProc = (WNDPROC)SetWindowLong(instance, GWL_WNDPROC, (DWORD)ICBWndProc);
	ButtonHWNDs.push_back(instance);
	ButtonInstances.push_back(this);
}

int ICButton::isICButton(HWND &hwnd) {
	std::vector<HWND>::iterator hwnd_iterator;
	if (hwnd && (hwnd_iterator = std::find(ButtonHWNDs.begin(), ButtonHWNDs.end(), hwnd)) != ButtonHWNDs.end()) {
		int index = hwnd_iterator - ButtonHWNDs.begin();
		return index;
	}
	return -1;
}

bool ICButton::isCursorOnButton(int xPos, int yPos) {
	return xPos >= 0 && yPos >= 0 && xPos <= width && yPos <= height;
}

void ICButton::drawButton(LPDRAWITEMSTRUCT &pDIS) {
	int index = isICButton(pDIS->hwndItem);
	if (index < 0) return;

	ICButton *instance = ButtonInstances[index];
	HWND hWnd = pDIS->hwndItem;
	HDC  hDC  = pDIS->hDC;
	//PAINTSTRUCT Ps;
	HBRUSH      NewBrush;
	RECT        rect = pDIS->rcItem;
	COLORREF    foreground;
	COLORREF    background;
	switch (instance->state) {
	case NORMAL:
		foreground = instance->fgNormalColor;
		background = instance->bgNormalColor;
		break;
	case HOVERED:
		foreground = instance->fgHoveredColor;
		background = instance->bgHoveredColor;
		break;
	case PUSHED:
		foreground = instance->fgPushedColor;
		background = instance->bgPushedColor;
		break;
	}
	//hDC = BeginPaint(hWnd, &Ps);
	//hDC = GetDC(hWnd);
	//GetClientRect(hWnd, &rect);
	NewBrush = CreateSolidBrush(background);
	SelectObject(hDC, NewBrush);
	SetBkColor(hDC, background);
	SetTextColor(hDC, foreground);
	//Rectangle(hDC, 0, 0, instance.width, instance.height);
	//SetRect(&rect, 0, 0, instance.width, instance.height);
	FillRect(hDC, &rect, NewBrush);
	DeleteObject(NewBrush);

	HFONT font = CreateFont(instance->fontsize, 0, 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, instance->font);
	HFONT hFontOld = (HFONT)SelectObject(hDC, font);
	WCHAR textBuffer[99];
	int len = SendMessage(ButtonHWNDs[index], WM_GETTEXT, ARRAYSIZE(textBuffer), (LPARAM)textBuffer);
	//TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, textBuffer, len);
	DrawText(hDC, textBuffer, len, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SelectObject(hDC, hFontOld);
	ReleaseDC(hWnd, hDC);
	//EndPaint(hWnd, &Ps);
}

void ICButton::ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam, int index) {
	if (index < 0) return;
	ButtonInstances[index]->state = NORMAL;
	int eventid = LOWORD(wParam);
	switch (eventid) {
	case BN_CLICKED:
		CallWindowProc(ButtonInstances[index]->callback, hWnd, uMsg, wParam, lParam);
		break;
	}
}

/*void ICButton::mouseHover(ICButton *instance) {
	if (instance == NULL) return;
	;
}*/

void ICButton::mouseLeave(ICButton *instance, ICButton *except) {
	if (instance == NULL) {
		std::vector<ICButton*>::iterator iter;
		for (iter = ButtonInstances.begin(); iter != ButtonInstances.end(); iter++) {
			if (*iter != except) mouseLeave(*iter);
		}
		return;
	}

	if (instance->state == PUSHED) {
		instance->state = HOVERED;
		InvalidateRect(instance->instance, NULL, TRUE);
		UpdateWindow(instance->instance);
	}
	else  if (instance->state != NORMAL) {
		instance->state = NORMAL;
		InvalidateRect(instance->instance, NULL, TRUE);
		UpdateWindow(instance->instance);
	}
}

/*void ICButton::mouseDown(ICButton *instance) {
	if (instance == NULL) return;
	;
}*/

/*void ICButton::mouseUp(ICButton *instance) {
	if (instance == NULL) return;
	return mouseHover(instance);
}*/


LRESULT CALLBACK ICButton::ICBWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LONG lParam) {
	int index = ICButton::isICButton(hWnd);
	if (index < 0 || !ButtonInstances[index]) return DefWindowProc(hWnd, uMsg, wParam, lParam);
	ICButton *instance = ButtonInstances[index];

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		HWND winobj = (HWND)lParam;
		if (winobj && index > -1)
		{
			ICButton::ButtonProc(winobj, uMsg, wParam, lParam, index);
			break;
		}
		break;
	}

	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
	{
		mouseLeave(instance);
		break;
	}

	case WM_LBUTTONUP:
	{
		//mouseLeave(NULL, instance);
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (instance->isCursorOnButton(xPos, yPos)) {
			if (instance->state == PUSHED) {
				CallWindowProc(ButtonInstances[index]->callback, hWnd, uMsg, wParam, lParam);
			}
			instance->state = NORMAL;
		} else if (instance->state != NORMAL) {
			instance->state = NORMAL;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
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

		//mouseLeave(NULL, instance);
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (instance->isCursorOnButton(xPos, yPos)) {
			if (wParam == MK_LBUTTON) {
				if (instance->state != PUSHED) {
					instance->state = PUSHED;
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;
			}
			else if (instance->state != HOVERED) {
				instance->state = HOVERED;
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				break;
			}
		}
		else if (instance->state == PUSHED) {
			instance->state = HOVERED;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	{
		//mouseLeave(NULL, instance);
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (instance->isCursorOnButton(xPos, yPos) && instance->state != PUSHED) {
			instance->state = PUSHED;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	}

	}

	return CallWindowProc(ButtonInstances[index]->oldWndProc, hWnd, uMsg, wParam, lParam);
}