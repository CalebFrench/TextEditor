#include <Compositor\Compositor.h>
#include <Glyph\Glyphs.h>

void Compositor::SetComposition(Size size, std::vector<Glyph*> composition) {
	size_ = size;
	composition_ = composition;
}

Glyph* SimpleCompositor::Compose() {
	BorderDecorator* doc = new BorderDecorator;
	doc->SetBoundingBox({ 4, 4, size_.w - 4, size_.h - 4 });

	BoundingDecorator* textArea = new BoundingDecorator;
	textArea->SetBoundingBox({ 4, 4, size_.w - 4, size_.h - 4 });

	ColGlyph* col = new ColGlyph;
	RowGlyph* row = new RowGlyph;
	for (auto child : composition_) {
		row->Insert(child);
		if (((CharGlyph*)child)->GetChar() == '\n') {
			col->Insert(row);
			row = new RowGlyph;
		}
	}

	textArea->Insert(col);
	doc->Insert(textArea);

	return doc;
}
