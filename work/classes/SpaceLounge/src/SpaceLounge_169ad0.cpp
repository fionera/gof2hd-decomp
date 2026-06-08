#include "class.h"

extern "C" void String_ctor_default(void *s);
extern "C" void Matrix_ctor(void *m);
extern "C" void SpaceLounge_init(SpaceLounge *self);
extern "C" void *operator_new(unsigned int size);
extern "C" void CutScene_ctor(void *cutscene, int id);
extern "C" int CutScene_isInitialized(void *cutscene);
extern "C" void CutScene_initialize(void *cutscene);
extern "C" void *Level_getEnemies(void *level);
extern "C" int Agent_getOffer(void *agent);
extern "C" void *Agent_getMission(void *agent);
extern "C" int Agent_hasAcceptedOffer(void *agent);
extern "C" int Mission_getType(void *mission);
extern "C" void ArrayRemove_AgentPtr(void *agent, void *array);
extern "C" void *Status_getSystem();
extern "C" int SolarSystem_getRace(void *system);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z);
extern "C" void EaseInOutMatrix_ctor(void *ease, void *from, void *to, int duration);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);

extern "C" void *SpaceLounge_ctor_camera_slot;

extern "C" SpaceLounge *_ZN11SpaceLoungeC2Ev(SpaceLounge *self)
{
    char from[60];
    char to[60];

    I(self, 0x8c) = 0;
    I(self, 0x90) = 0;
    I(self, 0x94) = 0;
    I(self, 0x98) = 0;
    I(self, 0x4c) = 0;
    I(self, 0x50) = 0;
    I(self, 0x54) = 0;
    I(self, 0x9c) = 0;
    I(self, 0xa0) = 0;
    String_ctor_default(B(self, 0xa4));
    Matrix_ctor(B(self, 0xc8));

    UC(self, 0xb0) = 0;
    P(self, 0x4) = 0;
    P(self, 0x8) = 0;
    P(self, 0xc) = 0;
    I(self, 0x34) = 0;
    UC(self, 0x36) = 0;
    P(self, 0x58) = 0;
    P(self, 0x5c) = 0;
    P(self, 0xc0) = 0;
    UC(self, 0x1c) = 0;
    I(self, 0x14) = 0;
    I(self, 0x18) = 0;
    P(self, 0x48) = 0;
    I(self, 0x20) = 0;
    P(self, 0x24) = 0;
    P(self, 0x28) = 0;
    P(self, 0x2c) = 0;
    P(self, 0x38) = 0;
    P(self, 0x3c) = 0;
    P(self, 0x40) = 0;
    P(self, 0x44) = 0;

    SpaceLounge_init(self);

    void *agents = P(self, 0x24);
    if (agents != 0) {
        for (unsigned i = 0; i < U(agents, 0x0); ++i) {
            void *agent = ((void **)P(agents, 0x4))[i];
            int offer = Agent_getOffer(agent);
            if ((offer == 6 || offer == 0) && Agent_getMission(agent) != 0) {
                void *mission = Agent_getMission(agent);
                if (Mission_getType(mission) == 0xc && Agent_hasAcceptedOffer(agent) != 0) {
                    ArrayRemove_AgentPtr(agent, P(self, 0x24));
                    SpaceLounge_init(self);
                    break;
                }
            }
            agents = P(self, 0x24);
        }
    }

    void *cutscene = P(self, 0x44);
    if (cutscene == 0) {
        cutscene = operator_new(0xa0);
        CutScene_ctor(cutscene, 4);
        P(self, 0x44) = cutscene;
    }
    while (CutScene_isInitialized(cutscene) == 0) {
        CutScene_initialize(cutscene);
        cutscene = P(self, 0x44);
    }

    int race = SolarSystem_getRace(Status_getSystem());
    MatrixSetTranslation(from, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(from, 0.0f, 0.0f, 0.0f);
    MatrixSetTranslation(to, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(to, 0.0f, 0.0f, 0.0f);
    void *ease = operator_new(0xf4);
    EaseInOutMatrix_ctor(ease, from, to, 3000);
    P(self, 0x48) = ease;

    void *cameraSlot = *(void **)&SpaceLounge_ctor_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = PaintCanvas_CameraGetCurrent(camera);
    PaintCanvas_CameraSetLocal(camera, current);
    UC(self, 0xb0) = 1;
    UC(self, 0xbd) = 0;
    return self;
}
