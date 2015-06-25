#include "Canvas.h"
#include "ElementsManager.h"
#include "MainWindow.h"
#include "Identicon.h"

const LPCWSTR Canvas::CanvasClassName = L"IDCCANVAS";
std::vector<Canvas*> Canvas::CvsInstances;
std::vector<HWND> Canvas::CvsHWNDs;

Canvas::Canvas() {}

Canvas::Canvas(HWND pwnd, int x, int y, int width, int height) {
	initialize(pwnd, x, y, width, height);
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
	CvsHWNDs.push_back(hWnd);
	CvsInstances.push_back(this);
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

int Canvas::indexOf(HWND &hWnd) {
	std::vector<HWND>::iterator it;
	if (hWnd && (it = std::find(CvsHWNDs.begin(), CvsHWNDs.end(), hWnd)) != CvsHWNDs.end()) {
		int index = it - CvsHWNDs.begin();
		return index;
	}
	return -1;
}

ID2D1HwndRenderTarget* Canvas::getRenderTarget(int index) {
	CvsInstances[index]->initRenderTarget();
	return CvsInstances[index]->RenderTarget;
}

void Canvas::cacheImage(int index) {
	
}

LRESULT CALLBACK Canvas::canvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int index = indexOf(hwnd);
	if (index < 0) return DefWindowProc(hwnd, uMsg, wParam, lParam);
	Canvas *instance = CvsInstances[index];

	switch (uMsg) {
	case WM_LBUTTONDOWN:
	{
		SendMessage(MainWindow::getHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	break;

	case WM_PAINT:
	{
		Identicon::draw();
		//PaintProc(hwnd);
		//ValidateRect(hwnd, NULL);
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void Canvas::addPolygon(std::vector<Points> points, bool invert) {
	;
}

HRESULT Canvas::initRenderTarget() {
	HRESULT hr = S_OK;

	if (!RenderTarget) {
		GetClientRect(hWnd, &CanvasRC);

		D2D1_SIZE_U size = D2D1::SizeU(
			CanvasRC.right - CanvasRC.left,
			CanvasRC.bottom - CanvasRC.top
		);

		// Create a Direct2D render target.
		hr = MainWindow::getD2DFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&RenderTarget
		);
	}

	return hr;
}

void Canvas::releaseRenderTarget() {
	SafeRelease(&RenderTarget);
}

HRESULT Canvas::clearCanvas(int index) {
	HRESULT hr = CvsInstances[index]->initRenderTarget();
	if (SUCCEEDED(hr)) {
		CvsInstances[index]->RenderTarget->BeginDraw();
		CvsInstances[index]->RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White, 1.f));
		hr = CvsInstances[index]->RenderTarget->EndDraw();
	}

	return hr;
}


