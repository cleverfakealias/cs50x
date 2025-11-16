from cs50 import get_int

while True:
    height = get_int("Height: ")
    if 1 <= height <= 8:
        break

for row in range(1, height + 1):
    spaces = (height - row)
    hashes = row

    # left side of the pyramid
    left_side = " " * spaces + "#" * hashes
    right_side = "#" * hashes
    print(left_side + "  " + right_side)
