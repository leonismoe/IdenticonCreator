#include "Identicon.h"
#include "MainWindow.h"
#include "Canvas.h"
#include "md5.h"
#include <ctime>

bool Identicon::isDraw = false;
int Identicon::cell = 112.f;
Canvas* Identicon::canvas = nullptr;
std::string Identicon::curmd5 = MD5(std::to_string(time(NULL))).hexdigest();

void Identicon::init(HWND hWnd) {
	canvas = new Canvas;
	canvas->initialize(hWnd, 13, 39, 448, 448);
}

void Identicon::create(LPSTR text) {
	draw(MD5(text).hexdigest());
}
void Identicon::create(LPWSTR text) {
	draw(MD5(text).hexdigest());
}
void Identicon::create(LPCSTR text) {
	draw(MD5(text).hexdigest());
}
void Identicon::create(LPCWSTR text) {
	draw(MD5(text).hexdigest());
}
void Identicon::create(std::string text) {
	draw(MD5(text).hexdigest());
}
void Identicon::create(std::wstring text) {
	draw(MD5(text).hexdigest());
}

void Identicon::draw() {
	draw(curmd5);
}

void Identicon::draw(const std::string md5) {
	canvas->clear();
	curmd5 = md5;
	double hue = std::stoi(&curmd5[25], nullptr, 16) / (double)0xfffffff;
	UINT AVAILABLE_COLORS[] = {
		0x4C4C4C, // 深灰
		correctedHsl(hue, 0.5, 0.6), // 中间色 (运行时确定)
		0xE6E6E6, // 亮灰
		correctedHsl(hue, 0.5, 0.8), // 亮色 (运行时确定)
		hsl(hue, 0.5, 0.4)  // 深色 (运行时确定)
	};
	std::vector<UINT> selectedColors;
	selectedColors.reserve(3);

	for (int i = 0; i < 3; ++i) {
		int colorIndex = hex2int(&md5[i + 8]) % 3;
		if ((colorIndex == 0 || colorIndex == 4) && (inVector(0, selectedColors) || inVector(4, selectedColors))
			|| (colorIndex == 2 || colorIndex == 3) && (inVector(2, selectedColors) || inVector(3, selectedColors))) {
			colorIndex = 1;
		}
		selectedColors.push_back(AVAILABLE_COLORS[colorIndex]);
	}

	int sideShape = hex2int(&md5[2]);
	int cornerShape = hex2int(&md5[4]);
	int centerShape = hex2int(&md5[1]);

	OUTER_SHAPES[sideShape % OUTER_SHAPES_LENGTH](1, 0, 0, selectedColors[0], cell, canvas);
	OUTER_SHAPES[sideShape % OUTER_SHAPES_LENGTH](0, 1, 0, selectedColors[0], cell, canvas);
	OUTER_SHAPES[cornerShape % OUTER_SHAPES_LENGTH](0, 0, 0, selectedColors[1], cell, canvas);
	CENTER_SHAPES[centerShape % CENTER_SHAPES_LENGTH](1, 1, 0, selectedColors[2], cell, canvas);
	CENTER_SHAPES[centerShape % CENTER_SHAPES_LENGTH](1, 1, 1, selectedColors[2], cell, canvas);
	CENTER_SHAPES[centerShape % CENTER_SHAPES_LENGTH](1, 1, 2, selectedColors[2], cell, canvas);
	CENTER_SHAPES[centerShape % CENTER_SHAPES_LENGTH](1, 1, 3, selectedColors[2], cell, canvas);
}

int Identicon::hex2int(const char *str, int len) {
	char *tmp = new char[len + 1];
	for (int i = 0; i < len; ++i) tmp[i] = str[i];
	tmp[len] = '\0';
	int result = std::stoi(tmp, nullptr, 16);
	delete[] tmp;
	return result;
}

