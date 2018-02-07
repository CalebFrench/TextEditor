#include "Glyph.h"


Glyph::Glyph(Pos pos, Box box, Glyph* parent)
	: pos_(pos), box_(box), parent_(parent), pad_({ 0 })
{}

Glyph::~Glyph() {

}

Pos Glyph::GetPos() {
	if (!parent_)
		return pos_;

	Pos pos = parent_->GetPos();
	pos.x += pos_.x;
	pos.y += pos_.y;
	return pos;
}

void Glyph::SetPos(Pos pos) {
	pos_ = pos;
}

Box Glyph::GetBox() {
	return box_;
}

void Glyph::SetBox(Box box) {
	box_ = box;
}

Pos Glyph::GetPad() {
	return pad_;
}

void Glyph::SetPad(Pos pad) {
	pad_ = pad;
}

Glyph* Glyph::GetParent() {
	return parent_;
}

void Glyph::SetParent(Glyph* parent) {
	parent_ = parent;
}

CharGlyph::CharGlyph(char c, Window& window, Glyph* parent)
	: Glyph({ 0 }, { 0 }, parent), c_(c)
{
	Size s = window.CharBounds(c_);
	box_ = { s.w, s.h };
}

void CharGlyph::Draw(Window& window, RGBA& color) {
	Pos pad = GetPad();
	Box box = GetBox();
	window.SetColor(color);
	window.DrawChar({ pad_.x, pad_.y, pad.x + box.w, pad.y + box.h }, c_);
}

Row::Row(Pos pos, Box box, Glyph* parent)
	: Glyph(pos, box, parent)
{}

void Row::Draw(Window& window, RGBA& color) {
	Pos pad = GetPad();
	if (!children_.empty()) {
		children_[0]->SetPad({ pad.x, pad.y });
		children_[0]->Draw(window, color);
	}

	float offset = pad.x;
	for (int i = 1; i < children_.size(); ++i) {
		Box box = children_[i - 1]->GetBox();
		offset += box.w;

		children_[i]->SetPad({ offset, pad.y });
		children_[i]->Draw(window, color);
	}
}

void Row::Add(Glyph* glyph) {
	glyph->SetParent(this);
	children_.push_back(glyph);
}

Box Row::GetBox() {
	box_ = { 0 };
	for (auto c : children_) {
		Box cbox = c->GetBox();
		box_.w += cbox.w;
		box_.h = box_.h > cbox.h ? box_.h : cbox.h;
	}

	return box_;
}

void Row::SetBox(Box box) {
}

Col::Col(Pos pos, Box box, Glyph* parent)
	: Glyph(pos, box, parent)
{}

void Col::Draw(Window& window, RGBA& color) {
	Pos pad = GetPad();
	if (!children_.empty()) {
		children_[0]->SetPad({ pad.x, pad.y });
		children_[0]->Draw(window, color);
	}

	float offset = pad.y;
	for (int i = 1; i < children_.size(); ++i) {
		Box box = children_[i - 1]->GetBox();
		offset += box.h;

		children_[i]->SetPad({ pad.x, offset });
		children_[i]->Draw(window, color);
	}
}

void Col::Add(Glyph* glyph) {
	glyph->SetParent(this);
	children_.push_back(glyph);
}

Box Col::GetBox() {
	box_ = { 0 };
	for (auto c : children_) {
		Box cbox = c->GetBox();
		box_.w = box_.w > cbox.w ? box_.w : cbox.w;
		box_.h += cbox.h;
	}

	return box_;
}

void Col::SetBox(Box box) {
}
