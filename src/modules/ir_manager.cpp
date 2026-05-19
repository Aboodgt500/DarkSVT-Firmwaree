#include "ir_manager.h"
#include "../core/display.h"
#include <IRremote.hpp>

bool tvBGoneActive = false;

struct IRCode {
    uint32_t code;
    uint8_t bits;
    uint16_t protocol;
};

const IRCode TV_CODES[] = {
    {0xE0E040BF, 32, NEC},
    {0x20DF10EF, 32, NEC},
    {0xA90, 12, SONY},
    {0x40040100BCBD, 48, PANASONIC},
    {0x0C, 12, PHILIPS},
    {0x2FD48B7, 32, NEC},
    {0x5EA1, 15, NEC},
    {0x20DF10EF, 32, NEC},
    {0x4FB40BF, 32, NEC},
    {0xE0E019E6, 32, NEC},
    {0x20DFA35C, 32, NEC},
    {0xA80, 12, SONY},
    {0xAF5, 12, SONY},
    {0xFB38C7, 32, NEC},
    {0xE0E080DF, 32, NEC},
    {0x10EF08F7, 32, NEC},
    {0xE117E817, 32, NEC},
    {0x00FF48B7, 32, NEC},
    {0x20DFC23D, 32, NEC},
    {0xE0E09966, 32, NEC},
};

const int TV_CODE_COUNT = sizeof(TV_CODES) / sizeof(TV_CODES[0]);

void initIR() {
    IrSender.begin(25);
}

void startTVBGone() {
    tvBGoneActive = true;
    showMsg("TV-B-GONE STARTED", C_OK);
}

void stopTVBGone() {
    tvBGoneActive = false;
    showMsg("TV-B-GONE STOPPED", C_INFO);
}

void runTVBGoneLoop() {
    if (!tvBGoneActive) return;
    for (int i=0; i<TV_CODE_COUNT; i++) {
        if (!tvBGoneActive) break;
        const IRCode* c = &TV_CODES[i];
        switch (c->protocol) {
            case NEC:
                IrSender.sendNEC(c->code, c->bits);
                break;
            case SONY:
                IrSender.sendSony(c->code, c->bits);
                break;
            case PANASONIC:
                IrSender.sendPanasonic(c->code >> 16, c->code & 0xFFFF);
                break;
            default:
                IrSender.sendNEC(c->code, c->bits);
                break;
        }
        delay(100);
    }
    delay(500);
}

void sendIRCode(uint32_t code, uint8_t protocol) {
    switch (protocol) {
        case 0: IrSender.sendNEC(code, 32); break;
        case 1: IrSender.sendSony(code, 12); break;
        case 2: IrSender.sendRC5(code, 12); break;
        case 3: IrSender.sendSamsung(code, 32); break;
        default: IrSender.sendNEC(code, 32); break;
    }
}

void sendIRRaw(const uint16_t* timings, uint8_t len) {
    IrSender.sendRaw(timings, len, 38);
}
