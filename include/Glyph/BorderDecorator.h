#pragma once
#include "BoundingDecorator.h"

class BorderDecorator : public BoundingDecorator {
public:
	void Draw(Window& window);
};