#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(RideStop a, RideStop b);
bool meets_distance_criteria(Ride *ride, Demand demand, double max_origin_distance, double max_destination_distance);

double get_distance(RideStop a, RideStop b)
{
    return hypot((a.x - b.x), (a.y - b.y));
}

bool meets_distance_criteria(Ride *ride, Demand demand, double max_origin_distance, double max_destination_distance)
{
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
