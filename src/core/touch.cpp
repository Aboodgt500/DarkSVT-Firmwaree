#include "touch.h"
#include "display.h"

void initTouch() {}

bool getTouch(TouchPoint& p) {
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
        p.x = x; p.y = y; p.pressed = true;
        return true;
    }
    p.pressed = false;
    return false;
}

bool touchIn(int x, int y, int w, int h) {
    TouchPoint p;
    if (!getTouch(p)) return false;
    return (p.x >= x && p.x <= x+w && p.y >= y && p.y <= y+h);
}
