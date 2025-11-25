import random
import numpy as np

def create_input_file(num_demands, filename):
    # Read the current input_100.txt
    with open('entradas-saidas/input_100.txt', 'r') as f:
        lines = f.readlines()

    # Parse existing demands
    demands = []
    for line in lines:
        parts = line.strip().split()
        if len(parts) == 6:
            demands.append(parts)

    # Create input file
    with open(f'entradas-saidas/{filename}', 'w') as f:
        # Write first 100 demands from input_100
        # for demand in demands:
        #     f.write(f"{demand[0]} {demand[1]} {demand[2]} {demand[3]} {demand[4]} {demand[5]}\n")
        
        # Generate remaining demands
        base_coords = [
            (0, 0), (0 , 100), (0, -100),
            (100, 0), (100, 100), (100, -100),
            (-100, 0), (-100, 100), (-100, -100)
        ]
        
        timestamp = 0

        for i in range(num_demands):
            demand_id = 100 + i
            timestamp += int(random.uniform(1, 6))
            
            # Pick random base coordinates and add variation
            origin_base = random.choice(base_coords)
            dest_base = random.choice(base_coords)
            
            origin_x = origin_base[0] + np.random.normal(0, 50)
            origin_y = origin_base[1] + np.random.normal(0, 50)
            dest_x = dest_base[0] + np.random.normal(0, 50)
            dest_y = dest_base[1] + np.random.normal(0, 50)
            
            f.write(f"{demand_id} {timestamp} {origin_x:.5f} {origin_y:.5f} {dest_x:.5f} {dest_y:.5f}\n")

    print(f"Created {filename} with {num_demands} demands 2 ")

# Create files
create_input_file(500, "input_500.txt")
create_input_file(1000, "input_1000.txt")
