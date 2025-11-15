#include <stdlib.h>
#ifndef UTIL_H
#include "util.h"
#endif
#ifndef TYPES_H
#include "types.h"
#endif
#include "ride.h"

void initialize(MinHeap *min_heap, int size);
void insert_new(MinHeap *min_heap, RideStop **stop_p, double speed);
RideStop get_next(MinHeap *min_heap); // TODO: Pointer?
RideStop peek(MinHeap *min_heap);
int get_parent_index(int index);     // TODO: Pointer?
int get_left_node_index(int index);  // TODO: Pointer?
int get_right_node_index(int index); // TODO: Pointer?

void initialize(MinHeap *min_heap, int size)
{
    min_heap->stops = (RideStop *)malloc(size * sizeof(RideStop));

    // Initialize every ride's ride counter to 0
    // TODO: ?
    // for (int i = 0; i < size; i++)
    //     min_heap->stops[i].demand_number = 0;

    min_heap->size = 0;
}

void insert_new(MinHeap *min_heap, RideStop **stop_p, double speed)
{
    RideStop *stop = *stop_p;

    int index = min_heap->size;
    int parent_index = get_parent_index(index);

    // double total_distance = get_ride_total_distance(**stop_p);
    // stop->time = ((double)stop->demands[0].time) + (total_distance / speed);
    stop->time = calculate_stop_time(stop, speed);
    if (stop->time == -1)
        printf("\nERRO: Erro calculando tempo de parada");

    min_heap->stops[index] = *stop;

    while (min_heap->stops[parent_index].time > min_heap->stops[index].time)
    {
        RideStop temp = min_heap->stops[parent_index];
        min_heap->stops[parent_index] = min_heap->stops[index];
        min_heap->stops[index] = temp;

        index = parent_index;
        parent_index = get_parent_index(index);
    }

    min_heap->size++;

    // *stop_p = NULL;
}

RideStop get_next(MinHeap *min_heap)
{
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
    return index / 2;
}
int get_left_node_index(int index)
{
    return index * 2 + 1;
}
int get_right_node_index(int index)
{
    return index * 2 + 2;
}