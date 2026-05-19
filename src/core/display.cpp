#include "display.h"

void initDisplay() {
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(C_BG);
}

void drawHeader(const char* title) {
    tft.fillRect(0, 0, 240, 24, C_ACCENT);
    tft.setTextColor(TFT_WHITE, C_ACCENT);
    tft.setTextSize(1);
    tft.setCursor(4, 6);
    tft.print(title);
}

void drawFooter(const char* status) {
    tft.fillRect(0, 296, 240, 24, C_DIM);
    tft.setTextColor(TFT_WHITE, C_DIM);
    tft.setCursor(4, 302);
    tft.print(status);
}

void drawButton(int x, int y, int w, int h, const char* label, bool selected) {
    uint16_t bg = selected ? C_ACCENT : C_DIM;
    uint16_t fg = selected ? TFT_WHITE : C_FG;
    tft.fillRoundRect(x, y, w, h, 4, bg);
    tft.setTextColor(fg, bg);
    int tw = tft.textWidth(label);
    tft.setCursor(x + (w-tw)/2, y + (h-8)/2);
    tft.print(label);
}

void drawProgress(int x, int y, int w, int percent) {
    tft.drawRect(x, y, w, 12, C_DIM);
    int fill = (w-2) * percent / 100;
    tft.fillRect(x+1, y+1, fill, 10, C_ACCENT);
}

void showMsg(const char* msg, uint16_t color) {
    tft.fillRect(10, 130, 220, 40, C_DIM);
    tft.drawRect(10, 130, 220, 40, color);
    tft.setTextColor(C_FG, C_DIM);
    tft.setCursor(20, 146);
    tft.print(msg);
    delay(1500);
}
