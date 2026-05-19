#pragma once
#include <RF24.h>
void initNRF24();
void startNRF24Scan();
void stopNRF24Scan();
void runNRF24ScanLoop();
extern bool nrf24ScanActive;
