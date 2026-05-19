#include "subghz_manager.h"
#include "../core/display.h"
#include <SmartRC_CC1101.h>

SmartRC_CC1101 cc1101;
bool subghzReplayActive = false;
bool subghzJamActive = false;
RFSignal lastSignal = {433920000, {}, 0, -100, "None"};

// Tesla charge port static payload (43 bytes OOK)
static uint8_t teslaPayload[43] = {
    0x02, 0xAA, 0xAA, 0xAA,
    0x29, 0x26, 0x55, 0x35,
    0x96, 0x99, 0x96, 0x5A,
    0x65, 0xA9, 0x6A, 0x9A,
    0x59, 0x6A, 0xA5, 0x65,
    0x55, 0x00
};

void initSubGHz() {
    cc1101.Init();
    cc1101.setMHZ(433.92);
    cc1101.SetRx();
}

void startReplayRecord() {
    subghzReplayActive = true;
    cc1101.setMHZ(433.92);
    cc1101.SetRx();
    lastSignal.len = 0;
    lastSignal.freq = 433920000;
    showMsg("RECORDING... Press STOP", C_OK);
}

void stopReplayRecord() {
    subghzReplayActive = false;
    showMsg("SIGNAL SAVED", C_OK);
}

void replayLastSignal() {
    if (lastSignal.len == 0) {
        showMsg("No signal recorded!", C_WARN);
        return;
    }
    cc1101.setMHZ(lastSignal.freq / 1000000.0);
    cc1101.SetTx();
    for (int i=0; i<lastSignal.len; i+=2) {
        if (i < lastSignal.len) {
            digitalWrite(CC1101_GDO0, HIGH);
            delayMicroseconds(lastSignal.timings[i]);
        }
        if (i+1 < lastSignal.len) {
            digitalWrite(CC1101_GDO0, LOW);
            delayMicroseconds(lastSignal.timings[i+1]);
        }
    }
    cc1101.SetRx();
    showMsg("SIGNAL REPLAYED", C_OK);
}

void sendTeslaOnFreq(float mhz) {
    cc1101.setMHZ(mhz);
    cc1101.SetTx();
    for (int rep=0; rep<5; rep++) {
        for (int i=0; i<43; i++) {
            uint8_t byte = teslaPayload[i];
            for (int b=0; b<8; b++) {
                if (byte & (0x80 >> b)) {
                    digitalWrite(CC1101_GDO0, HIGH);
                    delayMicroseconds(400);
                    digitalWrite(CC1101_GDO0, LOW);
                    delayMicroseconds(400);
                } else {
                    digitalWrite(CC1101_GDO0, HIGH);
                    delayMicroseconds(800);
                    digitalWrite(CC1101_GDO0, LOW);
                    delayMicroseconds(800);
                }
            }
        }
        delay(23);
    }
    cc1101.SetRx();
}

void openTeslaUS() {
    sendTeslaOnFreq(315.0);
    showMsg("Tesla US 315MHz sent", C_OK);
}

void openTeslaEU() {
    sendTeslaOnFreq(433.92);
    showMsg("Tesla EU 433.92MHz sent", C_OK);
}

void openTeslaBoth() {
    openTeslaUS();
    delay(500);
    openTeslaEU();
    showMsg("Tesla BOTH sent", C_OK);
}

void startSubGHzJam() {
    subghzJamActive = true;
    showMsg("SUBGHZ JAM STARTED", C_WARN);
}

void stopSubGHzJam() {
    subghzJamActive = false;
    cc1101.SetRx();
    showMsg("SUBGHZ JAM STOPPED", C_INFO);
}

static float jamFreqs[] = {315.0, 433.92, 868.0, 915.0};
static int jamIndex = 0;

void runSubGHzJamLoop() {
    if (!subghzJamActive) return;
    cc1101.setMHZ(jamFreqs[jamIndex]);
    cc1101.SetTx();
    for (int i=0; i<100; i++) {
        digitalWrite(CC1101_GDO0, random(2) ? HIGH : LOW);
        delayMicroseconds(random(50, 500));
    }
    jamIndex = (jamIndex + 1) % 4;
}

struct FreqEntry {
    const char* name;
    float mhz;
    const char* region;
    const char* usage;
};

