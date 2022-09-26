#include "i_box.h"


i_box::i_box() {}
i_box::~i_box() {}
i_box::i_box(int _lv, int _x, int _y) { lv = _lv; x = _x; y = _y; }
int i_box::getLv() { return lv; }
int i_box::getX() { return x; }
int i_box::getY() { return y; }