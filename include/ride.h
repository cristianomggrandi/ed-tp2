#ifndef RIDE_H
#define RIDE_H

#ifndef UTIL_H
#include "util.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

Ride *create_new_ride();
RideStop *create_new_stop(double x, double y, Ride *ride_p, RideStop *prev);
double calculate_ride_efficiency(Ride ride);
double get_sum_of_ride_demands_distances(Ride ride);
double get_ride_total_distance(Ride ride);
void add_ride_stops(Ride *ride, Demand demand);
void remove_last_added_stops(Ride *ride);
double calculate_stop_time(RideStop *stop, double speed);

Ride *create_new_ride()
{
    Ride *ride = (Ride *)malloc(sizeof(Ride));
    ride->demand_number = 0;
    ride->stop_number = 0;
    ride->stops = NULL;

    return ride;
}

RideStop *create_new_stop(double x, double y, Ride *ride_p, RideStop *prev)
{
    RideStop *stop = (RideStop *)malloc(sizeof(RideStop));
    stop->x = x;
    stop->y = y;
    stop->prev = prev;
    stop->next = NULL;
    stop->ride = ride_p;
    stop->type = 0; // TODO

    stop->time = 0; // TODO: Testar

    return stop;
}

double calculate_ride_efficiency(Ride ride)
{
    if (ride.stop_number < 2)
        return -1;

    if (ride.stop_number == 2)
        return 1.0;

    double total_distance = get_ride_total_distance(ride);
    if (total_distance == -1)
        printf("\nERRO: Erro calculando distancia total para calcular eficiencia");

    double original_distance = get_sum_of_ride_demands_distances(ride);

    double efficiency = original_distance / total_distance;

    return efficiency;
}

double get_sum_of_ride_demands_distances(Ride ride)
{
    double sum = 0;

    for (int i = 0; i < ride.demand_number; i++)
        sum += get_distance(ride.demands[i].origin, ride.demands[i].destination);

    return sum;
}

double get_ride_total_distance(Ride ride)
{
    RideStop *stop = ride.stops;

    if (stop == NULL || stop->next == NULL)
        return -1;

    double distance = 0;

    while (stop->next != NULL)
    {
        distance += get_distance(*stop, *(stop->next));
        stop = stop->next;
    }

    return distance;
}

void add_ride_stops(Ride *ride, Demand demand)
{
    if (ride->stop_number == 0)
    {
        RideStop *stop = create_new_stop(demand.origin.x, demand.origin.y, ride, NULL);
        stop->next = create_new_stop(demand.destination.x, demand.destination.y, ride, stop);

        ride->stops = stop;
        ride->stop_number = 2;

        return;
    }

    RideStop *stop = ride->stops;

    // Posiciona stop no stopo que liga a coleta k ao destino 0
    for (int i = 0; i < ride->stop_number / 2 - 1; i++)
    {
        stop = stop->next;
    }

    // Nova coleta
    RideStop *new_stop = create_new_stop(demand.origin.x, demand.origin.y, ride, stop);
    new_stop->next = stop->next;

    // Conecta a última coleta anterior à nova última coleta
    stop->next = new_stop;

    // Posiciona stop no fim da lista
    while (stop->next != NULL)
    {
        stop = stop->next;
    }

    // Preenche novo stop ao final da lista
    stop->next = create_new_stop(demand.destination.x, demand.destination.y, ride, stop);

    ride->stop_number += 2;
}

void remove_last_added_stops(Ride *ride)
{
    if (ride->stop_number == 0)
        return;

    if (ride->stop_number == 2)
    {
        free(ride->stops->next);
        free(ride->stops);
        ride->stops = NULL;
        ride->stop_number = 0;
        ride->demand_number = 0;
        return;
    }

    RideStop *stop = ride->stops;

    // Posiciona stop no ponto da coleta (k - 1)
    for (int i = 0; i < ride->stop_number / 2 - 2; i++)
        stop = stop->next;

    // Remove a coleta k e reconecta o anterior ao posterior
    RideStop *next_next_stop = stop->next->next;
    free(stop->next);
    stop->next = next_next_stop;

    // Posiciona stop no penúltimo destino
    while (stop->next->next != NULL)
        stop = stop->next;

    // Remove último destino
    free(stop->next);
    stop->next = NULL;

    ride->stop_number -= 2;
}

double calculate_stop_time(RideStop *stop, double speed)
{
    // printf("\nTESTE 1.44");
    printf("\nCURR_STOP: %p, %p", stop, stop->ride);

    if (stop->ride->stops == NULL)
        return -1;

    RideStop *curr_stop = stop->ride->stops;

    double distance = 0;

    while (curr_stop != stop)
    {
        if (curr_stop->next == NULL)
            return -1;

        distance += get_distance(*curr_stop, *curr_stop->next);
        curr_stop = curr_stop->next;
    }

    return distance / speed;
}
#endif