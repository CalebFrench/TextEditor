#pragma once


struct RGBA { float r, g, b, a;  };
struct Point2f { float x, y; };
struct Size2f { float w, h; };
struct BoundingBox2f { float l, t, r, b; };
struct Rect2f { Point2f pos; Size2f size; };

typedef Point2f Point;
typedef Size2f Size;
typedef BoundingBox2f BoundingBox;
typedef Rect2f Rect;

class WindowImpl {
public:
	virtual ~WindowImpl() {}

	virtual bool Process() = 0;
	virtual void Present() = 0;

	virtual void SetRect(const Rect& rect) = 0;
	virtual void SetFont(const char* const font) = 0;
	virtual void SetColor(const RGBA& rgba) = 0;
	virtual void SetClearColor(const RGBA& rgba) = 0;

	virtual void DrawLine(const Point& beg, const Point& end, const float width) = 0;
	virtual void DrawRect(const Rect& rect, const float width) = 0;
	virtual void DrawChar(const BoundingBox& box, const int c) = 0;
	virtual void DrawText(const BoundingBox& box, const char* const text) = 0;
	virtual void FillRect(const Rect& rect) = 0;

	virtual Size CharBounds(const int c) = 0;
	virtual Size TextBounds(const char* const text) = 0;
};

class Window {
public:
	Window(const char* const title, float width=800.0f, float height=450.0f);
	~Window();

	bool Process();
	void Clear();
	void Present();

	void GetRect(Rect& rect);
	void GetFont(const char* font);
	void GetColor(RGBA& rgba);
	void GetClearColor(RGBA& rgba);
	void SetRect(const Rect& rect);
	void SetFont(const char* const font);
	void SetColor(const RGBA& rgba);
	void SetClearColor(const RGBA& rgba) ;

	void DrawLine(const Point& beg, const Point& end, const float width);
	void DrawRect(const Rect& rect, const float width);
	void DrawChar(const BoundingBox& box, const int c);
	void DrawText(const BoundingBox& box, const char* const text);
	void FillRect(const Rect& rect);

	virtual Size CharBounds(const int c);
	virtual Size TextBounds(const char* const text);

private:
	WindowImpl* _impl;
	Rect _wnd_rect;
	char* _font;
	RGBA _fg, _bg;
};
