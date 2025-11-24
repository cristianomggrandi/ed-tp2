#include <stdlib.h>
#ifndef UTIL_H
#include "util.h"
#endif
#ifndef TYPES_H
#include "types.h"
#endif
#include "ride.h"

void initialize(MinHeap *min_heap, int size);
void insert_new(MinHeap *min_heap, RideStop *stop, double speed);
RideStop get_next(MinHeap *min_heap);
RideStop peek(MinHeap *min_heap);
int get_parent_index(int index);
int get_left_node_index(int index);
int get_right_node_index(int index);
int is_valid_minheap(MinHeap *min_heap);
void finalize(MinHeap *min_heap, double speed);

void print_all(MinHeap *min_heap)
{
    printf("\nPRINT HEAP %d:", min_heap->size);

    for (int i = 0; i < min_heap->size; i++)
        printf(" %lf", min_heap->stops[i].time);
}

void initialize(MinHeap *min_heap, int size)
{
    min_heap->stops = (RideStop *)malloc(size * sizeof(RideStop));
    min_heap->size = 0;
}

void insert_new(MinHeap *min_heap, RideStop *stop, double speed)
{
    int index = min_heap->size;

    stop->time = calculate_stop_time(stop, speed);
    if (stop->time == -1)
        printf("\nERRO: Erro calculando tempo de parada");

    min_heap->stops[index] = *stop;
    min_heap->size++;

    while (index > 0)
    {
        int parent_index = get_parent_index(index);
        if (min_heap->stops[parent_index].time <= min_heap->stops[index].time)
            break;

        RideStop temp = min_heap->stops[parent_index];
        min_heap->stops[parent_index] = min_heap->stops[index];
        min_heap->stops[index] = temp;

        index = parent_index;
    }
}

RideStop get_next(MinHeap *min_heap)
{
    if (min_heap->size == 0)
        printf("\nERRO: MinHeap vazio");

    RideStop next = min_heap->stops[0];

    min_heap->stops[0] = min_heap->stops[min_heap->size - 1];
    min_heap->size--;

    int index = 0;
    int left_node_index = get_left_node_index(index);
    int right_node_index = get_right_node_index(index);

    while (left_node_index < min_heap->size)
    {
        int node_time = min_heap->stops[index].time;
        int left_node_time = min_heap->stops[left_node_index].time;
        int right_node_time = min_heap->stops[right_node_index].time;

        if (node_time <= left_node_time && node_time <= right_node_time)
            break;

        if (left_node_time < right_node_time)
        {
            RideStop temp = min_heap->stops[index];
            min_heap->stops[index] = min_heap->stops[left_node_index];
            min_heap->stops[left_node_index] = temp;

            index = left_node_index;
        }
        else
        {
            RideStop temp = min_heap->stops[index];
            min_heap->stops[index] = min_heap->stops[right_node_index];
            min_heap->stops[right_node_index] = temp;

            index = right_node_index;
        }

        left_node_index = get_left_node_index(index);
        right_node_index = get_right_node_index(index);
    }

    return next;
}

RideStop peek(MinHeap *min_heap)
{
    return min_heap->stops[0];
}

int get_parent_index(int index)
{
    return (index - 1) / 2;
}
int get_left_node_index(int index)
{
    return index * 2 + 1;
}
int get_right_node_index(int index)
{
    return index * 2 + 2;
}

int is_valid_minheap(MinHeap *min_heap)
{
    if (min_heap == NULL || min_heap->size == 0)
        return 1;

    for (int i = 0; i < min_heap->size; i++)
    {
        int left = get_left_node_index(i);
        int right = get_right_node_index(i);

        if (left < min_heap->size && min_heap->stops[i].time > min_heap->stops[left].time)
        {
            printf("\nERRO HEAP: Pai[%d]=%.2lf > Filho Esquerdo[%d]=%.2lf",
                   i, min_heap->stops[i].time, left, min_heap->stops[left].time);
            return 0;
        }

        if (right < min_heap->size && min_heap->stops[i].time > min_heap->stops[right].time)
        {
            printf("\nERRO HEAP: Pai[%d]=%.2lf > Filho Direito[%d]=%.2lf",
                   i, min_heap->stops[i].time, right, min_heap->stops[right].time);
            return 0;
        }
    }

    return 1;
}

void finalize(MinHeap *min_heap, double speed)
{
    struct RideStop stop;

    while (min_heap->size > 0)
    {
        stop = get_next(min_heap);

        if (stop.next != NULL)
        {
            insert_new(min_heap, stop.next, speed);
            continue;
        }

        double total_distance = get_ride_total_distance(*stop.ride);

        Ride ride = *stop.ride;

        printf("%.2f %.2f %d", ride.end_time, total_distance, ride.stop_number);

        RideStop *ride_stop = ride.stops;
        while (ride_stop != NULL)
        {
            printf(" %.2f %.2f", ride_stop->x, ride_stop->y);

            RideStop *current = ride_stop;
            ride_stop = ride_stop->next;
            free(current);
        }

        for (int i = 0; i < ride.demand_number; i++)
            ride.demands[i].type = FINISHED;

        printf("\n");

        free(stop.ride);
    }

    free(min_heap->stops);
}
