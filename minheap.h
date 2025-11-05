#ifndef TYPES_H
#include "types.h"
#endif

void insert_new(struct MinHeap *min_heap, struct Demand demand);
struct Demand get_next(struct MinHeap *min_heap); // TODO: Pointer?
struct Demand peek(struct MinHeap *min_heap);
int get_parent_index(int index);     // TODO: Pointer?
int get_left_node_index(int index);  // TODO: Pointer?
int get_right_node_index(int index); // TODO: Pointer?

void insert_new(struct MinHeap *min_heap, struct Demand demand)
{
    int index = min_heap->size;
    int parent_index = get_parent_index(index);

    min_heap->events[index] = demand;

    while (min_heap->events[parent_index].time > min_heap->events[index].time)
    {
        struct Demand temp = min_heap->events[parent_index];
        min_heap->events[parent_index] = min_heap->events[index];
        min_heap->events[index] = temp;

        index = parent_index;
        parent_index = get_parent_index(index);
    }

    min_heap->size++;
}

struct Demand get_next(struct MinHeap *min_heap)
{
    struct Demand next = min_heap->events[0];

    min_heap->events[0] = min_heap->events[min_heap->size - 1];
    min_heap->size--;

    int index = 0;
    int left_node_index = get_left_node_index(index);
    int right_node_index = get_right_node_index(index);

    while (left_node_index < min_heap->size)
    {
        int node_time = min_heap->events[index].time;
        int left_node_time = min_heap->events[left_node_index].time;
        int right_node_time = min_heap->events[right_node_index].time;

        if (node_time <= left_node_time && node_time <= right_node_time)
            break;

        if (left_node_time < right_node_time)
        {
            struct Demand temp = min_heap->events[index];
            min_heap->events[index] = min_heap->events[left_node_index];
            min_heap->events[left_node_index] = temp;

            index = left_node_index;
        }
        else
        {
            struct Demand temp = min_heap->events[index];
            min_heap->events[index] = min_heap->events[right_node_index];
            min_heap->events[right_node_index] = temp;

            index = right_node_index;
        }

        left_node_index = get_left_node_index(index);
        right_node_index = get_right_node_index(index);
    }

    return next;
}

struct Demand peek(struct MinHeap *min_heap)
{
    return min_heap->events[0];
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