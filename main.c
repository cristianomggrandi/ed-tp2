#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"
#include "util.h"
#ifndef TYPES_H
#include "types.h"
#endif

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

    // struct Demand ride_demands[max_capacity];

    struct Ride rides[demand_number]; // Initialize all fields to 0
    for (int i = 0; i < demand_number; i++)
    {
        printf("Tem que ser sempre zero => %d\n", rides[i].demand_number);
    }

    // Counter for the number of demands on the current ride
    // int ride_demand_number = 0;

    // Counter for the number of combined rides
    int ride_number = 0;

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
    }

    fclose(file);

    for (int i = 0; i < demand_number; i++)
    {
        demand = demands[i];

        printf("\nTESTE 1: %d - %d", rides[ride_number].demand_number, demand.time);
        if (rides[ride_number].demand_number == 0)
        {
            rides[ride_number].demands[0] = demand;
            rides[ride_number].demand_number = 1;

            continue;
        }

        // Flag to know if it meets the distance criterias
        int meets_distance_criteria = 1;

        for (int j = 0; j < rides[ride_number].demand_number; j++)
        {
            printf("\nMAX DIST: %f - %f", max_origin_distance, max_destination_distance);
            printf("\nPontos: (%f, %f) (%f, %f)", demand.destination.x, demand.destination.y, rides[ride_number].demands[j].destination.x, rides[ride_number].demands[j].destination.y);
            printf("\nDIST: %f - %f", get_distance(demand.origin, rides[ride_number].demands[j].origin), get_distance(demand.destination, rides[ride_number].demands[j].destination));

            if (
                (get_distance(demand.origin, rides[ride_number].demands[j].origin) > max_origin_distance) ||
                (get_distance(demand.destination, rides[ride_number].demands[j].destination) > max_destination_distance))
            {
                meets_distance_criteria = 0;
                break;
            }
        }

        struct Demand main_demand = rides[ride_number].demands[0];
        if ((rides[ride_number].demand_number > 0) && ((rides[ride_number].demand_number == max_capacity) ||
                                                       (demand.time - main_demand.time) > max_departure_interval))
        {
            // Não respeita restrição de tempo máximo de intervalo de início de corrida, então deve finalizar a corrida atual e criar uma nova

            printf("\nNEW RIDE: %d -> %d", ride_number, ride_number + 1);
            ride_number++;
        }

        // TODO: Remover (é somente para log)
        if (meets_distance_criteria == 1)
        {
            double new_efficiency = calculate_efficiency(rides[ride_number], demand);
            printf("\nEfficiency: %f", new_efficiency);
        }

        if (meets_distance_criteria && calculate_efficiency(rides[ride_number], demand) > min_ride_efficiencty)
        {
            rides[ride_number].demands[rides[ride_number].demand_number] = demand;
            rides[ride_number].demand_number++;

            printf("\nADD: %d - %d", rides[ride_number].demand_number, demand.time);

            // ride_demand_number++;
        }
        else
        {
            printf("\nTESTE 5: %d - %d", rides[ride_number].demand_number, demand.time);
            ride_number++;
            // ride_demand_number = 0;
        }
    }

    for (int i = 0; i < ride_number; i++)
    {
        printf("\nRIDE %d: %d", i, rides[i].demand_number);
    }



    return 0;
}
