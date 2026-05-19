#pragma once
#include <SmartRC_CC1101.h>

struct RFSignal {
    uint32_t freq;
    uint16_t timings[512];
    uint16_t len;
    int8_t rssi;
    char name[32];
};

void initSubGHz();
void startReplayRecord();
void stopReplayRecord();
void replayLastSignal();
void openTeslaUS();
void openTeslaEU();
void openTeslaBoth();
void startSubGHzJam();
void stopSubGHzJam();
void runSubGHzJamLoop();
void showFrequencyList();
void runSubGHzLoop();

extern bool subghzReplayActive;
extern bool subghzJamActive;
extern RFSignal lastSignal;
extern SmartRC_CC1101 cc1101;
