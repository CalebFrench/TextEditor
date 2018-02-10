#pragma once
#include "Window.h"
#include <Windows.h>
#include <stdexcept>
#undef DrawText


class Win32WindowImpl : public WindowImpl {
	static LRESULT _stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		LRESULT result = 0;

		Win32WindowImpl* that = nullptr;
		if (msg == WM_CREATE) {
			that = (Win32WindowImpl*)((CREATESTRUCTA*)lparam)->lpCreateParams;
			SetWindowLongA(hwnd, GWLP_USERDATA, (LONG)that);
			return 0;
		}

		that = (Win32WindowImpl*)GetWindowLongA(hwnd, GWLP_USERDATA);
		if (that != nullptr) {
			that->Proc(msg, wparam, lparam);
		} else {
			result = DefWindowProcA(hwnd, msg, wparam, lparam);
		}

		return result;
	}

public:
	Win32WindowImpl(const char* const title)
		: _hwnd(NULL), _close(false), _dwStyle(WS_OVERLAPPEDWINDOW), _dwExStyle(NULL),
		_hdc(NULL), _hdc1(NULL), _fgb(NULL), _bgb(NULL), _hfont(NULL)
	{
		WNDCLASSEXA wcex = { 0 };
		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = GetModuleHandleA(NULL);
		wcex.lpfnWndProc = WndProc;
		wcex.lpszClassName = "Win32WindowImpl";
		wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		
		if (GetClassInfoExA(wcex.hInstance, wcex.lpszClassName, &wcex) == FALSE) {
			if (RegisterClassExA(&wcex) == TRUE) {
				throw std::runtime_error("unable to register window class");
			}
		}

		Create(title);
		SetFont("Consolas");
	}

	virtual ~Win32WindowImpl() {
		Destroy();
	}

	virtual bool Process() {
		MSG msg = { 0 };
		if (PeekMessageA(&msg, _hwnd, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return !_close;
	}

	virtual bool Present() {
		BOOL b = BitBlt(_hdc1, 0, 0, _w, _h, _hdc, 0, 0, SRCCOPY);
		return b;
	}

	virtual void GetRect(Rect& rect) {
		RECT rc = { 0 };
		GetClientRect(_hwnd, &rc);

		_w = rc.right - rc.left;
		_h = rc.bottom - rc.top;

		rect.pos.x = rc.left;
		rect.pos.y = rc.top;
		rect.size.w = _w;
		rect.size.h = _h;
	}

	virtual void SetRect(const Rect& rect) {
		RECT rc = { 0 };
		rc.left   = (int)rect.pos.x;
		rc.top    = (int)rect.pos.y;
		rc.right  = (int)(rect.pos.x + rect.size.w);
		rc.bottom = (int)(rect.pos.y + rect.size.h);

		AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, NULL);
		_w = rc.right - rc.left;
		_h = rc.bottom - rc.top;

		SetWindowPos(_hwnd, NULL,
			rc.left, rc.top, _w, _h,
			NULL);
	}

	virtual void SetFont(const char* const font) {
		_hfont = CreateFontA(
			14, 0, 0, 0,
			FW_DONTCARE,	// weight
			FALSE,			// italic
			FALSE,			// underline
			FALSE,			// strike
			DEFAULT_CHARSET,// charset
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY, // antialiasing
			VARIABLE_PITCH, font);
	}

	virtual void SetColor(const RGBA& rgba) {
		_fg = RGB(rgba.r * 255, rgba.g * 255, rgba.b * 255);
		if (_fgb != NULL) DeleteObject(_fgb);
		_fgb = CreateSolidBrush(_fg);
	}

	virtual void SetClearColor(const RGBA& rgba) {
		_bg = RGB(rgba.r*255, rgba.g*255, rgba.b*255);
		if (_bgb != NULL) DeleteObject(_bgb);
		_bgb = CreateSolidBrush(_bg);
	}

	virtual void DrawLine(const Point& beg, const Point& end, const float width) {
		HPEN hpen = CreatePen(NULL, (int)width, _fg);
		HPEN old = (HPEN)SelectObject(_hdc, hpen);
		
		MoveToEx(_hdc, (int)beg.x, (int)beg.y, NULL);
		LineTo(_hdc, (int)end.x, (int)end.y);
		
		SelectObject(_hdc, old);
		DeleteObject(hpen);
	}

	virtual void DrawRect(const Rect& rect, const float width) {
		HPEN hpen = CreatePen(NULL, (int)width, _fg);
		HPEN old = (HPEN)SelectObject(_hdc, hpen);

		float hw = width / 2;
		int x1 = int(rect.pos.x - hw), x2 = int(rect.pos.x + rect.size.w + hw);
		int y1 = int(rect.pos.y - hw), y2 = int(rect.pos.y + rect.size.h + hw);
		
		MoveToEx(_hdc, x1, y1, NULL);
		LineTo(_hdc, x1, y2);
		LineTo(_hdc, x2, y2);
		LineTo(_hdc, x2, y1);
		LineTo(_hdc, x1, y1);

		SelectObject(_hdc, old);
		DeleteObject(hpen);
	}

	virtual void DrawChar(const BoundingBox& box, const int c) {
		RECT rc = { 0 };
		rc.left = (LONG)box.l;
		rc.top = (LONG)box.t;
		rc.right = (LONG)box.r;
		rc.bottom = (LONG)box.b;

		SetTextColor(_hdc, _fg);
		SetBkColor(_hdc, _bg);
		SelectObject(_hdc, _hfont);

		DrawTextA(_hdc, (char*)&c, 1, &rc, DT_NOPREFIX);
	}

	virtual void DrawText(const BoundingBox& box, const char* const text) {
		RECT rc = { 0 };
		rc.left = (LONG)box.l;
		rc.top = (LONG)box.t;
		rc.right = (LONG)box.r;
		rc.bottom = (LONG)box.b;

		SetTextColor(_hdc, _fg);
		SetBkColor(_hdc, _bg);
		SelectObject(_hdc, _hfont);

		int i = DrawTextA(_hdc, text, strlen(text), &rc, DT_NOPREFIX);
	}

	virtual void FillRect(const Rect& rect) {
		RECT rc = { 0 };
		rc.left = (LONG)rect.pos.x;
		rc.top = (LONG)rect.pos.y;
		rc.right = (LONG)(rect.pos.x + rect.size.w);
		rc.bottom = (LONG)(rect.pos.y + rect.size.h);

		::FillRect(_hdc, &rc, _fgb);
	}

	virtual Size CharBounds(int c) {
		bool tab = c == '\t';
		bool nl = c == '\n';
		if (tab) c = ' ';
		if (nl)
			c = ' ';

		RECT rc = { 0 };
		SelectObject(_hdc, _hfont);
		DrawTextA(_hdc, (char*)&c, 1, &rc, DT_CALCRECT);

		int height = rc.bottom - rc.top;
		int width = rc.right - rc.left;
		width = tab ? width * 4 : width;
		width = nl ? 0 : width;
		return { (float)width, (float)height };
	}

	virtual Size TextBounds(const char* const text) {
		RECT rc = { 0 };
		DrawTextA(_hdc, text, strlen(text), &rc, DT_CALCRECT);

		return {
			(float)(rc.right - rc.left),
			(float)(rc.bottom - rc.top)
		};
	}

protected:
	virtual void Destroy() {
		if (_hdc) {
			DeleteDC(_hdc);
			_hdc = NULL;
		}

		if (_hwnd) {
			if (_hdc1) {
				ReleaseDC(_hwnd, _hdc1);
				_hdc1 = NULL;
			}
			DeleteObject(_hfont);
			DestroyWindow(_hwnd);
			_hwnd = NULL;

			_close = true;
		}
	}

	virtual void Create(const char* const title) {
		Destroy();
		_close = false;
		_hwnd = CreateWindowExA(
			_dwExStyle, "Win32WindowImpl",
			title, _dwStyle,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, GetModuleHandle(NULL), this);

		if (!_hwnd) {
			throw std::runtime_error("unable to create window");
		}

		_hdc1 = GetDC(_hwnd);
		if (!_hdc1) {
			Destroy();
			throw std::runtime_error("unable to get window device context");
		}

		_hdc = CreateCompatibleDC(_hdc1);
		if (!_hdc) {
			Destroy();
			throw std::runtime_error("unable to create memory device context");
		}

		UpdateWindow(_hwnd);
		ShowWindow(_hwnd, SW_SHOW);
	}

	virtual LRESULT Proc(UINT msg, WPARAM wparam, LPARAM lparam) {
		LRESULT result = 0;

		switch (msg) {
			case WM_CLOSE:
				Destroy();
				break;
			case WM_PAINT:
				break;
			case WM_ERASEBKGND:
				result = 1;
				break;
			case WM_SIZE:
					_w = LOWORD(lparam);
					_h = HIWORD(lparam);
					on_size_(_w, _h);
				break;
			default:
				result = DefWindowProcA(_hwnd, msg, wparam, lparam);
				break;
		}

		return result;
	}

	bool _close;
	HWND _hwnd;
	HDC _hdc, _hdc1;
	HFONT _hfont;
	COLORREF _fg, _bg;
	HBRUSH _fgb, _bgb;
	DWORD _dwStyle;
	DWORD _dwExStyle;
	int _w, _h;
};
