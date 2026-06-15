import struct
import os

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

W, H = 512, 512

# Visualize all raw files in images/
files = [f for f in os.listdir("images/") if f.endswith(".raw")]

for raw_file in files:
    path = f"images/{raw_file}"
    ppm_path = path.replace(".raw", ".ppm")
    
    pixels = load_raw(path, W, H)
    save_ppm(ppm_path, pixels, W, H)
    print(f"Saved: {ppm_path}")