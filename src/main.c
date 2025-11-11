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

    scanf("%d", &max_capacity);
    scanf("%lf", &speed);
    scanf("%lf", &max_departure_interval);
    scanf("%lf", &max_origin_distance);
    scanf("%lf", &max_destination_distance);
    scanf("%lf", &min_ride_efficiencty);
    scanf("%d", &demand_number);

    struct Demand demands[demand_number];
    struct Demand demand;

    struct MinHeap scheduler;
    initialize(&scheduler, demand_number);

    for (int i = 0; i < demand_number; i++)
    {
        scanf("%d %d %lf %lf %lf %lf",
              &demand.id,
              &demand.time,
              &demand.origin.x,
              &demand.origin.y,
              &demand.destination.x,
              &demand.destination.y);

        demands[i] = demand;
    }

    struct Ride new_ride;
    new_ride.demand_number = 0;

    for (int i = 0; i < demand_number; i++)
    {
        demand = demands[i];

        if (new_ride.demand_number == 0)
        {
            new_ride.demands[0] = demand;
            new_ride.demand_number = 1;

            continue;
        }

        // Flag to know if it meets the distance criterias
        int meets_distance_criteria = 1;

        for (int j = 0; j < new_ride.demand_number; j++)
        {
            if (
                (get_distance(demand.origin, new_ride.demands[j].origin) > max_origin_distance) ||
                (get_distance(demand.destination, new_ride.demands[j].destination) > max_destination_distance))
            {
                meets_distance_criteria = 0;
                break;
            }
        }

        struct Demand main_demand = new_ride.demands[0];

        if ((new_ride.demand_number > 0) &&
            (((meets_distance_criteria == 0) ||
              (new_ride.demand_number == max_capacity) || // TODO: Essa linha deve ir pro final do loop
              (double)(demand.time - main_demand.time) > max_departure_interval) ||
             (calculate_efficiency(new_ride, demand) < min_ride_efficiencty)))
        {
            // Doesn't respect the maximum distance criteria or the restriction for maximum interval betweend ride demands, so we finish this ride and create a new one

            insert_new(&scheduler, new_ride, speed);

            new_ride.demands[0] = demand;
            new_ride.demand_number = 1;

            continue;
        }

        new_ride.demands[new_ride.demand_number] = demand;
        new_ride.demand_number++;
    }

    // Account for the last ride
    insert_new(&scheduler, new_ride, speed);

    while (scheduler.size > 0)
    {
        struct Ride ride = get_next(&scheduler);

        double total_distance = get_ride_total_distance(ride);

        printf("\n%.2f %.2f %d ", ride.end_time, total_distance, 2 * ride.demand_number);

        printf("%.2f %.2f", ride.segments[0].start.x, ride.segments[0].start.y);
        for (int j = 0; j < ride.segment_number; j++)
            printf(" %.2f %.2f", ride.segments[j].end.x, ride.segments[j].end.y);
    }

    printf("\n");

    free(scheduler.rides);

    return 0;
}
