#ifndef _IDENTICON_H
#define _IDENTICON_H

#include <string>
#include <Windows.h>
#include <d2d1.h>
#include "Canvas.h"

// 判断 value 是否在 vector 容器中
#ifndef inVector
#define inVector(value, vector) (std::find(vector.begin(), vector.end(), value) != vector.end())
#endif

// 基本图形画图函数及其指针定义
// index: 绘图小块的索引   rotate: 初始旋转角   color: 绘制颜色   cell: 小块边长   canvas: 画布示例
#define IDRAWING_FUNC [](int index, int rotate, UINT32 color, float cell, Canvas* canvas)
typedef void(*IDRAWING_FUNCPTR)(int index, int rotate, UINT32 color, float cell, Canvas* canvas);

class Identicon {

public:
	// 中心、边缘基本图形画图函数的数组及其长度
	static const int CENTER_SHAPES_LENGTH;
	static const int OUTER_SHAPES_LENGTH;
	static const IDRAWING_FUNCPTR CENTER_SHAPES[];
	static const IDRAWING_FUNCPTR OUTER_SHAPES[];

	// Identicon 对象的初始化
	static void init(HWND hWnd);

	// 输入数据，计算 MD5 后生成 identicon 图形
	static void create(LPSTR text);
	static void create(LPWSTR text);
	static void create(LPCSTR text);
	static void create(LPCWSTR text);
	static void create(std::string text);
	static void create(std::wstring text);

	// 生成已缓存的 md5 的 identicon 图形
	static void draw();
	// 生成指定 md5 值的 identicon 图形
	static void draw(std::string md5);

	// 将 string 类型的 16 进制数值转换为 int
	// str: 待转换字符串   len: 字符串长度(不能超过 7，否则会溢出)
	static int hex2int(const char *str, int len = 1);

	// 将 HSL 色域的颜色转换为 unsigned int 表示的 RGB 颜色 (0x00rrggbb)
	static UINT hsl(double hue, double saturation, double lightness);
	static UINT correctedHsl(double h, double s, double l);

protected:
	static int cell; // 一小块的边长
	static std::string curmd5; // 缓存的 md5
	static Canvas *canvas; // Canvas 实例

	// hsl 的内部中间方法
	static inline double hue2rgb(double p, double q, double h);

};


// 初始化 D2D 路径
#define IDRAWING_PATH_BEGIN \
	ID2D1PathGeometry *PathGeometry = nullptr;\
	ID2D1GeometrySink *pSink = NULL;\
	MainWindow::getD2DFactory()->CreatePathGeometry(&PathGeometry);\
	PathGeometry->Open(&pSink);

// 闭合 D2D 路径
#define IDRAWING_PATH_CLOSE \
	pSink->Close();

// 绘制路径所表示的闭合图形
#define IDRAWING_PATH_DRAW \
	RenderTarget->FillGeometry(PathGeometry, brush);

// 释放 D2D 路径
#define IDRAWING_PATH_END \
	SafeRelease(&pSink);\
	SafeRelease(&PathGeometry);\

// 开始画图
#define IDRAWING_DRAW_BEGIN \
	ID2D1SolidColorBrush *brush;\
	ID2D1RenderTarget *RenderTarget = canvas->RenderTarget;\
	RenderTarget->CreateSolidColorBrush(\
		D2D1::ColorF(color),\
		&brush\
	);\
	RenderTarget->BeginDraw();\
	index = index % 4;\
	rotate = rotate % 4;\
	int x = index % 2;\
	int y = index > 1 ? 1 : 0;\
	D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(x * cell, y * cell);\
	D2D1_MATRIX_3X2_F cellrotate = x && y ? D2D1::Matrix3x2F::Identity() : D2D1::Matrix3x2F::Rotation(rotate * 90.0f, D2D1::Point2F(cell * .5f, cell * .5f));\
	int _totalr = x ^ y ? 8 : (x ? 1 : 4);\
	for(int _rotate = 0; _rotate < _totalr; ++_rotate) {\
		if(_rotate == 4) translation = D2D1::Matrix3x2F::Translation(y * cell, x * cell);\
		if(x && y) _rotate = rotate;\
		D2D1_MATRIX_3X2_F _rotation = D2D1::Matrix3x2F::Rotation(_rotate * 90.0f, D2D1::Point2F(cell * 2.f, cell * 2.f));\
		RenderTarget->SetTransform(cellrotate * translation * _rotation);

// 用画笔 brush 填充整个小块
#define IDRAWING_DRAW_FILL \
	RenderTarget->FillRectangle(D2D1::RectF(\
		0.f,\
		0.f,\
		cell + 0.f,\
		cell + 0.f\
	), brush);

// 设置画笔 brush 的颜色为白色
#define IDRAWING_DRAW_COLORRESET \
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::White, 1.f));

// 绘图结束
#define IDRAWING_DRAW_END \
	}\
	SafeRelease(&brush);\
	if (RenderTarget->EndDraw() == D2DERR_RECREATE_TARGET) {\
		SafeRelease(&RenderTarget);\
	}

#endif