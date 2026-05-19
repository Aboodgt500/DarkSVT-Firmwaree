#!/usr/bin/env python3
# ============================================================
# Merge 4 BIN files into 1 FULL.bin (like HaleHound)
# Usage: python merge_bins.py output/
# ============================================================

import sys
import os
import struct

def merge_bins(output_dir):
    bootloader = os.path.join(output_dir, "bootloader.bin")
    partitions = os.path.join(output_dir, "partitions.bin")
    boot_app0 = os.path.join(output_dir, "boot_app0.bin")
    firmware = os.path.join(output_dir, "DarkSVT-Ultimate-CYD-HAT.bin")
    
    if not all(os.path.exists(f) for f in [bootloader, partitions, boot_app0, firmware]):
        print("ERROR: Missing BIN files!")
        return
    
    # Read all files
    with open(bootloader, "rb") as f: bl = f.read()
    with open(partitions, "rb") as f: pt = f.read()
    with open(boot_app0, "rb") as f: ba = f.read()
    with open(firmware, "rb") as f: fw = f.read()
    
    # Create full image (4MB flash)
    full = bytearray(4 * 1024 * 1024)
    
    # Place at correct offsets
    full[0x1000:0x1000+len(bl)] = bl
    full[0x8000:0x8000+len(pt)] = pt
    full[0xe000:0xe000+len(ba)] = ba
    full[0x10000:0x10000+len(fw)] = fw
    
    # Write output
    output = os.path.join(output_dir, "DarkSVT-Ultimate-CYD-HAT-FULL.bin")
    with open(output, "wb") as f:
        f.write(full)
    
    print(f"Created: {output}")
    print(f"Size: {len(full)} bytes (4MB)")
    print("Flash at address 0x0 using esptool or Web Flasher")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python merge_bins.py <output_directory>")
        sys.exit(1)
    merge_bins(sys.argv[1])
