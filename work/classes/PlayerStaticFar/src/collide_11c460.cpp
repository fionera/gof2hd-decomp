#include "class.h"

// collide(x,y,z): true iff (x,y,z) lies strictly inside the +/- bound box centered
// at the stored integer position (fields 0x124/0x128/0x12c), where bound is the
// radius integer at *(this+4)+0x40 converted to float.
bool PlayerStaticFar::collide(float x, float y, float z)
{
    float px = (float)i32(this, 0x124);
    float dx = x - px;
    int boundI = i32(pp(this, 0x4), 0x40);
    float bound = (float)boundI;
    if (dx < bound) {
        float negBound = (float)(-boundI);
        if (dx > negBound) {
            float py = (float)i32(this, 0x128);
            float dy = y - py;
            if (dy < bound) {
                if (dy > negBound) {
                    float pz = (float)i32(this, 0x12c);
                    float dz = z - pz;
                    if (dz < bound && dz > negBound) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
