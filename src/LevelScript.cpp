// Offset accessors come from a single source each within this TU:
//   I() from Explosion.h, B() from PlayerEgo.h, P() from LevelScript.h.
#include "gof2/LevelScript.h"   // defines P()
#include "gof2/Explosion.h"     // defines I()
#include "gof2/KIPlayer.h"
#include "gof2/AEGeometry.h"
#include "gof2/Level.h"
// RadioMessage.h is intentionally NOT included here: it defines stub structs
// (PlayerEgo, Route, Level, Player, LevelScript, ...) that collide with the real
// class headers used by this TU. This TU only needs RadioMessage::trigger/finish,
// so forward-declare that minimal interface instead.
class RadioMessage {
public:
    void finish();
    void trigger();
};
#include "gof2/Status.h"
#include "gof2/Player.h"
#include "gof2/PlayerEgo.h"     // defines B()
#include "gof2/PlayerFighter.h"
#include "gof2/Route.h"
#include "gof2/SolarSystem.h"   // first to define struct RetStr
// Station.h also defines an identical struct RetStr; rename it inside that
// header so it doesn't collide with SolarSystem.h's copy within this TU.
#define RetStr RetStr
#include "gof2/Station.h"
#undef RetStr

// A by-value 3-float vector passed to the camera helpers (a math Vector laid out
// as 3 contiguous floats). Defined once here so all uses agree.
struct StackVector {
    char bytes[12];

    StackVector() {}
    StackVector(float x, float y, float z)
    {
        ((float *)bytes)[0] = x;
        ((float *)bytes)[1] = y;
        ((float *)bytes)[2] = z;
    }
};

extern "C" void TargetFollowCamera_setTarget(void *camera, void *target);
extern "C" void TargetFollowCamera_setTargetOffset(void *camera, const StackVector &offset);
extern "C" void TargetFollowCamera_setCamOffset(void *camera, const StackVector &offset);
extern "C" void *gStatus;
extern "C" void Player_setUnknown(void *player, bool enabled);
extern "C" void *gProgrammedStation;
extern "C" int Station_getIndex(void *station);
extern "C" void *StarSystem_getPlanetTargets(void *starSystem);
extern "C" void Player_setAutoPilotTarget(void *player, void *target);
extern "C" void *Explosion_dtor(void *explosion);
extern "C" void operator_delete(void *ptr);
extern "C" void *KIPlayer_getRoute(void *player);
extern "C" void *Level_getActiveMessages(Level *level);
extern "C" void TargetFollowCamera_update(TargetFollowCamera *camera, float delta, int a, int b);
extern "C" void Matrix_ctor(void *matrix);
extern "C" void Hud_drawTitleImage(Hud *hud, bool visible);
extern "C" void TargetFollowCamera_setLookAtCam(TargetFollowCamera *camera, bool enabled);

// ---- render3D_145d84.cpp ----
typedef void (*RenderProc)(void *);


static volatile RenderProc gRenderProc;

void LevelScript::render3D()
{
    if (P(this, 0xdc) != 0) {
        ((AEGeometry *)(P(this, 0xdc)))->render();
    }
    if (P(this, 0xd8) != 0) {
        ((AEGeometry *)(P(this, 0xd8)))->render();
    }
    if (P(this, 0xb8) != 0) {
        ((AEGeometry *)(P(this, 0xb8)))->render();
    }
    if (P(this, 0xbc) != 0) {
        ((AEGeometry *)(P(this, 0xbc)))->render();
    }
    if (P(this, 0xc0) != 0) {
        ((AEGeometry *)(P(this, 0xc0)))->render();
    }
    if (P(this, 0xc4) != 0) {
        ((AEGeometry *)(P(this, 0xc4)))->render();
    }
    if (P(this, 0xc8) != 0 && ((Explosion *)(P(this, 0xc8)))->isPlaying() != 0) {
        ((Explosion *)(P(this, 0xc8)))->render();
    }
    if (UC(this, 0xa8) != 0 && P(this, 0xac) != 0) {
        RenderProc render = gRenderProc;
        render(P(this, 0xac));
        render(P(this, 0xb0));
        return render(P(this, 0xb4));
    }
}

