#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(struct GeoPoint a, struct GeoPoint b);
double calculate_efficiency(struct Ride ride, struct Demand demand);
double get_ride_total_distance(struct Ride ride);

double get_distance(struct GeoPoint a, struct GeoPoint b)
{
    return hypot((a.x - b.x), (a.y - b.y));
    // return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double calculate_efficiency(struct Ride ride, struct Demand demand)
{
    if (ride.demand_number == 0)
        return 1.0;

    double old_distance = get_ride_total_distance(ride);

    double origin_distance = get_distance(ride.demands[ride.demand_number - 1].origin, demand.origin);
    double destination_distance = get_distance(ride.demands[ride.demand_number - 1].destination, demand.destination);

    return (old_distance + origin_distance + destination_distance) / (old_distance);
}

double get_ride_total_distance(struct Ride ride)
{
    if (ride.demand_number == 1)
        return get_distance(ride.demands[0].origin, ride.demands[0].destination);

    double distance = get_distance(ride.demands[ride.demand_number - 1].origin, ride.demands[0].destination);

    for (int i = 0; i < ride.demand_number - 1; i++)
    {
        distance +=
            get_distance(ride.demands[i].origin, ride.demands[i + 1].origin) +
            get_distance(ride.demands[i].destination, ride.demands[i + 1].destination);
    }

    return distance;
}
