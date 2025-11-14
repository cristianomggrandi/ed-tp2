#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#ifndef TYPES_H
#include "types.h"
#endif

double get_distance(GeoPoint a, GeoPoint b);

double get_distance(GeoPoint a, GeoPoint b)
{
    return hypot((a.x - b.x), (a.y - b.y));
}
#endif
