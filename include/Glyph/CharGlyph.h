#pragma once
#include "Glyph.h"

class CharGlyph : public Glyph {
public:
	CharGlyph(char c, RGBA& color, Window& window);

	void Draw(Window& window);

protected:
	char c_;
	RGBA color_;
};