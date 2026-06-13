// Offset accessors come from a single source each within this TU:
//   I() from Explosion.h, B() from PlayerEgo.h, P() from LevelScript.h.
#include "gof2/game/world/LevelScript.h"   // defines P()
#include "gof2/game/mission/Explosion.h"     // defines I()
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/world/StarSystem.h"
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/world/Level.h"
// RadioMessage.h is intentionally NOT included here: it defines stub structs
// (PlayerEgo, Route, Level, Player, LevelScript, ...) that collide with the real
// class headers used by this TU. This TU only needs RadioMessage::trigger/finish,
// so forward-declare that minimal interface instead.
class RadioMessage {
public:
    void finish();
    void trigger();
};
#include "gof2/game/mission/Status.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/ship/PlayerEgo.h"     // defines B()
#include "gof2/game/ship/PlayerFighter.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/world/Station.h"

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
    // The camera helpers take a Vector* whose first 3 floats are the offset; the
    // original ABI passed the address of this stack temporary. Expose that as an
    // implicit Vector* conversion so the recovered call sites read naturally.
    operator Vector *() { return (Vector *)bytes; }
};

extern "C" void *gStatus;
extern "C" void Player_setUnknown(void *player, bool enabled);
extern "C" void *gProgrammedStation;
extern "C" int Station_getIndex(void *station);
extern "C" void Player_setAutoPilotTarget(void *player, void *target);
extern "C" void *Explosion_dtor(void *explosion);

typedef void (*RenderProc)(void *);

static volatile RenderProc gRenderProc;

void LevelScript::render3D()
{
    if (this->m_pGeometry6 != 0) {
        ((AEGeometry *)(this->m_pGeometry6))->render();
    }
    if (this->m_pGeometry5 != 0) {
        ((AEGeometry *)(this->m_pGeometry5))->render();
    }
    if (this->m_pGeometry0 != 0) {
        ((AEGeometry *)(this->m_pGeometry0))->render();
    }
    if (this->m_pGeometry1 != 0) {
        ((AEGeometry *)(this->m_pGeometry1))->render();
    }
    if (this->m_pGeometry2 != 0) {
        ((AEGeometry *)(this->m_pGeometry2))->render();
    }
    if (this->m_pGeometry3 != 0) {
        ((AEGeometry *)(this->m_pGeometry3))->render();
    }
    if (this->m_pExplosion != 0 && ((Explosion *)(this->m_pExplosion))->isPlaying() != 0) {
        ((Explosion *)(this->m_pExplosion))->render();
    }
    if (this->field_0xa8 != 0 && this->m_pParticleGeom0 != 0) {
        RenderProc render = gRenderProc;
        render(this->m_pParticleGeom0);
        render(this->m_pParticleGeom1);
        return render(this->m_pParticleGeom2);
    }
}

uint8_t LevelScript::startSequenceOver()
{
    return this->m_bStartSequenceOver;
}

void LevelScript::resetCamera(Level *level)
{
    if (((Level *)(level))->getPlayer() != 0) {
        void *camera = this->m_pCamera;
        void *player = (void *)((Level *)(level))->getPlayer();
        ((TargetFollowCamera *)(camera))->setTarget(P(player, 0x8));
        this->m_pCamera->setTargetOffset(StackVector(0.0f, 600.0f, -650.0f));
        this->m_pCamera->setCamOffset(StackVector(0.0f, 600.0f, -1338.0f));
    }
}

void LevelScript::skipSequence()
{
    if (this->field_0x24 > 0 && ((Status *)(gStatus))->getCurrentCampaignMission() > 0) {
        this->field_0x24 = 0x1b59;
        void *player = (void *)((Level *)(this->m_pLevel))->getPlayer();
        return Player_setUnknown(P(player, 0), true);
    }
}

