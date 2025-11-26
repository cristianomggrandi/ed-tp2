#ifndef INVARIANTS_H
#define INVARIANTS_H

#include <stdio.h>
#include <stdbool.h>
#ifndef TYPES_H
#include "types.h"
#endif

// Valida que todos os tempos no MinHeap são não-negativos
// Retorna true se todos os tempos são válidos, false caso contrário
bool validate_heap_times(MinHeap *min_heap)
{
    if (min_heap == NULL)
    {
        printf("\nERRO INVARIANTE: Ponteiro de MinHeap é NULL");
        return false;
    }

    for (int i = 0; i < min_heap->size; i++)
    {
        if (min_heap->stops[i].time < 0)
        {
            printf("\nERRO INVARIANTE: Tempo negativo no heap na posição %d: %.2f", 
                   i, min_heap->stops[i].time);
            return false;
        }
    }

    return true;
}

// Valida que os tempos das paradas são monotonicamente crescentes ao longo da corrida
// Retorna true se os tempos são consistentes, false caso contrário
bool validate_ride_temporal_consistency(Ride *ride)
{
    if (ride == NULL || ride->stops == NULL)
        return true;

    RideStop *current = ride->stops;
    double prev_time = -1;

    while (current != NULL)
    {
        if (prev_time >= 0 && current->time < prev_time)
        {
            printf("\nERRO INVARIANTE: Tempos não são monotonicamente crescentes (%.2f < %.2f)", 
                   current->time, prev_time);
            return false;
        }

        prev_time = current->time;
        current = current->next;
    }

    return true;
}

#endif
