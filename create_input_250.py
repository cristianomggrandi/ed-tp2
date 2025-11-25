import random

# Read the current input_100.txt
with open('entradas-saidas/input_100.txt', 'r') as f:
    lines = f.readlines()

# Parse existing demands
demands = []
for line in lines:
    parts = line.strip().split()
    if len(parts) == 6:
        demands.append(parts)

# Create input_250.txt
with open('entradas-saidas/input_250.txt', 'w') as f:
    # Write first 100 demands from input_100
    for demand in demands:
        f.write(f"{demand[0]} {demand[1]} {demand[2]} {demand[3]} {demand[4]} {demand[5]}\n")
    
    # Generate 150 more demands
    base_coords = [
        (598.84786, 7789.96104), (611.99281, 7800.62069), (607.15222, 7805.69080),
        (608.43885, 7794.22339), (612.00284, 7800.73428), (607.13877, 7808.96927),
        (612.57422, 7805.19963), (607.16430, 7808.88753), (601.86546, 7789.64019),
        (607.21812, 7805.68399)
    ]
    
    for i in range(150):
        demand_id = 100 + i
        timestamp = 530 + i * 3
        
        # Pick random base coordinates and add variation
        origin_base = random.choice(base_coords)
        dest_base = random.choice(base_coords)
        
        origin_x = origin_base[0] + random.uniform(-100, 100)
        origin_y = origin_base[1] + random.uniform(-100, 100)
        dest_x = dest_base[0] + random.uniform(-100, 100)
        dest_y = dest_base[1] + random.uniform(-100, 100)
        
        f.write(f"{demand_id} {timestamp} {origin_x:.5f} {origin_y:.5f} {dest_x:.5f} {dest_y:.5f}\n")

print("Created input_250.txt with 250 demands")
