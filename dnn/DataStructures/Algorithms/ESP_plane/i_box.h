#pragma once

class i_box {
private:
	int lv; // level
	int x; // its lower-left corner lies at a point (x, y) which equals (a2^(lv), b2^(lv)) for some integers a, b.
	int y;
public:
	i_box();
	~i_box();
	i_box(int, int, int);
	int getLv();
	int getX();
	int getY();
};
