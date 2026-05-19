#include "sdcard.h"

static bool sdReady = false;

bool initSDCard() {
    if (!SD.begin(5)) { sdReady = false; return false; }
    sdReady = true;
    SD.mkdir("/subghz");
    SD.mkdir("/eapol");
    SD.mkdir("/wardriving");
    SD.mkdir("/loot");
    SD.mkdir("/firmware");
    SD.mkdir("/ir");
    SD.mkdir("/ducky");
    SD.mkdir("/js");
    return true;
}

bool sdOK() { return sdReady; }
