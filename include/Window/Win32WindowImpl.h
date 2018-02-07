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
		: _hwnd(NULL)
	{
		WNDCLASSEXA wcex = { 0 };
		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = GetModuleHandleA(NULL);
		wcex.lpfnWndProc = WndProc;
		wcex.lpszClassName = "Win32WindowImpl";

		if (GetClassInfoExA(wcex.hInstance, wcex.lpszClassName, &wcex) == FALSE) {
			if (RegisterClassExA(&wcex) == TRUE) {
				throw std::runtime_error("unable to register window class");
			}
		}

		Create(title);
	}

	virtual ~Win32WindowImpl() {
		Destroy();
	}

	virtual bool Process() {
		MSG msg = { 0 };
		while (PeekMessageA(&msg, _hwnd, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return !_close;
	}

	virtual void Present() {

	}

	virtual void SetRect(const Rect& rect) {
		RECT rc = { 0 };
		SetWindowPos(_hwnd, NULL,
			(int)rect.pos.x, (int)rect.pos.y,
			(int)(rect.pos.x + rect.size.w), (int)(rect.pos.y + rect.size.h),
			NULL);
	}

	virtual void SetColor(const RGBA& rgba) {
		_fg = RGB(rgba.r*255, rgba.g*255, rgba.b*255);
		_fgb = CreateSolidBrush(_fg);
	}

	virtual void SetClearColor(const RGBA& rgba) {
		_bg = RGB(rgba.r*255, rgba.g*255, rgba.b*255);
		_bgb = CreateSolidBrush(_bg);
	}

	virtual void DrawLine(const Point& beg, const Point& end, const float width) {
		HPEN hpen = CreatePen(NULL, (int)width, _fg);

		HDC hdc = GetDC(_hwnd);
		HPEN old = (HPEN)SelectObject(hdc, hpen);
		
		MoveToEx(hdc, (int)beg.x, (int)beg.y, NULL);
		LineTo(hdc, (int)end.x, (int)end.y);
		
		SelectObject(hdc, old);
		ReleaseDC(_hwnd, hdc);
	}

	virtual void DrawRect(const Rect& rect, const float width) {
		HPEN hpen = CreatePen(NULL, (int)width, _fg);
		float dw = width * 2;
		float hw = width / 2;
		int x1 = int(rect.pos.x + hw), x2 = int(rect.pos.x + rect.size.w - width);
		int y1 = int(rect.pos.y + hw), y2 = int(rect.pos.y + rect.size.h - width);
		
		HDC hdc = GetDC(_hwnd);
		HPEN old = (HPEN)SelectObject(hdc, hpen);
		
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x1, y2);
		LineTo(hdc, x2, y2);
		LineTo(hdc, x2, y1);
		LineTo(hdc, x1, y1);

		SelectObject(hdc, old);
		ReleaseDC(_hwnd, hdc);
	}

	virtual void DrawChar(const BoundingBox& box, const int c) {
		RECT rc = { 0 };
		rc.left = (LONG)box.l;
		rc.top = (LONG)box.t;
		rc.right = (LONG)box.r;
		rc.bottom = (LONG)box.b;

		HDC hdc = GetDC(_hwnd);
		SetTextColor(hdc, _fg);
		SetBkColor(hdc, _bg);

		DrawTextA(hdc, (char*)&c, 1, &rc, DT_NOPREFIX);

		ReleaseDC(_hwnd, hdc);
	}

	virtual void DrawText(const BoundingBox& box, const char* const text) {
		RECT rc = { 0 };
		rc.left = (LONG)box.l;
		rc.top = (LONG)box.t;
		rc.right = (LONG)box.r;
		rc.bottom = (LONG)box.b;

		HDC hdc = GetDC(_hwnd);
		SetTextColor(hdc, _fg);
		SetBkColor(hdc, _bg);

		DrawTextA(hdc, text, strlen(text), &rc, DT_NOPREFIX);

		ReleaseDC(_hwnd, hdc);
	}

	virtual void FillRect(const Rect& rect) {
		RECT rc = { 0 };
		rc.left = (LONG)rect.pos.x;
		rc.top = (LONG)rect.pos.y;
		rc.right = (LONG)(rect.pos.x + rect.size.w);
		rc.bottom = (LONG)(rect.pos.y + rect.size.h);

		HDC hdc = GetDC(_hwnd);

		::FillRect(hdc, &rc, _fgb);

		ReleaseDC(_hwnd, hdc);
	}

	virtual Size CharBounds(int c) {
		bool tab = c == '\t';
		bool nl = c == '\n';
		if (tab) c = ' ';
		if (nl)
			c = ' ';

		HDC hdc = GetDC(_hwnd);
		RECT rc = { 0 };
		DrawTextA(hdc, (char*)&c, 1, &rc, DT_CALCRECT);
		ReleaseDC(_hwnd, hdc);

		int height = rc.bottom - rc.top;
		int width = rc.right - rc.left;
		width = tab ? width * 8 : width;
		width = nl ? 0 : width;
		return { (float)width, (float)height };
	}

	virtual Size TextBounds(const char* const text) {
		HDC hdc = GetDC(_hwnd);
		RECT rc = { 0 };
		DrawTextA(hdc, text, strlen(text), &rc, DT_CALCRECT);
		ReleaseDC(_hwnd, hdc);

		return {
			(float)(rc.right - rc.left),
			(float)(rc.bottom - rc.top)
		};
	}

protected:
	virtual void Destroy() {
		if (_hwnd) {
			_close = true;
			DestroyWindow(_hwnd);
			_hwnd = NULL;
		}
	}

	virtual void Create(const char* const title) {
		Destroy();
		_close = false;
		_hwnd = CreateWindowExA(
			NULL, "Win32WindowImpl",
			title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, GetModuleHandle(NULL), this);

		if (!_hwnd) {
			throw std::runtime_error("unable to create window");
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
			default:
				result = DefWindowProcA(_hwnd, msg, wparam, lparam);
				break;
		}

		return result;
	}

	bool _close;
	HWND _hwnd;
	COLORREF _fg, _bg;
	HBRUSH _fgb, _bgb;
};
