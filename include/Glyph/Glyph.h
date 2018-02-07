#pragma once
#include <Window\Window.h>
#include <vector>

struct Pos {
	float x, y;
};

struct Box {
	float w, h;
};

class Glyph {
public:
	virtual ~Glyph();

	virtual void Draw(Window& window, RGBA& color) = 0;

	virtual Pos GetPos();
	virtual void SetPos(Pos pos);
	virtual Box GetBox();
	virtual void SetBox(Box box);
	virtual Pos GetPad();
	virtual void SetPad(Pos pad);
	virtual Glyph* GetParent();
	virtual void SetParent(Glyph* parent);

protected:
	Pos pos_;
	Box box_;
	Pos pad_;
	Glyph* parent_;

	Glyph(Pos pos, Box box, Glyph* parent);
};

class CharGlyph : public Glyph {
public:
	CharGlyph(char c, Window& window, Glyph* parent = nullptr);

	void Draw(Window& window, RGBA& rgba);

protected:
	char c_;
};

class Row : Glyph {
public:
	Row(Pos pos = { 0 }, Box box = { 0 }, Glyph* parent = nullptr);

	virtual void Draw(Window& window, RGBA& color);
	virtual void Add(Glyph* glyph);

	virtual Box GetBox();
	virtual void SetBox(Box box);

protected:
	std::vector<Glyph*> children_;
};

class Col : Glyph {
public:
	Col(Pos pos = { 0 }, Box box = { 0 }, Glyph* parent = nullptr);

	virtual void Draw(Window& window, RGBA& color);
	void Add(Glyph* glyph);

	virtual Box GetBox();
	virtual void SetBox(Box box);

protected:
	std::vector<Glyph*> children_;
};
