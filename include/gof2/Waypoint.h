#ifndef GOF2_WAYPOINT_H
#define GOF2_WAYPOINT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Waypoint;
struct Route;
struct Player;
struct KIPlayer;
struct AEGeometry;

using Vector = AbyssEngine::AEMath::Vector;

class Waypoint {
public:
    void* field_0x0;                    // +0x0
    Player* player;                  // +0x4
    uint8_t field_0x4c;                 // +0x4c
    float fx;                   // +0x58
    float fy;                   // +0x5c
    float fz;                   // +0x60
    uint8_t field_0x72;                 // +0x72
    int32_t x;                // +0x124
    int32_t y;                // +0x128
    int32_t z;                // +0x12c
    uint16_t state;               // +0x130
    Route* route;                 // +0x134

    Waypoint(int x, int y, int z, Route *route);
    ~Waypoint();
    void setActive(bool active);
    void reached();
    Vector getPosition();
    void reset();
};
#endif
