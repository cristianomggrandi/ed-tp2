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

            new_ride->demands[0] = demand;
            new_ride->demand_number = 1;

            add_ride_stops(new_ride, demand);

            if (i == demand_number - 1)
                insert_new(&scheduler, &new_ride, speed);

            continue;
        }

        // Flag to know if it meets the distance criterias
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

        if (!meets_distance_criteria)
        {
            // printf("\n%d => !meets_distance_criteria", i);
            should_stop = true;
        }
        else if ((double)(demand.time - main_demand.time) > max_departure_interval)
        {
            // printf("\n%d => (double)(demand.time - main_demand.time) > max_departure_interval", i);
            should_stop = true;
        }
        else
        {
            add_ride_stops(new_ride, demand);

            new_ride->demands[new_ride->demand_number] = demand;
            new_ride->demand_number++;

            double efficiency = calculate_ride_efficiency(*new_ride);
            if (efficiency < 0)
                printf("\nERRO: Quantidade de paradas invalida: %d", new_ride->stop_number);
            if (efficiency > 1)
                printf("\nERRO: Algo errado ocorreu no calculo de eficiencia: %lf", efficiency);

            if (efficiency < min_ride_efficiency)
            {
                should_stop = true;
                // printf("\n%d -> Removendo demanda: %d / %d", scheduler.size, demand.id, demand.time);
                remove_last_added_stops(new_ride);
                new_ride->demand_number--;
            }
            else if (new_ride->demand_number == max_capacity)
            {
                // printf("\n%d => new_ride->demand_number == max_capacity", i);
                should_stop = true;
            }
        }

        if (should_stop)
        {
            // TODO: Traduzir esse e todos os outros comentários para o português
            // Doesn't respect the maximum distance criteria or the restriction for maximum interval betweend ride demands, so we finish this ride and create a new one
            insert_new(&scheduler, &new_ride, speed);
            i--;

            continue;
        }

        new_ride->demands[new_ride->demand_number] = demand;
        new_ride->demand_number++;
    }

    while (scheduler.size > 0)
    {
        struct Ride ride = get_next(&scheduler);

        double total_distance = get_ride_total_distance(ride);
        if (total_distance == -1)
            printf("\nERRO: Erro calculando distância total para impressao");

        printf("\n%.2f %.2f %d", ride.end_time, total_distance, ride.stop_number);

        RideStop *stop = ride.stops;
        while (stop != NULL)
        {
            printf(" %.2f %.2f", stop->x, stop->y);
            stop = stop->next;
        }
    }

    printf("\n");

    free(new_ride);
    free(scheduler.rides);

    return 0;
}

// TODO: Alterar minheap para usar eventos e não corridas