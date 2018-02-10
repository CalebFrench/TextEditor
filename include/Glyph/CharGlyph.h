#pragma once
#include "Glyph.h"

class CharGlyph : public Glyph {
public:
	CharGlyph(int c, RGBA& color, Window& window);

	void Draw(Window& window);

	int GetChar();

protected:
	int c_;
	RGBA color_;
};