const FreqEntry GULF_FREQS[] = {
    {"Toyota 312", 312.0, "Global", "Car key"},
    {"Honda 314", 314.0, "Global", "Car key"},
    {"Tesla US", 315.0, "US/Gulf", "Tesla/Car"},
    {"Ford 310", 310.0, "US", "Car key"},
    {"Asian 304", 304.25, "Asia", "Car key"},
    {"EU 418", 418.0, "EU", "Car key"},
    {"UK 447", 447.0, "UK", "Car key"},
    {"CAME", 433.92, "EU/Gulf", "Garage"},
    {"Nice", 433.92, "EU/Gulf", "Gate"},
    {"BFT", 433.92, "EU/Gulf", "Gate"},
    {"FAAC", 433.92, "EU/Gulf", "Gate"},
    {"Somfy", 433.42, "EU", "Shutter"},
    {"Ditec", 433.92, "EU", "Gate"},
    {"Beninca", 433.92, "EU", "Gate"},
    {"Marantec", 433.92, "EU", "Garage"},
    {"Hormann", 868.3, "EU", "Garage"},
    {"Chamberlain", 390.0, "US", "Garage"},
    {"Genie", 390.0, "US", "Garage"},
    {"LiftMaster", 315.0, "US", "Garage"},
    {"Stanley", 310.0, "US", "Garage"},
    {"Z-Wave EU", 868.4, "EU", "Smart home"},
    {"Z-Wave US", 908.4, "US", "Smart home"},
    {"Zigbee EU", 868.0, "EU", "IoT"},
    {"Zigbee US", 915.0, "US", "IoT"},
    {"LoRa EU", 868.1, "EU", "LoRaWAN"},
    {"LoRa US", 915.0, "US", "LoRaWAN"},
    {"EnOcean", 868.3, "EU", "Energy"},
    {"W-MBus", 868.0, "EU", "Smart meter"},
    {"Tesla EU", 433.92, "EU", "Tesla"},
    {"Weather", 433.92, "Global", "Weather"},
    {"Oregon", 433.92, "Global", "Sensor"},
    {"LaCrosse", 433.92, "Global", "Sensor"},
    {"Alarm 433", 433.92, "Global", "Alarm"},
    {"Alarm 868", 868.3, "EU", "Alarm"},
    {"Alarm 315", 315.0, "US", "Alarm"},
    {"ISM 433", 433.0, "Global", "ISM"},
    {"ISM 868", 868.0, "EU", "ISM"},
    {"ISM 915", 915.0, "US", "ISM"},
    {"PMR 446", 446.0, "EU", "Walkie"},
    {"FRS 462", 462.0, "US", "Walkie"},
    {"GMRS 462", 462.0, "US", "Mobile"},
    {"Industrial", 490.0, "Global", "Industrial"},
    {"Pager 430", 430.0, "Global", "Pager"},
    {"Gulf Smart", 868.0, "Gulf", "Smart home"},
    {"Gulf Gate", 433.92, "Gulf", "Gate"},
    {"Gulf Car", 315.0, "Gulf", "Car"},
    {"Gulf IoT", 915.0, "Gulf", "IoT"},
    {"Gulf LoRa", 868.1, "Gulf", "LoRa"},
    {"Oman Gate", 433.92, "Oman", "Gate"},
    {"Oman Car", 315.0, "Oman", "Car"},
    {"UAE Smart", 868.0, "UAE", "Smart city"},
    {"Saudi IoT", 915.0, "Saudi", "IoT"},
    {"Qatar", 433.92, "Qatar", "Gate"},
    {"Kuwait", 433.92, "Kuwait", "Gate"},
    {"Bahrain", 433.92, "Bahrain", "Gate"},
};

const int GULF_FREQ_COUNT = sizeof(GULF_FREQS) / sizeof(GULF_FREQS[0]);

void showFrequencyList() {
    tft.fillScreen(C_BG);
    drawHeader("Gulf Frequencies");
    int y = 30;
    for (int i=0; i<10 && i<GULF_FREQ_COUNT; i++) {
        tft.setTextColor(C_FG, C_BG);
        tft.setCursor(4, y);
        tft.printf("%.2f %s", GULF_FREQS[i].mhz, GULF_FREQS[i].name);
        tft.setCursor(160, y);
        tft.setTextColor(C_DIM, C_BG);
        tft.print(GULF_FREQS[i].region);
        y += 18;
    }
    drawFooter("Scroll: Touch | Select: Tap");
}

void runSubGHzLoop() {
    if (!subghzReplayActive) return;
    int rssi = cc1101.getRssi();
    if (rssi > -80 && lastSignal.len < 512) {
        static unsigned long lastEdge = 0;
        unsigned long now = micros();
        if (now - lastEdge > 100) {
            lastSignal.timings[lastSignal.len++] = (now - lastEdge) / 10;
            lastEdge = now;
        }
    }
}
