#include <Window\Window.h>
#include <Window\Win32WindowImpl.h>


Window::Window(const char* const title, float width, float height) {
	_impl = new Win32WindowImpl(title);
	SetRect({ 100, 100, width, height });
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
	GetRect(_wnd_rect);
	_impl->FillRect({
		0, 0,
		_wnd_rect.size.w,
		_wnd_rect.size.h
	});
	SetColor(fg);
}

bool Window::Present() {
	return _impl->Present();
}

void Window::GetRect(Rect& rect) {
	_impl->GetRect(rect);
}

void Window::GetFont(const char* font) {
	font = _font;
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

void Window::SetFont(const char* const font) {
	if (_font) delete _font;
	int len = strlen(font);
	_font = new char[len+1];
	strcpy(_font, font);
	_font[len] = '\0';
	_impl->SetFont(font);
}

void Window::SetColor(const RGBA& rgba) {
	_fg = rgba;
	_impl->SetColor(_fg);
}

void Window::SetClearColor(const RGBA& rgba) {
	_bg = rgba;
	_impl->SetClearColor(_bg);
}

void Window::OnSize(std::function<void(int, int)> on_size) {
	_impl->OnSize(on_size);
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

