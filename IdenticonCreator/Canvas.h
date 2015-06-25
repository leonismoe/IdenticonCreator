#ifndef _CANVAS_H
#define _CANVAS_H

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment (lib, "d2d1.lib")

class Canvas {

public:
	const static LPCWSTR CanvasClassName;
	ID2D1HwndRenderTarget* RenderTarget = nullptr;

	Canvas();
	Canvas(HWND pwnd, int x, int y, int width, int height);
	~Canvas();
	void initialize(HWND pwnd, int x, int y, int width, int height);

	void registerCanvas();
	void unregisterCanvas();
	void initImage();
	void releaseImage();
	void saveImage();
	static LRESULT CALLBACK canvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT initRenderTarget();
	HRESULT clear();
	void releaseRenderTarget();


protected:
	int x;
	int y;
	int width;
	int height;
	int cvsID;

	HWND hWnd;
	RECT CanvasRC;
	D2D1_SIZE_U CanvasSize;

};

#endif