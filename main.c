#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"
#include "util.h"
#ifndef TYPES_H
#include "types.h"
#endif

#define MAX_SIZE 100

double calculate_efficiency();

// int main(int argc, char **argv)
int main()
{
    int max_capacity, demand_number;
    double speed, max_departure_interval, max_origin_distance, max_destination_distance, min_ride_efficiencty;

    FILE *file = fopen("inputs.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open inputs.txt\n");
        return 1;
    }

    fscanf(file, "%d", &max_capacity);
    fscanf(file, "%lf", &speed);
    fscanf(file, "%lf", &max_departure_interval);
    fscanf(file, "%lf", &max_origin_distance);
    fscanf(file, "%lf", &max_destination_distance);
    fscanf(file, "%lf", &min_ride_efficiencty);
    fscanf(file, "%d", &demand_number);

    // struct MinHeap min_heap;
    struct Demand demands[demand_number];
    struct Demand demand;
    struct Demand ride_demands[max_capacity];
    struct Ride rides[demand_number];
    int ride_demand_number = 0, ride_number = 0;

    // min_heap.size = 0;
    // min_heap.events = demands;

    for (int i = 0; i < demand_number; i++)
    {
        fscanf(file, "%d %d %lf %lf %lf %lf",
               &demand.id,
               &demand.time,
               &demand.origin.x,
               &demand.origin.y,
               &demand.destination.x,
               &demand.destination.y);

        demands[i] = demand;
        // insert_new(&min_heap, demand);
    }

    fclose(file);

    for (int i = 0; i < demand_number; i++)
    {
        demand = demands[i];

        // printf("\nTESTE 1: %d - %d", ride_demand_number, demand.time);
        if (ride_demand_number == 0)
        {
            ride_demands[ride_demand_number++] = demand;

            rides[ride_number].demands = ride_demands;
            rides[ride_number].demand_number = 1;
            ride_number++;

            continue;
        }

        struct Demand main_demand = ride_demands[0];

        if (ride_demand_number == max_capacity ||
            (demand.time - main_demand.time) > max_departure_interval
            // || (get_distance(demand.origin, main_demand.origin) > max_origin_distance) ||
            // (get_distance(demand.destination, main_demand.destination) > max_destination_distance)
        )
        {
            // TODO: Adicionar à lista de corridas

            printf("\nArray: (antes) ");
            for (int i = 0; i < ride_demand_number; i++)
                printf("%d.", ride_demands[i].id);
            printf("\n");

            // TODO?
            ride_demand_number = 0;
        }

        // TODO: Usar
        int meets_criteria = 1;

        for (int j = 0; j < ride_demand_number; j++)
        {
            if (
                (get_distance(demand.origin, ride_demands[j].origin) > max_origin_distance) ||
                (get_distance(demand.destination, ride_demands[j].destination) > max_destination_distance))
            {
                // TODO: Lidar com o caso de não ser compatível (criar nova corrida)
                meets_criteria = 0;
                break;
            }
        }

        double new_efficiency = calculate_efficiency(rides[ride_number - 1], demand);
        printf("\nEfficiency: %f", new_efficiency);

        if (new_efficiency > min_ride_efficiencty)
        {
            ride_demands[ride_demand_number + 1] = demand;
            ride_demand_number++;
        }

        // TODO: Lidar com caso em que não tem eficiência mínima (finalizar corrida e criar uma nova)
    }

    return 0;
}
