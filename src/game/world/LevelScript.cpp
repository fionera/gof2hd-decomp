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
#include "game/ship/PlayerFixedObject.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/world/Route.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "game/ship/PlayerWormHole.h"
#include "game/mission/Mission.h"
#include "game/mission/Objective.h"
#include "game/weapons/Radar.h"
#include "engine/core/GameText.h"

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

int LevelScript::process(int delta) {
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
            return (m_nFlags & 0xFF00) != 0 ? 1 : 0;
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

    // --- the per-mission campaign switch (disasm 0x13879e-0x144e36, the bulk of this function) ---
    // Guard (0x1387a0): only run the switch for a non-empty campaign mission (getMission() called
    // twice, short-circuited), or unconditionally for mission 42. Otherwise fall straight to the
    // post-switch tail (0x144e36).
    if ((Globals::status->getMission()->isCampaignMission() &&
         !Globals::status->getMission()->isEmpty()) ||
        Globals::status->getCurrentCampaignMission() == 42) {
        // Dispatch on the campaign mission id (fresh call at 0x1387ca; -Oz has no CSE). The two
        // range prongs (0x1387ce/0x1387d8) are checked before the linear compare chain, matching
        // the original's jump order.
        int cm = Globals::status->getCurrentCampaignMission();
        if (cm >= 131 && cm <= 145) {
            // DEFERRED 0x138a8a: mission 131..145 range handler (tbh sub-dispatch on m_nState).
        } else if (cm >= 64 && cm <= 73) {
            // DEFERRED 0x138c3e: mission 64..73 range handler.
        } else {
            switch (cm) {
            case 14: { // 0x13a9b2
                if (((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0) {
                    // DEFERRED 0x13ea84: radio-message #1 state-0 sub-branch.
                    break;
                }
                if (((RadioMessage *) ((*messages)[1]))->isOver() && m_nState == 1) {
                    // First-person approach cutscene done: hand control back to the player, hide the
                    // HUD/radar cinematic overlay and instantly kill the type-8 escort wave.
                    // FModSound::updateEvent3DAttributes(15, player pos, {0,0,0}, false) DEFERRED @0x13aa06
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    player->stopShooting(0);
                    m_pHud->visible = 0;
                    m_pRadar->field_0x58 = 0;
                    m_pLevel->flashScreen(3);
                    player->setComputerControlled(true);
                    player->setSpeed(0.0f);
                    player->player->removeAllGuns();
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(player->geometry);
                    m_pCamera->setPosition(player->getPosition());
                    m_pCamera->translate(1000.0f, 700.0f, 1000.0f);
                    for (unsigned int i = 0; i < level->getEnemies()->count; ++i) {
                        if ((*level->getEnemies())[i]->shipGroup == 8) {
                            (*level->getEnemies())[i]->player->setHitpoints(0);
                            (*level->getEnemies())[i]->setDead();
                        }
                    }
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    // -> shared tail 0x13eddc
                }
                break;
            }
            case 16: { // 0x13b014
                // Intro-approach cutscene exit (radio message #0). Once the intro chatter has fired
                // and we are still in state 0, hand control back to the computer, hide the HUD/radar
                // cinematic overlay and reframe the camera behind enemies[0].
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_pHud->visible = 0;
                    m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pRadar->field_0x58 = 0;
                    player->setComputerControlled(true);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                    Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                    camPos += Vector{6000.0f, 4000.0f, 47500.0f};
                    m_pCamera->setPosition(camPos);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 1;
                    // -> post-switch tail (0x144e36)
                }
                break;
            }
            case 21: { // 0x13ab24
                // If nobody has turned hostile yet, and any of enemies[1..3] already carries the
                // enemy flag, flip all of enemies[1..3] to permanent enemies.
                if (m_nState == 0) {
                    int i = 1;
                    while (i <= 3) {
                        Player *ship = (*level->getEnemies())[i]->player;
                        ++i;
                        if (ship->enemyFlagsLo != 0) {
                            for (int j = 1; j != 4; ++j) {
                                (*level->getEnemies())[j]->player->setAlwaysEnemy(true);
                            }
                            break;
                        }
                    }
                }
                // Once radio message #2 fires (still in state 0), commit enemies[1..3] as enemies.
                if (((RadioMessage *) ((*messages)[2]))->isTriggered() && m_nState == 0) {
                    for (int j = 1; j != 4; ++j) {
                        (*level->getEnemies())[j]->player->setAlwaysEnemy(true);
                    }
                    m_nState = 1; // shared tail 0x13aba4 -> 0x13edde
                }
                break;
            }
            case 24: { // 0x13a930
                if (((RadioMessage *) ((*messages)[3]))->isTriggered() && m_nState == 0) {
                    // DEFERRED 0x13e97e: radio-message #3 state-0 sub-branch.
                    break;
                }
                if (((RadioMessage *) ((*messages)[4]))->isTriggered() && m_nState == 1) {
                    // Radio message #4: spin up the exit wormhole (landmarks[3]).
                    Array<KIPlayer *> *landmarks = m_pLevel->getLandmarks();
                    (*landmarks)[3]->setVisible(true);
                    landmarks = m_pLevel->getLandmarks();
                    ((PlayerWormHole *) (*landmarks)[3])->reset(false);
                    landmarks = m_pLevel->getLandmarks();
                    ((PlayerWormHole *) (*landmarks)[3])->open();
                    // FModSound::play(0x22, ...) DEFERRED @0x13a9a2
                    m_nScriptTimerA = 0;
                    m_nScriptCounterA = 0;
                    m_nState = 2;
                    // DEFERRED 0x13c690: continues to the post-switch tail.
                } else {
                    // DEFERRED 0x13c68a
                }
                break;
            }
            case 29: { // 0x139ca2
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
                    // DEFERRED 0x13e1f6: radio-message #0 state-0 sub-branch.
                    break;
                }
                if (((RadioMessage *) ((*messages)[2]))->isOver() && m_nState == 1) {
                    // Cutscene over: retarget the camera to the player, restore the HUD/radar and
                    // player controls, drop the intro geometry and spawn the follow-up objective.
                    m_nState = 2;
                    m_pCamera->setTarget(player->geometry);
                    m_pHud->visible = 1;
                    m_pRadar->field_0x58 = 1;
                    m_nFlags = m_nFlags & 0xFF; // cinematicBreak_ (m_nFlags high byte @0x11) = 0
                    m_pCamera->setLookAtCam(false);
                    player->setComputerControlled(false);
                    player->player->setVulnerable(true);
                    if (m_pGeometry6 != nullptr) {
                        delete m_pGeometry6;
                    }
                    field_0x8 = 0;
                    field_0xc = 0;
                    m_nTimeLimit = 180000;
                    m_pGeometry6 = nullptr;
                    m_pLevel->objectivesA = new Objective(3, 180000, m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    // -> shared tail 0x13e3bc
                }
                break;
            }
            case 40: { // 0x13a4d8
                if (m_nState != 0) {
                    // DEFERRED 0x13e6b8: cold substate (m_nState != 0).
                    break;
                }
                if (!((RadioMessage *) ((*messages)[3]))->isTriggered()) {
                    // DEFERRED 0x13e6b4 -> tail.
                    break;
                }
                // Docking approach done (radio #3): freeze the target on the friend route's first
                // waypoint, wake it, then run the standard cutscene-exit teardown and reframe the
                // camera onto it. (r6/r4 GOT thunks resolve to Level::getFriendRoute / Route::getWaypoint.)
                ((PlayerFixedObject *) (*enemies)[0])->setMoving(true);
                float wx = (float) m_pLevel->getFriendRoute()->getWaypoint(0)->x;
                float wy = (float) m_pLevel->getFriendRoute()->getWaypoint(0)->y;
                float wz = (float) m_pLevel->getFriendRoute()->getWaypoint(0)->z;
                (*enemies)[0]->setPosition(wx, wy, wz); // vtable+0x48 == KIPlayer::setPosition(fff)
                (*enemies)[0]->awake();                 // vtable+0xc  == KIPlayer::awake()
                (*enemies)[0]->setVisible(true);
                (*enemies)[0]->autoPilotState = 1; // KIPlayer+0x28
                player->setTurretMode(false);
                resetCamera(m_pLevel);
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                player->player->setVulnerable(false);
                m_pHud->visible = 0;
                m_pRadar->field_0x58 = 0; // disasm stores m_pRadar+0x48 (same field the sibling exits zero)
                player->stopShooting(0);
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pCamera->setLookAtCam(true);
                Vector camPos40 = (*enemies)[0]->getPosition();
                m_pCamera->setPosition(camPos40);
                m_pCamera->translate(-7000.0f, 500.0f, 17000.0f);
                m_pCamera->setTarget((*enemies)[0]->geometry);
                m_pLevel->lodManager->forceUpdate(delta, false);
                m_nState = 1;
                // -> post-switch tail (0x144e36)
                break;
            }
            case 41: { // 0x139d56
                if (((RadioMessage *) ((*messages)[4]))->isTriggered() && m_nState == 0) {
                    // DEFERRED 0x13e400: radio-message #4 state-0 sub-branch.
                    break;
                }
                if (((RadioMessage *) ((*messages)[5]))->isTriggered() && m_nState == 1) {
                    // Radio message #5 in state 1: finish the mining-plant approach cutscene --
                    // stop rotating the camera, freeze the target, fire its emitters and re-aim.
                    m_nState = 2;
                    m_pCamera->setRotationAroundTarget(false);
                    (*enemies)[0]->geometry->updateReferenceMatrix();
                    m_pLevel->field_74->enableSystemEmit(field_0xe0, true);
                    m_pLevel->field_74->enableSystemEmit(field_0xe4, true);
                    KIPlayer *target = (*m_pLevel->getEnemies())[0];
                    target->player->setHitpoints(9999999);
                    ((PlayerFixedObject *) target)->setMoving(false);
                    m_pCamera->setTarget(target->geometry);
                    Vector pos = target->getPosition();
                    m_pCamera->setPosition(pos);
                    m_pCamera->translate(-3000.0f, -2000.0f, 3000.0f);
                    // FModSound::play(0x9b, ...) DEFERRED @0x139e1c
                    target->player->StopEngineSound();
                    m_nScriptTimerA = 0;
                    m_nScriptCounterA = 0;
                    // -> post-switch tail (0x144e36)
                } else {
                    // DEFERRED 0x13c5f6: message #5 not-triggered / wrong-state sub-branch.
                }
                break;
            }
            case 42: { // 0x13abaa
                if (messages == nullptr) {
                    // DEFERRED 0x13c1a4
                    break;
                }
                if (m_nState != 5) {
                    // DEFERRED 0x13c1a8
                    break;
                }
                if (!((RadioMessage *) ((*messages)[7]))->isOver()) {
                    // DEFERRED 0x13c1a4
                    break;
                }
                // FModSound::play(0x99, ...) DEFERRED @0x13abe2
                field_0x8 = 0;
                field_0xc = 0;
                if (level->objectivesB != nullptr) {
                    delete level->objectivesB;
                }
                level->objectivesB = nullptr;
                if (level->objectivesA != nullptr) {
                    delete level->objectivesA;
                }
                level->objectivesA = nullptr;
                Array<KIPlayer *> *landmarks = level->getLandmarks();
                (void) landmarks; // ((PlayerWormHole*)(*landmarks)[3])->setPosition(x,y,z) DEFERRED @0x13ac26 (vtable+0x48)
                landmarks = m_pLevel->getLandmarks();
                ((PlayerWormHole *) (*landmarks)[3])->open();
                m_nScriptTimerA = 0;
                m_nScriptCounterA = 0;
                m_nState = 6; // shared tail 0x13edde
                break;
            }
            case 56: { // 0x139f14
                if (m_nState == 1) {
                    // DEFERRED 0x13cda0
                    break;
                }
                if (m_nState != 0) {
                    break; // -> post-switch tail (0x144e36)
                }
                // Disable shooting for every friendly wingman before the cutscene.
                if (enemies != nullptr) {
                    for (unsigned int i = 0; i < enemies->count; ++i) {
                        Player *ship = (*enemies)[i]->player;
                        if (ship->isAlwaysFriend()) {
                            ship->setShootingEnabled(false);
                        }
                    }
                }
                m_nState = 1; // shared tail 0x13aba4 -> 0x13edde
                break;
            }
            case 80: { // 0x139e34
                if (m_nState != 0) {
                    // DEFERRED 0x13e630: cold substate (m_nState != 0).
                    break;
                }
                if (!((RadioMessage *) ((*messages)[0]))->isOver()) {
                    // DEFERRED 0x13e62c -> tail.
                    break;
                }
                // Intro cutscene: freeze the ship under AI control, hide the HUD/radar and pan the
                // camera to a fixed offset behind the intro geometry (m_pGeometry1).
                player->setTurretMode(false);
                resetCamera(m_pLevel);
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                player->stopShooting(0);
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->field_0x58 = 0;
                player->setComputerControlled(true);
                player->freeze = 1; // PlayerEgo+0x24 (byte flag): freeze the ship for the cutscene
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget(m_pGeometry1);
                Vector *camPos = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
                *camPos = m_pGeometry1->getPosition();
                *camPos += Vector{-20000.0f, 5000.0f, -35000.0f};
                m_pCamera->setPosition(*camPos);
                m_nScriptTimerA = 0;
                m_nScriptCounterA = 0;
                m_nState = 1;
                // FModSound::play(1121, ...) DEFERRED @0x139f0c
                // -> shared tail 0x14433e -> post-switch tail (0x144e36)
                break;
            }
            case 91: { // 0x13ac42
                if (m_nState != 0) {
                    // DEFERRED 0x13ebe8: cold substate (m_nState != 0).
                    break;
                }
                if (!((RadioMessage *) ((*messages)[3]))->isTriggered()) {
                    // DEFERRED 0x13ebe4 -> tail.
                    break;
                }
                // Docking approved: reset timers, activate the docking target and give it its
                // localized name.
                m_nScriptTimerB = 0;
                m_nScriptCounterB = 0;
                m_nState = 1;
                (*enemies)[0]->setActive(true);
                PlayerFixedObject *dockTarget = (PlayerFixedObject *) m_pLevel->getDockingTarget(0);
                String dockName(*Globals::gameText->getText(3211), false);
                dockTarget->setName(dockName);
                // -> tail (1410e2)
                break;
            }
            case 92: // 0x139ffe
                // DEFERRED 0x139ffe
                break;
            case 94: { // 0x13acac (tbh on m_nState 0..3)
                if (m_nState != 0) {
                    // DEFERRED 0x13e08e / 0x13dff4 / 0x13e15c: states 1/2/3 cold sub-branches.
                    break;
                }
                // state 0 (0x13acc2): cutscene-exit body. Guard -- only run once the mission has
                // progressed (level->field_178 > 6) or the 64-bit script counter (field_0x8:field_0xc)
                // has reached 60001.
                long long counter94 = *reinterpret_cast<long long *>(&field_0x8);
                if (!(m_pLevel->field_178 > 6) && counter94 < 60001) {
                    break; // -> post-switch tail (0x144e36)
                }
                // Place the two intro ships in a fixed formation, wake them, aim them straight and
                // hand them to the AI. vtable+0x48 == KIPlayer::setPosition(float,float,float) [slot 18],
                // vtable+0xc == KIPlayer::awake() [slot 3] (both virtual in KIPlayer.h -> normal calls).
                for (int i = 0; i != 2; ++i) {
                    float x = (float) (50000 + 3000 * i);
                    (*enemies)[i]->setPosition(x, 1000.0f, 140000.0f);
                    (*enemies)[i]->awake();
                    (*enemies)[i]->setVisible(true);
                    (*enemies)[i]->geometry->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
                    ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
                }
                // Standard cutscene-exit teardown (0x13ad72): hand control back, hide HUD/radar, then
                // reframe the camera behind enemies[0] using its own basis vectors.
                field_0xab = player->isInTurretMode();
                player->setTurretMode(false);
                resetCamera(m_pLevel);
                if (player->isInRocketControl()) {
                    player->setRocketControl(nullptr, nullptr);
                    player->killLiberator();
                    m_pCamera->setRumblePercentage(0.0f, 0);
                }
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                player->stopShooting(0);
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->field_0x58 = 0; // disasm stores m_pRadar+0x48 (same field the sibling exits zero)
                m_pCamera->setLookAtCam(true);
                AEGeometry *geo94 = (*enemies)[0]->geometry;
                m_pCamera->setTarget(geo94);
                Vector *camPos94 = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
                Vector *tmp94 = reinterpret_cast<Vector *>(&field_0x40);    // LevelScript scratch vec @0x40
                *camPos94 = geo94->getPosition();
                *tmp94 = geo94->getDirection();
                *camPos94 += *tmp94 * 8000.0f;
                *tmp94 = geo94->getRightVector();
                *camPos94 += *tmp94 * 1200.0f;
                *tmp94 = geo94->getUpVector();
                *camPos94 += *tmp94 * -300.0f;
                m_pCamera->setPosition(*camPos94);
                m_nScriptTimerA = 0; // shared tail 0x13b7e0
                m_nScriptCounterA = 0;
                // -> post-switch tail (0x144e36)
                break;
            }
            case 102: { // 0x13a658
                if (m_nState != 0) {
                    // DEFERRED 0x13e816: cold substate (m_nState != 0).
                    break;
                }
                if (!((RadioMessage *) ((*messages)[0]))->isTriggered()) {
                    // DEFERRED 0x13e812 -> tail.
                    break;
                }
                // Cutscene exit: hand control back, hide/freeze the player ship and reframe the camera
                // on enemies[0] at a fixed offset.
                player->setTurretMode(false);
                resetCamera(m_pLevel);
                if (player->isInRocketControl()) {
                    player->setRocketControl(nullptr, nullptr);
                    player->killLiberator();
                    m_pCamera->setRumblePercentage(0.0f, 0);
                }
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                player->stopShooting(0);
                player->setFreeze(true); // PlayerEgo+0x24 (byte flag) via setter
                player->setVisible(false);
                player->player->setVulnerable(false);
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->field_0x58 = 0;
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget((*enemies)[0]->geometry);
                Vector camPos = (*enemies)[0]->getPosition() + Vector{9000.0f, -7000.0f, 40000.0f};
                m_pCamera->setPosition(camPos);
                // FModSound::stop(...) DEFERRED @0x13a74e
                // FModSound::play(2240, ...) DEFERRED @0x13a760
                m_nScriptTimerA = 0;
                m_nScriptCounterA = 0;
                m_nState = 1; // shared tail 0x143d0c increments m_nState (0 -> 1) -> post-switch tail
                break;
            }
            case 105: { // 0x13aea4
                // Pre-dispatch head (runs for every state): while still early in the cutscene, creep
                // the two intro ships forward, and keep the cinematic-break flag latched.
                if (m_nState <= 2) {
                    float step = (float) (delta << 1);
                    for (int i = 0; i != 2; ++i) {
                        (*enemies)[i]->geometry->moveForward(step);
                    }
                }
                if (m_nState <= 4) {
                    m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                }
                // DEFERRED 0x13aee4: 10-way tbh sub-dispatch on (m_nState-1) for states 1..10
                // (per-state camera/geometry choreography). NOTE (pass 8): the AEMath "operators" here
                // are ordinary named PLT calls (operator+/-/*, VectorNormalize, MatrixGetPosition/Dir,
                // Vector::operator=/+=) usable as normal C++; no fn-pointer blocker remains -- only the
                // per-state FP immediates still need decoding.
                break;
            }
            case 114: { // 0x13afde
                if (m_nState != 0) {
                    break; // -> post-switch tail (0x144e36)
                }
                // Look for an active docked/type-8 escort among enemies[].
                for (unsigned int i = 0; i < enemies->count; ++i) {
                    KIPlayer *e = (*enemies)[i];
                    if (e->shipGroup == 8 && e->player->isActive()) {
                        // DEFERRED 0x1405c8: active escort found -> sub-branch.
                        break;
                    }
                }
                break;
            }
            case 125: { // 0x139f9c
                // Once radio message #1 has fired (still in state 0), build the scripted flight route
                // and hand it to both the level and the player, then arm the script timer.
                if (m_nState != 0) {
                    break; // -> tail (0x13b43e)
                }
                if (((RadioMessage *) ((*messages)[1]))->isTriggered()) {
                    m_nState = m_nState + 1; // 0 -> 1
                    // Route point table from .rodata @0x202778 (three ints).
                    int routePoints[3] = {-70000, 0, -130000};
                    Route *route = new Route(routePoints, 3);
                    m_pLevel->setPlayerRoute(route);
                    player->setRoute(route);
                    m_nScriptTimerA = 0;
                    m_nScriptCounterA = 0;
                    // -> tail (0x13b518)
                }
                break;
            }
            case 154: { // 0x13a76e
                if (m_nState != 0) {
                    // DEFERRED 0x13e8e8: cold substate (m_nState != 0).
                    break;
                }
                if (!((RadioMessage *) ((*messages)[0]))->isTriggered()) {
                    // DEFERRED 0x13e8e4 -> tail.
                    break;
                }
                // Cutscene exit: tear down rocket/turret/free-look control, freeze the ship under AI,
                // hide the HUD/radar, reframe the camera on enemies[2] and point the hostile escort
                // wave (shipGroup 8+1) at the player.
                if (player->isInRocketControl()) {
                    resetCamera(m_pLevel);
                    player->setRocketControl(nullptr, nullptr);
                    player->killLiberator();
                    m_pCamera->setRumblePercentage(0.0f, 0);
                }
                player->setTurretMode(false);
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                player->stopShooting(0);
                player->setComputerControlled(true);
                player->player->setVulnerable(false);
                player->setSpeed(0.0f);
                field_0xaa = player->autoTurretIsEnabled();
                if (field_0xaa != 0) {
                    player->setAutoTurret(false);
                }
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->field_0x58 = 0;
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget((*m_pLevel->getEnemies())[2]->geometry);
                Vector *camPos = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
                *camPos = (*m_pLevel->getEnemies())[2]->geometry->getPosition();
                *camPos += Vector{300.0f, 300.0f, 5800.0f};
                m_pCamera->setPosition(*camPos);
                ((PlayerFighter *) (*enemies)[0])->setAIDisabled(true);
                for (unsigned int i = 0; i < enemies->count; ++i) {
                    if ((*enemies)[i]->shipGroup == 9) {
                        AEGeometry *geo = (*enemies)[i]->geometry;
                        Vector playerPos = m_pLevel->getPlayer()->getPosition();
                        Vector enemyPos = (*enemies)[i]->getPosition();
                        Vector dir = AbyssEngine::AEMath::VectorNormalize(playerPos - enemyPos);
                        geo->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
                        ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
                    }
                }
                (*enemies)[1]->field_0x70 = 1;
                m_nScriptTimerA = 0;
                m_nScriptCounterA = 0;
                m_nState = m_nState + 1; // 0 -> 1
                m_pLevel->lodManager->forceUpdate(delta, false);
                // -> post-switch tail (0x144e36)
                break;
            }
            case 157: // 0x13a308
                // DEFERRED 0x13a308. Head: an index set (12/11/10 or 23/22/21) is chosen from a global
                // status byte, then for each visible marker geometry (m_pGeometry2 @0xc0, m_pGeometry3
                // @0xc4) it is repositioned onto (*enemies)[idx]->field_0x140's position and its
                // PaintCanvas transform is refreshed (PaintCanvas::TransformGetTransform +
                // Transform::Update). Dispatch on m_nState: state 0 spawns a marker AEGeometry into
                // m_pGeometry4 @0xcc (new AEGeometry(canvas) + setPosition + setDirection) then
                // m_nState=1. When radio[?]->isOver(): the standard cutscene-exit teardown runs --
                //   if isInRocketControl(): resetCamera/setRocketControl(0,0)/killLiberator/rumble 0;
                //   player->player->setVulnerable(false); player->setFreeze(true) [PlayerEgo+0x24];
                //   camera reframe on m_pGeometry4 + translate; m_nState=2.
                // NOTE (pass 8): the KIPlayer vtable slots are resolved -- +0x48 == setPosition(fff),
                // +0xc == awake() (both virtual in KIPlayer.h). The PaintCanvas/Transform matrix work
                // is via GOT singletons (Globals::Canvas) with named methods. Remaining undecoded: the
                // global status byte selecting the marker index set and KIPlayer::field_0x140.
                break;
            case 158: // 0x13885c (nested dispatch on m_nState: 1/2/3 -> 0x138878/0x13d724/0x13d74e)
                // DEFERRED 0x13885c
                break;
            default:
                break;
            }
        }
    }

    // --- post-switch "mission tail" (disasm 0x144e36); runs for every mission after the switch ---
    // Gated on field_0xa9 (the escort/target fail-check enable flag).
    if (field_0xa9 != 0) {
        if (m_nState == 0) {
            // While the target has not yet turned hostile: once the player closes to <= 49999
            // units of enemies[0], flip the whole escort group to permanent enemies (radio #14).
            Vector playerPos = player->getPosition();
            Vector enemyPos = (*enemies)[0]->getPosition();
            Vector diff = playerPos - enemyPos;
            int distance = (int) AbyssEngine::AEMath::VectorLength(diff);
            if (distance <= 49999 && player->player->field_5e == 0) {
                for (unsigned int i = 0; i < enemies->count; ++i) {
                    (*enemies)[i]->player->setAlwaysEnemy(true);
                    if (i != 0 && (*enemies)[i - 1]->field_0x3e != 0) {
                        break;
                    }
                }
                m_pLevel->createRadioMessage(14, ((SolarSystem *) Globals::status->getSystem())->getRace());
                m_nState = 1;
            }
        }
        if (m_nState <= 1) {
            // If the mission target (enemies[0]) has been destroyed, fail the mission: instant-kill
            // the remaining flagged escorts, drop the destination from the station route list, and
            // send the "mission failed" radio message (#15) + tear down the player's autopilot/route.
            if ((*enemies)[0]->player->getHitpoints() <= 0) {
                for (unsigned int i = 1; i < enemies->count; ++i) {
                    if ((*enemies)[i]->field_0x3e != 0) {
                        (*enemies)[i]->player->damage(9999999);
                    }
                }
                Array<int> *stationList = reinterpret_cast<Array<int> *>(m_pLevel->field_90);
                for (unsigned int i = 0; i < stationList->count; ++i) {
                    if ((*stationList)[i] == Globals::status->getStation()->getIndex()) {
                        (*stationList)[i] = -1;
                    }
                }
                m_pLevel->createRadioMessage(15, ((SolarSystem *) Globals::status->getSystem())->getRace());
                player->setAutoPilot(nullptr);
                player->setRoute(nullptr);
                m_pLevel->setPlayerRoute(nullptr);
                m_nState = 2;
            }
        }
    }

    // epilogue (0x144f56): the function returns the cinematicBreak flag -- the high byte of
    // m_nFlags @0x11 -- normalised to 0/1.
    return (m_nFlags & 0xFF00) != 0 ? 1 : 0;
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
