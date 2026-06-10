#ifndef GOF2_ROUTE_H
#define GOF2_ROUTE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - Route class (waypoint navigation path).
// Android libgof2hdaa.so, armv7 Thumb. Top-level class (no AbyssEngine namespace),
// per each work-item Sig line. Field offsets recovered per-method from the target
// disassembly; access fields via byte-offset casts from `this`.
//
// Object layout (recovered):
//   +0x00  int                   current waypoint index
//   +0x04  uint8_t               looping flag
//   +0x0c  Array<Waypoint*>*     waypoints
//   +0x10  Array<KIPlayer*>*     docking targets
//   +0x14  Array<int>*           docking times


struct Route;
struct Waypoint;
struct KIPlayer;



// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {

} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessor via byte offset.

struct Route {
    int32_t field_0x0;                  // +0x0
    void* field_0xc;                    // +0xc
    void* field_0x10;                   // +0x10
};
#endif
