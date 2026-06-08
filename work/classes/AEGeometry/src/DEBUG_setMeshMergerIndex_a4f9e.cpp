#include "class.h"

// AEGeometry::DEBUG_setMeshMergerIndex(int, LodMeshMerger*) -> strd r1,r2,[r0,#0x0] ; bx lr
struct LodMeshMerger;
struct AEGeometry { void DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b); };
void AEGeometry::DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b)
{
    this->f_0 = a;
    this->f_4 = (void *)b;
}
