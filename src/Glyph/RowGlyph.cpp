#include <Glyph\RowGlyph.h>


void RowGlyph::Update() {
	box_ = { 0 };
	for (auto child : children_) {
		child->SetOffset({ box_.r, 0 });

		Size size = child->GetSize();
		box_.r += size.w;
		box_.b = box_.b > size.h ? box_.b : size.h;
	}
}
