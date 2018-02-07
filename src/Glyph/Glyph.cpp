#include <Glyph\Glyph.h>


Glyph::Glyph()
	: box_({ 0 }), off_({ 0 }), parent_(nullptr)
{}

Glyph::~Glyph()
{}

Size Glyph::GetSize() {
	return {
		box_.r - box_.l,
		box_.b - box_.t
	};
}

BoundingBox Glyph::GetBoundingBox() {
	BoundingBox box = box_;
	Point off = GetOffset();
	box.l += off.x;
	box.t += off.y;
	box.r += off.x;
	box.b += off.y;

	if (parent_) {
		BoundingBox pbox = parent_->GetBoundingBox();
		box.l += pbox.l;
		box.t += pbox.t;
		box.r += pbox.l;
		box.b += pbox.t;
		box.r = pbox.r > 0 && pbox.r < box.r ? pbox.r : box.r;
		box.b = pbox.b > 0 && pbox.b < box.b ? pbox.b : box.b;
	}
	return box;
}

void Glyph::SetBoundingBox(BoundingBox box)
{}

Point Glyph::GetOffset() {
	return off_;
}

void Glyph::SetOffset(Point offset) {
	off_ = offset;
}

Glyph* Glyph::GetParent() {
	return parent_;
}

void Glyph::SetParent(Glyph* parent) {
	parent_ = parent;
}

Glyph* DecoratorGlyph::GetChild() {
	return child_;
}

void DecoratorGlyph::Add(Glyph* child) {
	child->SetParent(this);
	child_ = child;
}

void DecoratorGlyph::Draw(Window& window) {
	BoundingBox box = GetBoundingBox();
	BoundingBox cbox = child_->GetBoundingBox();
	if ((box.r < 0 || cbox.l <= box.r)
			&& (box.b < 0 || cbox.t <= box.b)) {
		child_->Draw(window);
	}
}

void ContainerGlyph::Draw(Window& window) {
	BoundingBox box = GetBoundingBox();
	for (auto child : children_) {
		BoundingBox cbox = child->GetBoundingBox();
		if ((box.r < 0 || cbox.l <= box.r)
				&& (box.b < 0 || cbox.t <= box.b)) {
			child->Draw(window);
		}
	}
}

void ContainerGlyph::Add(Glyph* glyph) {
	glyph->SetParent(this);
	children_.push_back(glyph);
	Update();
}
