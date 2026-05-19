#include "rfid_scanner.h"
#include "../core/display.h"
#include <MFRC522.h>

MFRC522 mfrc522(13, 255);
bool nfcScanActive = false;

void initRFID() {
    SPI.begin();
    mfrc522.PCD_Init();
}

void startNFCScan() {
    nfcScanActive = true;
    showMsg("NFC SCAN STARTED", C_OK);
}

void stopNFCScan() {
    nfcScanActive = false;
    showMsg("NFC SCAN STOPPED", C_INFO);
}

void runNFCScanLoop() {
    if (!nfcScanActive) return;
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        tft.fillScreen(C_BG);
        drawHeader("NFC Tag Found");
        tft.setTextColor(C_OK, C_BG);
        tft.setCursor(10, 40);
        tft.print("UID: ");
        for (byte i=0; i<mfrc522.uid.size; i++) {
            tft.printf("%02X", mfrc522.uid.uidByte[i]);
        }
        tft.setCursor(10, 60);
        tft.printf("Type: %s", mfrc522.PICC_GetTypeName(mfrc522.PICC_GetType(mfrc522.uid.sak)));
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        delay(2000);
    }
}