// ---- startSequenceOver_14603a.cpp ----
uint8_t LevelScript::startSequenceOver()
{
    return UC(this, 0x21);
}

// ---- resetCamera_145ca8.cpp ----
void LevelScript::resetCamera(Level *level)
{
    if (((Level *)(level))->getPlayer() != 0) {
        void *camera = P(this, 0x14);
        void *player = (void *)((Level *)(level))->getPlayer();
        TargetFollowCamera_setTarget(camera, P(player, 0x8));
        TargetFollowCamera_setTargetOffset(P(this, 0x14), StackVector(0.0f, 600.0f, -650.0f));
        TargetFollowCamera_setCamOffset(P(this, 0x14), StackVector(0.0f, 600.0f, -1338.0f));
    }
}

// ---- skipSequence_145e08.cpp ----
void LevelScript::skipSequence()
{
    if (I(this, 0x24) > 0 && ((Status *)(gStatus))->getCurrentCampaignMission() > 0) {
        I(this, 0x24) = 0x1b59;
        void *player = (void *)((Level *)((Level *)P(this, 0x18)))->getPlayer();
        return Player_setUnknown(P(player, 0), true);
    }
}

// ---- startSequence_146040.cpp ----
uint8_t LevelScript::startSequence()
{
    return UC(this, 0x20);
}

// ---- setAutoPilotToProgrammedStation_137b68.cpp ----
void LevelScript::setAutoPilotToProgrammedStation()
{
    void **programmedStation = &gProgrammedStation;
    if (*programmedStation != 0) {
        void **status = &gStatus;
        if (((Station *)(((Status *)(*status))->getStation()))->equals((Station *)*programmedStation) != 0) {
            *programmedStation = 0;
            return;
        }

        void *target;
        void *player;
        if (((SolarSystem *)(((Status *)(*status))->getSystem()))->stationIsInSystem((Station *)*programmedStation) != 0) {
            player = (void *)((Level *)((Level *)P(this, 0x18)))->getPlayer();
            void *targets = StarSystem_getPlanetTargets((void *)((Level *)((Level *)P(this, 0x18)))->getStarSystem());
            void *system = (void *)((Status *)(*status))->getSystem();
            int stationIndex = Station_getIndex(*programmedStation);
            int targetIndex = ((SolarSystem *)(system))->getStationEnumIndex(stationIndex);
            target = ((void **)P(targets, 4))[targetIndex];
        } else if (((SolarSystem *)(((Status *)(*status))->getSystem()))->currentOrbitHasWarpGate() != 0) {
            player = (void *)((Level *)((Level *)P(this, 0x18)))->getPlayer();
            void *landmarks = (void *)((Level *)((Level *)P(this, 0x18)))->getLandmarks();
            target = ((void **)P(landmarks, 4))[1];
        } else {
            int warpGateIndex = ((SolarSystem *)(((Status *)(*status))->getSystem()))->getWarpGateEnumIndex();
            if (warpGateIndex < 0) {
                return;
            }
            player = (void *)((Level *)((Level *)P(this, 0x18)))->getPlayer();
            void *targets = StarSystem_getPlanetTargets((void *)((Level *)((Level *)P(this, 0x18)))->getStarSystem());
            target = ((void **)P(targets, 4))[warpGateIndex];
        }
        return Player_setAutoPilotTarget(player, target);
    }
}

// ---- _LevelScript_137c3c.cpp ----
LevelScript::~LevelScript()
{
    void *geometry = P(this, 0xdc);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xdc) = 0;

    geometry = P(this, 0xd8);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xd8) = 0;

    geometry = P(this, 0xb8);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xb8) = 0;

    geometry = P(this, 0xbc);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xbc) = 0;

    geometry = P(this, 0xc0);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xc0) = 0;

    geometry = P(this, 0xc4);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xc4) = 0;

    geometry = P(this, 0xac);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xac) = 0;

    geometry = P(this, 0xb0);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xb0) = 0;

    geometry = P(this, 0xb4);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xb4) = 0;

    void *explosion = P(this, 0xc8);
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
    }
    P(this, 0xc8) = 0;

    geometry = P(this, 0xcc);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        operator_delete(geometry);
    }
    P(this, 0xcc) = 0;
}

