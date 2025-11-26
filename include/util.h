#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(RideStop a, RideStop b);
bool meets_distance_criteria(Ride *ride, Demand demand, double max_origin_distance, double max_destination_distance);

// Calcula a distância euclidiana entre duas paradas usando hypot() para estabilidade numérica
double get_distance(RideStop a, RideStop b)
{
    return hypot((a.x - b.x), (a.y - b.y));
}

// Verifica se a demanda satisfaz os critérios de distância (alpha e beta) em relação às demandas da corrida
bool meets_distance_criteria(Ride *ride, Demand demand, double max_origin_distance, double max_destination_distance)
{
    if (ride->demand_number > MAX_CAPACITY)
    {
        printf("\nERRO: demand_number (%d) excede MAX_CAPACITY (%d)", ride->demand_number, MAX_CAPACITY);
        return false;
    }

    for (int j = 0; j < ride->demand_number; j++)
    {
        if (
            (get_distance(demand.origin, ride->demands[j].origin) > max_origin_distance) ||
            (get_distance(demand.destination, ride->demands[j].destination) > max_destination_distance))
        {
            return false;
        }
    }

    return true;
}
#endif
