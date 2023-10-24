# thanks chatgpt

from PIL import Image
import sys

def print_pixel_rgb(image_path):
    # Open the image
    image = Image.open(image_path)

    # Convert the image to RGB mode (in case it's in a different mode)
    image = image.convert("RGB")

    # Get the width and height of the image
    width, height = image.size


    # Iterate over each pixel and print its RGB values
    with open("outp.txt" , "w") as f:
        for y in range(height):
            for x in range(width):
                r, g, b = image.getpixel((x, y))
                f.write(f"{r} {g} {b}\n")
    # Close the image
    image.close()

# Example usage
image_path = sys.argv[1]
print_pixel_rgb(image_path)
