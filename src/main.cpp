// ============================================================
// DarkSVT Ultimate v2.0
// Updated for 2026 libraries (SmartRC v3, NimBLE 1.4.2)
// Based on ESP32-DIV + Bruce open-source code
// ============================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// Core
#include "core/config.h"
#include "core/display.h"
#include "core/touch.h"
#include "core/sdcard.h"
#include "core/settings.h"

// Modules
#include "modules/wifi_deauther.h"
#include "modules/wifi_beacon.h"
#include "modules/ble_spammer.h"
#include "modules/subghz_manager.h"
#include "modules/nrf24_scanner.h"
#include "modules/ir_manager.h"
#include "modules/rfid_scanner.h"
#include "modules/jamdetect.h"
#include "modules/tools.h"

// UI
#include "ui/menu.h"
#include "ui/splash.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n========================================");
    Serial.println("  DarkSVT Ultimate v2.0");
    Serial.println("  2026 Edition | Gulf Region");
    Serial.println("========================================\n");

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    showSplashScreen();
    delay(2000);

    initTouch();
    initSDCard();
    initSettings();

    initWiFiDeauther();
    initSubGHz();
    initNRF24();
    initIR();
    initRFID();

    drawMainMenu();
}

void loop() {
    handleTouchLoop();
    if (deauthActive) runDeauthLoop();
    if (beaconActive) runBeaconLoop();
    if (bleSpamActive) runBLESpamLoop();
    if (subghzJamActive) runSubGHzJamLoop();
    if (nrf24ScanActive) runNRF24ScanLoop();
    if (jamDetectActive) runJamDetectLoop();
    if (tvBGoneActive) runTVBGoneLoop();
    if (nfcScanActive) runNFCScanLoop();
}
