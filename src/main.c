#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minheap.h"
#include "util.h"
#ifndef TYPES_H
#include "types.h"
#endif
#include "ride.h"

int main()
{
    int max_capacity, demand_number;
    double speed, max_departure_interval, max_origin_distance, max_destination_distance, min_ride_efficiency;

    scanf("%d", &max_capacity);
    scanf("%lf", &speed);
    scanf("%lf", &max_departure_interval);
    scanf("%lf", &max_origin_distance);
    scanf("%lf", &max_destination_distance);
    scanf("%lf", &min_ride_efficiency);
    scanf("%d", &demand_number);

    struct MinHeap scheduler;
    initialize(&scheduler, demand_number);

    struct Ride *new_ride = NULL;

    for (int i = 0; i < demand_number; i++)
    {
        struct Demand demand;
        demand.type = DEMANDED;

        scanf("%d %d %lf %lf %lf %lf",
              &demand.id,
              &demand.time,
              &demand.origin.x,
              &demand.origin.y,
              &demand.destination.x,
              &demand.destination.y);

        if (new_ride == NULL)
        {
            new_ride = create_new_ride();

            add_ride_demand(new_ride, demand);
            add_ride_stops(new_ride, demand, speed);

            if (i == demand_number - 1)
            {
                insert_new(&scheduler, new_ride->stops, speed);
                new_ride = NULL;
                break;
            }

            continue;
        }

        struct Demand main_demand = new_ride->demands[0];

        bool should_stop = false;
        bool should_create_new_ride = false;

        if (!meets_distance_criteria(new_ride, demand, max_origin_distance, max_destination_distance))
        {
            should_stop = true;
            should_create_new_ride = true;
        }
        else if ((double)(demand.time - main_demand.time) > max_departure_interval)
        {
            should_stop = true;
            should_create_new_ride = true;
        }
        else
        {
            add_ride_demand(new_ride, demand);
            add_ride_stops(new_ride, demand, speed);

            double efficiency = calculate_ride_efficiency(*new_ride);

            if (efficiency < min_ride_efficiency)
            {
                should_stop = true;
                should_create_new_ride = true;

                remove_last_added_stops(new_ride);
                remove_last_added_demand(new_ride);
            }
            else if (new_ride->demand_number == max_capacity)
            {
                should_stop = true;
            }
        }

        if (i == demand_number - 1)
            should_stop = true;

        if (should_stop)
        {
            insert_new(&scheduler, new_ride->stops, speed);

            new_ride = NULL;

            if (should_create_new_ride)
            {
                new_ride = create_new_ride();

                add_ride_demand(new_ride, demand);
                add_ride_stops(new_ride, demand, speed);

                if (i == demand_number - 1)
                {
                    insert_new(&scheduler, new_ride->stops, speed);
                    new_ride = NULL;
                }
            }

            continue;
        }
    }

    finalize(&scheduler, speed);

    free(new_ride);

    return 0;
}