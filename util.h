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
    double old_distance = get_ride_total_distance(ride);

    printf("\nAntes 1: %f, %f", ride.demands[ride.demand_number - 1].origin.x, ride.demands[ride.demand_number - 1].origin.y);
    printf("\nAntes 2: %f, %f", demand.origin.x, demand.origin.y);
    double origin_distance = get_distance(ride.demands[ride.demand_number - 1].origin, demand.origin);
    double destination_distance = get_distance(ride.demands[ride.demand_number - 1].destination, demand.destination);

    printf("\nDist 1: %f", origin_distance);
    printf("\nDist 2: %f", destination_distance);

    return (old_distance) / (old_distance + origin_distance + destination_distance);
}

double get_ride_total_distance(struct Ride ride)
{
    double distance = get_distance(ride.demands[ride.demand_number - 1].origin, ride.demands[0].destination);

    for (int i = 0; i < ride.demand_number - 1; i++)
    {
        distance +=
            get_distance(ride.demands[i].origin, ride.demands[i + 1].origin) +
            get_distance(ride.demands[i].destination, ride.demands[i + 1].destination);
    }

    return distance;
}