// ---- canSkipCutsceneNow_145e44.cpp ----
uint32_t LevelScript::canSkipCutsceneNow()
{
    void **status = &gStatus;
    if (((Status *)(*status))->getCurrentCampaignMission() == 0x9a) {
        if (!((uint32_t)(I(this, 0x1c) - 1) < 9)) {
            return 0;
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9d) {
        if (!((uint32_t)(I(this, 0x1c) - 2) < 3)) {
            return 0;
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9e && I(this, 0x1c) > 1) {
        return 0;
    }
    return 1;
}

// ---- resetStartSequenceOver_146030.cpp ----
void LevelScript::resetStartSequenceOver()
{
    char *self = B(this, 0);
    *(volatile int *)(self + 0x24) = 0;
    *(volatile uint8_t *)(self + 0x21) = 0;
}

// ---- skipCutscene_145e94.cpp ----
typedef void *(*LevelListProc)(Level *);
typedef void (*ReadWaypointProc)(StackVector *out, void *waypoint);
typedef void (*SetVectorProc)(void *self, StackVector *value);
typedef void (*VirtualCommandProc)(void *self, int a, int b, int c);


static volatile LevelListProc gLevelListProc;

static inline void *firstListEntry(void *list)
{
    return ((void **)P(list, 4))[0];
}

void LevelScript::skipCutscene()
{
    StackVector position;
    void **status = &gStatus;

    int mission = ((Status *)(*status))->getCurrentCampaignMission();
    if (mission == 0x9a) {
        if ((uint32_t)(I(this, 0x1c) - 1) < 9) {
            I(this, 0x1c) = 9;
            for (int i = 0; i != 8; ++i) {
                void *messages = (void *)((Level *)((Level *)P(this, 0x18)))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->trigger();
                messages = (void *)((Level *)((Level *)P(this, 0x18)))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->finish();
            }

            I(this, 0x90) = 0x7d1;
            I(this, 0x94) = 0;

            LevelListProc getList = gLevelListProc;
            void *list = getList((Level *)P(this, 0x18));
            ((PlayerFighter *)(firstListEntry(list)))->setAIDisabled(false);

            list = getList((Level *)P(this, 0x18));
            void *fighter = firstListEntry(list);

            list = getList((Level *)P(this, 0x18));
            void *route = KIPlayer_getRoute(firstListEntry(list));
            void *waypoint = ((Route *)(route))->getWaypoint();
            ReadWaypointProc readWaypoint = *(ReadWaypointProc *)((char *)P(waypoint, 0) + 0x28);
            readWaypoint(&position, waypoint);
            SetVectorProc setVector = *(SetVectorProc *)((char *)P(fighter, 0) + 0x44);
            setVector(fighter, &position);
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9d) {
        if (I(this, 0x1c) <= 4) {
            for (int i = 0; i != 4; ++i) {
                void *messages = (void *)((Level *)((Level *)P(this, 0x18)))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->trigger();
                messages = (void *)((Level *)((Level *)P(this, 0x18)))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->finish();
            }
            I(this, 0x90) = 0x4651;
            I(this, 0x94) = 0;
            I(this, 0x1c) = 4;
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9e) {
        I(this, 0x98) = 0x2ee1;
        I(this, 0x9c) = 0;
        for (int i = 0; i != 3; ++i) {
            void *messages = (void *)((Level *)((Level *)P(this, 0x18)))->getMessages();
            ((RadioMessage *)(((void **)P(messages, 4))[i]))->trigger();
            messages = (void *)((Level *)((Level *)P(this, 0x18)))->getMessages();
            ((RadioMessage *)(((void **)P(messages, 4))[i]))->finish();
        }

        LevelListProc getList = gLevelListProc;
        void *list = getList((Level *)P(this, 0x18));
        void *player = firstListEntry(list);
        VirtualCommandProc command = *(VirtualCommandProc *)((char *)P(player, 0) + 0x48);
        command(player, 0x000ba51e, 0, 0x000ba4b6);

        list = getList((Level *)P(this, 0x18));
        ((KIPlayer *)(firstListEntry(list)))->setVisible(true);

        list = getList((Level *)P(this, 0x18));
        I(this, 0x1c) = 2;
    }
}

// ---- process_137d18.cpp ----
void LevelScript::process(int delta)
{
    Level *level = (Level *)P(this, 0x18);
    void *messages = (void *)((Level *)(level))->getMessages();
    void *player = (void *)((Level *)(level))->getPlayer();
    void *activeMessages = Level_getActiveMessages(level);
    int mission = ((Status *)(gStatus))->getCurrentCampaignMission();

    (void)messages;
    (void)player;
    (void)activeMessages;

    if (mission != 0) {
        float frameDelta = (float)delta * 0.001f;
        TargetFollowCamera_update((TargetFollowCamera *)P(this, 0x14), frameDelta, 0, 0);
    }
}

// ---- lookBehind_145d24.cpp ----
void LevelScript::lookBehind()
{
    TargetFollowCamera_setTargetOffset(P(this, 0x14), StackVector(0.0f, 0.0f, -950.0f));
    TargetFollowCamera_setCamOffset(P(this, 0x14), StackVector(0.0f, 600.0f, 2230.0f));
}

// ---- LevelScript_135adc.cpp ----
LevelScript::LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera)
{
    P(this, 0x48) = 0;
    I(this, 0x38) = 0;
    I(this, 0x3c) = 0;
    I(this, 0x40) = 0;
    I(this, 0x44) = 0;
    I(this, 0x28) = 0;
    I(this, 0x2c) = 0;
    I(this, 0x30) = 0;
    I(this, 0x34) = 0;

    Matrix_ctor(B(this, 0x4c));

    P(this, 0xd0) = hud;
    P(this, 0xd4) = radar;
    P(this, 0x14) = camera;
    P(this, 0x18) = level;
    Hud_drawTitleImage(hud, false);

    I(this, 0x1c) = 0;
    US(this, 0x20) = 1;
    US(this, 0x10) = 0x100;

    I(this, 0x8c) = 0;
    I(this, 0x90) = 0;
    I(this, 0x94) = 0;
    I(this, 0x98) = 0;
    P(this, 0xac) = 0;
    P(this, 0xb0) = 0;
    P(this, 0xb4) = 0;
    P(this, 0xb8) = 0;
    P(this, 0xbc) = 0;
    P(this, 0xc0) = 0;
    P(this, 0xc4) = 0;
    P(this, 0xc8) = 0;
    P(this, 0xcc) = 0;
    I(this, 0x8) = 0;
    I(this, 0xc) = 0;
    I(this, 0x0) = ((Level *)(level))->getTimeLimit();
    I(this, 0x24) = 0;
    P(this, 0xd8) = 0;
    P(this, 0xdc) = 0;
    US(this, 0x12) = 0;

    TargetFollowCamera_setLookAtCam(camera, true);

    void *player = (void *)((Level *)(level))->getPlayer();
    if (player == 0) {
        UC(this, 0x20) = 0;
    } else {
        ((Player *)(P(player, 0)))->setVulnerable(false);
    }

    player = (void *)((Level *)(level))->getPlayer();
    ((PlayerEgo *)(player))->setCollide(false);

    if (((Status *)(gStatus))->getCurrentCampaignMission() == 0) {
        I(this, 0x90) = 0;
        I(this, 0x94) = 0;
        UC(this, 0x21) = 1;
        TargetFollowCamera_setLookAtCam(camera, true);
    }
}
