#ifndef TYPES_H
#define TYPES_H

#define MAX_CAPACITY 100

typedef struct RideStop
{
    // 1 - Coleta
    // 2 - Entrega
    int type;
    double x, y;
    double time;
    double distance;
    struct RideStop *next;
    struct RideStop *prev;
    struct Ride *ride;
} RideStop;

typedef struct Demand
{
    int id;
    int time;
    RideStop origin;
    RideStop destination;
} Demand;

typedef struct Segment
{
    // 1 - Coleta (2 pontos de embarque)
    // 2 - Entrega (2 pontos de desembarque)
    // 3 - Deslocamento (1 embarque e um desembarque)
    int type;
    RideStop start;
    RideStop end;
    // int duration;
    double distance;
    struct Segment *next;
} Segment;

typedef struct Ride
{
    int demand_number;
    Demand demands[MAX_CAPACITY];
    int stop_number;
    RideStop *stops;
    double end_time;
} Ride;

typedef struct MinHeap
{
    int size;
    RideStop *stops;
} MinHeap;
#endif