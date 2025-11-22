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

    struct Ride *new_ride = NULL;

    for (int i = 0; i < demand_number; i++)
    {
        demand = demands[i];

        if (new_ride == NULL)
        {
            new_ride = create_new_ride();

            add_ride_demand(new_ride, demand);
            add_ride_stops(new_ride, demand, speed);

            if (i == demand_number - 1)
            {
                insert_new(&scheduler, new_ride->stops, speed);
                // free(new_ride);
                new_ride = NULL;
                break;
            }

            continue;
        }

        int meets_distance_criteria = 1;

        for (int j = 0; j < new_ride->demand_number; j++)
        {
            if (
                (get_distance(demand.origin, new_ride->demands[j].origin) > max_origin_distance) ||
                (get_distance(demand.destination, new_ride->demands[j].destination) > max_destination_distance))
            {
                meets_distance_criteria = 0;
                break;
            }
        }

        struct Demand main_demand = new_ride->demands[0];

        bool should_stop = false;
        bool should_create_new_ride = false;

        if (i == demand_number - 1)
        {
            should_stop = true;
            should_create_new_ride = true;
        }
        else if (!meets_distance_criteria)
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
            if (efficiency < 0)
                printf("\nERRO: Quantidade de paradas invalida: %d", new_ride->stop_number);
            if (efficiency > 1)
                printf("\nERRO: Algo errado ocorreu no calculo de eficiencia: %lf", efficiency);

            if (efficiency < min_ride_efficiency)
            {
                should_stop = true;
                should_create_new_ride = true;

                remove_last_added_stops(new_ride);
                remove_last_added_demand(new_ride);

                should_create_new_ride = true;
            }
            else if (new_ride->demand_number == max_capacity)
            {
                should_stop = true;
            }
        }

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

    struct RideStop *stop = malloc(sizeof(RideStop));

    while (scheduler.size > 0)
    {
        get_next(&scheduler, stop);

        if (stop->next != NULL)
        {
            insert_new(&scheduler, stop->next, speed);
            continue;
        }

        double total_distance = get_ride_total_distance(*stop->ride);
        if (total_distance == -1)
            printf("\nERRO: Erro calculando distância total para impressao");

        Ride ride = *stop->ride;

        printf("\n%.2f %.2f %d", ride.end_time, total_distance, ride.stop_number);

        RideStop *ride_stop = ride.stops;
        while (ride_stop != NULL)
        {
            printf(" %.2f %.2f", ride_stop->x, ride_stop->y);

            RideStop *current = ride_stop;
            ride_stop = ride_stop->next;
            free(current);
        }

        free(stop->ride);
    }

    printf("\n");

    free(stop);
    free(new_ride);
    free(scheduler.stops);

    return 0;
}