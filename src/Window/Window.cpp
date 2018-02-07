#include <Window\Window.h>
#include <Window\Win32WindowImpl.h>


Window::Window(const char* const title, float width, float height) {
	_impl = new Win32WindowImpl(title);
	SetRect({ 0, 0, width, height });
}

Window::~Window() {
	delete _impl;
	_impl = nullptr;
}

bool Window::Process() {
	return _impl->Process();
}

void Window::Clear() {
	RGBA fg, bg;
	GetColor(fg);
	GetClearColor(bg);
	SetColor(bg);
	_impl->FillRect(_wnd_rect);
	SetColor(fg);
}

void Window::Present() {
	_impl->Present();
}

void Window::GetRect(Rect& rect) {
	rect = _wnd_rect;
}

void Window::GetColor(RGBA& rgba) {
	rgba = _fg;
}

void Window::GetClearColor(RGBA& rgba) {
	rgba = _bg;
}

void Window::SetRect(const Rect& rect) {
	_wnd_rect = rect;
	_impl->SetRect(_wnd_rect);
}

void Window::SetColor(const RGBA& rgba) {
	_fg = rgba;
	_impl->SetColor(_fg);
}

void Window::SetClearColor(const RGBA& rgba) {
	_bg = rgba;
	_impl->SetClearColor(_bg);
}

void Window::DrawLine(const Point& beg, const Point& end, const float width) {
	_impl->DrawLine(beg, end, width);
}

void Window::DrawRect(const Rect& rect, const float width) {
	_impl->DrawRect(rect, width);
}

void Window::DrawChar(const BoundingBox& box, const int c) {
	_impl->DrawChar(box, c);
}

void Window::DrawText(const BoundingBox& box, const char* const text) {
	_impl->DrawText(box, text);
}

void Window::FillRect(const Rect& rect) {
	_impl->FillRect(rect);
}

Size Window::CharBounds(const int c) {
	return _impl->CharBounds(c);
}

Size Window::TextBounds(const char* const text) {
	return _impl->TextBounds(text);
}
