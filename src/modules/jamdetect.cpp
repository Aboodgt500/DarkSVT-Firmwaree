#include "jamdetect.h"
#include "../core/display.h"
#include <WiFi.h>
#include <SmartRC_CC1101.h>
extern SmartRC_CC1101 cc1101;

bool jamDetectActive = false;
static int deauthCount = 0;
static unsigned long lastCheck = 0;

void initJamDetect() {}

void startJamDetect() {
    jamDetectActive = true;
    deauthCount = 0;
    lastCheck = millis();
    showMsg("JAM DETECT ACTIVE", C_OK);
}

void stopJamDetect() {
    jamDetectActive = false;
    showMsg("JAM DETECT STOPPED", C_INFO);
}

void runJamDetectLoop() {
    if (!jamDetectActive) return;
    if (millis() - lastCheck > 1000) {
        if (WiFi.status() == WL_DISCONNECTED && deauthCount > 10) {
            showMsg("DEAUTH ATTACK DETECTED!", C_WARN);
        }
        int rssi = cc1101.getRssi();
        if (rssi > -40) {
            showMsg("SUBGHZ JAMMING DETECTED!", C_WARN);
        }
        lastCheck = millis();
        deauthCount = 0;
    }
}
