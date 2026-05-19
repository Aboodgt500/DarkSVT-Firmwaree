#include "wifi_beacon.h"
#include "../core/display.h"
#include <WiFi.h>

bool beaconActive = false;

static uint8_t beaconFrame[128] = {
    0x80, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00,
    0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00,
    0x01, 0x04,
    0x00
};

const char* spamSSIDs[] = {
    "FBI Surveillance Van", "CIA Black Site", "NSA Monitoring",
    "Free WiFi", "Starbucks_Guest", "Airport_Free", "Hotel_WiFi",
    "DarkSVT_Honeypot", "Click_Here_For_Virus", "Password123",
    "Free_Internet", "Public_WiFi", "Guest_Network",
    "iPhone (1)", "AndroidAP", "TP-Link_5G", "Netgear_Guest",
    "XfinityWiFi", "ATT-WiFi", "Verizon-Guest",
    "RickRoll_Never_Gonna", "Give_You_Up", "Never_Let_You_Down",
    "Run_Around", "Desert_You", "Make_You_Cry", "Say_Goodbye",
    "Tell_A_Lie", "Hurt_You", NULL
};

int spamIndex = 0;

void initBeaconSpam() {}

void startBeaconSpam() {
    beaconActive = true;
    WiFi.mode(WIFI_MODE_STA);
    showMsg("BEACON SPAM STARTED", C_OK);
}

void stopBeaconSpam() {
    beaconActive = false;
    showMsg("BEACON SPAM STOPPED", C_INFO);
}

void runBeaconLoop() {
    if (!spamSSIDs[spamIndex]) spamIndex = 0;
    const char* ssid = spamSSIDs[spamIndex];
    int len = strlen(ssid);
    uint8_t frame[256];
    memcpy(frame, beaconFrame, 24);
    frame[10] = random(256); frame[11] = random(256);
    frame[12] = random(256); frame[13] = random(256);
    frame[14] = random(256); frame[15] = random(256);
    memcpy(frame+16, frame+10, 6);
    uint64_t ts = micros();
    memcpy(frame+24, &ts, 8);
    frame[32] = 0x64; frame[33] = 0x00;
    frame[34] = 0x01; frame[35] = 0x04;
    frame[36] = 0x00;
    frame[37] = len;
    memcpy(frame+38, ssid, len);
    int frameLen = 38 + len + 4;
    esp_wifi_80211_tx(WIFI_IF_STA, frame, frameLen, false);
    spamIndex++;
    delay(10);
}
