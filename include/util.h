#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(GeoPoint a, GeoPoint b);
double calculate_efficiency(Ride ride);
double get_sum_of_ride_demands_distances(Ride ride);
double get_ride_total_distance(Ride ride);
double get_distance_without_last_demand(Ride ride);
void add_ride_stops(Ride *ride, Demand demand);
void remove_last_added_stops(Ride *ride);
void get_ride_stop(Ride *ride);

double get_distance(GeoPoint a, GeoPoint b)
{
    return hypot((a.x - b.x), (a.y - b.y));
}

double calculate_efficiency(Ride ride)
{
    if (ride.stop_number < 2)
        return -1;

    if (ride.stop_number == 2)
        return 1.0;

    double total_distance = get_ride_total_distance(ride);
    if (total_distance == -1)
        printf("\nERRO: Erro calculando distancia total para calcular eficiencia");

    // double original_distance = get_distance_without_last_demand(ride);

    // double original_distance = get_distance(ride.demands[0].origin, ride.demands[0].destination);
    double original_distance = get_sum_of_ride_demands_distances(ride);

    double efficiency = original_distance / total_distance;
    // double efficiency2 = total_distance / original_distance;

    printf("\nEFF.: %lf / %lf = %lf", original_distance, total_distance, efficiency);
    // printf("\nEFF 2.: %lf / %lf = %lf", total_distance, original_distance, efficiency2);

    return efficiency;
}

double get_sum_of_ride_demands_distances(Ride ride)
{
    double sum = 0;

    // for (int i = 0; i < ride.demand_number; i++)
    //     printf("\nPONTO: (%lf, %lf) => (%lf, %lf)", ride.demands[i].origin.x, ride.demands[i].origin.y, ride.demands[i].destination.x, ride.demands[i].destination.y);

    for (int i = 0; i < ride.demand_number; i++)
        sum += get_distance(ride.demands[i].origin, ride.demands[i].destination);

    return sum;
}

double get_ride_total_distance(Ride ride)
{
    GeoPoint *stop = ride.stops;

    if (stop == NULL || stop->next == NULL)
        return -1;

    double distance = 0;

    while (stop->next != NULL)
    {
        // printf("\nANTES: %lf", distance);
        // printf("\nANTES get_distance: %p -> %p", stop, stop->next);
        // printf("\nSUM: (%lf, %lf) e (%lf, %lf)", stop->x, stop->y, stop->next->x, stop->next->y);
        distance += get_distance(*stop, *(stop->next));
        // printf("\nDEPOIS: %lf", distance);
        stop = stop->next;
    }

    return distance;
}

double get_distance_without_last_demand(Ride ride)
{
    GeoPoint *stop = ride.stops;

    if (stop == NULL || stop->next == NULL)
        return -1;

    double distance = 0;
    int i = 0;

    while (stop->next != NULL)
    {
        GeoPoint *next = stop->next;

        // Penúltimo destino
        if (i == ride.stop_number - 2)
            break;

        if ((i == ride.stop_number / 2 - 2) && next->next != NULL)
        {
            next = next->next;
            i++;
        }

        // printf("\nSUM 2: (%lf, %lf) e (%lf, %lf)", stop->x, stop->y, stop->next->x, stop->next->y);
        distance += get_distance(*stop, *next);

        stop = next;

        // if (i == ride.stop_number / 2 - 2 || i == ride.stop_number - 1)
        //     stop = stop->next;

        i++;
    }

    return distance;
}

