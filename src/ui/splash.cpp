#include "splash.h"
#include "../core/display.h"

void showSplashScreen() {
    tft.fillScreen(C_BG);
    tft.drawRect(10, 10, 220, 300, C_ACCENT);
    tft.drawRect(12, 12, 216, 296, C_ACCENT2);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(3);
    tft.setCursor(25, 60);
    tft.print("DARK");
    tft.setCursor(25, 90);
    tft.print("SVT");
    tft.setTextColor(C_ACCENT2, C_BG);
    tft.setTextSize(1);
    tft.setCursor(25, 130);
    tft.print("ULTIMATE v2.0");
    tft.setTextColor(C_DIM, C_BG);
    tft.setCursor(25, 150);
    tft.print("More than Bruce + HaleHound");
    tft.setCursor(25, 170);
    tft.print("63+ Tools | Gulf Edition");
    tft.drawRect(25, 200, 190, 12, C_DIM);
    for (int i=0; i<100; i+=5) {
        tft.fillRect(26, 201, i*1.88, 10, C_ACCENT);
        delay(20);
    }
    tft.setTextColor(C_OK, C_BG);
    tft.setCursor(25, 230);
    tft.print("Loading complete");
    tft.setTextColor(C_WARN, C_BG);
    tft.setCursor(25, 250);
    tft.print("Touch screen to begin");
}
