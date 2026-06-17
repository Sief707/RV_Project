import struct
import os
import sys
import subprocess
import re

# ── Update config.hpp ────────────────────────────
def update_config(W, H):
    path = "include/config.hpp"

    with open(path, 'r') as f:
        content = f.read()

    content = re.sub(
        r'(IMAGE_WIDTH\s*=\s*)\d+',
        f'IMAGE_WIDTH  = {W}',
        content)

    content = re.sub(
        r'(IMAGE_HEIGHT\s*=\s*)\d+',
        f'IMAGE_HEIGHT = {H}',
        content)

    with open(path, 'w') as f:
        f.write(content)

    print(f"Updated config.hpp → {W}x{H}")

# ── Ask user for input ───────────────────────────
print("═══════════════════════════════════")
print(" Image Preparation Tool")
print("═══════════════════════════════════")

image_path = input("Image path (e.g. images/lion.jpg): ").strip()

if not os.path.exists(image_path):
    print(f"ERROR: file not found: {image_path}")
    sys.exit(1)

W = int(input("Width  (e.g. 512): ").strip())
H = int(input("Height (e.g. 390): ").strip())

# ── Update config.hpp ────────────────────────────
update_config(W, H)

# ── Convert image to raw grayscale ───────────────
print(f"\nConverting {image_path} → images/input.raw ({W}x{H})")

subprocess.run([
    "convert",
    image_path,
    "-colorspace", "Gray",
    "-resize", f"{W}x{H}!",
    "-depth", "8",
    f"gray:images/input.raw"
])

def load_raw(filename, width, height):
    with open(filename, 'rb') as f:
        data = f.read(width * height)
    return list(data)

def save_ppm(filename, pixels, width, height):
    with open(filename, 'w') as f:
        f.write(f"P2\n{width} {height}\n255\n")
        for y in range(height):
            row = [str(pixels[y * width + x]) for x in range(width)]
            f.write(" ".join(row) + "\n")

# W, H = 512, 390

# Visualize all raw files in images/
files = [f for f in os.listdir("images/") if f.endswith(".raw")]

for raw_file in files:
    path = f"images/{raw_file}"
    ppm_path = path.replace(".raw", ".ppm")
    
    pixels = load_raw(path, W, H)
    save_ppm(ppm_path, pixels, W, H)
    print(f"Saved: {ppm_path}")