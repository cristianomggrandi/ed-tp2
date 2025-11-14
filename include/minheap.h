#include <stdlib.h>
#ifndef UTIL_H
#include "util.h"
#endif
#ifndef TYPES_H
#include "types.h"
#endif
#include "ride.h"

void initialize(MinHeap *min_heap, int array_size);
void insert_new(MinHeap *min_heap, Ride **ride, double speed);
Ride get_next(MinHeap *min_heap); // TODO: Pointer?
Ride peek(MinHeap *min_heap);
int get_parent_index(int index);     // TODO: Pointer?
int get_left_node_index(int index);  // TODO: Pointer?
int get_right_node_index(int index); // TODO: Pointer?

void initialize(MinHeap *min_heap, int array_size)
{
    min_heap->rides = (Ride *)malloc(array_size * sizeof(Ride));

    // Initialize every ride's ride counter to 0
    for (int i = 0; i < array_size; i++)
        min_heap->rides[i].demand_number = 0;

    min_heap->size = 0;
}

void insert_new(MinHeap *min_heap, Ride **ride_p, double speed)
{
    Ride *ride = *ride_p;

    int index = min_heap->size;
    int parent_index = get_parent_index(index);

    double total_distance = get_ride_total_distance(*ride);
    ride->end_time = ((double)ride->demands[0].time) + (total_distance / speed);

    min_heap->rides[index] = *ride;

    while (min_heap->rides[parent_index].end_time > min_heap->rides[index].end_time)
    {
        Ride temp = min_heap->rides[parent_index];
        min_heap->rides[parent_index] = min_heap->rides[index];
        min_heap->rides[index] = temp;

        index = parent_index;
        parent_index = get_parent_index(index);
    }

    min_heap->size++;

    *ride_p = NULL;
}

Ride get_next(MinHeap *min_heap)
{
    Ride next = min_heap->rides[0];

    min_heap->rides[0] = min_heap->rides[min_heap->size - 1];
    min_heap->size--;

    int index = 0;
    int left_node_index = get_left_node_index(index);
    int right_node_index = get_right_node_index(index);

    while (left_node_index < min_heap->size)
    {
        int node_time = min_heap->rides[index].end_time;
        int left_node_time = min_heap->rides[left_node_index].end_time;
        int right_node_time = min_heap->rides[right_node_index].end_time;

        if (node_time <= left_node_time && node_time <= right_node_time)
            break;

        if (left_node_time < right_node_time)
        {
            Ride temp = min_heap->rides[index];
            min_heap->rides[index] = min_heap->rides[left_node_index];
            min_heap->rides[left_node_index] = temp;

            index = left_node_index;
        }
        else
        {
            Ride temp = min_heap->rides[index];
            min_heap->rides[index] = min_heap->rides[right_node_index];
            min_heap->rides[right_node_index] = temp;

            index = right_node_index;
        }

        left_node_index = get_left_node_index(index);
        right_node_index = get_right_node_index(index);
    }

    return next;
}

Ride peek(MinHeap *min_heap)
{
    return min_heap->rides[0];
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