inline double Identicon::hue2rgb(double p, double q, double h) {
	if (h < 0.0) h += 1.0;
	if (h > 1.0) h -= 1.0;
	if (6.0*h < 1.0) return p + 6.0*(q - p)*h;
	if (2.0*h < 1.0) return q;
	if (3.0*h < 2.0) return p + 6.0*(q - p)*(2.0 / 3.0 - h);
	return p;
}
UINT Identicon::hsl(double h, double s, double l) {
	UINT color = 0;
	double q = l < 0.5 ? l*(s + 1.0) : l + s - l*s;
	if (s == 0 || q <= 0) {
		int t = (int)(255.0 * l) & 0xff;
		color = (t << 4) | (t << 2) | t;
		return color;
	}
	double p = 2.0 * l - q;
	double hk = h / 360;
	color |= ((int)(255.0*hue2rgb(p, q, h + 1.0 / 3.0)) & 0xff) << 16;
	color |= ((int)(255.0*hue2rgb(p, q, h)) & 0xff) << 8;
	color |= (int)(255.0*hue2rgb(p, q, h - 1.0 / 3.0)) & 0xff;
	return color;
}
UINT Identicon::correctedHsl(double h, double s, double l) {
	static double correctors[] = { 0.95, 1, 1, 1, 0.7, 0.8, 0.8 };
	return hsl(h, s, 1 - correctors[(int)(h * 6 + 0.5)] * (1 - l));
}


const int Identicon::CENTER_SHAPES_LENGTH = 13;
const int Identicon::OUTER_SHAPES_LENGTH = 4;
const IDRAWING_FUNCPTR Identicon::CENTER_SHAPES[] = {
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float k = cell * .42f;
			pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, 0));
			pSink->AddLine(D2D1::Point2F(cell, cell - 2*k));
			pSink->AddLine(D2D1::Point2F(cell - k, cell));
			pSink->AddLine(D2D1::Point2F(0, cell));
			pSink->AddLine(D2D1::Point2F(0, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float w = cell * .5f;
			float h = cell * .8f;
			pSink->BeginFigure(D2D1::Point2F(w, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, 0));
			pSink->AddLine(D2D1::Point2F(cell, h));
			pSink->AddLine(D2D1::Point2F(w, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_DRAW_BEGIN
			RenderTarget->FillRectangle(D2D1::RectF(
				cell / 3.f,
				cell / 3.f,
				cell,
				cell
			), brush);
		IDRAWING_DRAW_END
	},
	IDRAWING_FUNC {
		IDRAWING_DRAW_BEGIN
			float inner = cell * .1f;
			float outer = cell * .25f;
			RenderTarget->FillRectangle(D2D1::RectF(
				outer,
				outer,
				cell - inner,
				cell - inner
			), brush);
		IDRAWING_DRAW_END
	},
	IDRAWING_FUNC {
		IDRAWING_DRAW_BEGIN
			RenderTarget->FillEllipse(D2D1::Ellipse(
				D2D1::Point2F(cell * .55f, cell * .55f),
				cell * .25f,
				cell * .25f
			), brush);
			RenderTarget->EndDraw();
		IDRAWING_DRAW_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float inner = cell * .1f;
			float outer = inner * 4;
			pSink->BeginFigure(D2D1::Point2F(outer, outer), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell - inner, outer));
			pSink->AddLine(D2D1::Point2F(outer + (cell - outer - inner) / 2.f, cell - inner));
			pSink->AddLine(D2D1::Point2F(outer, outer));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
			IDRAWING_DRAW_FILL
			IDRAWING_DRAW_COLORRESET
			IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float inner = cell * .1f;
			float outer = inner * 4;
			pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, 0));
			pSink->AddLine(D2D1::Point2F(cell, cell * 0.7f));
			pSink->AddLine(D2D1::Point2F(cell * 0.4f, cell * 0.4f));
			pSink->AddLine(D2D1::Point2F(cell * 0.7f, cell));
			pSink->AddLine(D2D1::Point2F(0, cell));
			pSink->AddLine(D2D1::Point2F(0, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float inner = cell * .1f;
			float outer = inner * 4;
			pSink->BeginFigure(D2D1::Point2F(cell, cell * .5f), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, cell));
			pSink->AddLine(D2D1::Point2F(cell * .5f, cell));
			pSink->AddLine(D2D1::Point2F(cell, cell * .5f));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float inner = cell * .1f;
			float outer = inner * 4;
			pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, 0));
			pSink->AddLine(D2D1::Point2F(cell, cell * .5f));
			pSink->AddLine(D2D1::Point2F(cell * .5f, cell));
			pSink->AddLine(D2D1::Point2F(0, cell));
			pSink->AddLine(D2D1::Point2F(0, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_DRAW_BEGIN
			IDRAWING_DRAW_FILL
			IDRAWING_DRAW_COLORRESET
			float inner = cell * 0.14f;
			float outer = cell * 0.35f;
			RenderTarget->FillRectangle(D2D1::RectF(
				outer,
				outer,
				cell - inner,
				cell - inner
			), brush);
		IDRAWING_DRAW_END
	},
	IDRAWING_FUNC {
		IDRAWING_DRAW_BEGIN
			IDRAWING_DRAW_FILL
			IDRAWING_DRAW_COLORRESET
			float inner = cell * 0.12f;
			float outer = cell * 0.3f;
			RenderTarget->FillEllipse(D2D1::Ellipse(
				D2D1::Point2F((cell - inner + outer) / 2.f, (cell - inner + outer) / 2.f),
				(cell - inner - outer) / 2.f,
				(cell - inner - outer) / 2.f
			), brush);
		IDRAWING_DRAW_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			float m = cell * .36f;
			pSink->BeginFigure(D2D1::Point2F(cell - m, cell - 2 * m), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, cell - m));
			pSink->AddLine(D2D1::Point2F(cell - m, cell));
			pSink->AddLine(D2D1::Point2F(cell - 2 * m, cell - m));
			pSink->AddLine(D2D1::Point2F(cell - m, cell - 2 * m));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
			IDRAWING_DRAW_FILL
			IDRAWING_DRAW_COLORRESET
			IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			pSink->BeginFigure(D2D1::Point2F(cell * .4f, cell), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddArc(
				D2D1::ArcSegment(
				D2D1::Point2F(cell, cell * .4f), // end point
				D2D1::SizeF(cell * .6f, cell * .6f),
				0.0f, // rotation angle
				D2D1_SWEEP_DIRECTION_CLOCKWISE,
				D2D1_ARC_SIZE_SMALL
			));
			pSink->AddLine(D2D1::Point2F(cell, cell));
			pSink->AddLine(D2D1::Point2F(cell * .4f, cell));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	}
};

