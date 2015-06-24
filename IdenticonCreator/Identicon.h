#ifndef _IDENTICON_H
#define _IDENTICON_H

#include <string>
#include <Windows.h>
#include <d2d1.h>

#ifndef inVector
#define inVector(value, vector) (std::find(vector.begin(), vector.end(), value) != vector.end())
#endif
#define IDRAWING_FUNC [](int x, int y, int rotate, UINT32 color, float cell)
typedef void(*IDRAWING_FUNCPTR)(int x, int y, int rotate, UINT32 color, float cell);

class Identicon {
	
public:
	static const int CENTER_SHAPES_LENGTH;
	static const int OUTER_SHAPES_LENGTH;
	static const IDRAWING_FUNCPTR CENTER_SHAPES[];
	static const IDRAWING_FUNCPTR OUTER_SHAPES[];

	static void create(LPSTR text);
	static void create(LPWSTR text);
	static void create(LPCSTR text);
	static void create(LPCWSTR text);
	static void create(std::string text);
	static void create(std::wstring text);

	static void draw();
	static void draw(std::string md5);

	static int hex2int(const char *str, int len = 1);

	static UINT hsl(double hue, double saturation, double lightness);
	static UINT correctedHsl(double h, double s, double l);

protected:
	static bool isDraw;
	static int cell;
	static std::string curmd5;

	static inline double hue2rgb(double p, double q, double h);


};


#define IDRAWING_PATH_BEGIN \
	ID2D1PathGeometry *PathGeometry;\
	ID2D1GeometrySink *pSink = NULL;\
	MainWindow::getD2DFactory()->CreatePathGeometry(&PathGeometry);\
	PathGeometry->Open(&pSink);

#define IDRAWING_PATH_CLOSE \
	pSink->Close();

#define IDRAWING_PATH_DRAW \
	RenderTarget->FillGeometry(PathGeometry, brush);

#define IDRAWING_PATH_END \
	SafeRelease(&pSink);\
	SafeRelease(&PathGeometry);

#define IDRAWING_DRAW_BEGIN \
	ID2D1SolidColorBrush *brush;\
	ID2D1RenderTarget *RenderTarget = Canvas::getRenderTarget(0);\
	RenderTarget->CreateSolidColorBrush(\
		D2D1::ColorF(color),\
		&brush\
	);\
	RenderTarget->BeginDraw();\
	D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(x * cell, y * cell);\
	for(; rotate >= 0 && rotate < 4; ++rotate) {\
	D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(rotate * 90.0f, D2D1::Point2F(cell * 2.f, cell * 2.f));\
	RenderTarget->SetTransform(translation * rotation);

#define IDRAWING_DRAW_FILL \
	RenderTarget->FillRectangle(D2D1::RectF(\
		0.f,\
		0.f,\
		cell + 0.f,\
		cell + 0.f\
	), brush);

#define IDRAWING_DRAW_COLORRESET \
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::White, 1.f));

#define IDRAWING_DRAW_END \
	}\
	SafeRelease(&brush);\
	if (RenderTarget->EndDraw() == D2DERR_RECREATE_TARGET) {\
		SafeRelease(&RenderTarget);\
	}

#endif