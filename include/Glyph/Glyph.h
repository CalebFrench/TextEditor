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

	virtual void Draw(Window& window) = 0;

	virtual Size GetSize();
	virtual BoundingBox GetBoundingBox();
	virtual void SetBoundingBox(BoundingBox box);
	virtual Point GetOffset();
	virtual void SetOffset(Point offset);
	virtual Glyph* GetParent();
	virtual void SetParent(Glyph* parent);

protected:
	BoundingBox box_;
	Point off_;
	Glyph* parent_;

	Glyph();
};

class DecoratorGlyph : public Glyph {
public:
	virtual void Insert(Glyph* glyph, size_t pos = -1);
	virtual void Remove(size_t pos = -1);
	virtual void Draw(Window& window);

	virtual Glyph* GetChild(size_t pos = -1);

protected:
	Glyph* child_;
};

class ContainerGlyph : public Glyph {
public:
	virtual void Update() = 0;
	virtual void Insert(Glyph* glyph, size_t pos = -1);
	virtual void Remove(size_t pos = -1);
	virtual void Draw(Window& window);

	virtual Glyph* GetChild(size_t pos = -1);
	virtual std::vector<Glyph*> GetChildren();


protected:
	std::vector<Glyph*> children_;
};
