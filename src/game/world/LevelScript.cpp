#include "game/world/LevelScript.h"
#include "game/core/Globals.h"
#include "game/core/RadioMessage.h"
#include "game/ship/Player.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"
#include "game/ui/Hud.h"
#include "game/ui/Layout.h"
#include "game/world/StarSystem.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/render/AEGeometry.h"
#include "game/world/Level.h"
#include "game/mission/Status.h"
#include "game/ship/PlayerEgo.h"
#include "game/ship/PlayerFighter.h"
#include "game/world/Route.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "game/ship/PlayerWormHole.h"

static Station *gProgrammedStation = nullptr;

struct StackVector {
    float v[3];

    StackVector() {
    }

    StackVector(float x, float y, float z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    operator Vector *() { return reinterpret_cast<Vector *>(v); }
};

typedef void (*RenderProc)(AEGeometry *);

typedef unsigned char * (*LevelListProc)(Level *);

typedef void (*ReadWaypointProc)(StackVector *out, Waypoint *waypoint);

typedef void (*SetVectorProc)(unsigned char *self, StackVector *value);

typedef void (*VirtualCommandProc)(unsigned char *self, int a, int b, int c);

static RenderProc gRenderProc;
static LevelListProc gLevelListProc;

static inline unsigned char *arrayData(unsigned char *array) {
    return *reinterpret_cast<unsigned char **>(array + 4);
}

static inline unsigned char *firstListEntry(unsigned char *list) {
    return reinterpret_cast<unsigned char **>(arrayData(list))[0];
}

void LevelScript::render3D() {
    if (m_pGeometry6) m_pGeometry6->render();
    if (m_pGeometry5) m_pGeometry5->render();
    if (m_pGeometry0) m_pGeometry0->render();
    if (m_pGeometry1) m_pGeometry1->render();
    if (m_pGeometry2) m_pGeometry2->render();
    if (m_pGeometry3) m_pGeometry3->render();

    if (m_pExplosion &&m_pExplosion


    ->
    isPlaying()
    )
    {
        m_pExplosion->render();
    }

    if (m_bRenderParticles && m_pParticleGeom0) {
        RenderProc render = gRenderProc;
        render(m_pParticleGeom0);
        render(m_pParticleGeom1);
        render(m_pParticleGeom2);
    }
}

uint8_t LevelScript::startSequenceOver() {
    return m_bStartSequenceOver;
}

void LevelScript::resetCamera(Level *level) {
    if (level->getPlayer() != 0) {
        PlayerEgo *player = (PlayerEgo *) level->getPlayer();
        m_pCamera->setTarget(player->geometry);
        m_pCamera->setTargetOffset(Vector{0.0f, 600.0f, -650.0f});
        m_pCamera->setCamOffset(Vector{0.0f, 600.0f, -1338.0f});
    }
}

void LevelScript::skipSequence() {
    if (field_0x24 > 0 && Globals::status->getCurrentCampaignMission() > 0) {
        field_0x24 = 0x1b59;
        PlayerEgo *player = m_pLevel->getPlayer();
        (void) player; /* virtual Player flag setter — resolved via vtable */
    }
}

uint8_t LevelScript::startSequence() {
    return (uint8_t) m_bStartSequence;
}

void LevelScript::setAutoPilotToProgrammedStation() {
    if (gProgrammedStation == 0) {
        return;
    }

    Station *programmed = gProgrammedStation;
    if (Globals::status->getStation()->equals(programmed)) {
        gProgrammedStation = 0;
        return;
    }

    SolarSystem *system = (SolarSystem *) Globals::status->getSystem();
    PlayerEgo *player;
    unsigned char *target;

    if (system->stationIsInSystem(programmed)) {
        player = m_pLevel->getPlayer();
        unsigned char *targets = (unsigned char *) ((StarSystem *) m_pLevel->getStarSystem())->getPlanetTargets();
        int targetIndex = system->getStationEnumIndex(programmed->getIndex());
        target = reinterpret_cast<unsigned char **>(arrayData(targets))[targetIndex];
    } else if (system->currentOrbitHasWarpGate()) {
        player = m_pLevel->getPlayer();
        Array<KIPlayer *> *landmarks = m_pLevel->getLandmarks();
        target = (unsigned char *) (*landmarks)[1];
    } else {
        int warpGateIndex = system->getWarpGateEnumIndex();
        if (warpGateIndex < 0) {
            return;
        }
        player = m_pLevel->getPlayer();
        unsigned char *targets = (unsigned char *) ((StarSystem *) m_pLevel->getStarSystem())->getPlanetTargets();
        target = reinterpret_cast<unsigned char **>(arrayData(targets))[warpGateIndex];
    }

    (void) player;
    (void) target; /* virtual Player autopilot-target setter — resolved via vtable */
}

LevelScript::~LevelScript() {
    delete m_pGeometry6;
    m_pGeometry6 = 0;
    delete m_pGeometry5;
    m_pGeometry5 = 0;
    delete m_pGeometry0;
    m_pGeometry0 = 0;
    delete m_pGeometry1;
    m_pGeometry1 = 0;
    delete m_pGeometry2;
    m_pGeometry2 = 0;
    delete m_pGeometry3;
    m_pGeometry3 = 0;
    delete m_pParticleGeom0;
    m_pParticleGeom0 = 0;
    delete m_pParticleGeom1;
    m_pParticleGeom1 = 0;
    delete m_pParticleGeom2;
    m_pParticleGeom2 = 0;
    delete m_pExplosion;
    m_pExplosion = 0;
    delete m_pGeometry4;
    m_pGeometry4 = 0;
}

uint32_t LevelScript::canSkipCutsceneNow() {
    int mission = Globals::status->getCurrentCampaignMission();
    if (mission == 0x9a) {
        if (!((uint32_t)(m_nState - 1) < 9)) {
            return 0;
        }
    } else if (mission == 0x9d) {
        if (!((uint32_t)(m_nState - 2) < 3)) {
            return 0;
        }
    } else if (mission == 0x9e && m_nState > 1) {
        return 0;
    }
    return 1;
}

void LevelScript::resetStartSequenceOver() {
    field_0x24 = 0;
    m_bStartSequenceOver = 0;
}

void LevelScript::skipCutscene() {
    StackVector position;

    int mission = Globals::status->getCurrentCampaignMission();
    if (mission == 0x9a) {
        if ((uint32_t)(m_nState - 1) < 9) {
            m_nState = 9;
            for (int i = 0; i != 8; ++i) {
                Array<void *> *messages = m_pLevel->getMessages();
                // lint: void_ptr (matches cross-file Level::getMessages() -> Array<void*>*)
                ((RadioMessage *) ((*messages)[i]))->trigger();
                messages = m_pLevel->getMessages();
                ((RadioMessage *) ((*messages)[i]))->finish();
            }

            m_nScriptTimerA = 0x7d1;
            m_nScriptCounterA = 0;

            LevelListProc getList = gLevelListProc;
            unsigned char *list = getList(m_pLevel);
            ((PlayerFighter *) firstListEntry(list))->setAIDisabled(false);

            list = getList(m_pLevel);
            unsigned char *fighter = firstListEntry(list);

            list = getList(m_pLevel);
            Route *route = ((KIPlayer *) firstListEntry(list))->getRoute();
            Waypoint *waypoint = route->getWaypoint();
            ReadWaypointProc readWaypoint =
                    *(ReadWaypointProc *) (*reinterpret_cast<char **>(waypoint) + 0x28);
            readWaypoint(&position, waypoint);
            SetVectorProc setVector =
                    *(SetVectorProc *) (*reinterpret_cast<char **>(fighter) + 0x44);
            setVector(fighter, &position);
        }
    } else if (mission == 0x9d) {
        if (m_nState <= 4) {
            for (int i = 0; i != 4; ++i) {
                Array<void *> *messages = m_pLevel->getMessages();
                // lint: void_ptr (matches cross-file Level::getMessages() -> Array<void*>*)
                ((RadioMessage *) ((*messages)[i]))->trigger();
                messages = m_pLevel->getMessages();
                ((RadioMessage *) ((*messages)[i]))->finish();
            }
            m_nScriptTimerA = 0x4651;
            m_nScriptCounterA = 0;
            m_nState = 4;
        }
    } else if (mission == 0x9e) {
        m_nScriptTimerB = 0x2ee1;
        m_nScriptCounterB = 0;
        for (int i = 0; i != 3; ++i) {
            Array<void *> *messages = m_pLevel->getMessages();
            // lint: void_ptr (matches cross-file Level::getMessages() -> Array<void*>*)
            ((RadioMessage *) ((*messages)[i]))->trigger();
            messages = m_pLevel->getMessages();
            ((RadioMessage *) ((*messages)[i]))->finish();
        }

        LevelListProc getList = gLevelListProc;
        unsigned char *list = getList(m_pLevel);
        unsigned char *player = firstListEntry(list);
        VirtualCommandProc command =
                *(VirtualCommandProc *) (*reinterpret_cast<char **>(player) + 0x48);
        command(player, 0x000ba51e, 0, 0x000ba4b6);

        list = getList(m_pLevel);
        ((KIPlayer *) firstListEntry(list))->setVisible(true);

        list = getList(m_pLevel);
        m_nState = 2;
    }
}

void LevelScript::process(int delta) {
    // Reconstruction pass 1 of _ZN11LevelScript7processEi (original 0xdf90 bytes @ 0x137d18).
    // Implemented faithfully here: prologue accessors + mission dispatch (0x137d18),
    // the mission==1 intro state machine (0x137d82-0x137e82) and the shared "active" tail
    // (0x137f7c-0x13812a). The mission==0 intro choreography and the giant mission>=2 general
    // state machine / per-mission switch are DEFERRED (see markers below) for a later pass.
    Level *level = m_pLevel;
    Array<void *> *messages = level->getMessages(); // lint: void_ptr (matches Level::getMessages() -> Array<void*>*)
    PlayerEgo *player = level->getPlayer();
    Array<KIPlayer *> *enemies = level->getEnemies();
    int mission = Globals::status->getCurrentCampaignMission();

    if (mission == 0) {
        // DEFERRED (pass 1): mission-0 intro cutscene choreography, disasm 0x137e84-0x137f76.
        // Positions/directions the three intro ships, drives the camera local matrix via
        // PaintCanvas Camera{GetCurrent,GetLocal}+MatrixSetTranslation, and gates on m_nState.
        // Not decoded here (ambiguous FP register fills for the direction vectors). Falls through
        // to the shared active tail below, matching the original.
        (void) player;
    } else if (mission == 1) {
        // mission==1 intro (disasm 0x137d82). Slowly rotates the player ship and pans the camera
        // while radio message #2 plays; on fade-out it switches to the next scene.
        m_pLevel->lodManager->forceUpdate(delta, true);

        // player->rotate(0, angle, angle) with angle = ((delta<<1)&~15) * 2^-16 * 2*pi (0x137d90).
        float angle = (float) ((delta << 1) & -16) * 1.52587890625e-05f * 6.2831855f;
        player->rotate(0.0f, angle, angle);

        // enemies[0]: sample z, put to sleep, then creep forward proportional to depth (0x137dbe).
        Vector enemyPos = (*enemies)[0]->getPosition();
        (*enemies)[0]->setToSleep();
        float depth = enemyPos.z / -5000.0f;
        if (!(depth < 1.0f)) {
            depth = 1.0f;
        }
        (*enemies)[0]->geometry->moveForward((float) delta * 0.2f * depth);

        // Radio message #2 drives the fade-out and scene switch (0x137e18).
        RadioMessage *msg2 = (RadioMessage *) ((*messages)[2]);
        if (msg2->isOver() && m_nState == 0) {
            m_nState = 1;
            Globals::layout->startFade(true, 255, 5000);
        }
        if (m_nState == 1 && !Globals::layout->isFading()) {
            // DEFERRED (pass 1): scene transition at disasm 0x138a66 -- once the fade completes,
            // the original calls ApplicationManager::SetCurrentApplicationModule(scenes[1]) and
            // returns. Faithful early-out placeholder until that tail is decoded.
            return;
        }
        m_pCamera->translate((float) ((double) delta * 0.1), 0.0f, 0.0f);
    } else {
        // mission>=2 path (disasm 0x1381b2). This is "Chain A": a long chain of per-mission
        // *intro* choreographies (approach cutscenes) that each run once, then fall through to
        // the shared "active" tail below.
        //
        // DEFERRED (pass 2): the Chain A per-mission intro choreographies are not decoded here.
        //   - mission 0x51 alien-orbit approach: disasm 0x1381b2-0x13832c (+ substates 0x138f08,
        //     0x139136); heavy FP camera/vector work.
        //   - mission 0x4e (78): disasm 0x13832c; mission chain continues at 0x138574 ...
        // These converge to the shared tail (0x137f7c). We faithfully fall through to it so the
        // mission>=2 *general state machine* (the passedTime>7000 deactivation) now runs -- this
        // was unreachable in pass 1 (it returned early here).
    }

    // --- shared "active" tail / general state machine (disasm 0x137f7c); runs for all missions ---
    if (m_bStartSequence != 0) {
        m_pLevel->lodManager->forceUpdate(30, false);

        if (Globals::status->getWingmen() != 0) {
            Array<KIPlayer *> *wingmen = m_pLevel->getEnemies();
            for (unsigned int i = 0; i < wingmen->count; ++i) {
                if ((*wingmen)[i]->isWingMan()) {
                    // DEFERRED (pass 1): (*wingmen)[i] flag @0x12d = 1 (no named field @0x12d yet).
                }
            }
        }

        field_0x24 += delta; // passedTime += delta
        if (m_pLevel->getPlayer() != 0 &&
            Globals::status->getCurrentCampaignMission() >= 2 &&
            field_0x24 >= 7001) {
            field_0x24 = 0;
            m_bStartSequence = 0;    // active = false
            m_bStartSequenceOver = 1; // startSequenceIsOver = true
            m_pCamera->setLookAtCam(false);
            ((Player *) m_pLevel->getPlayer())->setVulnerable(true);
            m_nFlags = m_nFlags & 0xFF; // clear cinematicBreak_ (high byte @0x11)
            m_pLevel->getPlayer()->setCollide(true);

            if (Level::programmedStation != 0) {
                if (!Globals::status->getStation()->equals(Level::programmedStation)) {
                    m_pHud->hudEvent(5, m_pLevel->getPlayer(), 0);
                }
            }

            Level::initStreamOutPosition = 0;
            if (Level::doInstantJump == 0) {
                setAutoPilotToProgrammedStation();
            }

            if (Globals::status->getWingmen() != 0) {
                Array<KIPlayer *> *wingmen = m_pLevel->getEnemies();
                for (unsigned int i = 0; i < wingmen->count; ++i) {
                    if ((*wingmen)[i]->isWingMan()) {
                        // DEFERRED (pass 1): (*wingmen)[i] flag @0x12d = 0 (no named field @0x12d).
                    }
                }
            }
        }
    }

    // --- pre-switch mission specials (disasm 0x13812a); run for all missions ---
    // Re-read the campaign mission (fresh call at 0x13812e; -Oz has no CSE).
    int switchMission = Globals::status->getCurrentCampaignMission();
    if (switchMission == 5) {
        // disasm 0x13813c: on the first frame, park enemies[0] ahead of the player and wake it.
        if (m_nState == 0) {
            Vector *scratch = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
            *scratch = player->getPosition();
            (*enemies)[0]->setPosition(scratch->x + 5000.0f, scratch->y, scratch->z + 30000.0f);
            (*enemies)[0]->geometry->rotate(0.0f, 3.1415927f, 0.0f); // const @0x13853c = pi
            (*enemies)[0]->awake();
            m_nState = 1;
        }
    } else if (switchMission == 26 && Globals::status->inAlienOrbit()) {
        // disasm 0x1382dc: in alien orbit, once radio message #1 fires, spin up the exit wormhole.
        if (((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0) {
            m_nState = 1;
            Array<KIPlayer *> *landmarks = m_pLevel->getLandmarks();
            ((PlayerWormHole *) (*landmarks)[3])->reset(true);
        }
    }
    // DEFERRED (pass 2): further pre-switch specials -- mission 50 (disasm 0x1383e4) and mission 51
    // (disasm 0x13876c) share the exit-wormhole cutscene body at 0x139ac2.

    // DEFERRED (pass 2): the per-mission campaign switch (disasm 0x13879e-0x144e36, ~50KB, the bulk
    // of this function). Guarded by Status::getMission()->isCampaignMission() && !isEmpty() (or
    // mission==42). The dispatch on getCurrentCampaignMission() (0x1387ca) reaches, by mission id:
    //   14->0x13a9b2  16->0x13b014  21->0x13ab24  24->0x13a930  29->0x139ca2  40->0x13a4d8
    //   41->0x139d56  42->0x13abaa  56->0x139f14  80->0x139e34  91->0x13ac42  92->0x139ffe
    //   94->0x13acac  102->0x13a658 105->0x13aea4 114->0x13afde 125->0x139f9c 154->0x13a76e
    //   157->0x13a308 158 (inline)  64..73->0x138c3e  131..145->0x138a8a
    // Each target is a dense per-frame cutscene handler (radio-message gated state machine with
    // camera/particle/sound choreography and GOT-loaded helper thunks). Not decoded in this pass
    // to avoid fabrication; the v2 bodies diverge substantially from the DeepOpen v1 switch.
}

void LevelScript::lookBehind() {
    m_pCamera->setTargetOffset(Vector{0.0f, 0.0f, -950.0f});
    m_pCamera->setCamOffset(Vector{0.0f, 600.0f, 2230.0f});
}

LevelScript::LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera) {
    field_0x48 = 0;
    field_0x38 = 0;
    field_0x3c = 0;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x28 = 0;
    field_0x2c = 0;
    field_0x30 = 0;
    field_0x34 = 0;

    m_matrix = AbyssEngine::AEMath::Matrix();

    m_pHud = hud;
    m_pRadar = radar;
    m_pCamera = camera;
    m_pLevel = level;
    hud->drawTitleImage(false);

    m_nState = 0;
    m_bStartSequence = 1;
    m_nFlags = 0x100;

    field_0x8c = 0;
    m_nScriptTimerA = 0;
    m_nScriptCounterA = 0;
    m_nScriptTimerB = 0;
    m_pParticleGeom0 = 0;
    m_pParticleGeom1 = 0;
    m_pParticleGeom2 = 0;
    m_pGeometry0 = 0;
    m_pGeometry1 = 0;
    m_pGeometry2 = 0;
    m_pGeometry3 = 0;
    m_pExplosion = 0;
    m_pGeometry4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    m_nTimeLimit = level->getTimeLimit();
    field_0x24 = 0;
    m_pGeometry5 = 0;
    m_pGeometry6 = 0;
    field_0x12 = 0;

    camera->setLookAtCam(true);

    PlayerEgo *player = level->getPlayer();
    if (player == 0) {
        m_bStartSequence = 0;
    } else {
        ((Player *) player)->setVulnerable(false);
    }

    player = level->getPlayer();
    player->setCollide(false);

    if (Globals::status->getCurrentCampaignMission() == 0) {
        m_nScriptTimerA = 0;
        m_nScriptCounterA = 0;
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
    }
}

int LevelScript::getEvent() {
    return m_nState;
}

void LevelScript::setEvent(int event) {
    m_nState = event;
}
