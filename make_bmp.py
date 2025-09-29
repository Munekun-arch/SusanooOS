from PIL import Image

def make_bmp(filename, color):
    img = Image.new("RGB", (32, 32), color)
    img.save(filename, "BMP")

make_bmp("red32.bmp", (255, 0, 0))     # 赤
make_bmp("green32.bmp", (0, 255, 0))   # 緑
make_bmp("blue32.bmp", (0, 0, 255))    # 青

