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
#include "engine/render/PaintCanvas.h"
#include "engine/math/AEMath.h"
#include "game/ship/Ship.h"
#include "game/ship/PlayerGasCloud.h"
#include "engine/core/AERandom.h"
#include "engine/audio/FModSound.h"

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
        // mission==0 intro cutscene (disasm 0x137e84). A short state machine (m_nState) that
        // stages three "enemy" ships in front of a repositioned player/camera while radio
        // message #2 plays, wobbles them with the UI pulse, then hands off to later substates.
        RadioMessage *introMsg = (RadioMessage *) ((*messages)[2]);
        if (introMsg->isOver() && m_nState == 0) {
            // state 0 setup, once message #2 finishes (disasm 0x137e9c). FP fills decoded from
            // the constant pool @0x1380d0..0x1380f8 (see per-line values below).
            m_nState = 1;
            player->setPosition(18000.0f, -12000.0f, -40000.0f);          // 0x1380d4/d0/d8
            m_pCamera->setPosition(-12000.0f, 2000.0f, -500.0f);          // 0x1380d0/e0/dc
            // Set the camera's live local matrix translation to the same point (0x137ec2).
            AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
            Matrix *local = reinterpret_cast<Matrix *>(
                    canvas->CameraGetLocal(canvas->CameraGetCurrent()));
            AbyssEngine::AEMath::MatrixSetTranslation(*local, -12000.0f, 2000.0f, -500.0f);
            // Park the three intro ships (vtable+0x48 == KIPlayer::setPosition(fff), 0x137eec).
            (*enemies)[0]->setPosition(-10000.0f, 500.0f, 0.0f);          // 0x1380e8/ec
            (*enemies)[1]->setPosition(-10000.0f, -300.0f, -1700.0f);     // 0x1380e8/f0/f4
            (*enemies)[2]->setPosition(-10000.0f, -200.0f, 2000.0f);      // 0x1380e8/f8/e0
            // Face them +X with +Y up, and make them visible (0x137f34).
            for (int i = 0; i < 3; ++i) {
                (*enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                (*enemies)[i]->geometry->setVisible(true);
            }
            m_pLevel->lodManager->forceUpdate(30, false);
            // falls through to the shared active tail (0x137f76 -> 0x137f7c)
        } else if (m_nState <= 2) {
            // Per-frame pulse wobble of the three ships while the intro plays (disasm 0x1383fe).
            float wobble = Globals::layout->getPulseValue(0.0005f) - 0.5f; // 0x138558, -0.5 @0x138412
            for (int i = 0; i < 3; ++i) {
                (*enemies)[i]->geometry->translate(0.0f, wobble, 0.0f);
            }
            if (m_nState == 1) {
                // 0x138440: once message #6 finishes, lock the camera onto ship 0 and reframe.
                RadioMessage *msg6 = (RadioMessage *) ((*messages)[6]);
                if (msg6->isOver()) {
                    m_nState = 2;
                    m_pCamera->setTarget((*enemies)[0]->geometry);
                    m_pCamera->setPosition(-5000.0f, 300.0f, -5000.0f); // 0x13855c/560
                }
            }
            if (m_nState == 2) {
                // State 2 (0x138482): keep radio event 142 (0x8e) playing, drift the camera in
                // (translate scale 0.2/2.2 @0x138568/0x13856c), then once message #7 finishes
                // (0x1384d6) advance to state 3. State 3 (0x1384f8) makes the three intro ships
                // visible/exhaust-lit, wakes them, parks them (enemy+0x128 = 50000, @0x1384ee) and
                // marks their pilots as enemies (player->enemyFlagsLo = 1). Falls through to tail.
                if (!Globals::sound->isPlaying(142)) {
                    Globals::sound->stop(Globals::sound->currentMusicEvent);
                    Globals::sound->play(142, nullptr, nullptr, 0.0f);
                }
                m_pCamera->translate((float) delta * 0.2f, 0.0f, (float) delta * 2.2f);
                if (((RadioMessage *) ((*messages)[7]))->isOver()) {
                    m_nState = 3;
                    for (int i = 0; i < 3; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->setExhaustVisible(true);
                        (*enemies)[i]->geometry->setVisible(true);
                        (*enemies)[i]->awake();                          // vtable+0xc
                        ((PlayerFighter *) (*enemies)[i])->field_0x128 = 50000;
                        (*enemies)[i]->player->enemyFlagsLo = 1;
                    }
                }
            }
        } else {
            // DEFERRED 0x1389c8: mission-0 states >2 (post-approach continuation).
        }
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
    } else if (mission == 0x51 && Globals::status->inAlienOrbit()) {
        // Chain A, mission 0x51 alien-orbit approach (disasm 0x1381ca). Common per-frame part:
        // hold the LOD detail, pan the camera in toward the station, and latch the cinematic break.
        m_pLevel->lodManager->forceUpdate(delta, true);
        m_pCamera->translate((float) (2 * delta), 0.0f, (float) (-4 * delta)); // 0x1381de/0x1381e8
        m_nFlags = (uint16_t) ((m_nFlags & 0xFF) | 0x100); // strb 1 -> m_nFlags high byte (cinematicBreak_) @0x11
        // DEFERRED 0x13821e: the m_nState choreography machine. State 0 stages the station model
        //   m_pGeometry4 (@0xd8) at the origin, drives its PaintCanvas Transform animation
        //   (TransformGetTransform -> SetAnimationState 3 then 1) and plays FModSound event 160
        //   (0xa0), advancing on a 64-bit script counter (this+0x90 pair) >= 10001. State 1 lives
        //   at 0x138f08, the remaining states at 0x139136. Falls through to the shared tail.
    } else {
        // mission>=2 path (disasm 0x1381b2). This is "Chain A": a long chain of per-mission
        // *intro* choreographies (approach cutscenes) that each run once, then fall through to
        // the shared "active" tail below.
        //
        // DEFERRED (pass 2): the remaining Chain A per-mission intro choreographies are not decoded.
        //   - mission 0x4e (78): disasm 0x138338 (64-bit script-counter gate at 0x1edd), then
        //     mission chain continues at 0x138574 ...
        //   - mission 26 alien-orbit (nested, disasm 0x1382dc): radio #1 triggered -> reset the
        //     wormhole landmark [3]; substate 0x13879e.
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
            // Range prong (0x138a8a, tbh cm-131). Most of 131..145 fall straight to the tail;
            // only 131/135/139/142/144/145 have bodies.
            switch (cm) {
            case 131: { // 0x138aac
                if (m_nState == 2) {
                    // DEFERRED 0x13c8fc: state-2 body.
                    break;
                }
                if (m_nState != 1) {
                    break; // -> post-switch tail (0x144e36)
                }
                // Cinematic camera follow (state 1): take the render camera's current local matrix,
                // offset its translation ahead of the player along the player's dir/right/up basis
                // (dir*delta*2.01 + right*delta*0.03 + up*delta*0.01, into tempVec @0x28 with scratch
                // @0x40), fix the TargetFollowCamera and drive it with the retranslated matrix. Then
                // advance the 64-bit script counter (m_nScriptTimerA:m_nScriptCounterA) by delta;
                // once it reaches 3001 hand off to state 2 and reset the counter.
                AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                Matrix local = *reinterpret_cast<Matrix *>(
                        canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                Vector *tempVec = reinterpret_cast<Vector *>(&field_0x28);
                Vector *scratch = reinterpret_cast<Vector *>(&field_0x40);
                *tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                *scratch = player->GetDirVector();
                *tempVec += *scratch * ((float) delta * 2.01f);
                *scratch = player->geometry->getRightVector();
                *tempVec += *scratch * ((float) delta * 0.03f);
                *scratch = player->GetUpVector();
                *tempVec += *scratch * ((float) delta * 0.01f);
                m_pCamera->setFixed(true);
                AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec);
                m_pCamera->setLocal(local);
                long long *counter = reinterpret_cast<long long *>(&m_nScriptTimerA);
                *counter += delta;
                if (*counter >= 3001) {
                    m_nState = 2;
                    *counter = 0;
                }
                break; // -> post-switch tail (0x144e36)
            }
            case 144: { // 0x1395e0
                // Every frame: creep the tracked marker (m_pGeometry4) forward, latch the cinematic
                // break flag and pan the camera. FP immediates from the .rodata pool:
                //   @0x139714 = 0.05f, @0x139718 = 0.28f (translate deltas).
                m_pGeometry4->moveForward((float) (delta * 5));
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pCamera->translate(0.0f, (float) delta * 0.05f, (float) delta * 0.28f);
                if (m_nState == 3) {
                    // DEFERRED 0x13d59e: state-3 body.
                    break;
                }
                if (m_nState == 2) {
                    // DEFERRED 0x13d6c0: state-2 body.
                    break;
                }
                if (m_nState != 1) {
                    break; // -> post-switch tail (0x144e36)
                }
                // state 1: keep the LODs warm and lock the camera onto the marker.
                m_pLevel->lodManager->forceUpdate(delta, false);
                m_pCamera->setTarget(m_pGeometry4);
                m_nState = 2; // 0x144c0c: m_nState = 2 -> post-switch tail
                break;
            }
            case 142: { // 0x139544
                // Dispatch on m_nState: states 0/1 run the head here, state 2 lives in the shared
                // tail 0x142d94, any higher state falls straight through to the post-switch tail.
                // The raw KIPlayer vtable slots this handler dispatches through are resolved:
                //   vtable+0x1c == setSpeed(float) [slot 7], +0x28 == getPosition() [slot 10].
                if (m_nState > 1) {
                    // --- shared tail 0x142d94 (state 2): once the player is carrying any of the
                    // plot cargo items (ids 0xc9..0xcd), nudge the drifting target to a slow crawl
                    // and hand off to state 3 (via tail 0x1438d4 -> m_nState = 3). getShip() is
                    // re-fetched per test (-Oz has no CSE). ---
                    if (m_nState == 2) {
                        if (Globals::status->getShip()->hasCargo(0xc9, 1) ||
                            Globals::status->getShip()->hasCargo(0xca, 1) ||
                            Globals::status->getShip()->hasCargo(0xcb, 1) ||
                            Globals::status->getShip()->hasCargo(0xcc, 1) ||
                            Globals::status->getShip()->hasCargo(0xcd, 1)) {
                            (*enemies)[0]->setSpeed(2.1f); // vtable+0x1c, const @0x142e28
                            m_nState = 3;
                        }
                    }
                    break; // -> post-switch tail (0x144e36)
                }
                // Head (states 0 and 1): once the target has coasted to within 3000 units of its
                // next route waypoint, stop it dead. FP immediate @0x139710 = 3000.0f.
                Vector targetPos = (*enemies)[0]->getPosition();                              // vtable+0x28
                Vector waypointPos = (*enemies)[0]->getRoute()->getWaypoint(0)->getPosition(); // vtable+0x28
                if (AbyssEngine::AEMath::VectorLength(targetPos - waypointPos) < 3000.0f) {
                    (*enemies)[0]->setSpeed(0.0f); // vtable+0x1c
                }
                if (m_nState != 0) {
                    // --- shared tail 0x13fa64 (state 1): if any gas cloud has started sparking,
                    // advance to state 2. getGasClouds()/count are re-fetched per iteration. ---
                    if (m_pLevel->getGasClouds() != nullptr) {
                        for (unsigned int i = 0; i < m_pLevel->getGasClouds()->count; ++i) {
                            if (((PlayerGasCloud *) (*m_pLevel->getGasClouds())[i])->getSparks() != nullptr) {
                                m_nState = 2;
                            }
                        }
                    }
                    break; // -> post-switch tail (0x144e36)
                }
                // state 0 (tail 0x13fa62 route-null path folds to the post-switch tail): once the
                // player is flying the scripted route and has reached its first waypoint (the
                // waypoint's low state byte @0x130 has been cleared to a non-zero marker), advance
                // to state 1 (via tail 0x144c0c -> m_nState = 1). getRoute() returns int (a Route*
                // ABI-wise), matching the established PlayerEgo::getRoute() call sites.
                Route *playerRoute = reinterpret_cast<Route *>(static_cast<intptr_t>(player->getRoute()));
                if (playerRoute != nullptr &&
                    reinterpret_cast<uint8_t &>(playerRoute->getWaypoint(0)->state) != 0) {
                    m_nState = 1;
                }
                break;
            }
            case 135: { // 0x1391c0
                if (m_nState == 0) {
                    // state 0 (0x13c50e): once the player has docked (or the 64-bit script counter
                    // {field_0x8:field_0xc} has reached 60001), advance to state 1 and (re)home every
                    // group-8 escort into a widening formation ahead of the player, re-aim it at the
                    // player and wipe its cargo list.
                    if (!player->isDockedToDockingPoint() &&
                        *reinterpret_cast<long long *>(&field_0x8) < 60001) {
                        // Not-yet-progressed: 0x13c524 branches to the trampoline 0x141198, which
                        // reloads m_nState and falls into the shared state tail at 0x1391ca. For
                        // m_nState==0 that tail immediately returns via `if (m_nState < 1) break`, so
                        // this path is a plain break (the states>=1 tail is handled below).
                        break; // -> post-switch tail (0x144e36)
                    }
                    m_nState = 1;
                    int xOff = 25000;
                    for (unsigned int i = 0; i < enemies->count; ++i) {
                        if ((*enemies)[i]->shipGroup == 8) {
                            Vector playerPos = player->getPosition();
                            (*enemies)[i]->setPosition(playerPos + Vector{(float) xOff, 5000.0f, 25000.0f}); // vtable+0x44
                            (*enemies)[i]->awake();       // vtable+0xc
                            (*enemies)[i]->setVisible(true);
                            (*enemies)[i]->player->setEnemy(player->player);
                            delete (*enemies)[i]->cargo;
                            (*enemies)[i]->cargo = nullptr;
                        }
                        xOff += 1000;
                    }
                    reinterpret_cast<long long &>(m_nScriptTimerA) = 0; // 64-bit script counter @0x90
                    break; // -> post-switch tail (0x144e36)
                }
                if (m_nState < 1) {
                    break; // negative state -> post-switch tail (0x144e36)
                }
                // states >= 1: accumulate the 64-bit script counter {m_nScriptTimerA:m_nScriptCounterA}.
                reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                if (m_nState == 1) {
                    // Advance once the mission's production quota is at least half filled.
                    int statusValue = ((Mission *) Globals::status->getCampaignMission())->getStatusValue();
                    int amount = ((Mission *) Globals::status->getCampaignMission())->getProductionGoodAmount();
                    if (statusValue >= amount / 2) {
                        m_nState++;
                    }
                }
                if (reinterpret_cast<long long &>(m_nScriptTimerA) < 75001) {
                    break; // -> post-switch tail (0x144e36)
                }
                if (m_nState == 2) {
                    m_nState = 3;
                }
                // Once the timer elapses, revive every dead group-8 escort into the same widening
                // formation and re-aim it at the player.
                int xOff = 25000;
                for (unsigned int i = 0; i < enemies->count; ++i) {
                    if ((*enemies)[i]->shipGroup == 8 && (*enemies)[i]->isDead()) {
                        Vector playerPos = player->getPosition();
                        (*enemies)[i]->revive();          // vtable+0x18
                        (*enemies)[i]->setPosition(playerPos + Vector{(float) xOff, 5000.0f, 25000.0f}); // vtable+0x44
                        (*enemies)[i]->player->setEnemy(player->player);
                        delete (*enemies)[i]->cargo;
                        (*enemies)[i]->cargo = nullptr;
                    }
                    xOff += 1000;
                }
                reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                break; // -> post-switch tail (0x144e36)
            }
            case 139: { // 0x13965c
                // Count how many of the leading docking-object pair are NOT at docking type 3. This
                // count (r6) is consumed by the shared conclusion tail 0x13cca8 below (@0x13cd64).
                int dockingCount = 0;
                for (int i = 0; i < 2; ++i) {
                    if (((PlayerFixedObject *) (*enemies)[i])->getDockingType() != 3) {
                        dockingCount++;
                    }
                }
                if (m_nState == 0) {
                    // state 0 (0x139688): once the player has won the hacking mini-game at a valid dock,
                    // rename that dock and clear its docking type, then advance to state 1 (which falls
                    // straight into the shared conclusion tail below).
                    if (player->isDockedToDockingPoint() && player->hackingWon() &&
                        player->getHackingGameDockIndex() >= 0) {
                        ((PlayerFixedObject *) level->getDockingTarget(player->getHackingGameDockIndex()))
                            ->setName(String("kJIWQhkbw+d4d44Y9dqVSwNS7N453I", false));
                        ((PlayerFixedObject *) m_pLevel->getDockingTarget(player->getHackingGameDockIndex()))
                            ->setDockingType(0);
                        m_nState++;
                    }
                }
                // --- shared cross-case conclusion tail 0x13cca8 (entered with m_nState in r0 and the
                // docking count in r6; also reached from cm135/case-94). Its body only runs when the
                // incoming state == 1. ---
                if (m_nState == 1) {
                    if (!(*enemies)[0]->player->isAlwaysEnemy() &&
                        ((RadioMessage *) ((*messages)[1]))->isTriggered()) {
                        // Turn every non-wingman, non-flagged escort hostile (count re-fetched each pass).
                        for (unsigned int i = 0; i < level->getEnemies()->count; ++i) {
                            if (!(*enemies)[i]->isWingMan() && (*enemies)[i]->field_0x3f_b == 0) {
                                (*enemies)[i]->player->turnEnemy();
                            }
                        }
                    }
                    if (player->isDockedToDockingPoint() && player->hackingWon() &&
                        player->getHackingGameDockIndex() >= 0) {
                        PlayerFixedObject *target =
                            (PlayerFixedObject *) level->getDockingTarget(player->getHackingGameDockIndex());
                        if (dockingCount == 1 && target->getDockingType() == 3) {
                            ((PlayerFixedObject *) m_pLevel->getDockingTarget(player->getHackingGameDockIndex()))
                                ->setDockingType(2);
                            player->setDockingState(2);
                            Globals::status->field_178 = 0;
                            m_nState++; // shared tail 0x13953c increments m_nState
                        }
                    }
                }
                break; // -> post-switch tail (0x144e36)
            }
            case 145: { // 0x1392f6
                // Head (all states): drive all 13 escorts forward; once the fight is underway
                // (state >= 2) have them return fire on the player. FP step @0x1392fe = (float)(2*delta).
                float step = (float) (delta << 1);
                for (int i = 0; i < 13; ++i) {
                    (*enemies)[i]->geometry->moveForward(step);
                    if (i != 0 && m_nState >= 2) {
                        if (m_nState != 4 || *reinterpret_cast<long long *>(&m_nScriptTimerA) <= 1000) {
                            (*enemies)[i]->player->shoot(0, delta, false);
                        }
                    }
                }
                if (m_nState == 0) {
                    // state 0 (0x1393d4): wire every escort to fight the wing leader (enemies[14]), spawn
                    // the boss explosion effect, advance to state 1, and fall into the state-1 body.
                    for (int i = 0; i < 13; ++i) {
                        (*enemies)[i]->setEnemies(nullptr);
                        (*enemies)[i]->player->setEnemy((*enemies)[14]->player);
                    }
                    m_pExplosion = new Explosion(0);
                    m_pExplosion->addFireStreaks();
                    m_pExplosion->setScaling(12.0f); // const @0x139708
                    m_nState = 1;
                } else if (m_nState >= 2) {
                    // states 2..5: shared cross-case fight-conclusion tail 0x13cacc (tbh m_nState-2):
                    //   state 2 -> 0x13cae0, state 3 -> 0x13f53c, state 4 -> 0x13f5fc, state 5 -> 0x13f6c6.
                    // All four share the shape: script-timer-A (0x90) += delta (the shared 64-bit counter
                    // pointer is cached at [sp+0x40] = &m_nScriptTimerA); gate (state2 <1001, state3 <5001,
                    // state4 <324, state5 <=10000); one-shot payload; counter=0; m_nState++. Note the
                    // shared tail keeps r8 = &enemies.data (enemies+4), so [*r8 + 0x34/0x38] == boss[13]/
                    // boss[14] == (*enemies)[13] / (*enemies)[14].
                    if (m_nState == 4) {
                        // State 4 (0x13f5fc): keep the boss explosion animating; once 324 ms have elapsed
                        // hand the boss's final pose to its wreck (enemies[13] <- enemies[14]'s matrix),
                        // swap their visibility/active flags, latch the boss wreck flag (field_0x74) and
                        // kick off the wreck's PaintCanvas transform animation. Explosion::update takes
                        // (int)(delta*0.5) and the null camera overload.
                        reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                        m_pExplosion->update((int) ((float) delta * 0.5f), (TargetFollowCamera *) nullptr);
                        if (reinterpret_cast<long long &>(m_nScriptTimerA) < 324) {
                            break; // -> post-switch tail (0x144e36)
                        }
                        KIPlayer *wreck = (*enemies)[13]; // boss[13]
                        KIPlayer *boss = (*enemies)[14];  // boss[14]
                        wreck->geometry->setMatrix(boss->geometry->getMatrix());
                        wreck->setVisible(true);
                        wreck->setActive(true);
                        boss->setVisible(false);
                        boss->setActive(false);
                        boss->field_0x74 = 1;
                        AbyssEngine::Transform *anim = (AbyssEngine::Transform *)
                            Globals::Canvas->TransformGetTransform(wreck->geometry->transform);
                        anim->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                        m_nState++; // 4 -> 5 (shared set-state tail 0x1438d6)
                        break; // -> post-switch tail (0x144e36)
                    }
                    // Behaviour of the remaining (deferred) states:
                    //   s2 has enemies[0] return fire, spawns a tracking AEGeometry(Globals::Canvas) into
                    //     m_pGeometry* and points the camera at it (orientation via a deep enemy-linkage
                    //     chain @+0x18 -- DEFERRED).
                    //   s3 snapshots boss[14]'s matrix into the particle system + Explosion::start; the
                    //     tracking geom's advance speed comes from the same deep chain @+0x50 (DEFERRED).
                    //   s4 copies boss[14]->boss[13], swaps visibility and starts boss[13]'s Transform
                    //     animation (Explosion::update((int)(delta*0.5f)); threshold 324).
                    //   s5 runs that animation (Transform::Update(delta*0.3)), scatters the 13 fighters
                    //     outward (counter/100000, forward counter*delta*0.001), then banishes them to
                    //     setPosition(50000,50000,50000) and restores full player control (threshold 10000).
                    // FP pool: 0.3 (@0x13fa88), 100000 (@0x13fa8c), 0.001 (@0x13fa90), 50000 (@0x13fa94).
                    // DEFERRED 0x13cacc: two deep enemy-linkage chains (s2 orientation @+0x18, s3 speed
                    // @+0x50) are the only genuinely opaque parts; not decoded this pass.
                    break; // -> post-switch tail (0x144e36)
                }
                // state 1 (0x1393fe): once the intro radio (message #0) is over, run the standard
                // cutscene-exit teardown and reframe the camera ahead of enemies[0] along its own basis.
                // FP immediates: dir scale @0x13970c = 18000.0, right scale @0x139710 = 3000.0.
                if (!((RadioMessage *) ((*messages)[0]))->isOver()) {
                    // DEFERRED 0x13caca: radio-not-over -> shared tail.
                    break; // -> post-switch tail (0x144e36)
                }
                player->setTurretMode(false);
                if (player->isInRocketControl()) {
                    player->setRocketControl(nullptr, nullptr);
                    player->killLiberator();
                    m_pCamera->setRumblePercentage(0.0f, 0);
                }
                resetCamera(m_pLevel);
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                player->stopShooting(0);
                player->player->setVulnerable(false);
                player->setVisible(false);
                player->setFreeze(true);
                m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->field_0x58 = 0; // disasm stores m_pRadar+0x48 (same field the siblings exit zero)
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                Vector *camPos = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
                *camPos = (*enemies)[0]->geometry->getPosition();
                *camPos += (*enemies)[0]->geometry->getDirection() * 18000.0f;
                *camPos += (*enemies)[0]->geometry->getRightVector() * 3000.0f;
                m_pCamera->setPosition(*camPos);
                (*enemies)[0]->setVisible(true);
                (*enemies)[0]->setActive(true);
                reinterpret_cast<long long &>(m_nScriptTimerA) = 0; // 64-bit script counter @0x90
                m_nState++; // shared tail 0x13953c increments m_nState
                break;
            }
            default:
                break; // -> post-switch tail (0x144e36)
            }
        } else if (cm >= 64 && cm <= 73) {
            // Range prong (0x138c3e, tbh cm-64). Only 64/67/69/70/73 have bodies; the rest fall to
            // the tail.
            switch (cm) {
            case 64: { // 0x138c56
                if (m_nState == 0) {
                    // DEFERRED 0x13cba4: state-0 body.
                    break;
                }
                if (m_nState != 2) {
                    // DEFERRED 0x13ecf2: state-1 (and other) body.
                    break;
                }
                (*enemies)[0]->field_0x24 = 1; // -> shared tail 0x13ede4
                break;
            }
            case 67: { // 0x139892
                if (((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0) {
                    // Radio #1 (0x13df7c): silence the lead escort (enemies[10]) -- stop it dead, clear
                    // its enemy list and cut its exhaust -- then advance to state 1.
                    m_nState = 1;
                    (*enemies)[10]->setSpeed(0.0f); // KIPlayer vtable+0x1c [slot 7]
                    (*enemies)[10]->setEnemies(nullptr);
                    ((PlayerFighter *) (*enemies)[10])->setExhaustVisible(false);
                    break;
                }
                if (((RadioMessage *) ((*messages)[3]))->isOver() && m_nState == 1) {
                    // Approach cutscene over (radio #3): slow the lead escort (enemies[10]) to a
                    // crawl, relight its exhaust, then run the standard cutscene-exit teardown and
                    // reframe the camera on it. FP immediates from the .rodata pool:
                    //   @0x139b50 = 3.0f (setSpeed), @0x139b58 = 6000.0f, @0x139b54 = -200.0f,
                    //   @0x139b4c = 1000.0f (camera offset x/y/z).
                    (*enemies)[10]->setSpeed(3.0f); // KIPlayer vtable+0x1c [slot 7]
                    ((PlayerFighter *) (*enemies)[10])->setExhaustVisible(true);
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
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*enemies)[10]->geometry);
                    Vector camPos = (*enemies)[10]->getPosition() + Vector{6000.0f, -200.0f, 1000.0f};
                    m_pCamera->setPosition(camPos);
                    player->player->setVulnerable(false);
                    player->freeze = 1; // PlayerEgo+0x24
                    m_nState = 2;
                } else if (m_nState == 2) {
                    // State-2 conclude (0x13c3ec): once the follow-up radio message (#4) is over,
                    // hand control back to the player, restore the HUD/radar, reframe the camera on
                    // the player ship, banish the lead escort (enemies[10]) far off-screen and disable
                    // it, then advance to state 3 and clear the cinematic-break flag. FP immediate from
                    // the .rodata pool: @0x13c550 = 500000.0f (banish coordinate x/y/z).
                    if (!((RadioMessage *) ((*messages)[4]))->isOver()) {
                        break; // -> post-switch tail (0x144e36)
                    }
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    player->player->setVulnerable(true);
                    player->freeze = 0; // PlayerEgo+0x24
                    m_pCamera->setLookAtCam(false);
                    m_pCamera->setTarget(player->geometry);
                    player->setComputerControlled(false);
                    m_pHud->visible = 1;
                    m_pRadar->field_0x58 = 1;
                    resetCamera(m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    (*enemies)[10]->setPosition(500000.0f, 500000.0f, 500000.0f); // vtable+0x48 [slot 18]
                    (*enemies)[10]->setActive(false);
                    (*enemies)[10]->setVisible(false);
                    m_nState = 3;
                    m_nFlags = m_nFlags & 0xFF; // clear cinematicBreak_ (m_nFlags high byte @0x11)
                } else {
                    // DEFERRED 0x13da68: state-3 (radio #5) sub-branch.
                }
                break;
            }
            case 69: { // 0x139720
                // Cutscene-exit teardown (state 0): once the intro chatter (radio #0) has fired,
                // hand control back to the AI, hide the HUD/radar cinematic overlay and reframe the
                // camera behind enemies[0] along its own facing. FP immediates from the .rodata
                // pool: @0x139b40 = 10000.0f (dir scale), @0x139b44/48/4c = {600, 300, 1000}.
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
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
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                    Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                    Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                    camPos += dir * 10000.0f;
                    camPos += Vector{600.0f, 300.0f, 1000.0f};
                    m_pCamera->setPosition(camPos);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 1; // shared tail 0x139aba (m_nState = 1) -> post-switch tail
                } else if (m_nState == 1) {
                    // State-1 conclude (0x13bf9c): keep the LODs warm; once radio #1 is over hand
                    // control back to the computer, restore the HUD/radar, reframe the camera on the
                    // player, then put enemies[0] to sleep and park it far off-screen. FP immediate
                    // from the .rodata pool: @0x13c188 = -5000000.0f (parked-position z).
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    if (((RadioMessage *) ((*messages)[1]))->isOver()) {
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        m_pHud->visible = 1;
                        m_pRadar->field_0x58 = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nFlags = m_nFlags & 0xFF; // cinematicBreak_ (m_nFlags high byte @0x11) = 0
                        m_nState = 2;
                        (*m_pLevel->getEnemies())[0]->setActive(false);
                        (*m_pLevel->getEnemies())[0]->setToSleep();
                        (*m_pLevel->getEnemies())[0]->setPosition(0.0f, 0.0f, -5000000.0f); // vtable+0x48
                        (*m_pLevel->getEnemies())[0]->setVisible(false);
                    }
                }
                break;
            }
            case 70: { // 0x1399a2
                // Cutscene-exit teardown variant (state 0): like case 69 but reframes closer, points
                // the camera to the opposite side and freezes the player ship. FP immediates from
                // the .rodata pool: @0x139b60 = 3000.0f (dir scale), @0x139b64/68/6c = {-600, -300, -1000}.
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
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
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                    Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                    Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                    camPos += dir * 3000.0f;
                    camPos += Vector{-600.0f, -300.0f, -1000.0f};
                    m_pCamera->setPosition(camPos);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    player->freeze = 1; // PlayerEgo+0x24
                    m_nState = 1; // shared tail 0x139aba (m_nState = 1) -> post-switch tail
                } else if (m_nState == 2) {
                    // State-2 conclude (0x13ec3c): keep the LODs warm; once radio message #2 is over
                    // hand control back to the player, restore the HUD/radar, reframe the camera on the
                    // player and clear the cinematic-break flag. Then buff the lead enemy (enemies[0]):
                    // triple its max hitpoints, set a cruise speed and a fixed rotate mode.
                    // FP immediate from the .rodata pool: @0x13edbc = 10.0f (cruise speed).
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    if (!((RadioMessage *) ((*messages)[2]))->isOver()) {
                        break; // -> post-switch tail (0x144e36)
                    }
                    m_pCamera->setLookAtCam(false);
                    m_pCamera->setTarget(player->geometry);
                    player->setComputerControlled(false);
                    m_pHud->visible = 1;
                    m_pRadar->field_0x58 = 1;
                    resetCamera(m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nFlags = m_nFlags & 0xFF; // clear cinematicBreak_ (m_nFlags high byte @0x11)
                    m_nState = 3;
                    Player *lead = (*m_pLevel->getEnemies())[0]->player;
                    lead->setMaxHitpoints((*m_pLevel->getEnemies())[0]->player->getMaxHitpoints() * 3);
                    (*m_pLevel->getEnemies())[0]->setSpeed(10.0f); // vtable+0x1c [slot 7]
                    ((PlayerFighter *) (*m_pLevel->getEnemies())[0])->setRotate(5);
                } else if (m_nState == 1) {
                    // State-1 conclude (0x13c04e): keep the LODs warm; once radio #1 is over commit
                    // enemies[0] (a PlayerFighter) as a permanent enemy -- seed its state fields, drop
                    // its route -- then reframe the camera ahead of it along its own facing and unfreeze
                    // the player. FP immediates from the .rodata pool: @0x13c194 = 2000.0f (dir scale),
                    // @0x13c198/9c/a0 = {-600, 800, -1000} (camera offset); field seed @0x13c190 = 100000.
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    if (((RadioMessage *) ((*messages)[1]))->isOver()) {
                        (*m_pLevel->getEnemies())[0]->player->setAlwaysFriend(false);
                        (*m_pLevel->getEnemies())[0]->player->setAlwaysEnemy(true);
                        ((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x128 = 100000;
                        (*m_pLevel->getEnemies())[0]->field_0x38 = 0;
                        ((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x12d = 0;
                        ((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x12c = 0;
                        (*m_pLevel->getEnemies())[0]->setRoute(nullptr);
                        Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();          // vtable+0x28
                        Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                        camPos += dir * 2000.0f;
                        camPos += Vector{-600.0f, 800.0f, -1000.0f};
                        m_pCamera->setPosition(camPos);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nState = 2;
                        player->freeze = 0; // PlayerEgo+0x24
                    }
                }
                break;
            }
            case 73: { // 0x139834 (tbh on m_nState 0..3)
                if (m_nState > 3) {
                    break; // -> post-switch tail (0x144e36)
                }
                if (m_nState == 0) {
                    // Proximity gate: once the player closes to within 49999 units of enemies[8],
                    // advance to state 1 (via tail 0x1438d4 -> m_nState = 1). FP const @ 49999.
                    Vector playerPos = player->getPosition();
                    Vector enemyPos = (*enemies)[8]->getPosition(); // vtable+0x28
                    if ((int) AbyssEngine::AEMath::VectorLength(playerPos - enemyPos) <= 49999) {
                        m_nState = 1;
                    }
                    break;
                }
                if (m_nState == 1) { // 0x13db3c
                    // Wait for any late escort (enemies[8+]) whose EMP shield has been knocked below
                    // its max, then advance to state 2 (via shared tail 0x1438d2 -> m_nState = 2).
                    for (unsigned int i = 8; i < enemies->count; ++i) {
                        if ((*enemies)[i]->field_0x40 != 0 &&
                            (*enemies)[i]->player->getEmpPoints() < (*enemies)[i]->player->getMaxEmpPoints()) {
                            m_nState = 2;
                            break;
                        }
                    }
                    break;
                }
                if (m_nState == 2) { // 0x13db76
                    // Once any EMP-disabled escort (enemies[8+]) is found, disarm and stop the first
                    // live escort (shared tail 0x142f0c) and advance to state 3 (0x144c0c -> m_nState = 3).
                    for (unsigned int i = 8; i < enemies->count; ++i) {
                        if ((*enemies)[i]->field_0x40 != 0 && (*enemies)[i]->player->empDisabledByte != 0) {
                            for (unsigned int j = 8; j < enemies->count; ++j) {
                                if ((*enemies)[j]->field_0x40 != 0) {
                                    (*enemies)[j]->field_0x24 = 1;
                                    ((PlayerFixedObject *) (*enemies)[j])->setMoving(false);
                                    break;
                                }
                            }
                            m_nState = 3;
                            break;
                        }
                    }
                    break;
                }
                // state 3 (0x13dbd8): once radio message #7 fires, scatter the late escorts
                // (enemies[4..7]) to random positions far around the player -- magnitude
                // 35000..44999 on ±X/±Z, -5000..4999 on Y (Globals::rnd->nextInt) -- wake each one,
                // then advance to state 4 (shared tail 0x13e80c -> 0x1438d4). No FP immediates: the
                // components are integer arithmetic converted with vcvt.f32.s32.
                if (!((RadioMessage *) ((*messages)[7]))->isTriggered()) {
                    break; // -> post-switch tail (0x144e36)
                }
                Vector scatterOrigin = player->getPosition();
                for (int i = 4; i != 8; ++i) {
                    int magX = 35000 + Globals::rnd->nextInt(10000);
                    int signX = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
                    int y = Globals::rnd->nextInt(10000) - 5000;
                    int magZ = 35000 + Globals::rnd->nextInt(10000);
                    int signZ = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
                    Vector offset{(float) (signX * magX), (float) y, (float) (signZ * magZ)};
                    (*enemies)[i]->setPosition(scatterOrigin + offset); // vtable+0x44
                    (*enemies)[i]->awake();                             // vtable+0xc
                }
                m_nState = 4;
                break;
            }
            default:
                break; // -> post-switch tail (0x144e36)
            }
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
            case 92: { // 0x139ffe
                // Head (0x139ffe): when m_nState==0 and radio message #2 has fired, advance 0->1.
                // Then a 7-way tbh on (m_nState-1) dispatches states 1..7.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[2]))->isTriggered()) {
                    m_nState = 1;
                }
                if (m_nState == 1) {
                    // State 1 (0x13a036): gated on Globals::status->field_178 == 0. Set the campaign
                    // Mission type to 4, park the three intro fighters (enemies[0..2]) in a fixed
                    // formation relative to the reference ship enemies[3], run the standard cutscene-
                    // exit teardown, reframe the camera on enemies[0]'s basis, clear the docking
                    // target's type and give it an empty name, then advance (shared tail 0x143d0a:
                    // m_nState++). FP immediates from the .rodata pool @0x13a2d4..: formation offsets
                    //   e0 {-76000, 5000, 5000}, e1 {-79000, 4900, 7500}, e2 {-79000, 4900, 2500}
                    //   (@0x13a2d4/d8/dc/e0/e4/e8); camera scales @0x13a2f4/0x13a2fc/0x13a300 =
                    //   7000 / 700 / 300. Docking name literal @0x1ca472 is the empty string.
                    if (Globals::status->field_178 != 0) {
                        break; // -> post-switch tail (0x144e36)
                    }
                    ((Mission *) Globals::status->getCampaignMission())->setType(4);
                    reinterpret_cast<long long &>(m_nScriptTimerA) = 0;

                    Vector ref = (*enemies)[3]->geometry->getPosition();
                    (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f}); // vtable+0x44
                    (*enemies)[1]->setPosition(ref + Vector{-79000.0f, 4900.0f, 7500.0f});
                    (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
                    for (int i = 0; i < 3; ++i) {
                        (*enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                        ((PlayerFighter *) (*enemies)[i])->field_0x128 = 0;
                        (*enemies)[i]->field_0x38 = 3;
                        (*enemies)[i]->awake();       // vtable+0xc
                        (*enemies)[i]->setVisible(true);
                    }

                    field_0xab = player->isInTurretMode();
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    if (player->isInRocketControl()) {
                        player->setRocketControl(nullptr, nullptr);
                        player->killLiberator();
                        m_pCamera->setRumblePercentage(0.0f, 0);
                    }
                    player->setFreeLookMode(false);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_nFlags = (m_nFlags & 0xFF) | 0x100; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pHud->visible = 0;
                    m_pRadar->field_0x58 = 0;

                    AEGeometry *geo = (*enemies)[0]->geometry;
                    m_pCamera->setTarget(geo);
                    Vector *acc = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
                    Vector *tmp = reinterpret_cast<Vector *>(&field_0x40); // LevelScript scratch vec @0x40
                    *acc = geo->getPosition();
                    *tmp = geo->getDirection();
                    *acc += *tmp * 7000.0f;
                    *tmp = geo->getRightVector();
                    *acc += *tmp * 700.0f;
                    *tmp = geo->getUpVector();
                    *acc += *tmp * 300.0f;
                    m_pCamera->setPosition(*acc);

                    ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(0);
                    ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setName(String("", false));
                    m_nScriptTimerA = 0;
                    m_nScriptCounterA = 0;
                    m_nState++; // shared tail 0x143d0a increments m_nState
                } else if (m_nState == 2) {
                    // State 2 (0x13fb7a): camera fly-by. Creep the three intro ships forward at
                    // per-ship speeds (2.0/2.08/2.05 * delta; pool @0x13fab8 = {2.05, 2.08}), then once
                    // radio message #5 fires reset both 64-bit script counters, advance to state 3 and
                    // snap the follow-camera behind enemies[0] along its own basis. The offsets are
                    // driven by the just-cleared script-timer-B counter (0 this frame): dir (ctrB+3900),
                    // right (ctrB+500), up (400 + ctrB*0.004). Scale constants @0x13fef4/0x13fef0 =
                    // 400.0 / 0.004.
                    for (int i = 0; i < 3; ++i) {
                        float sp = (i == 0) ? 2.0f : (i == 1 ? 2.08f : 2.05f);
                        (*enemies)[i]->geometry->moveForward(sp * (float) delta);
                    }
                    if (!((RadioMessage *) ((*messages)[5]))->isTriggered()) {
                        break; // -> post-switch tail (0x144e36)
                    }
                    reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    reinterpret_cast<long long &>(m_nScriptTimerB) = 0;
                    m_nState++; // 2 -> 3
                    AEGeometry *geo = (*enemies)[0]->geometry;
                    m_pCamera->setTarget(geo);
                    Vector *acc = reinterpret_cast<Vector *>(&field_0x28); // LevelScript tempVec @0x28
                    Vector *tmp = reinterpret_cast<Vector *>(&field_0x40); // LevelScript scratch vec @0x40
                    long long ctrB = reinterpret_cast<long long &>(m_nScriptTimerB);
                    *acc = geo->getPosition();
                    *tmp = geo->getDirection();
                    *acc += *tmp * (float) (ctrB + 3900);
                    *tmp = geo->getRightVector();
                    *acc += *tmp * (float) (ctrB + 500);
                    *tmp = geo->getUpVector();
                    *acc += *tmp * (400.0f + (float) ctrB * 0.004f);
                    m_pCamera->setPosition(*acc);
                } else if (m_nState == 6) {
                    // State 6 (0x140214): pure timer -- accumulate the 64-bit script-timer-A counter
                    // and once it passes 3000 advance to state 7 (0x141e72 -> shared set-state tail
                    // 0x1438d4 stores m_nState = 7).
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) >= 3001) {
                        m_nState = 7;
                    }
                } else if (m_nState == 3) {
                    // State 3 (0x13fcde): advance script-timer-B by delta*0.05, reframe the follow
                    // camera by driving its PaintCanvas local matrix to chase enemies[0]'s basis,
                    // creep the three fighters (2.0/2.3/2.2 * delta), and once radio message #5 is
                    // over enable cloaking on them and advance. FP pool @0x13fef0..0x13ff14:
                    // 0.004/400/0.05/1.7/0.03; move speeds @0x13ff14/0x13ff18 = 2.2/2.3.
                    reinterpret_cast<long long &>(m_nScriptTimerB) =
                            (long long) ((float) delta * 0.05f +
                                         (float) reinterpret_cast<long long &>(m_nScriptTimerB));
                    {
                        // Reframe: snapshot the camera's PaintCanvas local matrix, rebuild its
                        // translation from enemies[0]'s basis, and push it back (setLocal).
                        AEGeometry *geo = (*enemies)[0]->geometry;
                        m_pCamera->setTarget(geo);
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *acc = reinterpret_cast<Vector *>(&field_0x28);
                        Vector *tmp = reinterpret_cast<Vector *>(&field_0x40);
                        *acc = AbyssEngine::AEMath::MatrixGetPosition(local);
                        *tmp = geo->getDirection();
                        *acc += *tmp * ((float) delta * 1.7f);
                        *tmp = geo->getRightVector();
                        *acc += *tmp * ((float) (-delta) * 0.03f);
                        *tmp = geo->getUpVector();
                        *acc += *tmp * ((float) delta * 0.05f * 0.004f);
                        m_pCamera->setFixed(true);
                        m_pCamera->setLocal(AbyssEngine::AEMath::MatrixSetTranslation(local, *acc));
                    }
                    for (int i = 0; i < 3; ++i) {
                        float sp = (i == 0) ? 2.0f : (i == 1 ? 2.3f : 2.2f);
                        (*enemies)[i]->geometry->moveForward(sp * (float) delta);
                    }
                    if (!((RadioMessage *) ((*messages)[5]))->isOver()) {
                        break; // -> post-switch tail (0x144e36)
                    }
                    for (int i = 0; i < 3; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->setCloakingPossible(true);
                        ((PlayerFighter *) (*enemies)[i])->cloak(18000, false);
                    }
                    reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    m_nState++;
                } else if (m_nState == 4) {
                    // State 4 (0x13ff1c): like state 3 but also advances script-timer-A by delta;
                    // identical camera reframe + fighter creep. Advances to state 5 once timer-A
                    // passes 2000.
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    reinterpret_cast<long long &>(m_nScriptTimerB) =
                            (long long) ((float) delta * 0.05f +
                                         (float) reinterpret_cast<long long &>(m_nScriptTimerB));
                    {
                        // Reframe: snapshot the camera's PaintCanvas local matrix, rebuild its
                        // translation from enemies[0]'s basis, and push it back (setLocal).
                        AEGeometry *geo = (*enemies)[0]->geometry;
                        m_pCamera->setTarget(geo);
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *acc = reinterpret_cast<Vector *>(&field_0x28);
                        Vector *tmp = reinterpret_cast<Vector *>(&field_0x40);
                        *acc = AbyssEngine::AEMath::MatrixGetPosition(local);
                        *tmp = geo->getDirection();
                        *acc += *tmp * ((float) delta * 1.7f);
                        *tmp = geo->getRightVector();
                        *acc += *tmp * ((float) (-delta) * 0.03f);
                        *tmp = geo->getUpVector();
                        *acc += *tmp * ((float) delta * 0.05f * 0.004f);
                        m_pCamera->setFixed(true);
                        m_pCamera->setLocal(AbyssEngine::AEMath::MatrixSetTranslation(local, *acc));
                    }
                    for (int i = 0; i < 3; ++i) {
                        float sp = (i == 0) ? 2.0f : (i == 1 ? 2.3f : 2.2f);
                        (*enemies)[i]->geometry->moveForward(sp * (float) delta);
                    }
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) > 2000) {
                        m_nState++;
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    }
                } else if (m_nState == 5) {
                    // State 5 (0x14011a): advance timer-A by delta, creep the fighters faster
                    // (6.0/4.6/4.4 * delta, pool @0x140248), then once timer-A passes 2000 disable
                    // the fighters' AI, park them (field_0x128=50000, field_0x38=4), unfix the
                    // camera onto the player and hand control back through the shared look-behind
                    // tail (0x144fd4/0x144fe2).
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    for (int i = 0; i < 3; ++i) {
                        float sp = (i == 0) ? 6.0f : (i == 1 ? 4.6f : 4.4f);
                        (*enemies)[i]->geometry->moveForward(sp * (float) delta);
                    }
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) < 2001) {
                        break; // -> post-switch tail (0x144e36)
                    }
                    for (int i = 0; i < 3; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                        ((PlayerFighter *) (*enemies)[i])->field_0x128 = 50000;
                        (*enemies)[i]->field_0x38 = 4;
                    }
                    m_pCamera->setFixed(false);
                    m_pCamera->setTarget(player->geometry);
                    bool wasDocked = player->isDockedToDockingPoint();
                    m_pCamera->setLookAtCam(false);
                    if (wasDocked) {
                        lookBehind();
                        m_pCamera->setRotationAroundTarget(true);
                        player->setFreeLookMode(true);
                        if (field_0xab != 0) {
                            player->setTurretMode(true);
                        }
                    } else {
                        // not-docked path (0x144fd4): snap the camera to the staged position.
                        m_pCamera->setPosition(*reinterpret_cast<Vector *>(&field_0x40));
                    }
                    // shared look-behind tail (0x144fe2): re-show HUD/radar, force a LOD update,
                    // clear the cinematic-break flag, reset timer-A and advance the state.
                    m_pHud->visible = 1;
                    m_pRadar->field_0x58 = 1;   // radar+0x48
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nFlags &= 0xFF;           // clear cinematicBreak_ (m_nFlags high byte @0x11)
                    reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    m_nState++;
                } else if (m_nState == 7) {
                    // State 7 (0x140250): finish the docking cutscene once radio message #8 fires.
                    // Dock the player, run the full cutscene-exit teardown, reframe the follow
                    // camera behind the player (dir*7000 + right*700, pool @0x1405fc/0x140600),
                    // then advance. If the radio hasn't fired the original falls to sub-tail
                    // 0x1435ec (still DEFERRED).
                    if (!((RadioMessage *) ((*messages)[8]))->isTriggered()) {
                        // DEFERRED 0x1435ec: radio-#8-not-triggered continuation.
                        break;
                    }
                    if (player->isDockedToDockingPoint()) {
                        m_pLevel->getPlayer()->setAutoPilot(nullptr);   // Level::getPlayer via GOT
                        m_pRadar->lockedEnemy = nullptr;                // radar+4
                        m_pRadar->dockNavPtr = nullptr;                 // radar+8
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pLevel->getPlayer()->dockToDockingPoint(nullptr, m_pRadar);
                        player->setDockingState(3);
                        m_pCamera->setRotationAroundTarget(false);
                    }
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
                    m_pRadar->field_0x58 = 0;             // radar+0x48
                    player->player->setVulnerable(false);
                    player->setComputerControlled(true);
                    player->setCollide(false);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(player->geometry);
                    Vector *acc = reinterpret_cast<Vector *>(&field_0x28);
                    Vector *tmp = reinterpret_cast<Vector *>(&field_0x40);
                    *acc = player->geometry->getPosition();
                    *tmp = player->geometry->getDirection();
                    *acc += *tmp * 7000.0f;
                    *tmp = player->geometry->getRightVector();
                    *acc += *tmp * 700.0f;
                    m_pCamera->setPosition(*acc);
                    reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    m_nState++;
                }
                break;
            }
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
                // 10-way tbh sub-dispatch on (m_nState-1) for states 1..10 (0x13aee4; table @0x13aef0):
                //   s1 0x13af3c, s2 0x141e78, s3 0x142062, s4 0x142138, s5 0x142272, s6 0x1422cc,
                //   s7 0x1426c4, s8 0x1429dc, s9 0x142a04, s10 0x142bf2. States 0 and 14+ fall straight
                //   to the shared 0x144e18 tail; states 11/12/13 divert to their own bodies (0x143eee/
                //   0x144d24/0x144c38, still DEFERRED) via the range check at 0x143edc.
                if (m_nState == 11) {
                    // DEFERRED 0x143eee: state-11 body (64-bit counter += delta gate at 2501, then
                    // spawns/animates a marker AEGeometry into m_pGeometry6 @0xdc).
                    break;
                }
                if (m_nState == 12) {
                    // DEFERRED 0x144d24: state-12 body.
                    break;
                }
                if (m_nState == 13) {
                    // DEFERRED 0x144c38: state-13 body.
                    break;
                }
                if (m_nState == 1) {
                    // State 1 (0x13af3c): drift the camera along the player's basis; after 13s -> s2.
                    // FP: @0x13b1d4 = 6.08f (dir weight), @0x13b1d8 = 0.2f (right weight),
                    // @0x13b1dc = 0.4f (drift scale).
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) >= 13001) {
                        m_nState = 2;
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    }
                } else if (m_nState == 2) {
                    // State 2 (0x141e78): keep drifting the camera (0.9x the s1 rate) and, once radio
                    // message #1 finishes, split the two escorts onto mirrored fly-away routes derived
                    // from the player's own basis (route point magnitude 900000, see 0x141f24). FP pool
                    // @0x142018..0x142024 = 6.08/0.2/0.4/0.9; route consts @0x142028..0x142030 =
                    // 900000/-900000/-90000.
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta * 0.9f;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    if (((RadioMessage *) ((*messages)[1]))->isOver()) {
                        Vector r = player->geometry->getRightVector();
                        Vector d = player->geometry->getDirection();
                        int route0[3] = {
                                (int) (r.x * 900000.0f + d.x * -900000.0f),
                                (int) (r.y * 900000.0f + d.y * -900000.0f),
                                (int) (r.z * 900000.0f - d.z * 900000.0f),
                        };
                        int route1[3] = {
                                (int) (d.x * -900000.0f - r.x * 900000.0f),
                                (int) (d.y * -900000.0f - r.y * 900000.0f),
                                (int) (-(r.z * 900000.0f) - d.z * 90000.0f),
                        };
                        Route *escort0Route = new Route(route0, 3);
                        Route *escort1Route = new Route(route1, 3);
                        (*enemies)[0]->setRoute(escort0Route);
                        (*enemies)[1]->setRoute(escort1Route);
                        for (int i = 0; i < 2; ++i) {
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                            ((PlayerFighter *) (*enemies)[i])->field_0x12c = 1;
                        }
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                        m_nState = m_nState + 1; // 2 -> 3
                    }
                } else if (m_nState == 3) {
                    // State 3 (0x142062): counter += delta, slow camera drift (0.4x), keep both escorts
                    // AI-tagged, and after 3s advance to state 4 (via the shared 0x142266 tail).
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta * 0.4f;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    for (int i = 0; i < 2; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->field_0x12c = 1;
                    }
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) > 3000) {
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                        m_nState = m_nState + 1; // 3 -> 4
                    }
                } else if (m_nState == 4) {
                    // State 4 (0x142138): counter += delta, very slow drift (0.1x); after 3s hand control
                    // back to the player and tear the cutscene framing down (restore HUD/radar, clear the
                    // cinematic-break flag, unhide the escort exhausts), then advance to state 5.
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta * 0.1f;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    for (int i = 0; i < 2; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->field_0x12c = 1;
                    }
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) > 3000) {
                        m_pLevel->getPlayer()->resetGunDelay();
                        player->setComputerControlled(false);
                        m_pCamera->setLookAtCam(false);
                        m_pHud->visible = 1;
                        m_pRadar->field_0x58 = 1; // radar restore flag @0x48
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nFlags = m_nFlags & 0xFF; // clear cinematicBreak_ (m_nFlags high byte @0x11)
                        for (int i = 0; i < 2; ++i) {
                            (*enemies)[i]->field_0x74 = 1;
                        }
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                        m_nState = m_nState + 1; // 4 -> 5
                    }
                } else if (m_nState == 5) {
                    // State 5 (0x142272): after 10s, deactivate both escorts (setActive(false) +
                    // setSpeed(0)) and advance to state 6.
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) >= 10001) {
                        for (int i = 0; i < 2; ++i) {
                            (*enemies)[i]->setActive(false);
                            (*enemies)[i]->setSpeed(0.0f); // KIPlayer vtable+0x1c
                        }
                        m_nState = m_nState + 1; // 5 -> 6
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    }
                } else if (m_nState == 6) {
                    // DEFERRED 0x1422cc: state-6 body. After counter>=24001 it cloaks the 5-ship
                    // formation (enemies[0..4]) along the sun's light direction (StarSystem::
                    // getLightDirection, VectorNormalize, offset 100000 @0x142a90), reframes the
                    // PaintCanvas camera-local matrix and drives a long fixed-camera cutscene.
                } else if (m_nState == 7) {
                    // DEFERRED 0x1426c4: state-7 body. Drives the PaintCanvas camera-local matrix
                    // (CameraGetLocal/MatrixGetPosition/MatrixSetTranslation/setLocal) chasing the
                    // formation; 64-bit counter derives a normalized time (l2f/f2lz, 2.5 scale @0x142704);
                    // counter>=10001 restores player control. FP @0x142790 = 6.08.
                } else if (m_nState == 8) {
                    // State 8 (0x1429dc): pure timer -- after 60s advance to state 9.
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) >= 60001) {
                        m_nState = 9;
                    }
                } else if (m_nState == 9) {
                    // State 9 (0x142a04): every ~110s scatter the dead escorts (enemies[2..4]) as debris
                    // along the sun direction, then once the player's route waypoint goes live hand back
                    // control. RNG scatter @0x142a94: base +/- Globals::rnd->nextInt(1000) offsets, per
                    // ship stagger 10000/15000/20000 (step 5000), sun offset 100000 (@0x142a90), right
                    // nudge -30000 (@0x142e10).
                    reinterpret_cast<long long &>(m_nScriptTimerA) += delta;
                    if (reinterpret_cast<long long &>(m_nScriptTimerA) >= 110001) {
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                        Vector lightDir = AbyssEngine::AEMath::VectorNormalize(
                                m_pLevel->getStarSystem()->getLightDirection());
                        Vector playerPos = player->geometry->getPosition();
                        int off = 10000;
                        for (int i = 2; i < 5; ++i) {
                            if ((*enemies)[i]->isDead()) {
                                (*enemies)[i]->revive(); // KIPlayer vtable+0x18
                                int rx = Globals::rnd->nextInt(1000);
                                int ry = Globals::rnd->nextInt(1000);
                                int rz = Globals::rnd->nextInt(1000);
                                (*enemies)[i]->setPosition(
                                        playerPos.x + lightDir.x * 100000.0f + (float) off + (float) rx,
                                        (float) ry,
                                        playerPos.z + lightDir.z * 100000.0f + (float) rz);
                                AEGeometry *geo = (*enemies)[i]->geometry;
                                geo->setDirection(-lightDir, Vector{0.0f, 1.0f, 0.0f});
                                Vector right = geo->getRightVector();
                                geo->translate(right * -30000.0f);
                            }
                            off += 5000;
                        }
                    }
                    if (player->getHitpoints() >= 1 &&
                        reinterpret_cast<Route *>(player->getRoute()) != nullptr &&
                        reinterpret_cast<Route *>(player->getRoute())->getWaypoint(0)->state != 0) {
                        player->player->setVulnerable(false);
                        player->stopBoost();
                        m_nState = m_nState + 1; // 9 -> 10
                        reinterpret_cast<long long &>(m_nScriptTimerA) = 0;
                    }
                } else if (m_nState == 10) {
                    // DEFERRED 0x142bf2: state-10 body. On radio message #7 over, runs the full
                    // cutscene-exit teardown (setTurretMode/resetCamera/rocket-control/free-look/HUD)
                    // and reframes on enemies[2]; not-over path diverts to 0x143ed8.
                }
                // 0x144e18: shared tail for case-105 states 0..10 and 14+ (states 11/12/13 return
                // above). Advance the drifting station marker geometry (m_pGeometry6 @0xdc).
                if (m_pGeometry6 != nullptr) {
                    m_pGeometry6->moveForward((float) (delta * 13));
                }
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
            case 158: { // 0x13885c (nested dispatch on m_nState: 1/2/3 -> 0x138878/0x13d724/0x13d74e)
                if (m_nState == 3) {
                    // DEFERRED 0x13d74e: state-3 body (proximity check + AERandom gate).
                    break;
                }
                if (m_nState == 2) {
                    // DEFERRED 0x13d724: state-2 body (script-counter gate then 0x1403c2).
                    break;
                }
                if (m_nState != 1) {
                    break; // -> post-switch tail (0x144e36)
                }
                // State 1 @0x138878: a per-frame fly-in animation. Advance the 64-bit script counter
                // (m_nScriptTimerA:m_nScriptCounterA) by delta, derive a normalized time from it, pan
                // the camera, re-aim the marker (m_pGeometry4 @0xcc) toward a fixed world point and, past
                // successive counter thresholds, moveForward it, wake enemies[0], and hand off. FP
                // immediates decoded from the .rodata pool:
                //   @0x1389ac = -34000.0f, @0x1389b0 = 4.8f, @0x1389b4 = 0.7f (camera pan);
                //   @0x1389b8 = 9000.0f, @0x1389bc = -13000.0f (aim target x/z); @0x143964 = 2000.0f.
                long long *counter = reinterpret_cast<long long *>(&m_nScriptTimerA);
                *counter += delta;
                float t = (float) *counter / -34000.0f + 1.0f;
                m_pCamera->translate((float) delta * 4.8f * t, 0.0f, (float) (-delta) * 0.7f * t);

                Vector aimTarget{9000.0f, 0.0f, -13000.0f};
                Vector aimDir = aimTarget - m_pGeometry4->getPosition();
                m_pGeometry4->setDirection(AbyssEngine::AEMath::VectorNormalize(aimDir),
                                           Vector{0.0f, 1.0f, 0.0f});

                if (*counter <= 29999) {
                    m_pGeometry4->moveForward((float) delta * 2.5f);
                }
                if (*counter > 25000) {
                    // Reveal enemies[0] once the marker has closed most of the distance.
                    if (!(*enemies)[0]->isVisible()) {
                        (*enemies)[0]->setVisible(true);
                        (*enemies)[0]->setActive(true);
                    }
                }
                // Continuation (0x14384e): drag enemies[0] onto the marker, follow it and slowly spin
                // the player, then two counter gates finish the fly-in.
                (*enemies)[0]->geometry->setPosition(m_pGeometry4->getPosition());
                m_pCamera->setTarget((*enemies)[0]->geometry);
                player->geometry->rotate((float) delta / 2000.0f, (float) delta / 2000.0f,
                                         (float) delta / 2000.0f);
                if (*counter >= 20000) {
                    ((PlayerFighter *) (*enemies)[0])->setExhaustVisible(false);
                    if (*counter >= 34000) {
                        m_nScriptTimerA = 0;
                        m_nScriptCounterA = 0;
                        m_nScriptTimerB = 0;
                        m_nScriptCounterB = 0;
                        ((PlayerFighter *) (*enemies)[0])->setExhaustVisible(true);
                        m_nState = 2;
                    }
                }
                break;
            }
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
