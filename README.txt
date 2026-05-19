============================================
  DarkSVT Ultimate v2.0 - Build Ready
  2026 Edition | 63+ Tools | Gulf Region
============================================

IMPORTANT: This is SOURCE CODE.
You MUST build it on your PC to get .bin files.

WHAT'S NEW IN v2.0:
-------------------
  - Updated for 2026 libraries (SmartRC v3.0.0, NimBLE 1.4.2)
  - Real logic (not stubs) for core tools
  - partitions.csv included (was missing in v1.0!)
  - merge_bins.py to create FULL.bin like HaleHound
  - Step-by-step builder for beginners
  - Based on ESP32-DIV + Bruce open-source code

REAL IMPLEMENTATIONS:
---------------------
  WiFi Deauther:     esp_wifi_80211_tx raw frames
  Beacon Spam:       29 SSIDs + RickRoll lyrics
  BLE Spam:          NimBLE AirPods/Samsung/Google
  CC1101 Replay:     SmartRC v3 OOP API
  Tesla Opener:      Static payload 315/433 MHz
  SubGHz Jammer:     Sweep 315->433->868->915
  50+ Frequencies:   Gulf/Oman/UAE/Saudi/Qatar/Kuwait/Bahrain
  NRF24 Scanner:     126 channel sweep + RPD
  TV-B-Gone:         20 brands (NEC/Sony/Panasonic/Philips)
  IR Transmitter:    NEC/Sony/RC5/Samsung protocols
  NFC Scanner:       MFRC522 UID + type detection
  Jam Detect:        WiFi disconnect + SubGHz RSSI
  Touch Menu:        8 categories + submenus
  Splash Screen:     Animated progress bar

HOW TO BUILD (7 STEPS):
-----------------------
STEP 1: Install Python 3.10-3.13
  https://python.org
  IMPORTANT: Check 'Add Python to PATH'
  IMPORTANT: Do NOT use Python 3.14!

STEP 2: Install Git (optional but recommended)
  https://git-scm.com/download/win

STEP 3: Extract this ZIP to a folder
  Example: C:\Users\Asus\DarkSVT_v2\

STEP 4: Right-click BUILD_DarkSVT.ps1
  Select 'Run with PowerShell'
  If blocked: Run as Administrator

STEP 5: Wait 10-20 minutes
  First build downloads libraries (~2GB)
  Do NOT close the window!

STEP 6: Find output files
  C:\Users\Asus\DarkSVT_Build_v2\output\
  Files:
    bootloader.bin
    partitions.bin
    boot_app0.bin
    DarkSVT-Ultimate-CYD-HAT.bin
    DarkSVT-Ultimate-CYD-HAT-FULL.bin  <-- Single file!
    FLASH.bat
    WebFlash.html
    README.txt

STEP 7: Flash to your device
  Method A: Double-click FLASH.bat
  Method B: Open WebFlash.html -> esp.huhn.me
  Method C: SD Card OTA

TROUBLESHOOTING:
----------------
  'Python not found':
    - Reinstall Python and check 'Add to PATH'
    - Restart PowerShell after install

  'Build failed':
    - Check Python version: python --version
    - Must be 3.10, 3.11, 3.12, or 3.13
    - Run: pio run -e esp32-cyd-hat --verbose
    - Check internet connection

  'pio not found':
    - Restart PowerShell after PlatformIO install
    - Or run: $env:Path += ';C:\Users\YOU\.platformio\penv\Scripts'

  Black screen after flash:
    - Use 4-file method instead of FULL.bin
    - Check COM port in Device Manager
    - Install CH340 driver

  'No port found' in Web Flasher:
    - Install CH340 driver: http://www.wch-ic.com/downloads/CH341SER_ZIP.html
    - Use USB-DATA cable (not charger-only)
    - Try different USB port

ESPTOOL COMMANDS:
-----------------
  4-Files Method (Recommended):
    esptool.py --chip esp32 --port COM4 write_flash \
      0x1000 bootloader.bin \
      0x8000 partitions.bin \
      0xe000 boot_app0.bin \
      0x10000 DarkSVT-Ultimate-CYD-HAT.bin

  Single File Method (Easier):
    esptool.py --chip esp32 --port COM4 write_flash 0x0 \
      DarkSVT-Ultimate-CYD-HAT-FULL.bin

  Erase Flash (if bricked):
    esptool.py --chip esp32 --port COM4 erase_flash

NOTE:
-----
This project is based on open-source code from ESP32-DIV
and Bruce firmware. HaleHound-CYD source code was removed
from GitHub in 2026 and is no longer available.

Educational/Research Use Only.