void add_ride_stops(Ride *ride, Demand demand)
{
    // TODO: Mover funções de Ride para um arquivo isolado

    {
        GeoPoint *stop = ride->stops;

        printf("\nAdicionar ANTES:");

        while (stop != NULL)
        {
            printf("\n(%lf, %lf)", stop->x, stop->y);
            stop = stop->next;
        }
    }

    if (ride->stop_number == 0)
    {
        GeoPoint *stop = malloc(sizeof(GeoPoint));
        stop->next = malloc(sizeof(GeoPoint));

        stop->x = demand.origin.x;
        stop->y = demand.origin.y;
        // printf("\nSTART: %lf, %lf", stop->x, stop->y);
        stop->next->x = demand.destination.x;
        stop->next->y = demand.destination.y;
        // printf("\nEND: %lf, %lf", stop->next->x, stop->next->y);

        stop->next->next = NULL;

        ride->stops = stop;
        ride->stop_number = 2;

        return;
    }

    GeoPoint *stop = ride->stops;

    // Posiciona stop no stopo que liga a coleta k ao destino 0
    for (int i = 0; i < ride->stop_number / 2 - 1; i++)
    {
        stop = stop->next;
    }

    // printf("\nPOINTER 1: %p -> %p", stop, stop->next);

    // Nova coleta
    GeoPoint *new_stop = malloc(sizeof(GeoPoint));
    new_stop->next = stop->next;
    new_stop->x = demand.origin.x;
    new_stop->y = demand.origin.y;

    // Conecta a última coleta anterior à nova última coleta
    stop->next = new_stop;

    // printf("\nPOINTER 2: %p -> %p", stop, stop->next);

    // Posiciona stop no fim da lista
    while (stop->next != NULL)
    {
        stop = stop->next;
    }

    // printf("\nPOINTER 3: %p -> %p", stop, stop->next);
    // Preenche novo stop ao final da lista
    stop->next = malloc(sizeof(GeoPoint));
    stop->next->x = demand.destination.x;
    stop->next->y = demand.destination.y;
    stop->next->next = NULL;
    // printf("\nPOINTER 4: %p -> %p", stop, stop->next);

    ride->stop_number += 2;

    {
        GeoPoint *stop = ride->stops;

        printf("\nAdicionar DEPOIS:");

        while (stop != NULL)
        {
            printf("\n(%lf, %lf)", stop->x, stop->y);
            stop = stop->next;
        }
    }
}

void remove_last_added_stops(Ride *ride)
{
    if (ride->stop_number == 0)
        return;

    if (ride->stop_number == 2)
    {
        printf("\nstopnumber == 2 => %p -> %p", ride->stops, ride->stops->next);

        free(ride->stops->next);
        free(ride->stops);
        ride->stops = NULL;
        ride->stop_number = 0;
        ride->demand_number = 0;
        return;
    }

    GeoPoint *stop = ride->stops;

    // Posiciona stop no ponto da coleta (k - 1)
    for (int i = 0; i < ride->stop_number / 2 - 2; i++)
        stop = stop->next;

    // Remove a coleta k e reconecta o anterior ao posterior
    GeoPoint *next_next_stop = stop->next->next;
    free(stop->next);
    stop->next = next_next_stop;

    // Posiciona stop no penúltimo destino
    while (stop->next->next != NULL)
        stop = stop->next;

    // printf("\nPOINTER REMOCAO: %p -> %p -> %p", stop, stop->next, stop->next->next);

    // Remove último destino
    free(stop->next);
    stop->next = NULL;

    ride->stop_number -= 2;
}

// void get_ride_segments(Ride *ride)
// {
// TODO: Linked list
// ride->segment_number = 2 * ride->demand_number - 1;

// for (int i = 0; i < ride->demand_number - 1; i++)
// {
//     Segment collect_segment;
//     collect_segment.start = ride->demands[i].origin;
//     collect_segment.end = ride->demands[i + 1].origin;

//     ride->segments[i] = collect_segment;

//     Segment destination_segment;
//     destination_segment.start = ride->demands[i].destination;
//     destination_segment.end = ride->demands[i + 1].destination;

//     ride->segments[i + ride->demand_number] = destination_segment;
// }

// Segment main_ride_segment;
// main_ride_segment.start = ride->demands[ride->demand_number - 1].origin;
// main_ride_segment.end = ride->demands[0].destination;

// ride->segments[ride->demand_number - 1] = main_ride_segment;
// }
#endif
