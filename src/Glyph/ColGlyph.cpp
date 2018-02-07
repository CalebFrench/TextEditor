#include <Glyph\ColGlyph.h>


void ColGlyph::Update() {
	box_ = { 0 };
	for (auto child : children_) {
		child->SetOffset({ 0, box_.b });

		Size size = child->GetSize();
		box_.r = box_.r > size.w ? box_.r : size.w;
		box_.b += size.h;
	}
}
