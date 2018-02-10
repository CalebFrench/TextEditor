#include <Glyph\CharGlyph.h>


CharGlyph::CharGlyph(int c, RGBA& color, Window& window)
	: c_(c), color_(color)
{
	Size s = window.CharBounds(c_);
	box_ = { 0, 0, s.w, s.h };
}

void CharGlyph::Draw(Window& window) {
	window.SetColor(color_);
	auto box = GetBoundingBox();
	window.DrawChar(box, c_);
}

int CharGlyph::GetChar() {
	return c_;
}