uint8_t LevelScript::startSequence()
{
    return (uint8_t)this->m_bStartSequence;
}

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
            player = (void *)((Level *)(this->m_pLevel))->getPlayer();
            void *targets = ((StarSystem *)((void *)((Level *)(this->m_pLevel))->getStarSystem()))->getPlanetTargets();
            void *system = (void *)((Status *)(*status))->getSystem();
            int stationIndex = Station_getIndex(*programmedStation);
            int targetIndex = ((SolarSystem *)(system))->getStationEnumIndex(stationIndex);
            target = ((void **)P(targets, 4))[targetIndex];
        } else if (((SolarSystem *)(((Status *)(*status))->getSystem()))->currentOrbitHasWarpGate() != 0) {
            player = (void *)((Level *)(this->m_pLevel))->getPlayer();
            void *landmarks = (void *)((Level *)(this->m_pLevel))->getLandmarks();
            target = ((void **)P(landmarks, 4))[1];
        } else {
            int warpGateIndex = ((SolarSystem *)(((Status *)(*status))->getSystem()))->getWarpGateEnumIndex();
            if (warpGateIndex < 0) {
                return;
            }
            player = (void *)((Level *)(this->m_pLevel))->getPlayer();
            void *targets = ((StarSystem *)((void *)((Level *)(this->m_pLevel))->getStarSystem()))->getPlanetTargets();
            target = ((void **)P(targets, 4))[warpGateIndex];
        }
        return Player_setAutoPilotTarget(player, target);
    }
}

LevelScript::~LevelScript()
{
    void *geometry = this->m_pGeometry6;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry6 = 0;

    geometry = this->m_pGeometry5;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry5 = 0;

    geometry = this->m_pGeometry0;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry0 = 0;

    geometry = this->m_pGeometry1;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry1 = 0;

    geometry = this->m_pGeometry2;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry2 = 0;

    geometry = this->m_pGeometry3;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry3 = 0;

    geometry = this->m_pParticleGeom0;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pParticleGeom0 = 0;

    geometry = this->m_pParticleGeom1;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pParticleGeom1 = 0;

    geometry = this->m_pParticleGeom2;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pParticleGeom2 = 0;

    void *explosion = this->m_pExplosion;
    if (explosion != 0) {
        ::operator delete(Explosion_dtor(explosion));
    }
    this->m_pExplosion = 0;

    geometry = this->m_pGeometry4;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    this->m_pGeometry4 = 0;
}

