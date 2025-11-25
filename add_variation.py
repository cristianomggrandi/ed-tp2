import random

with open('entradas-saidas/input_100.txt', 'r') as f:
    lines = f.readlines()

with open('entradas-saidas/input_100.txt', 'w') as f:
    for i, line in enumerate(lines):
        if i < 7:
            f.write(line)
        else:
            parts = line.strip().split()
            if len(parts) == 6:
                demand_id = parts[0]
                timestamp = parts[1]
                origin_x = float(parts[2]) + random.uniform(-30, 30)
                origin_y = float(parts[3]) + random.uniform(-30, 30)
                dest_x = float(parts[4]) + random.uniform(-30, 30)
                dest_y = float(parts[5]) + random.uniform(-30, 30)
                f.write(f"{demand_id} {timestamp} {origin_x:.5f} {origin_y:.5f} {dest_x:.5f} {dest_y:.5f}\n")

print("Coordinates updated with random variations between -30 and 30")
