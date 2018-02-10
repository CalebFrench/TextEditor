#include <Glyph\BorderDecorator.h>


void BorderDecorator::Draw(Window& window) {
	child_->Draw(window);

	BoundingBox box = GetBoundingBox();
	window.DrawRect({
		box.l, box.t,
		box.r - box.l,
		box.b - box.t
	}, 2.0f);
}