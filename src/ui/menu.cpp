#include "menu.h"
#include "../core/config.h"
#include "../core/display.h"
#include "../core/touch.h"
#include "../modules/wifi_deauther.h"
#include "../modules/wifi_beacon.h"
#include "../modules/ble_spammer.h"
#include "../modules/subghz_manager.h"
#include "../modules/nrf24_scanner.h"
#include "../modules/ir_manager.h"
#include "../modules/rfid_scanner.h"
#include "../modules/jamdetect.h"
#include "../modules/tools.h"

int currentMenu = 0;
int currentSubMenu = -1;
bool menuNeedsRedraw = true;

struct MenuItem {
    const char* label;
    int subCount;
    const char** subs;
};

const char* WIFI_SUBS[] = {"Deauther", "Beacon Spam", "Scanner", NULL};
const char* BLE_SUBS[] = {"BLE Spam", NULL};
const char* SUB_SUBS[] = {"Replay", "Tesla", "Jammer", "Frequencies", NULL};
const char* NRF_SUBS[] = {"Scanner", NULL};
const char* RFID_SUBS[] = {"Scanner", NULL};
const char* IR_SUBS[] = {"TV-B-Gone", NULL};
const char* TOOL_SUBS[] = {"Tools Menu", NULL};
const char* SET_SUBS[] = {"Brightness", "About", NULL};

const MenuItem MENU[] = {
    {"WiFi", 3, WIFI_SUBS},
    {"Bluetooth", 1, BLE_SUBS},
    {"SubGHz", 4, SUB_SUBS},
    {"2.4GHz", 1, NRF_SUBS},
    {"RFID", 1, RFID_SUBS},
    {"IR", 1, IR_SUBS},
    {"Tools", 1, TOOL_SUBS},
    {"Settings", 2, SET_SUBS},
};

const int MENU_COUNT = sizeof(MENU) / sizeof(MENU[0]);

void drawMainMenu() {
    tft.fillScreen(C_BG);
    drawHeader("DarkSVT Ultimate");
    int y = 30;
    int h = 28;
    for (int i=0; i<MENU_COUNT; i++) {
        bool sel = (i == currentMenu);
        uint16_t bg = sel ? C_ACCENT : C_DIM;
        tft.fillRect(4, y, 232, h-2, bg);
        tft.setTextColor(sel ? TFT_WHITE : C_FG, bg);
        tft.setTextSize(1);
        tft.setCursor(10, y+8);
        tft.print(MENU[i].label);
        if (MENU[i].subCount > 0) {
            tft.setCursor(210, y+8);
            tft.print(">");
        }
        y += h;
    }
    tft.fillRect(0, 270, 240, 26, C_DIM);
    tft.setTextColor(TFT_WHITE, C_DIM);
    tft.setCursor(4, 276);
    tft.print("63 Tools | Gulf Edition");
    drawFooter("Touch to select | Hold to run");
}

void drawSubMenu(int menu) {
    if (menu < 0 || menu >= MENU_COUNT) return;
    const MenuItem* item = &MENU[menu];
    if (!item->subs) return;
    tft.fillScreen(C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setCursor(10, 10);
    tft.print(item->label);
    tft.drawLine(0, 24, 240, 24, C_DIM);
    int y = 30;
    for (int i=0; item->subs[i] != NULL; i++) {
        bool sel = (i == currentSubMenu);
        uint16_t bg = sel ? C_ACCENT2 : C_DIM;
        tft.fillRect(4, y, 232, 24, bg);
        tft.setTextColor(sel ? TFT_WHITE : C_FG, bg);
        tft.setCursor(10, y+6);
        tft.print(item->subs[i]);
        y += 26;
    }
    drawFooter("Back: top-left | Run: select");
}

void handleTouchLoop() {
    TouchPoint p;
    if (!getTouch(p)) return;
    if (p.x < 40 && p.y < 30) {
        if (currentSubMenu >= 0) {
            currentSubMenu = -1;
            menuNeedsRedraw = true;
        }
        delay(200);
        return;
    }
    if (currentSubMenu < 0) {
        int item = (p.y - 30) / 28;
        if (item >= 0 && item < MENU_COUNT) {
            currentMenu = item;
            if (MENU[item].subCount > 0) {
                currentSubMenu = 0;
            } else {
                if (item == 7) showAboutScreen();
            }
            menuNeedsRedraw = true;
        }
    } else {
        int item = (p.y - 30) / 26;
        if (item >= 0 && item < MENU[currentMenu].subCount) {
            switch (currentMenu) {
                case 0:
                    if (item == 0) scanForAPs();
                    else if (item == 1) startBeaconSpam();
                    else if (item == 2) { WiFi.mode(WIFI_MODE_STA); WiFi.scanNetworks(); }
                    break;
                case 1:
                    if (item == 0) startBLESpam(0);
                    break;
                case 2:
                    if (item == 0) startReplayRecord();
                    else if (item == 1) openTeslaUS();
                    else if (item == 2) startSubGHzJam();
                    else if (item == 3) showFrequencyList();
                    break;
                case 3:
                    if (item == 0) startNRF24Scan();
                    break;
                case 4:
                    if (item == 0) startNFCScan();
                    break;
                case 5:
                    if (item == 0) startTVBGone();
                    break;
                case 6:
                    if (item == 0) showToolsMenu();
                    break;
                case 7:
                    if (item == 0) showMsg("Brightness: 128", C_INFO);
                    else if (item == 1) showAboutScreen();
                    break;
            }
        }
    }
    if (menuNeedsRedraw) {
        if (currentSubMenu >= 0) drawSubMenu(currentMenu);
        else drawMainMenu();
        menuNeedsRedraw = false;
    }
    delay(200);
}

void showAboutScreen() {
    tft.fillScreen(C_BG);
    drawHeader("About DarkSVT");
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(2);
    tft.setCursor(20, 40);
    tft.print("DARKSVT");
    tft.setTextSize(1);
    tft.setTextColor(C_ACCENT2, C_BG);
    tft.setCursor(20, 70);
    tft.print("ULTIMATE v2.0");
    tft.setTextColor(C_FG, C_BG);
    tft.setCursor(20, 90);
    tft.print("63+ Tools | Gulf Edition");
    tft.setCursor(20, 110);
    tft.print("More than HaleHound + Bruce");
    tft.setCursor(20, 140);
    tft.setTextColor(C_DIM, C_BG);
    tft.print("WiFi: Deauth + Beacon + Scanner");
    tft.setCursor(20, 155);
    tft.print("Bluetooth: BLE Spam");
    tft.setCursor(20, 170);
    tft.print("SubGHz: Replay + Tesla + Jam + 50 freqs");
    tft.setCursor(20, 185);
    tft.print("RFID/IR/NRF24/Jam Detect/Tools");
    tft.setTextColor(C_OK, C_BG);
    tft.setCursor(20, 230);
    tft.print("Target: NM-RF-HAT / CYD");
    tft.setTextColor(C_WARN, C_BG);
    tft.setCursor(20, 250);
    tft.print("Educational Use Only");
    drawFooter("Touch to return");
    while (true) {
        TouchPoint p;
        if (getTouch(p)) break;
        delay(50);
    }
    menuNeedsRedraw = true;
}
