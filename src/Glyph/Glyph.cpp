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

void DecoratorGlyph::Insert(Glyph* child, size_t pos) {
	child->SetParent(this);
	child_ = child;
}

void DecoratorGlyph::Remove(size_t pos) {
	delete child_;
	child_ = nullptr;
}

void DecoratorGlyph::Draw(Window& window) {
	BoundingBox box = GetBoundingBox();
	BoundingBox cbox = child_->GetBoundingBox();
	if ((box.r < 0 || cbox.l <= box.r)
			&& (box.b < 0 || cbox.t <= box.b)) {
		child_->Draw(window);
	}
}

Glyph* DecoratorGlyph::GetChild(size_t pos) {
	return child_;
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

void ContainerGlyph::Insert(Glyph* child, size_t pos) {
	child->SetParent(this);

	auto itr = children_.begin();
	for (size_t i = 0; itr != children_.end() && i < pos; ++i)
		++itr;

	children_.insert(itr, child);
	Update();
}

void ContainerGlyph::Remove(size_t pos) {
	auto itr = children_.begin();
	for (size_t i = 0; itr != children_.end() && i < pos; ++i)
		++itr;

	delete *itr;
	children_.erase(itr);
	Update();
}

Glyph* ContainerGlyph::GetChild(size_t pos) {
	auto itr = children_.begin();
	for (size_t i = 0; itr != children_.end() && i < pos; ++i)
		++itr;

	return *itr;
}


std::vector<Glyph*> ContainerGlyph::GetChildren() {
	return children_;
}
