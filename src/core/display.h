#pragma once
#include <TFT_eSPI.h>
extern TFT_eSPI tft;

void initDisplay();
void drawHeader(const char* title);
void drawFooter(const char* status);
void drawButton(int x, int y, int w, int h, const char* label, bool selected);
void drawProgress(int x, int y, int w, int percent);
void showMsg(const char* msg, uint16_t color);
