import os, sys
from PIL import Image

def upscale_directory(src_dir, dst_dir, scale=4):
    os.makedirs(dst_dir, exist_ok=True)
    for fn in os.listdir(src_dir):
        if not fn.lower().endswith('.png'):
            continue
        src_path = os.path.join(src_dir, fn)
        img = Image.open(src_path).convert('RGBA')
        new_size = (img.width*scale, img.height*scale)
        upscaled = img.resize(new_size, Image.NEAREST)
        dst_path = os.path.join(dst_dir, fn)
        upscaled.save(dst_path)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: texture_upscale.py dump_dir hd_dir [scale]')
        sys.exit(1)
    scale = int(sys.argv[3]) if len(sys.argv) >=4 else 4
    upscale_directory(sys.argv[1], sys.argv[2], scale)
