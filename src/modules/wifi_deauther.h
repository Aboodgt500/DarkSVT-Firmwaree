#pragma once
#include <WiFi.h>

struct APInfo {
    uint8_t bssid[6];
    char ssid[33];
    int8_t rssi;
    uint8_t channel;
    bool selected;
};

void initWiFiDeauther();
void scanForAPs();
void drawAPList();
void selectAP(int idx);
void startDeauth();
void stopDeauth();
void runDeauthLoop();

extern bool deauthActive;
extern APInfo apList[32];
extern int apCount;
extern int selectedAP;
