#include "class.h"

struct StackVector {
    char bytes[12];

    StackVector(float x, float y, float z)
    {
        ((float *)bytes)[0] = x;
        ((float *)bytes)[1] = y;
        ((float *)bytes)[2] = z;
    }
};

extern "C" void TargetFollowCamera_setTargetOffset(void *camera, const StackVector &offset);
extern "C" void TargetFollowCamera_setCamOffset(void *camera, const StackVector &offset);

void LevelScript::lookBehind()
{
    TargetFollowCamera_setTargetOffset(P(this, 0x14), StackVector(0.0f, 0.0f, -950.0f));
    TargetFollowCamera_setCamOffset(P(this, 0x14), StackVector(0.0f, 600.0f, 2230.0f));
}
