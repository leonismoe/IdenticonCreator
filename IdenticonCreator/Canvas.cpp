#include "Canvas.h"
#include "ElementsManager.h"
#include "MainWindow.h"
#include "Identicon.h"

const LPCWSTR Canvas::CanvasClassName = L"IDCCANVAS";

Canvas::Canvas() {}
Canvas::Canvas(HWND pwnd, int x, int y, int width, int height) {
	initialize(pwnd, x, y, width, height);
}

Canvas::~Canvas() {
	SafeRelease(&RenderTarget);
}

void Canvas::initialize(HWND pwnd, int x, int y, int width, int height) {
	registerCanvas();
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	cvsID = ElementsManager::createElement(ElementsManager::ETYPES::CANVAS);
	hWnd = CreateWindowEx(0, CanvasClassName, L"reg",
		(WS_CHILD | WS_VISIBLE) & ~WS_BORDER,
		x, y, width, height, pwnd, (HMENU)cvsID, 0, 0);
}

void Canvas::registerCanvas() {
	static bool inited = false;
	if (inited) return;
	WNDCLASSEX wcx = { sizeof(WNDCLASSEX) };
	wcx.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = canvasProc;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.lpszClassName = CanvasClassName;
	RegisterClassEx(&wcx);
	inited = true;
}

void Canvas::unregisterCanvas() {
	UnregisterClass(CanvasClassName, NULL);
}

void Canvas::initImage() {}
void Canvas::releaseImage() {}
void Canvas::saveImage() {}

LRESULT CALLBACK Canvas::canvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_LBUTTONDOWN:
	{
		SendMessage(MainWindow::getHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	break;

	case WM_PAINT:
	{
		//Identicon::draw();
		//PaintProc(hwnd);
		//ValidateRect(hwnd, NULL);
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


HRESULT Canvas::initRenderTarget() {
	HRESULT hr = S_OK;

	if (!RenderTarget) {
		GetClientRect(hWnd, &CanvasRC);

		CanvasSize = D2D1::SizeU(
			CanvasRC.right - CanvasRC.left,
			CanvasRC.bottom - CanvasRC.top
		);

		// Create a Direct2D render target.
		hr = MainWindow::getD2DFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, CanvasSize),
			&RenderTarget
		);
	}

	return hr;
}

void Canvas::releaseRenderTarget() {
	SafeRelease(&RenderTarget);
}

HRESULT Canvas::clear() {
	HRESULT hr = initRenderTarget();
	if (SUCCEEDED(hr)) {
		RenderTarget->BeginDraw();
		RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White, 1.f));
		hr = RenderTarget->EndDraw();
	}

	return hr;
}


