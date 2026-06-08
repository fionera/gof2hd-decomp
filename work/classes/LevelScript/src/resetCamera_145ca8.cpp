#include "class.h"

extern "C" void *Level_getPlayer(Level *level);
extern "C" void TargetFollowCamera_setTarget(void *camera, void *target);

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

void LevelScript::resetCamera(Level *level)
{
    if (Level_getPlayer(level) != 0) {
        void *camera = this->m_pCamera;
        void *player = Level_getPlayer(level);
        TargetFollowCamera_setTarget(camera, player->f_8);
        TargetFollowCamera_setTargetOffset(this->m_pCamera, StackVector(0.0f, 600.0f, -650.0f));
        TargetFollowCamera_setCamOffset(this->m_pCamera, StackVector(0.0f, 600.0f, -1338.0f));
    }
}
