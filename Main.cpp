#include <cstdlib>
#include "Window.h"
#include "Glyph.h"

Glyph* MakeDoc(Window& window, std::vector<std::string> strs) {
	Row* doc = new Row();

	Col* col = new Col();
	for (auto str : strs) {
		Row* row = new Row();
		for (auto c : str) {
			row->Add(new CharGlyph(c, window));
		}
		col->Add((Glyph*)row);
	}
	doc->Add((Glyph*)col);

	return (Glyph*)doc;
}

int main(int argc, char* argv[]) {
	Window window("Lexi");
	window.SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	window.SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	std::vector<std::string> lines(1);
	FILE* file = fopen("Window.cpp", "r");
	while (!feof(file)) {
		int c = fgetc(file);
		lines.back().push_back(c);
		if (c == '\n') {
			lines.emplace_back();
		}
	}

	RGBA glyph_color = { 0.8f, 0.8f, 0.8f, 1.0f };
	Glyph* glyph = MakeDoc(window, lines);

	do {
		window.Clear();

		glyph->Draw(window, glyph_color);
		
		window.Present();
	} while (window.Process());

	//std::system("pause");
	return 0;
}