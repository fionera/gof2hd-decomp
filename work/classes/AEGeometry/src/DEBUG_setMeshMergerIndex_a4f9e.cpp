#include "class.h"

// AEGeometry::DEBUG_setMeshMergerIndex(int, LodMeshMerger*) -> strd r1,r2,[r0,#0x0] ; bx lr
struct LodMeshMerger;
struct AEGeometry { void DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b); };
void AEGeometry::DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b)
{
    i32(this, 0) = a;
    pp(this, 4) = (void *)b;
}
