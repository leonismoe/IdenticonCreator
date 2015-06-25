#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <windows.h>
//#include <windowsx.h>
#include <d2d1.h>
//#include <dwmapi.h>
#pragma comment (lib, "d2d1.lib")


template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	static const LPWSTR ClassName;
	static const LPWSTR WindowTitle;

	static const int width;
	static const int height;
	static float dpiX, dpiY;

	// The windows procedure.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static const HWND getHwnd();
	static ID2D1Factory* getD2DFactory();
	static ID2D1HwndRenderTarget* getRenderTarget();

	HRESULT Initialize(HINSTANCE hInstance = NULL);
	void RunMessageLoop();

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	HRESULT OnRender();

	HWND hWnd = NULL;
	ID2D1Factory* Direct2dFactory = nullptr;
	ID2D1HwndRenderTarget* RenderTarget = nullptr;

	static MainWindow *instance;
	HICON AppIcon;
	HBITMAP AppBackground;

};

#endif
