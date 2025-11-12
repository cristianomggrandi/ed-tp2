#ifndef TYPES_H
#define TYPES_H

#define MAX_CAPACITY 10

typedef struct GeoPoint
{
    double x, y;
    struct GeoPoint *next;
} GeoPoint;

typedef struct Demand
{
    int id;
    int time;
    GeoPoint origin;
    GeoPoint destination;
} Demand;

typedef struct RideStop
{
    // 1 - Coleta
    // 2 - Entrega
    int type;
    GeoPoint coordinates;
    int demand_id;
} RideStop;

typedef struct Segment
{
    // 1 - Coleta (2 pontos de embarque)
    // 2 - Entrega (2 pontos de desembarque)
    // 3 - Deslocamento (1 embarque e um desembarque)
    int type;
    GeoPoint start;
    GeoPoint end;
    // int duration;
    double distance;
    struct Segment *next;
} Segment;

typedef struct Ride
{
    int demand_number;
    Demand demands[MAX_CAPACITY];
    int stop_number;
    GeoPoint *stops;
    double end_time;
} Ride;

typedef struct MinHeap
{
    int size;
    Ride *rides;
} MinHeap;
#endif