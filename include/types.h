#ifndef TYPES_H
#define TYPES_H

#define MAX_CAPACITY 100

typedef enum RideStopType
{
    ORIGIN = 1,
    DESTINATION
} RideStopType;

typedef struct RideStop
{
    double x, y;
    double time;
    double distance;
    RideStopType type;
    int demandId;
    struct RideStop *next;
    struct RideStop *prev;
    struct Ride *ride;
} RideStop;

typedef enum DemandStatus
{
    DEMANDED = 1,
    SINGLE,
    COMBINED,
    FINISHED
} DemandStatus;

typedef struct Demand
{
    int id;
    DemandStatus type;
    int time;
    RideStop origin;
    RideStop destination;
} Demand;

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