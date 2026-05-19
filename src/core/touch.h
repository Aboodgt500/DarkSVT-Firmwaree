#pragma once
struct TouchPoint { uint16_t x; uint16_t y; bool pressed; };
void initTouch();
bool getTouch(TouchPoint& p);
bool touchIn(int x, int y, int w, int h);
