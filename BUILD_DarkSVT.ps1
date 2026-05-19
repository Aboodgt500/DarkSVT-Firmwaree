# ============================================================
# DarkSVT Ultimate v2.0 Builder
# Step-by-step for beginners
# ============================================================

$ErrorActionPreference = "Stop"
$WorkDir = "$env:USERPROFILE\DarkSVT_Build_v2"
$OutputDir = "$WorkDir\output"

function Write-Color($Text, $Color="White") {
    Write-Host $Text -ForegroundColor $Color
}

Write-Host "========================================" -ForegroundColor Red
Write-Host "  DarkSVT Ultimate v2.0 Builder" -ForegroundColor Red
Write-Host "  2026 Edition | Full Logic" -ForegroundColor Red
Write-Host "========================================" -ForegroundColor Red

# STEP 1: Check Python
Write-Color "[STEP 1/7] Checking Python..." "Cyan"
$py = Get-Command python -ErrorAction SilentlyContinue
if (-not $py) {
    Write-Color "ERROR: Python not found!" "Red"
    Write-Color "Install Python 3.10-3.13 from python.org" "Yellow"
    Write-Color "IMPORTANT: Check 'Add Python to PATH' during install" "Yellow"
    exit 1
}
$ver = & python --version 2>&1
Write-Color "  Found: $ver" "Green"

# STEP 2: Check Git
Write-Color "[STEP 2/7] Checking Git..." "Cyan"
$git = Get-Command git -ErrorAction SilentlyContinue
if (-not $git) {
    Write-Color "WARNING: Git not found. Some libraries may fail." "Yellow"
}

# STEP 3: Clean workspace
Write-Color "[STEP 3/7] Preparing workspace..." "Cyan"
if (Test-Path $WorkDir) {
    Remove-Item -Recurse -Force $WorkDir -ErrorAction SilentlyContinue
}
New-Item -ItemType Directory -Path $WorkDir | Out-Null
New-Item -ItemType Directory -Path $OutputDir | Out-Null

# STEP 4: Copy source
Write-Color "[STEP 4/7] Copying source files..." "Cyan"
$src = Split-Path $MyInvocation.MyCommand.Path -Parent
Copy-Item -Recurse "$src\src" "$WorkDir\src" -Force
Copy-Item "$src\platformio.ini" "$WorkDir\platformio.ini" -Force
Copy-Item "$src\partitions.csv" "$WorkDir\partitions.csv" -Force
Write-Color "  Source copied." "Green"

# STEP 5: Install PlatformIO
Write-Color "[STEP 5/7] Installing PlatformIO..." "Cyan"
Write-Color "  This may take 2-3 minutes..." "Yellow"
& python -m pip install --upgrade pip
& python -m pip install platformio
if ($LASTEXITCODE -ne 0) {
    Write-Color "Trying with --user flag..." "Yellow"
    & python -m pip install --user platformio
}

# STEP 6: BUILD
Write-Color "[STEP 6/7] Building firmware..." "Cyan"
Write-Color "  THIS TAKES 10-20 MINUTES!" "Yellow"
Write-Color "  Do NOT close this window!" "Yellow"
Set-Location $WorkDir
& pio run -e esp32-cyd-hat
if ($LASTEXITCODE -ne 0) {
    Write-Color "BUILD FAILED!" "Red"
    Write-Color "Common fixes:" "Yellow"
    Write-Color "  1. Check Python version (must be 3.10-3.13)" "White"
    Write-Color "  2. Run: pio run -e esp32-cyd-hat --verbose" "White"
    Write-Color "  3. Check internet connection" "White"
    exit 1
}

# STEP 7: Extract files
Write-Color "[STEP 7/7] Extracting BIN files..." "Cyan"
$buildDir = "$WorkDir\.pio\build\esp32-cyd-hat"
$files = @(
    @{Name="bootloader.bin"; Addr="0x1000"},
    @{Name="partitions.bin"; Addr="0x8000"},
    @{Name="boot_app0.bin"; Addr="0xe000"},
    @{Name="firmware.bin"; Dest="DarkSVT-Ultimate-CYD-HAT.bin"; Addr="0x10000"}
)
foreach ($f in $files) {
    $srcFile = Get-ChildItem -Path $buildDir -Filter $f.Name -Recurse | Select-Object -First 1
    if ($srcFile) {
        $dest = if ($f.Dest) { $f.Dest } else { $f.Name }
        Copy-Item $srcFile.FullName "$OutputDir\$dest" -Force
        Write-Color "  [OK] $dest -> $($f.Addr)" "Green"
    } else {
        Write-Color "  [MISSING] $($f.Name)" "Red"
    }
}

# Create FULL.bin
Write-Color "Creating FULL.bin (single file flash)..." "Cyan"
& python "$src\merge_bins.py" "$OutputDir"

# Create flash scripts
$flashBat = @'
@echo off
echo ============================================
echo   DarkSVT Ultimate v2.0 - Flash Tool
echo   2026 Edition | Gulf Region
echo ============================================
set /p PORT="Enter COM port (default COM4): "
if "%PORT%"=="" set PORT=COM4
echo.
echo Method 1: 4-Files (Recommended)
echo.
esptool.py --chip esp32 --baud 115200 --port %PORT% write_flash ^
  0x1000   bootloader.bin ^
  0x8000   partitions.bin ^
  0xe000   boot_app0.bin ^
  0x10000  DarkSVT-Ultimate-CYD-HAT.bin
echo.
echo OR Method 2: Single File (Easier)
echo.
esptool.py --chip esp32 --baud 115200 --port %PORT% write_flash 0x0 DarkSVT-Ultimate-CYD-HAT-FULL.bin
echo.
echo DONE! Unplug and reconnect USB.
pause
'@
Set-Content -Path "$OutputDir\FLASH.bat" -Value $flashBat

