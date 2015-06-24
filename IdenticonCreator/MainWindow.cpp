#include "MainWindow.h"
#include "ICButton.h"
#include "TextInput.h"
#include "Canvas.h"
#include "Callbacks.h"
#include "resource.h"

const LPWSTR MainWindow::ClassName = L"IdenticonCreator";
const LPWSTR MainWindow::WindowTitle = L"identicon 生成器";
MainWindow *MainWindow::instance = nullptr;

const int MainWindow::width = 800;
const int MainWindow::height = 500;

ICButton closeBtn;
ICButton minimizeBtn;
TextInput textInputField;
Canvas canvasInstance;

MainWindow::MainWindow() : hWnd(NULL), Direct2dFactory(NULL), RenderTarget(NULL) {
}

MainWindow::~MainWindow() {
	SafeRelease(&Direct2dFactory);
	SafeRelease(&RenderTarget);
}

void MainWindow::RunMessageLoop() {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT MainWindow::Initialize(HINSTANCE hInstance) {
	HRESULT hr = S_OK;
	instance = this;
	hInstance = hInstance ? hInstance : HINST_THISCOMPONENT;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr)) {
		AppIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APPLICATION));

		// Register the window class.
		WNDCLASSEX wcx = { sizeof(WNDCLASSEX) };
		wcx.style = CS_DROPSHADOW;
		wcx.lpfnWndProc = MainWindow::WndProc;
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = sizeof(LONG_PTR);
		wcx.hInstance = hInstance;
		wcx.hbrBackground = NULL;
		wcx.lpszMenuName = NULL;
		wcx.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcx.hIcon = AppIcon;
		wcx.hIconSm = AppIcon;
		wcx.lpszClassName = ClassName;

		RegisterClassEx(&wcx);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		Direct2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		// 创建主窗口
		hWnd = CreateWindowEx(
			0,                              // 扩展窗口样式，见 https://msdn.microsoft.com/zh-CN/library/windows/desktop/ff700543.aspx
			ClassName,                     // Window class
			WindowTitle,                    // 窗口标题
			WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,            // 窗口样式，见 https://msdn.microsoft.com/zh-CN/library/windows/desktop/ms632600.aspx

			// 窗口大小和定位
			CW_USEDEFAULT, // 横坐标
			CW_USEDEFAULT, // 纵坐标
			static_cast<UINT>(ceil(width * dpiX / 96.f)), // 窗口宽度
			static_cast<UINT>(ceil(height * dpiY / 96.f)), // 窗口高度

			NULL,       // 父窗口句柄
			NULL,       // 菜单句柄
			hInstance,  // 实例句柄
			this        // Additional application data
		);

		hr = hWnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr)) {
			AppBackground = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND));

			closeBtn.initialize(hWnd, L"r", 770, 0, CloseBtnCallback, ICButton::DEFAULT_WIDTH, ICButton::DEFAULT_HEIGHT, ICButton::DEFAULT_FONTSIZE, L"Webdings", CLOSEBTN_NORMAL_FG, CLOSEBTN_NORMAL_BG, CLOSEBTN_HOVERED_FG, CLOSEBTN_HOVERED_BG, CLOSEBTN_PUSHED_FG, CLOSEBTN_PUSHED_BG);
			minimizeBtn.initialize(hWnd, L"0", 740, 0, MinimizeBtnCallback, ICButton::DEFAULT_WIDTH, ICButton::DEFAULT_HEIGHT, ICButton::DEFAULT_FONTSIZE, L"Webdings");
			textInputField.initialize(hWnd, 495, 448, TextInputCallback, 295, 43);
			canvasInstance.initialize(hWnd, 13, 39, 448, 448);

			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
		}
	}

	return hr;
}

const HWND MainWindow::getHwnd() {
	return instance->hWnd;
}

ID2D1Factory* MainWindow::getD2DFactory() {
	return instance->Direct2dFactory;
}

ID2D1HwndRenderTarget* MainWindow::getRenderTarget() {
	return instance->RenderTarget;
}

HRESULT MainWindow::CreateDeviceIndependentResources() {
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Direct2dFactory);

	return hr;
}

HRESULT MainWindow::CreateDeviceResources() {
	HRESULT hr = S_OK;

	if (!RenderTarget) {
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = Direct2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&RenderTarget
		);
	}

	return hr;
}

void MainWindow::DiscardDeviceResources() {
	SafeRelease(&RenderTarget);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	bool wasHandled = false;

	if (instance) {
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		result = 1;
		wasHandled = true;
		break;

		case WM_NCCALCSIZE:
		{
			//this kills the window frame and title bar we added with
			//WS_THICKFRAME and WS_CAPTION
		}
		result = 0;
		wasHandled = true;
		break;

		case WM_DRAWITEM:
		{
			if (ElementsManager::getTypeByID(wParam) == ElementsManager::ETYPES::BUTTON) {
				LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
				ICButton::drawButton(pDIS);
			}
			/*else if (ElementsManager::getTypeByID(wParam) == ElementsManager::ETYPES::CANVAS) {
				LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
				Canvas::PaintProc(pDIS->hwndItem);
			}*/
		}
		break;

		case WM_COMMAND:
		{
			HWND winobj = (HWND)lParam;
			if (TextInput::isTextInput(winobj) > -1) {
				return CallWindowProc(TextInput::InputWndProc, winobj, uMsg, wParam, lParam);
			}
			else if (ICButton::isICButton(winobj) > -1) {
				return CallWindowProc(ICButton::ICBWndProc, winobj, uMsg, wParam, lParam);
			}
		}
		break;

		case WM_MOUSEMOVE:
		case WM_MOUSELEAVE:
		case WM_MOUSEHOVER:
		case WM_NCMOUSEHOVER:
		{
			//ICButton::mouseLeave();
		}
		break;

		case WM_PAINT:
		{
			instance->OnRender();
			//ValidateRect(hwnd, NULL);
		}
		result = 1;
		wasHandled = true;
		break;

		case WM_LBUTTONDOWN:
		{
			SetFocus(hwnd);
			SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
		result = 0;
		wasHandled = true;
		break;

		}
	}

	if (!wasHandled) {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

HRESULT MainWindow::OnRender() {
	HRESULT hr = S_OK;

	PAINTSTRUCT ps;
	RECT        rect;
	HDC hDC = BeginPaint(hWnd, &ps);
	FillRect(hDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	// 背景
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC(hDC);
	SelectObject(hdcMem, AppBackground);
	GetObject(AppBackground, sizeof(bm), &bm);
	BitBlt(hDC, 0, 26, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);

	// 窗口图标
	DrawIconEx(hDC, 3, 3, AppIcon, 18, 18, 0, NULL, DI_NORMAL | DI_COMPAT);
	// 窗口标题栏
	HFONT font = CreateFont(21, 0, 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, ICButton::DEFAULT_FONT);
	HFONT hFontOld = (HFONT)SelectObject(hDC, font);
	GetClientRect(hWnd, &rect);
	rect.left = rect.bottom = 26;
	DrawText(hDC, WindowTitle, -1, &rect, DT_SINGLELINE | DT_VCENTER);
	SelectObject(hDC, hFontOld);
	EndPaint(hWnd, &ps);

	return hr;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL))) {
		MainWindow app;

		if (SUCCEEDED(app.Initialize())) {
			app.RunMessageLoop();
		}
		CoUninitialize();
	}

	return 0;
}