const IDRAWING_FUNCPTR Identicon::OUTER_SHAPES[] = {
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, 0));
			pSink->AddLine(D2D1::Point2F(cell, cell));
			pSink->AddLine(D2D1::Point2F(0, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, 0));
			pSink->AddLine(D2D1::Point2F(cell, cell / 2.f));
			pSink->AddLine(D2D1::Point2F(0, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_PATH_BEGIN
			pSink->BeginFigure(D2D1::Point2F(cell / 2.f, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(cell, cell / 2.f));
			pSink->AddLine(D2D1::Point2F(cell / 2.f, cell));
			pSink->AddLine(D2D1::Point2F(0, cell / 2.f));
			pSink->AddLine(D2D1::Point2F(cell / 2.f, 0));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		IDRAWING_PATH_CLOSE

		IDRAWING_DRAW_BEGIN
		IDRAWING_PATH_DRAW
		IDRAWING_DRAW_END
		IDRAWING_PATH_END
	},
	IDRAWING_FUNC {
		IDRAWING_DRAW_BEGIN
			RenderTarget->FillEllipse(D2D1::Ellipse(
				D2D1::Point2F(cell / 2.f, cell / 2.f),
				cell / 3.f,
				cell / 3.f
			), brush);
		IDRAWING_DRAW_END
	}
};
