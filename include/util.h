#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(RideStop a, RideStop b);

double get_distance(RideStop a, RideStop b)
{
    return hypot((a.x - b.x), (a.y - b.y));
}
#endif
