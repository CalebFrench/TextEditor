#include <cstdlib>
#include <Window\Window.h>
#include <Glyph\Glyphs.h>
#include <Compositor\Compositor.h>
#include <Windows.h>

std::vector<Glyph*> ReadFile(const char* const file_name, RGBA& color, Window& window) {
	std::vector<Glyph*> glyphs;
	FILE* file = fopen(file_name, "r");
	if (!file)
		return glyphs;

	while (!feof(file)) {
		int c = fgetc(file);
		glyphs.push_back(new CharGlyph(c, color, window));
	}

	return glyphs;
}

int main(int argc, char* argv[]) {
	Window window("Lexi");
	window.SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	window.SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	std::vector<Glyph*> glyphs = ReadFile("src/Window/Window.cpp", RGBA{ 0.9f, 0.9f, 0.9f, 1.0f }, window);
	Compositor* compositor = new SimpleCompositor;

	Rect rect;
	window.GetRect(rect);
	compositor->SetComposition({ rect.size.w, rect.size.h }, glyphs);
	Glyph* doc = compositor->Compose();
	
	do {
		window.Clear();

		doc->Draw(window);

		window.Present();
	} while (window.Process());

	return 0;
}