#!/usr/bin/env python3
"""
Split an up-scaled PNG into DS-sized tiles and name them so
melonDS HD replacement code can pick them up automatically.

    python split_hd_texture.py --png input.png
                               --native 32 32
                               --crc DEADBEEF
                               --outdir UserData/HD_Textures/replace/
"""
import argparse, os
from pathlib import Path
from PIL import Image

ap = argparse.ArgumentParser()
ap.add_argument('--png', required=True)
ap.add_argument('--native', nargs=2, type=int, required=True)
ap.add_argument('--crc', required=True)
ap.add_argument('--outdir', required=True)
args = ap.parse_args()

upscale = Image.open(args.png).convert('RGBA')
nat_w, nat_h = args.native
scale = upscale.width // nat_w
assert upscale.width % nat_w == 0 and upscale.height % nat_h == 0, "Size mismatch"

native = upscale.resize((nat_w, nat_h), Image.BOX)
file = f"tex_{args.crc}_{nat_w}x{nat_h}.png"
Path(args.outdir).mkdir(parents=True, exist_ok=True)
native.save(Path(args.outdir)/file)
print("Wrote", file)