$webHtml = @'
<!DOCTYPE html>
<html>
<head><title>DarkSVT Flash Helper</title></head>
<body style="background:#0a0a0a;color:#fff;font-family:monospace;text-align:center;padding:40px;">
<h1 style="color:#ff0040;">DarkSVT Ultimate v2.0</h1>
<p>2026 Edition | 63+ Tools | Gulf Region</p>
<hr style="border-color:#ff0040;">
<p>1. Open <a href="https://esp.huhn.me" style="color:#ff3366;">esp.huhn.me</a></p>
<p>2. Choose method:</p>
<h3 style="color:#ff3366;">Method A: 4 Files (Best)</h3>
<table style="margin:10px auto;color:#888;">
<tr><td>0x1000</td><td>bootloader.bin</td></tr>
<tr><td>0x8000</td><td>partitions.bin</td></tr>
<tr><td>0xe000</td><td>boot_app0.bin</td></tr>
<tr><td>0x10000</td><td>DarkSVT-Ultimate-CYD-HAT.bin</td></tr>
</table>
<h3 style="color:#ff3366;">Method B: 1 File (Easy)</h3>
<table style="margin:10px auto;color:#888;">
<tr><td>0x0</td><td>DarkSVT-Ultimate-CYD-HAT-FULL.bin</td></tr>
</table>
<p>3. Click Program and wait 2-3 minutes.</p>
<hr style="border-color:#ff0040;">
<p style="font-size:0.8em;color:#666;">DarkSVT Team | Educational Use Only</p>
</body>
</html>
'@
Set-Content -Path "$OutputDir\WebFlash.html" -Value $webHtml

$readme = "============================================`n" +
           "  DarkSVT Ultimate v2.0 Firmware`n" +
           "  2026 Edition | 63+ Tools | Gulf Region`n" +
           "============================================`n`n" +
           "FILES:`n" +
           "  bootloader.bin                    -> 0x1000`n" +
           "  partitions.bin                    -> 0x8000`n" +
           "  boot_app0.bin                     -> 0xe000`n" +
           "  DarkSVT-Ultimate-CYD-HAT.bin      -> 0x10000`n" +
           "  DarkSVT-Ultimate-CYD-HAT-FULL.bin -> 0x0 (single file)`n`n" +
           "FLASH METHODS:`n" +
           "1. ESPTOOL (Command Line):`n" +
           "   FLASH.bat (edit COM port)`n`n" +
           "2. Web Flasher (Browser):`n" +
           "   Open WebFlash.html -> Click link -> esp.huhn.me`n" +
           "   Upload files with addresses above`n`n" +
           "3. SD Card OTA:`n" +
           "   Copy .bin to /firmware/ on SD`n" +
           "   Device: Tools -> OTA Update`n`n" +
           "FEATURES (63+ tools with REAL logic):`n" +
           "WiFi (3):`n" +
           "  - Deauther (raw 802.11 frame injection)`n" +
           "  - Beacon Spam (29 fake SSIDs + RickRoll)`n" +
           "  - Scanner (list nearby APs)`n" +
           "Bluetooth (1):`n" +
           "  - BLE Spam (AirPods/Samsung/Google Fast Pair)`n" +
           "SubGHz (4):`n" +
           "  - Replay Attack (record + replay RF signals)`n" +
           "  - Tesla Charge Opener (315/433 MHz)`n" +
           "  - Jammer (sweep 315/433/868/915 MHz)`n" +
           "  - 50+ Gulf Frequencies database`n" +
           "2.4GHz (1):`n" +
           "  - NRF24 Scanner (126 channel sweep)`n" +
           "RFID (1):`n" +
           "  - NFC Scanner (MFRC522 UID + type)`n" +
           "IR (1):`n" +
           "  - TV-B-Gone (20 TV brands)`n" +
           "Tools (11):`n" +
           "  - Serial, GPS, Radio Test, LED, Mic, QR, JS, OTA, SD, Calc, Clock`n" +
           "Jam Detect (1):`n" +
           "  - WiFi Guardian + SubGHz Sentinel`n" +
           "UI:`n" +
           "  - Touch menu (8 categories)`n" +
           "  - Splash screen + About screen`n" +
           "  - SD auto-directories`n`n" +
           "HARDWARE:`n" +
           "  - ESP32-2432S028 (CYD) or NM-RF-HAT`n" +
           "  - CC1101 (SubGHz 300-928 MHz)`n" +
           "  - NRF24L01+PA+LNA (2.4GHz)`n" +
           "  - MFRC522 (NFC/RFID)`n" +
           "  - IR LED on GPIO 25`n" +
           "  - Optional: GPS, LED strip, iButton`n`n" +
           "GULF FREQUENCIES (50+):`n" +
           "  304.25, 310, 315, 318, 330, 345, 390,`n" +
           "  418, 433.07, 433.92, 434.42, 447, 462,`n" +
           "  490, 868.0, 868.1, 868.3, 868.4, 868.5,`n" +
           "  915, 921, 928 MHz + more`n`n" +
           "Build Date: " + (Get-Date -Format "yyyy-MM-dd HH:mm")
Set-Content -Path "$OutputDir\README.txt" -Value $readme

Write-Color "`nBUILD COMPLETE!" "Green"
Write-Color "Output folder: $OutputDir" "Cyan"
Get-ChildItem $OutputDir | ForEach-Object { Write-Color "  $($_.Name)" "White" }
Write-Color "`nNext steps:" "Yellow"
Write-Color "  1. Connect your NM-RF-HAT via USB" "White"
Write-Color "  2. Run FLASH.bat OR open WebFlash.html" "White"
Write-Color "  3. Flash and power cycle" "White"
pause
