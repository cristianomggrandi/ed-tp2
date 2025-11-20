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
        // printf("\nNEWRIDE: %p", new_ride);

        demand = demands[i];
        // printf("\nDEMAND %d: %p => %d", i, new_ride, demand.time);

        if (new_ride == NULL)
        {
            new_ride = create_new_ride();

            // new_ride->demands[0] = demand;
            // new_ride->demand_number = 1;
            add_ride_demand(new_ride, demand);
            add_ride_stops(new_ride, demand, speed);

            if (i == demand_number - 1)
            {
                // printf("\nINSERT FORA 1: %p -> %lf => %lf e %lf", new_ride->stops, new_ride->stops->time, new_ride->stops->distance, new_ride->stops->distance);
                insert_new(&scheduler, new_ride->stops, speed);
                new_ride = NULL;
                break;
            }

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
            add_ride_demand(new_ride, demand);
            add_ride_stops(new_ride, demand, speed);

            // new_ride->demands[new_ride->demand_number] = demand;
            // new_ride->demand_number++;

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
                remove_last_added_demand(new_ride);
                i--;
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
            // printf("\nINSERT FORA 2: %p -> %lf => %lf e %lf", new_ride->stops, new_ride->stops->time, new_ride->stops->distance, new_ride->stops->distance);
            insert_new(&scheduler, new_ride->stops, speed);
            new_ride = NULL;

            // TODO: Era isso aqui o tempo todo?
            // TODO: Conferir se precisa
            // i--;

            continue;
        }

        // add_ride_demand(new_ride, demand);
    }

    // for (int i = 0; i < scheduler.size; i++)
    // {
    //     printf("\nSCHED.: %lf, %lf", scheduler.stops[i].x, scheduler.stops[i].y);

    //     struct RideStop *stop = scheduler.stops;

    //     if (stop->next != NULL)
    //     {
    //         printf("\nSCHED DPS: %lf, %lf", stop->x, stop->y);
    //     }
    // }

    struct RideStop *stop = malloc(sizeof(RideStop));

    while (scheduler.size > 0)
    {
        get_next(&scheduler, stop);
        // printf("\nTESTE: %p -> %p / (%lf, %lf)", stop, stop->next, stop->x, stop->y);
        // printf("\nSIZE: %d", scheduler.size);
        // printf("\nTIME: %lf", stop->time);
        // printf("\nTESTE: %p -> %p / (%lf, %lf)", stop, stop->next, stop->x, stop->y);

        if (stop->next != NULL)
        {
            // printf("\nPRE INSERT 1: %p -> %p", stop, stop->next);

            insert_new(&scheduler, stop->next, speed);
            continue;
        }

        double total_distance = get_ride_total_distance(*stop->ride);
        if (total_distance == -1)
            printf("\nERRO: Erro calculando distância total para impressao");

        // double total_distance = calculate_ride_total_time(*stop->ride);

        Ride ride = *stop->ride;

        // TODO: Não está imprimindo todas as paradas, só algumas
        printf("\n%.2f %.2f %d", ride.end_time, total_distance, ride.stop_number);

        RideStop *ride_stop = ride.stops;
        while (ride_stop != NULL)
        {
            printf(" %.2f %.2f", ride_stop->x, ride_stop->y);
            ride_stop = ride_stop->next;
        }

        // TODO: Free each stop
    }

    printf("\n");

    free(new_ride);
    free(scheduler.stops);

    return 0;
}

// TODO: Alterar minheap para usar eventos e não corridas