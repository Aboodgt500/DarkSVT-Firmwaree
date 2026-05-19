#include "nrf24_scanner.h"
#include "../core/display.h"
#include <RF24.h>

RF24 radio(17, 16);
bool nrf24ScanActive = false;

void initNRF24() {
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_1MBPS);
    radio.setChannel(0);
    radio.startListening();
}

void startNRF24Scan() {
    nrf24ScanActive = true;
    radio.startListening();
    showMsg("NRF24 SCAN STARTED", C_OK);
}

void stopNRF24Scan() {
    nrf24ScanActive = false;
    showMsg("NRF24 SCAN STOPPED", C_INFO);
}

void runNRF24ScanLoop() {
    if (!nrf24ScanActive) return;
    static uint8_t ch = 0;
    radio.setChannel(ch);
    if (radio.testRPD()) {
        tft.fillRect(10, 100 + ch, 4, 1, C_ACCENT);
    }
    ch = (ch + 1) % 126;
}
