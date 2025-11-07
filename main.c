#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"
#include "util.h"
#ifndef TYPES_H
#include "types.h"
#endif

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

    struct Demand demands[demand_number];
    struct Demand demand;

    struct Ride rides[demand_number];

    // Initialize every ride's demand counter to 0
    for (int i = 0; i < demand_number; i++)
        rides[i].demand_number = 0;

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
            if (
                (get_distance(demand.origin, rides[ride_number].demands[j].origin) > max_origin_distance) ||
                (get_distance(demand.destination, rides[ride_number].demands[j].destination) > max_destination_distance))
            {
                meets_distance_criteria = 0;
                break;
            }
        }

        struct Demand main_demand = rides[ride_number].demands[0];

        if ((rides[ride_number].demand_number > 0) &&
                ((meets_distance_criteria == 0) ||
                 (rides[ride_number].demand_number == max_capacity) || // TODO: Essa linha deve ir pro final do loop
                 (double)(demand.time - main_demand.time) > max_departure_interval) ||
            (calculate_efficiency(rides[ride_number], demand) < min_ride_efficiencty))
        {
            // Doesn't respect the maximum distance criteria or the restriction for maximum interval betweend ride demands, so we finish this ride and create a new one
            ride_number++;
            rides[ride_number].demands[0] = demand;
            rides[ride_number].demand_number = 1;

            continue;
        }

        rides[ride_number].demands[rides[ride_number].demand_number] = demand;
        rides[ride_number].demand_number++;

        // printf("\nADD: %d - %d", rides[ride_number].demand_number, demand.time);
    }

    // Account for the last ride
    ride_number++;

    for (int i = 0; i < ride_number; i++)
    {
        double total_distance = get_ride_total_distance(rides[i]);

        get_ride_segments(&(rides[i]));

        printf("\n%.2f %.2f %d ", rides[i].demands[0].time + (total_distance / speed), total_distance, 2 * rides[i].demand_number);

        printf("%.2f %.2f", rides[i].segments[0].start.x, rides[i].segments[0].start.y);
        for (int j = 0; j < rides[i].segment_number; j++)
            printf(" %.2f %.2f", rides[i].segments[j].end.x, rides[i].segments[j].end.y);
    }
    printf("\n");

    return 0;
}
