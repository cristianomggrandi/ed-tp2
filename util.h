#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(struct GeoPoint a, struct GeoPoint b);
double calculate_efficiency(struct Ride ride, struct Demand demand);
double get_sum_of_ride_demands_distances(struct Ride ride);
double get_ride_total_distance(struct Ride ride);
void get_ride_segments(struct Ride *ride);

double get_distance(struct GeoPoint a, struct GeoPoint b)
{
    return hypot((a.x - b.x), (a.y - b.y));
}

double calculate_efficiency(struct Ride ride, struct Demand demand)
{
    if (ride.demand_number == 0)
        return 1.0;

    double old_total_distance = get_sum_of_ride_demands_distances(ride);
    double demand_distance = get_distance(demand.origin, demand.destination);

    double old_distance = get_ride_total_distance(ride);
    double origin_distance = get_distance(ride.demands[ride.demand_number - 1].origin, demand.origin);
    double destination_distance = get_distance(ride.demands[ride.demand_number - 1].destination, demand.destination);
    double new_distance = old_distance + origin_distance + destination_distance;

    double efficiency = (old_total_distance + demand_distance) / new_distance;

    return efficiency;
}

double get_sum_of_ride_demands_distances(struct Ride ride)
{
    double sum = 0;

    for (int i = 0; i < ride.demand_number; i++)
        sum += get_distance(ride.demands[i].origin, ride.demands[i].destination);

    return sum;
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

void get_ride_segments(struct Ride *ride)
{
    ride->segment_number = 2 * ride->demand_number - 1;

    for (int i = 0; i < ride->demand_number - 1; i++)
    {
        struct Segment collect_segment;
        collect_segment.start = ride->demands[i].origin;
        collect_segment.end = ride->demands[i + 1].origin;

        ride->segments[i] = collect_segment;

        struct Segment destination_segment;
        destination_segment.start = ride->demands[i].destination;
        destination_segment.end = ride->demands[i + 1].destination;

        ride->segments[i + ride->demand_number] = destination_segment;
    }

    struct Segment main_ride_segment;
    main_ride_segment.start = ride->demands[ride->demand_number - 1].origin;
    main_ride_segment.end = ride->demands[0].destination;

    ride->segments[ride->demand_number - 1] = main_ride_segment;
}
