#pragma once
#include "..\Window\Window.h"
#include <vector>


class Glyph;
class ContainerGlyph;
class Compositor {
public:
	void SetComposition(Size size, std::vector<Glyph*> composition);
	virtual Glyph* Compose() = 0;

protected:
	Size size_;
	std::vector<Glyph*> composition_;
};

class SimpleCompositor : public Compositor {
public:
	virtual Glyph* Compose();
};