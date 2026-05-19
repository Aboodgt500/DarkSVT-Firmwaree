#pragma once
void initIR();
void startTVBGone();
void stopTVBGone();
void runTVBGoneLoop();
void sendIRCode(uint32_t code, uint8_t protocol);
void sendIRRaw(const uint16_t* timings, uint8_t len);
extern bool tvBGoneActive;
