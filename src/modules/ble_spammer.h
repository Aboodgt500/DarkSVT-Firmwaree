#pragma once
#include <NimBLEDevice.h>
void initBLESpammer();
void startBLESpam(int mode);
void stopBLESpam();
void runBLESpamLoop();
extern bool bleSpamActive;
extern int bleSpamMode;
