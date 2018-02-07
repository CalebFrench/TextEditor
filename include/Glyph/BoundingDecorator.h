#pragma once
#include "Glyph.h"


class BoundingDecorator : public DecoratorGlyph {
public:
	virtual void SetBoundingBox(BoundingBox box);
};