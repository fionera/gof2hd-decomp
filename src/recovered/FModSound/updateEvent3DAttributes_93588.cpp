#include "class.h"

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

// FModSound::updateEvent3DAttributes(int, Vector*, Vector*, bool)
//   events[p1] = updateEvent3DAttributes(events[p1], p1, p2, p3, p4)
struct FModSound {
    void *updateEvent3DAttributes(void *event, int idx, Vector *a, Vector *b, bool c);
    void updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c);
};
void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c)
{
    void **slot = (void **)((char *)this + idx * 4 + OFF_EVENTS);
    *slot = updateEvent3DAttributes(*slot, idx, a, b, c);
}
