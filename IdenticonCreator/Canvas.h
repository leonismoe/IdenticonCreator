#ifndef _CANVAS_H
#define _CANVAS_H

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment (lib, "d2d1.lib")

typedef struct {
	int x;
	int y;
} Points;

class Canvas {

public:
	const static LPCWSTR CanvasClassName;

	Canvas();
	Canvas(HWND pwnd, int x, int y, int width, int height);
	void initialize(HWND pwnd, int x, int y, int width, int height);
	static int indexOf(HWND &hWnd);
	static ID2D1HwndRenderTarget* Canvas::getRenderTarget(int index);

	void registerCanvas();
	void unregisterCanvas();
	static void PaintProc(HWND hwnd);
	static LRESULT CALLBACK canvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void beginPaint();
	void endPaint();
	void addPolygon(std::vector<Points> points, bool invert = false);

	HRESULT initRenderTarget();
	static HRESULT clearCanvas(int index);
	void releaseRenderTarget();


protected:
	int x;
	int y;
	int width;
	int height;
	int cvsID;

	HWND hWnd;
	RECT CanvasRC;
	ID2D1HwndRenderTarget* RenderTarget = nullptr;

	static std::vector<Canvas*> CvsInstances;
	static std::vector<HWND> CvsHWNDs;

};

#endif