struct GeoPoint
{
    double x, y;
};

struct Demand
{
    int id;
    int time;
    struct GeoPoint origin;
    struct GeoPoint destination;
};

struct RideStop
{
    // 1 - Coleta
    // 2 - Entrega
    int type;
    struct GeoPoint coordinates;
    int demand_id;
};

struct Segment
{
    // 1 - Coleta (2 pontos de embarque)
    // 2 - Entrega (2 pontos de desembarque)
    // 3 - Deslocamento (1 embarque e um desembarque)
    int type;
    struct GeoPoint start;
    struct GeoPoint end;
    int duration;
    double distance;
};

struct Ride
{
    int *demands;
    int duration;
    struct Segments *segments;
    double distance;
    double efficiency;
};

struct MinHeap
{
    int size;
    struct Demand *events;
};
