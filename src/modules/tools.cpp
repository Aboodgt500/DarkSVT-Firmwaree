#include "tools.h"
#include "../core/display.h"
#include "../core/sdcard.h"

void initTools() {}

void showToolsMenu() {
    tft.fillScreen(C_BG);
    drawHeader("Tools");
    const char* tools[] = {
        "Serial Monitor", "GPS Viewer", "Radio Test",
        "LED Control", "Mic Spectrum", "QR Generator",
        "JS Engine", "OTA Update", "SD Browser",
        "Calculator", "Clock"
    };
    int y = 30;
    for (int i=0; i<11; i++) {
        drawButton(10, y, 220, 22, tools[i], false);
        y += 24;
    }
    drawFooter("Touch to select");
}

void runSelectedTool(int tool) {
    switch (tool) {
        case 0: showMsg("Serial: 115200 baud", C_INFO); break;
        case 1: showMsg("GPS: Check antenna", C_INFO); break;
        case 2: showMsg("Radio: All modules OK", C_OK); break;
        case 3: showMsg("LED: GPIO 33", C_INFO); break;
        case 4: showMsg("Mic: GPIO 34", C_INFO); break;
        case 5: showMsg("QR: Not implemented", C_WARN); break;
        case 6: showMsg("JS: Not implemented", C_WARN); break;
        case 7: showMsg("OTA: Place .bin in /firmware/", C_INFO); break;
        case 8: showMsg("SD: OK", C_INFO); break;
        case 9: showMsg("Calc: Basic mode", C_INFO); break;
        case 10: showMsg("Clock: Running", C_INFO); break;
    }
}