uint32_t LevelScript::canSkipCutsceneNow()
{
    void **status = &gStatus;
    if (((Status *)(*status))->getCurrentCampaignMission() == 0x9a) {
        if (!((uint32_t)(this->m_nState - 1) < 9)) {
            return 0;
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9d) {
        if (!((uint32_t)(this->m_nState - 2) < 3)) {
            return 0;
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9e && this->m_nState > 1) {
        return 0;
    }
    return 1;
}

void LevelScript::resetStartSequenceOver()
{
    this->field_0x24 = 0;
    this->m_bStartSequenceOver = 0;
}

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
        if ((uint32_t)(this->m_nState - 1) < 9) {
            this->m_nState = 9;
            for (int i = 0; i != 8; ++i) {
                void *messages = (void *)((Level *)(this->m_pLevel))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->trigger();
                messages = (void *)((Level *)(this->m_pLevel))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->finish();
            }

            this->field_0x90 = 0x7d1;
            this->field_0x94 = 0;

            LevelListProc getList = gLevelListProc;
            void *list = getList(this->m_pLevel);
            ((PlayerFighter *)(firstListEntry(list)))->setAIDisabled(false);

            list = getList(this->m_pLevel);
            void *fighter = firstListEntry(list);

            list = getList(this->m_pLevel);
            void *route = ((KIPlayer *)(firstListEntry(list)))->getRoute();
            void *waypoint = ((Route *)(route))->getWaypoint();
            ReadWaypointProc readWaypoint = *(ReadWaypointProc *)((char *)P(waypoint, 0) + 0x28);
            readWaypoint(&position, waypoint);
            SetVectorProc setVector = *(SetVectorProc *)((char *)P(fighter, 0) + 0x44);
            setVector(fighter, &position);
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9d) {
        if (this->m_nState <= 4) {
            for (int i = 0; i != 4; ++i) {
                void *messages = (void *)((Level *)(this->m_pLevel))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->trigger();
                messages = (void *)((Level *)(this->m_pLevel))->getMessages();
                ((RadioMessage *)(((void **)P(messages, 4))[i]))->finish();
            }
            this->field_0x90 = 0x4651;
            this->field_0x94 = 0;
            this->m_nState = 4;
        }
    } else if (((Status *)(*status))->getCurrentCampaignMission() == 0x9e) {
        this->field_0x98 = 0x2ee1;
        this->field_0x9c = 0;
        for (int i = 0; i != 3; ++i) {
            void *messages = (void *)((Level *)(this->m_pLevel))->getMessages();
            ((RadioMessage *)(((void **)P(messages, 4))[i]))->trigger();
            messages = (void *)((Level *)(this->m_pLevel))->getMessages();
            ((RadioMessage *)(((void **)P(messages, 4))[i]))->finish();
        }

        LevelListProc getList = gLevelListProc;
        void *list = getList(this->m_pLevel);
        void *player = firstListEntry(list);
        VirtualCommandProc command = *(VirtualCommandProc *)((char *)P(player, 0) + 0x48);
        command(player, 0x000ba51e, 0, 0x000ba4b6);

        list = getList(this->m_pLevel);
        ((KIPlayer *)(firstListEntry(list)))->setVisible(true);

        list = getList(this->m_pLevel);
        this->m_nState = 2;
    }
}

void LevelScript::process(int delta)
{
    Level *level = this->m_pLevel;
    void *messages = (void *)((Level *)(level))->getMessages();
    void *player = (void *)((Level *)(level))->getPlayer();
    void *activeMessages = ((Level *)(level))->getActiveMessages();
    int mission = ((Status *)(gStatus))->getCurrentCampaignMission();

    (void)messages;
    (void)player;
    (void)activeMessages;

    if (mission != 0) {
        float frameDelta = (float)delta * 0.001f;
        ((TargetFollowCamera *)((TargetFollowCamera *)this->m_pCamera))->update(frameDelta);
    }
}

void LevelScript::lookBehind()
{
    ((TargetFollowCamera *)(this->m_pCamera))->setTargetOffset(StackVector(0.0f, 0.0f, -950.0f));
    ((TargetFollowCamera *)(this->m_pCamera))->setCamOffset(StackVector(0.0f, 600.0f, 2230.0f));
}

LevelScript::LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera)
{
    this->field_0x48 = 0;
    this->field_0x38 = 0;
    this->field_0x3c = 0;
    this->field_0x40 = 0;
    this->field_0x44 = 0;
    this->field_0x28 = 0;
    this->field_0x2c = 0;
    this->field_0x30 = 0;
    this->field_0x34 = 0;

    this->m_matrix.initIdentity();

    this->m_pHud = hud;
    this->m_pRadar = radar;
    this->m_pCamera = camera;
    this->m_pLevel = level;
    ((Hud *)(hud))->drawTitleImage(false);

    this->m_nState = 0;
    this->m_bStartSequence = 1;
    this->m_nFlags = 0x100;

    this->field_0x8c = 0;
    this->field_0x90 = 0;
    this->field_0x94 = 0;
    this->field_0x98 = 0;
    this->m_pParticleGeom0 = 0;
    this->m_pParticleGeom1 = 0;
    this->m_pParticleGeom2 = 0;
    this->m_pGeometry0 = 0;
    this->m_pGeometry1 = 0;
    this->m_pGeometry2 = 0;
    this->m_pGeometry3 = 0;
    this->m_pExplosion = 0;
    this->m_pGeometry4 = 0;
    this->field_0x8 = 0;
    this->field_0xc = 0;
    this->m_nTimeLimit = ((Level *)(level))->getTimeLimit();
    this->field_0x24 = 0;
    this->m_pGeometry5 = 0;
    this->m_pGeometry6 = 0;
    this->field_0x12 = 0;

    ((TargetFollowCamera *)(camera))->setLookAtCam(true);

    void *player = (void *)((Level *)(level))->getPlayer();
    if (player == 0) {
        this->m_bStartSequence = 0;
    } else {
        ((Player *)(P(player, 0)))->setVulnerable(false);
    }

    player = (void *)((Level *)(level))->getPlayer();
    ((PlayerEgo *)(player))->setCollide(false);

    if (((Status *)(gStatus))->getCurrentCampaignMission() == 0) {
        this->field_0x90 = 0;
        this->field_0x94 = 0;
        this->m_bStartSequenceOver = 1;
        ((TargetFollowCamera *)(camera))->setLookAtCam(true);
    }
}

// LevelScript::getEvent() / setEvent() -- accessors for the current scripted-event
// id (m_nState, +0x1c). The cinematic/start-sequence state machine reads this to
// decide which step of the level intro is playing.
int LevelScript::getEvent() {
    return this->m_nState;
}

void LevelScript::setEvent(int event) {
    this->m_nState = event;
}
