#pragma once
#include <MFRC522.h>
void initRFID();
void startNFCScan();
void stopNFCScan();
void runNFCScanLoop();
extern bool nfcScanActive;
