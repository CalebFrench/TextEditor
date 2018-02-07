#include <cstdlib>
#include <Window\Window.h>
#include <Glyph\Glyphs.h>


Glyph* MakeDoc(Window& window, std::vector<std::string> strs) {
	BoundingGlyph* doc = new BoundingGlyph();
	doc->SetBoundingBox({ 4, 4, 10000, 10000 });

	ColGlyph* col = new ColGlyph();
	for (auto str : strs) {
		RowGlyph* row = new RowGlyph();
		for (auto c : str) {
			row->Add(new CharGlyph(c, RGBA{ 0.9f, 0.9f, 0.9f, 1.0f }, window));
		}
		col->Add((Glyph*)row);
	}
	doc->Add((Glyph*)col);

	return (Glyph*)doc;
}

std::vector<std::string> ReadLines(const char* const file_name) {
	std::vector<std::string> lines;
	FILE* file = fopen(file_name, "r");
	if (!file)
		return lines;

	lines.emplace_back();
	while (!feof(file)) {
		int c = fgetc(file);
		lines.back().push_back(c);
		if (c == '\n') {
			lines.emplace_back();
		}
	}

	return lines;
}

int main(int argc, char* argv[]) {
	Window window("Lexi");
	window.SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	window.SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	std::vector<std::string> lines = ReadLines("src/Window/Window.cpp");
	Glyph* doc = MakeDoc(window, lines);

	do {
		window.Clear();

		doc->Draw(window);
		
		window.Present();
	} while (window.Process());

	//std::system("pause");
	return 0;
}