#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"
// TODO: Add ifndef
// https://stackoverflow.com/questions/44673885/c-redefinition-of-struct
// #include "types.h"

#define MAX_SIZE 100

int main(int argc, char **argv)
{
    struct MinHeap min_heap;
    min_heap.size = 0;
    struct Demand demands[MAX_SIZE];
    min_heap.events = demands;

    // for (int i = 0; i < 7; i++)
    // {
    //     struct Demand demand;
    //     demand.time = 6 - i;
    //     insert_new(&min_heap, demand);
    // }

    struct Demand demand;
    demand.time = 3;
    insert_new(&min_heap, demand);
    demand.time = 6;
    insert_new(&min_heap, demand);
    demand.time = 1;
    insert_new(&min_heap, demand);
    demand.time = 1;
    insert_new(&min_heap, demand);
    demand.time = 2;
    insert_new(&min_heap, demand);
    demand.time = 5;
    insert_new(&min_heap, demand);
    demand.time = 6;
    insert_new(&min_heap, demand);

    printf("\nArray: ");
    while (min_heap.size > 0)
        printf("%d", get_next(&min_heap).time);
    printf("\n");

    return 0;
}
