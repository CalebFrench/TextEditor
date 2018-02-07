#pragma once
#include "Glyph.h"


class BoundingGlyph : public DecoratorGlyph {
public:
	virtual void SetBoundingBox(BoundingBox box);
};