#include <cstdlib>
#include <Window\Window.h>
#include <Glyph\Glyph.h>


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
	Glyph* glyph = MakeDoc(window, lines);

	RGBA glyph_color = { 0.8f, 0.8f, 0.8f, 1.0f };
	do {
		window.Clear();

		glyph->Draw(window, glyph_color);
		
		window.Present();
	} while (window.Process());

	//std::system("pause");
	return 0;
}