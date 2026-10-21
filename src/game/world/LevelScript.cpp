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
#include "game/ship/PlayerStation.h"
#include "game/mission/Mission.h"
#include "game/mission/Objective.h"
#include "game/weapons/Radar.h"
#include "game/weapons/Gun.h"
#include "engine/core/GameText.h"
#include "engine/render/PaintCanvas.h"
#include "engine/math/AEMath.h"
#include "game/ship/Ship.h"
#include "game/ship/PlayerGasCloud.h"
#include "engine/core/AERandom.h"
#include "engine/audio/FModSound.h"
#include "engine/math/Transform.h"
#include "engine/core/ApplicationManager.h"
#include "game/world/Standing.h"
#include "game/world/Galaxy.h"


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

typedef unsigned char * (*LevelListProc)(Level *);

typedef void (*ReadWaypointProc)(StackVector *out, Waypoint *waypoint);

typedef void (*SetVectorProc)(unsigned char *self, StackVector *value);

typedef void (*VirtualCommandProc)(unsigned char *self, int a, int b, int c);

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

    if (m_pExplosion && m_pExplosion->isPlaying()) {
        m_pExplosion->render();
    }

    if (m_bRenderParticles && m_pParticleGeom0) {
        m_pParticleGeom0->render();
        m_pParticleGeom1->render();
        m_pParticleGeom2->render();
    }
}

bool LevelScript::startSequenceOver() {
    return m_bStartSequenceOver;
}

void LevelScript::resetCamera(Level *level) {
    if (level->getPlayer() != 0) {
        TargetFollowCamera *camera = m_pCamera;
        PlayerEgo *player = (PlayerEgo *) level->getPlayer();
        camera->setTarget(player->geometry);
        m_pCamera->setTargetOffset(Vector{0.0f, 600.0f, -650.0f});
        m_pCamera->setCamOffset(Vector{0.0f, 600.0f, -1338.0f});
    }
}

void LevelScript::skipSequence() {
    if (field_0x24 > 0 && Globals::status->getCurrentCampaignMission() > 0) {
        field_0x24 = 0x1b59;
        m_pLevel->getPlayer()->player->setVulnerable(true);
    }
}

bool LevelScript::startSequence() {
    return m_bStartSequence;
}

void LevelScript::setAutoPilotToProgrammedStation() {
    if (Level::programmedStation == nullptr) {
        return;
    }
    if (Globals::status->getStation()->equals(Level::programmedStation)) {
        Level::programmedStation = nullptr;
        return;
    }

    if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->stationIsInSystem(Level::programmedStation)) {
        m_pLevel->getPlayer()->setAutoPilot(
                (*(Array<KIPlayer *> *) m_pLevel->getStarSystem()->getPlanetTargets())[
                        ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStationEnumIndex(
                                Level::programmedStation->getIndex())]);
    } else if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->currentOrbitHasWarpGate()) {
        m_pLevel->getPlayer()->setAutoPilot((*m_pLevel->getLandmarks())[1]);
    } else {
        int warpGate = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getWarpGateEnumIndex();
        if (warpGate < 0) {
            return;
        }
        m_pLevel->getPlayer()->setAutoPilot(
                (*(Array<KIPlayer *> *) m_pLevel->getStarSystem()->getPlanetTargets())[warpGate]);
    }
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

bool LevelScript::canSkipCutsceneNow() {
    if (Globals::status->getCurrentCampaignMission() == 0x9a) {
        if (!(m_nState >= 1 && m_nState <= 9)) {
            return 0;
        }
    } else if (Globals::status->getCurrentCampaignMission() == 0x9d) {
        if (!(m_nState >= 2 && m_nState <= 4)) {
            return 0;
        }
    } else if (Globals::status->getCurrentCampaignMission() == 0x9e && m_nState > 1) {
        return 0;
    }
    return 1;
}

void LevelScript::resetStartSequenceOver() {
    m_bStartSequenceOver = 0;
    field_0x24 = 0;
}

void LevelScript::skipCutscene() {
    if (Globals::status->getCurrentCampaignMission() == 0x9a) {
        if (m_nState >= 1 && m_nState <= 9) {
            m_nState = 9;
            for (int i = 0; i != 8; ++i) {
                (*m_pLevel->getMessages())[i]->trigger();
                (*m_pLevel->getMessages())[i]->finish();
            }
            m_nScriptTimerA = 2001;
            ((PlayerFighter *) (*m_pLevel->getEnemies())[0])->setAIDisabled(false);
            KIPlayer *fighter = (*m_pLevel->getEnemies())[0];
            Vector position = (*m_pLevel->getEnemies())[0]->getRoute()->getWaypoint(0)->getPosition();
            fighter->setPosition(position);
        }
    } else if (Globals::status->getCurrentCampaignMission() == 0x9d) {
        if (m_nState <= 4) {
            for (int i = 0; i != 4; ++i) {
                (*m_pLevel->getMessages())[i]->trigger();
                (*m_pLevel->getMessages())[i]->finish();
            }
            m_nScriptTimerA = 18001;
            m_nState = 4;
        }
    } else if (Globals::status->getCurrentCampaignMission() == 0x9e) {
        m_nScriptTimerB = 12001;
        for (int i = 0; i != 3; ++i) {
            (*m_pLevel->getMessages())[i]->trigger();
            (*m_pLevel->getMessages())[i]->finish();
        }
        (*m_pLevel->getEnemies())[0]->setPosition(-2077.0f, 0.0f, 20233.0f);
        (*m_pLevel->getEnemies())[0]->setVisible(true);
        (*m_pLevel->getEnemies())[0]->setActive(true);
        m_nState = 2;
    }
}

bool LevelScript::process(int delta) {
    // Reconstruction pass 1 of _ZN11LevelScript7processEi (original 0xdf90 bytes @ 0x137d18).
    // Implemented faithfully here: prologue accessors + mission dispatch (0x137d18),
    // the mission==1 intro state machine (0x137d82-0x137e82) and the shared "active" tail
    // (0x137f7c-0x13812a). The mission==0 intro choreography and the giant mission>=2 general
    // state machine / per-mission switch are DEFERRED (see markers below) for a later pass.
    Level *level = m_pLevel;
    Array<RadioMessage *> *messages = level->getMessages();
    PlayerEgo *player = level->getPlayer();
    Array<KIPlayer *> *enemies = level->getEnemies();
    if (Globals::status->getCurrentCampaignMission() == 0) {
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
        } else if (m_nState == 3) {
            // State 3 (0x1389d4): drift the camera in toward the intro ships, then once radio
            // message #8 finishes hand control back to the player and reframe onto the player ship.
            // FP immediates @0x1389c0 = 0.2f, @0x1389c4 = 2.2f.
            m_pCamera->translate((float) delta * 0.2f, 0.0f, (float) delta * 2.2f);
            if (((RadioMessage *) ((*messages)[8]))->isOver()) {
                m_nScriptTimerA = 0; // strd 0 -> this+0x90 (64-bit)
                m_nState = 4;
                player->setComputerControlled(false);
                player->player->field_5e = 0;
                m_pRadar->enabled = 1; // disasm stores m_pRadar+0x48 (same field the siblings exit)
                m_pLevel->getPlayer()->setCollide(true);
                m_bCinematicBreak = 0; // clear cinematicBreak_ (m_nFlags high byte @0x11)
                m_pCamera->setLookAtCam(false);
                m_pCamera->setTarget(player->geometry);
                m_bStartSequence = 0;               // strb 0 -> this+0x20
                reinterpret_cast<uint8_t &>(field_0x12) = 1; // strb 1 -> this+0x12 (low byte)
            }
            // falls through to the shared active tail (0x137f7c)
        } else if (m_nState == 4) {
            // State 4 (0x138d2a): once radio message #10 finishes, run the standard cutscene-exit
            // teardown -- stop event 143's radio sting and replay it, drop the player out of turret
            // and free-look, reset the camera, re-enable collision, then reframe the camera onto the
            // player ship, rotate it (yaw = pi/2), and translate it clear. FP immediates from the
            // .rodata pool @0x138e30..0x138e3c: yaw = 1.5707964f, translate = {25000, -200, -1000}.
            if (((RadioMessage *) ((*messages)[10]))->isOver()) {
                Globals::sound->stop(Globals::sound->currentMusicEvent);
                Globals::sound->play(143, nullptr, nullptr, 0.0f);
                player->setTurretMode(false);
                resetCamera(m_pLevel);
                player->setFreeLookMode(false);
                m_pCamera->enableFirstPersonCam(false);
                player->hideShipForFirstPersonCameraView(false);
                m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pLevel->getPlayer()->setCollide(false);
                m_pRadar->enabled = 0; // disasm stores m_pRadar+0x48 (same field the siblings enter)
                player->setComputerControlled(true);
                player->player->removeAllGuns();
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget(player->geometry);
                player->geometry->setRotation(0.0f, 1.5707964f, 0.0f);
                Vector playerPos = player->geometry->getPosition();
                m_pCamera->setPosition(playerPos);
                m_pCamera->translate(25000.0f, -200.0f, -1000.0f);
                m_nState = 5;
            }
            // falls through to the shared active tail (0x13d4ec -> 0x137f7c)
        } else if (m_nState == 5 && ((RadioMessage *) ((*messages)[12]))->isOver()) {
            // State 5: brake the player towards the station, starting the docking sounds once the
            // ship has slowed to cruise speed.
            Vector pos = player->getPosition();
            int speedBits = player->getSpeed();
            if (reinterpret_cast<float &>(speedBits) == 2.0f) {
                Globals::sound->play(157, nullptr, nullptr, 0.0f);
                Vector zero157{0.0f, 0.0f, 0.0f};
                Globals::sound->updateEvent3DAttributes(157, &pos, &zero157, false);
                Globals::sound->play(158, nullptr, nullptr, 0.0f);
                Vector zero158{0.0f, 0.0f, 0.0f};
                Globals::sound->updateEvent3DAttributes(158, &pos, &zero158, false);
                Globals::sound->stop(player->field_0x1c);
                Globals::sound->play(161, nullptr, nullptr, 0.0f);
            }
            Vector zero161{0.0f, 0.0f, 0.0f};
            Globals::sound->updateEvent3DAttributes(161, &pos, &zero161, false);
            speedBits = player->getSpeed();
            player->setSpeed(reinterpret_cast<float &>(speedBits) * 0.98f);
            if (((RadioMessage *) ((*messages)[13]))->isOver()) {
                m_pCamera->translate((float) ((double) delta * 0.3), 0.0f, (float) -delta * 0.2f);
            }
            if (((RadioMessage *) ((*messages)[14]))->isTriggered()) {
                m_nScriptTimerA = 0;
                m_nState = 6;
            }
        } else if (m_nState == 6) {
            // State 6: the station's docking bay opens around the player.
            Vector zero{0.0f, 0.0f, 0.0f};
            Vector pos = player->getPosition();
            Globals::sound->updateEvent3DAttributes(161, &pos, &zero, false);
            m_pCamera->translate((float) ((double) delta * 0.3), 0.0f, (float) -delta * 0.2f);
            m_nScriptTimerA += delta;
            float rumble = (float) m_nScriptTimerA / 4000.0f;
            if (!(rumble < 1.0f)) {
                rumble = 1.0f;
            }
            m_pCamera->setRumblePercentage(rumble, 20);
            if (((RadioMessage *) ((*messages)[15]))->isOver()) {
                m_nState = 7;
                m_pGeometry5->setPosition(player->getPosition());
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                Globals::sound->play(160, nullptr, nullptr, 0.0f);
                Globals::sound->stop(161);
                m_nScriptTimerA = 0;
            }
        } else if (m_nState == 7) {
            m_nScriptTimerA += delta;
            m_pCamera->translate((float) ((double) delta * 0.3), 0.0f, (float) -delta * 0.2f);
            AEGeometry *dockModel = m_pGeometry5;
            AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
            dockModel->setDirection(AbyssEngine::AEMath::MatrixGetDir(
                                            *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()))),
                                    Vector{0.0f, 1.0f, 0.0f});
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                    ->Update((long long) delta, false);
            if (m_nScriptTimerA >= 2001) {
                Globals::sound->stop(158);
                player->setVisible(false);
                m_pCamera->setRumblePercentage(0.0f, 0);
            }
            if (!((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                         ->animating) {
                Globals::sound->stop(Globals::sound->currentMusicEvent);
                Globals::sound->play(141, nullptr, nullptr, 0.0f);
                m_nState = 8;
                m_nScriptTimerA = 0;
            }
        } else if (m_nState == 8) {
            m_nScriptTimerA += delta;
            float rumble = (float) m_nScriptTimerA / -3000.0f + 1.0f;
            if (rumble < 0.0f) {
                rumble = 0.0f;
            }
            m_pCamera->setRumblePercentage(rumble, 20);
            if (m_nScriptTimerA >= 4001) {
                m_nState = 9;
                m_nScriptTimerA = 0;
                m_pCamera->setRumblePercentage(0.0f, 0);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
            }
        } else if (m_nState == 9) {
            // State 9: jump to the intro planet.
            m_nState = 10;
            m_pLevel->getStarSystem()->switchPlanetForIntro();
            m_pLevel->switchSkyboxForIntro();
            player->setPosition(0.0f, 0.0f, 0.0f);
            m_pLevel->particleRenderBoolPtr->reset();
            m_pLevel->particleEmitBoolPtr->reset();
            player->geometry->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
            player->rotate(0.7853982f, 0.7853982f, 0.7853982f);
            m_pGeometry5->setPosition(player->getPosition());
            m_pCamera->setPosition(5000.0f, 500.0f, -10000.0f);
            m_nScriptTimerA = 0;
        } else if (m_nState == 10) {
            m_nScriptTimerA += delta;
            float rumble = (float) m_nScriptTimerA / 2000.0f;
            if (!(rumble < 1.0f)) {
                rumble = 1.0f;
            }
            m_pCamera->setRumblePercentage(rumble, 20);
            if (m_nScriptTimerA >= 2001) {
                Globals::sound->play(159, nullptr, nullptr, 0.0f);
                m_nState = 11;
                m_pGeometry5->setPosition(player->getPosition());
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                m_nScriptTimerA = 0;
            }
        } else if (m_nState == 11) {
            m_nScriptTimerA += delta;
            float rumble = (float) m_nScriptTimerA / -2500.0f + 1.0f;
            if (rumble < 0.0f) {
                rumble = 0.0f;
            }
            m_pCamera->setRumblePercentage(rumble, 20);
            AEGeometry *dockModel = m_pGeometry5;
            AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
            dockModel->setDirection(AbyssEngine::AEMath::MatrixGetDir(
                                            *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()))),
                                    Vector{0.0f, 1.0f, 0.0f});
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                    ->Update((long long) delta, false);
            if (m_nScriptTimerA >= 2501) {
                m_nState = 12;
                player->StopEngineSound();
                player->field_0x1c = 156;
                player->PlayEngineSound();
                player->startSmokeEmission();
                player->setVisible(true);
                player->setExhaustVisible(false);
                player->setSpeed(2.0f);
            }
        } else if (m_nState == 12) {
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                    ->Update((long long) delta, false);
            float spin = (float) delta / 3000.0f;
            player->rotate(spin, spin, spin);
            if (((RadioMessage *) ((*messages)[17]))->isOver()) {
                m_tempVec = player->getPosition();
                m_tempVec += Vector{-2000.0f, -2000.0f, -5000.0f};
                m_pCamera->setPosition(m_tempVec);
                m_nScriptTimerA = 0;
                m_nState = 13;
            }
        } else if (m_nState == 13) {
            m_nScriptTimerA += delta;
            float spin = (float) delta / 4000.0f;
            player->rotate(spin, spin, spin);
            if (m_nScriptTimerA >= 6001) {
                m_tempVec = player->getPosition();
                m_tempVec += Vector{700.0f, 0.0f, -1700.0f};
                m_pCamera->setPosition(m_tempVec);
                m_nState = 14;
                m_nScriptTimerA = 0;
            }
        } else if (m_nState == 14) {
            m_nScriptTimerA += delta;
            float spin = (float) delta / 5000.0f;
            player->rotate(spin, spin, spin);
            m_pCamera->translate(0.0f, 0.0f, (float) (-2 * delta));
            if (m_nScriptTimerA >= 12001) {
                m_tempVec = player->getPosition();
                m_tempVec += Vector{-3000.0f, 3500.0f, -6700.0f};
                m_pCamera->setPosition(m_tempVec);
                m_nState = 15;
                m_nScriptTimerA = 0;
            }
        } else if (m_nState == 15 && ((RadioMessage *) ((*messages)[22]))->isOver()) {
            Globals::layout->startFade(true, 255, 5000);
            m_nState = 16;
        } else if (m_nState == 16 && !Globals::layout->isFading()) {
            Globals::layout->enableFillScreen(true);
            Globals::status->nextCampaignMission(true);
            Globals::switch_to_target_setting = 1;
            Globals::appManager->SetCurrentApplicationModule(2);
            Level::initStreamOutPosition = 0;
            return 0;
        }
    } else if (Globals::status->getCurrentCampaignMission() == 1) {
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
            Globals::layout->enableFillScreen(true);
            Globals::switch_to_target_setting = 0;
            Globals::appManager->SetCurrentApplicationModule(5);
            return 0;
        }
        m_pCamera->translate((float) ((double) delta * 0.1), 0.0f, 0.0f);
    } else if (Globals::status->getCurrentCampaignMission() == 0x51 && Globals::status->inAlienOrbit()) {
        // Chain A, mission 81 alien-orbit arrival (0x1381ca).
        m_pLevel->lodManager->forceUpdate(delta, true);
        m_pCamera->translate((float) (2 * delta), 0.0f, (float) (-4 * delta));
        m_bCinematicBreak = 1;
        switch (m_nState) {
            case 0: {
                // Build the arrival rumble up, then seat the station model and play its sting.
                m_nScriptTimerA += delta;
                float rumble = (float) m_nScriptTimerA / 10000.0f;
                if (!(rumble < 1.0f)) {
                    rumble = 1.0f;
                }
                m_pCamera->setRumblePercentage(rumble, 100);
                if (m_nScriptTime >= 10001) {
                    m_pGeometry5->setPosition(Vector{0.0f, 0.0f, 10000.0f});
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                            ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                    Globals::sound->play(160, nullptr, nullptr, 0.0f);
                    m_nState = 1;
                    m_nScriptTimerA = 0;
                    return 1;
                }
                break;
            }
            case 1: {
                // Fade the arrival rumble out, keep the station model facing the camera.
                m_nScriptTimerA += delta;
                float rumble = (float) m_nScriptTimerA / 4000.0f;
                if (!(rumble < 1.0f)) {
                    rumble = 1.0f;
                }
                m_pCamera->setRumblePercentage(1.0f - rumble, 100);
                AEGeometry *dockModel = m_pGeometry5;
                AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                dockModel->setDirection(AbyssEngine::AEMath::MatrixGetDir(
                                                *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()))),
                                        Vector{0.0f, 1.0f, 0.0f});
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                        ->Update((long long) delta, false);
                if (m_nScriptTime >= 14001) {
                    m_nState = 2;
                    m_pCamera->setRumblePercentage(0.0f, 0);
                } else if (m_nScriptTime >= 12001) {
                    ((PlayerStation *) (*enemies)[0])->rootGeometry->setVisible(true);
                }
                break;
            }
        }
        if (m_nState == 2) {
            if (((RadioMessage *) ((*messages)[3]))->isOver()) {
                Globals::layout->startFade(true, 255, 5000);
                m_nState = 3;
                return 1;
            }
        }
        if (m_nState == 3) {
            if (!Globals::layout->isFading()) {
                Globals::layout->enableFillScreen(true);
                Globals::status->nextCampaignMission(true);
                Globals::switch_to_target_setting = 0;
                Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(100));
                Globals::status->getStation()->setAttackedFriends(false);
                Level::comingFromAlienWorld = 0;
                Globals::appManager->SetCurrentApplicationModule(5);
                Level::initStreamOutPosition = 0;
                return 0;
            }
        }
        return 1;
    } else if (Globals::status->getCurrentCampaignMission() == 0x4e) {
        // Chain A, mission 0x4e intro/approach cutscene (disasm 0x138338). Once the 64-bit script
        // counter field_0x8:field_0xc reaches 7901, keep advancing the approached station's
        // cinematic transform; then a 12-way tbh on (m_nState-1) (disasm 0x138378) runs the
        // approach choreography substates s1..s12. Substate epilogues re-share the mission tail at
        // 0x13c88c in the original; here they fall through to the shared active tail below (pass-2
        // structural approximation, kept consistent).
        if (m_nScriptTime >= 7901) {
            // 0x13834e: advance landmarks[0]'s station-root transform animation this frame.
            // landmarks[0]'s [+0x140] field is PlayerStation::rootGeometry (offset 0x140).
            PlayerStation *station = (PlayerStation *) (*m_pLevel->getLandmarks())[0];
            AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                    Globals::Canvas->TransformGetTransform(station->rootGeometry->transform);
            t->Update((long long) delta, false); // 0x138374
        }
        // 0x138378: dispatch on (m_nState - 1) -> substates s1..s12 (states 0 / >12 do nothing).
        switch (m_nState) {
            case 1: { // s1 @0x1383a0: lock camera onto the station and start its approach animation.
                m_pCamera->setTarget(((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry);
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                        Globals::Canvas->TransformGetTransform(
                                ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry->transform);
                t->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                m_nState = 2;
                break;
            }
            case 2: { // s2 @0x13ba5c: pan the camera in; once counter>=7001 reframe it.
                m_pCamera->translate(-(float) delta * 0.5f, (float) delta * 0.3f, (float) delta); // 0.3 @0x13b93c
                if (m_nScriptTime >= 7001) {
                    m_nState = 3;
                    m_pCamera->setPosition(20000.0f, 5000.0f, 30000.0f); // pool @0x13bda0/a4/a8
                }
                break;
            }
            case 3: { // s3 @0x13b8dc: start the approach music (1122), advance at counter>=9001.
                if (m_nScriptTime >= 7901) {
                    if (!Globals::sound->isPlaying(1122)) {
                        Globals::sound->play(1122, nullptr, nullptr, 0.0f);
                    }
                    if (m_nScriptTime >= 9001) {
                        m_nState = 4;
                    }
                }
                break;
            }
            case 4: { // s4 @0x13baba: at counter>=18001 reframe the camera and hide the player ship.
                if (m_nScriptTime >= 18001) {
                    m_nState = 5;
                    m_pCamera->setPosition(8000.0f, 31000.0f, -9000.0f); // pool @0x13bdac/b0/b4
                    player->freeze = 1;       // player+0x24
                    player->setVisible(false);
                }
                break;
            }
            case 5: { // s5 @0x13b8c0: at counter>=18001 reset the frame timer and advance.
                if (m_nScriptTime >= 18001) {
                    m_nScriptTimerA = 0;
                    m_nState = 6;
                }
                break;
            }
            case 7: { // s7 @0x13bc18: at counter>=27001 advance.
                if (m_nScriptTime >= 27001) {
                    m_nState = 8;
                }
                break;
            }
            case 9: { // s9 @0x13bc34: pan the camera, ramp the rumble on the frame timer, then at
                      // timer>=6001 seat the docking model, play its arrival sting and advance.
                m_pCamera->translate(-(float) delta * 0.7f, (float) delta, (float) (delta * 3)); // 0.7 @0x13b944
                m_nScriptTimerA += delta;
                float rumble = (float) m_nScriptTimerA / 10000.0f; // @0x13bdbc
                if (!(rumble < 1.0f)) {
                    rumble = 1.0f;
                }
                m_pCamera->setRumblePercentage(rumble, 100);
                if (m_nScriptTimerA >= 6001) {
                    m_pGeometry5->setPosition(Vector{0.0f, 0.0f, 14000.0f}); // z @0x13bdc0
                    AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                            Globals::Canvas->TransformGetTransform(m_pGeometry5->transform);
                    t->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                    t = (AbyssEngine::Transform *)
                            Globals::Canvas->TransformGetTransform(m_pGeometry5->transform);
                    t->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                    Globals::sound->play(160, nullptr, nullptr, 0.0f);
                    m_nState = 10;
                    m_nScriptTimerA = 0;
                }
                break;
            }
            case 6: { // s6 @0x13bb7c: drift escort ship [1] outward; at timer>=1100-delta advance to s7.
                m_nScriptTimerA =
                        (long long) ((float) delta * 0.3f +
                                     (float) m_nScriptTimerA); // 0.3 @0x13b93c
                Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
                (*enemies)[1]->geometry->translate(Vector{(float) delta * 0.3f, 0.0f, 0.0f});
                (*enemies)[1]->geometry->rotate(Vector{-(float) delta * 9.0e-4f, 0.0f, 0.0f}); // @0x13bdb8
                if (m_nScriptTimerA >= 1100 - delta) {
                    m_nState = 7;
                    m_nScriptTimerA = 0;
                }
                break;
            }
            case 8: { // s8 @0x13baf2: drift escort ship [0]; stop at timer>=1100-delta, at >=2001 advance.
                m_nScriptTimerA =
                        (long long) ((float) delta * 0.3f +
                                     (float) m_nScriptTimerA); // 0.3 @0x13b93c
                if (m_nScriptTimerA < 1100 - delta) {
                    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
                    (*enemies)[0]->geometry->translate(Vector{-(float) delta * 0.3f, 0.0f, 0.0f});
                    (*enemies)[0]->geometry->rotate(Vector{-(float) delta * 9.0e-4f, 0.0f, 0.0f}); // @0x13bdb8
                } else if (m_nScriptTimerA >= 2001) {
                    m_nState = 9;
                    m_nScriptTimerA = 0;
                    m_pCamera->setPosition(-12000.0f, 5000.0f, 15000.0f); // pool @0x13c8f0/c8c4/c8f4
                }
                break;
            }
            case 10: { // s10 @0x13b948: pan the camera further, ramp the rumble DOWN over the frame
                       // timer, re-aim the docking model along the current camera direction, then at
                       // timer>=4001 advance to s11 and clear the rumble.
                m_pCamera->translate(-(float) delta * 0.7f, (float) delta, (float) (delta * 4)); // 0.7 @0x13b944
                m_nScriptTimerA += delta;
                float rumble = (float) m_nScriptTimerA / 4000.0f; // @0x13b930
                if (!(rumble < 1.0f)) {
                    rumble = 1.0f;
                }
                m_pCamera->setRumblePercentage(1.0f - rumble, 100);
                AEGeometry *dockModel = m_pGeometry5;
                AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                Vector camDir = AbyssEngine::AEMath::MatrixGetDir(
                        *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent())));
                dockModel->setDirection(camDir, Vector{0.0f, 1.0f, 0.0f});
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                        Globals::Canvas->TransformGetTransform(dockModel->transform);
                t->Update((long long) delta, false);
                if (m_nScriptTimerA >= 4001) {
                    m_nState = 11;
                    m_nScriptTimerA = 0;
                    m_pCamera->setRumblePercentage(0.0f, 0);
                }
                break;
            }
            case 12: { // s12 @0x13b7f2: advance the frame timer, refresh LOD, keep panning the camera;
                       // once timer>=5001 hand control back to the player (setTarget(player->geometry),
                       // stop the look-at cam, re-enable the player) and advance to state 13.
                m_nScriptTimerA += delta;
                m_pLevel->lodManager->forceUpdate(30, false);
                m_pCamera->translate(-(float) delta * 0.3f, (float) delta * 0.3f, (float) delta * 0.5f); // 0.3 @0x13b93c
                if (m_nScriptTimerA >= 5001) {
                    m_pCamera->setTarget(player->geometry);
                    m_pHud->visible = 1;
                    // DEFERRED 0x13b876: m_pRadar byte @0x48 = 1 -- lands inside imageWidth in our
                    //   Radar model (no clean named 1-byte field there yet).
                    m_bCinematicBreak = 0; // field_0x11 = 0 (m_nFlags high byte)
                    m_pCamera->setLookAtCam(false);
                    player->setComputerControlled(false);
                    player->player->setVulnerable(true);
                    player->freeze = 0;
                    player->setVisible(true);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 13;
                }
                break;
            }
            default: { // s11 @0x13bd2c: the wingmen swarm past, then the camera hands off to the tail ship.
                // Drift the camera, then once 3s elapse push every trailing wingman forward and reframe
                // the camera onto the ship ten slots from the end (the mission's designated tail ship).
                m_pCamera->translate(-(float) delta * 0.7f, (float) delta, (float) delta * 3.0f); // 0.7 @0x13b944
                m_nScriptTimerA += delta;
                if (m_nScriptTimerA < 3001) {
                    break;
                }
                // enemies[2..] are the escorting wingmen: nudge each along -Z and wake it.
                for (unsigned int i = 2; i < enemies->count; ++i) {
                    (*enemies)[i]->translate(Vector{0.0f, 0.0f, -50000.0f}); // -50000 @0x13bdcc
                    (*enemies)[i]->awake();                                  // vtable+0xc
                }
                KIPlayer *tailShip = (*enemies)[enemies->count - 10];
                m_pCamera->setTarget(tailShip->parentGeometry);
                Vector *camPos = &m_tempVec; // LevelScript tempVec @0x28
                *camPos = tailShip->parentGeometry->getPosition();
                *camPos += Vector{2000.0f, 500.0f, -20000.0f}; // @0x13bdd0/d4/d8
                m_pCamera->setPosition(*camPos);
                m_pLevel->lodManager->forceUpdate(30, false);
                m_nState = 12;
                m_nScriptTimerA = 0;
                break;
            }
        }
    } else if (Globals::status->getCurrentCampaignMission() == 89) {
        // Chain A, mission 89 supernova intro (0x138574).
        m_pLevel->lodManager->forceUpdate(delta, true);
        m_pCamera->setTarget(m_pGeometry4);
        m_bCinematicBreak = 1;
        if (m_nState <= 2) {
            float approach = (float) m_nScriptTime / -35000.0f + 1.0f;
            float step = (float) delta;
            m_pCamera->translate(approach * step, 0.0f, 2.0f * step * approach);
            AEGeometry *ship = m_pGeometry4;
            ship->translate(-ship->getRightVector() * step * 7.0f * ((float) m_nScriptTime / -50000.0f + 1.0f));
        }
        ((PlayerFixedObject *) (*enemies)[11])->moveForward(delta);
        switch (m_nState) {
            case 1:
                if (m_nScriptTime >= 30001) {
                    m_nState = 2;
                    m_tempVec = *m_pCamera->getPosition();
                    m_tempVec.y += 500.0f;
                    (*m_pLevel->getEnemies())[0]->setPosition(m_tempVec);
                    (*m_pLevel->getEnemies())[0]->geometry->moveForward(-10000.0f);
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry6->transform))
                            ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry6->transform))
                            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                    m_pGeometry6->setDirection(-(*m_pLevel->getEnemies())[0]->geometry->getDirection(),
                                               Vector{0.0f, 1.0f, 0.0f});
                    m_pGeometry6->setPosition((*m_pLevel->getEnemies())[0]->geometry->getPosition());
                    return 1;
                }
                break;
            case 2: {
                float distance = AbyssEngine::AEMath::VectorLength(
                        *m_pCamera->getPosition() - (*m_pLevel->getEnemies())[0]->geometry->getPosition());
                float rumble = distance / 7000.0f;
                if (!(rumble < 1.0f)) {
                    rumble = 1.0f;
                }
                m_pCamera->setRumblePercentage((1.0f - rumble) * 100.0f, 30);
                if (distance < 200000.0f) {
                    (*m_pLevel->getEnemies())[0]->geometry->moveForward((float) (delta * 10) + distance * 0.1f);
                }
                m_pGeometry6->setPosition((*m_pLevel->getEnemies())[0]->geometry->getPosition());
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry6->transform))
                        ->Update((long long) ((float) delta * 0.5f), false);
                if (m_nScriptTime >= 38001) {
                    if (m_nScriptTime - delta <= 38000) {
                        Globals::sound->play(2248, nullptr, nullptr, 0.0f);
                    }
                    AEGeometry *planet = (*(Array<AEGeometry *> *) m_pLevel->getStarSystem()->getPlanets())[0];
                    planet->setScaling(planet->getScaling() * 0.95f);
                    if (m_nScriptTime >= 39001) {
                        Globals::layout->startFade(true, -1, 500);
                        m_nState = 3;
                        return 1;
                    }
                }
                break;
            }
            case 3: {
                bool fading = Globals::layout->isFading();
                StarSystem *stars = m_pLevel->getStarSystem();
                if (fading) {
                    AEGeometry *planet = (*(Array<AEGeometry *> *) stars->getPlanets())[0];
                    if (planet->getScaling().x < 10.0f) {
                        planet->setScaling(planet->getScaling() * 4.0f);
                    }
                    m_pCamera->setRumblePercentage(1.0f, 100);
                } else {
                    stars->switchSunForSupernovaIntro();
                    Globals::layout->startFade(false, -1, 10000);
                    (*enemies)[2]->setVisible(true);
                    for (unsigned int i = 3; i < enemies->count; i++) {
                        (*enemies)[i]->player->setHitpoints(0);
                    }
                    m_nScriptTimerA = 0;
                    m_nState = 4;
                }
                break;
            }
            case 4:
                m_pLevel->getStarSystem()->scaleSunDuringSupernovaIntro(delta);
                m_pLevel->getStarSystem()->updateSupernova(delta);
                if (m_nScriptTimerA <= 7000 && m_nScriptTimerA + delta > 7000) {
                    Globals::layout->startFade(true, 255, delta + 1000);
                }
                m_nScriptTimerA += delta;
                if (m_nScriptTimerA >= 8000) {
                    m_nState = 5;
                }
                break;
            case 5:
                if (!Globals::layout->isFading()) {
                    Globals::layout->enableFillScreen(true);
                    Globals::status->nextCampaignMission(true);
                    Globals::switch_to_target_setting = 0;
                    Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
                    Globals::status->getStation()->setAttackedFriends(false);
                    Level::comingFromAlienWorld = 0;
                    Globals::appManager->SetCurrentApplicationModule(5);
                    Level::initStreamOutPosition = 0;
                    return 0;
                }
                break;
        }
        return 1;
    } else if (Globals::status->getCurrentCampaignMission() == 106) {
        // Chain A, mission 106: the cloaked fighter reveals itself and jumps away (0x138c7a).
        if (m_nState == 1) {
            m_pCamera->translate((float) -delta * 0.3f, (float) -delta * 0.05f, (float) delta * 0.1f);
            if (((RadioMessage *) ((*messages)[2]))->isOver()) {
                m_nScriptTimerA = 0;
                m_nState = 2;
                (*enemies)[0]->awake();
                (*enemies)[0]->setVisible(true);
                ((PlayerFighter *) (*enemies)[0])->setCloakingPossible(false);
                ((PlayerFighter *) (*enemies)[0])->setAIDisabled(true);
            }
        } else if (m_nState == 2) {
            m_nScriptTimerA += delta;
            m_pLevel->lodManager->forceUpdate(0, false);
            if (m_nScriptTimerA >= 3001) {
                AEGeometry *ship = (*enemies)[0]->geometry;
                m_pCamera->setTarget(ship);
                m_tempVec = ship->getPosition();
                m_tempVec += ship->getDirection() * 8000.0f;
                m_tempVec += ship->getUpVector() * 500.0f;
                m_tempVec += ship->getRightVector() * 1000.0f;
                m_pCamera->setPosition(m_tempVec);
                m_nState = 3;
                m_nScriptTimerA = 0;
            } else {
                m_pCamera->translate((float) -delta * 0.1f, (float) -delta * 0.1f, (float) delta * 0.4f);
            }
        } else if (m_nState == 3 && ((RadioMessage *) ((*messages)[4]))->isTriggered()) {
            m_nState = 4;
        } else if (m_nState == 4 && ((RadioMessage *) ((*messages)[4]))->isOver()) {
            m_pCamera->setFixed(true);
            ((PlayerFighter *) (*enemies)[0])->setAIDisabled(false);
            (*enemies)[0]->player->setHitpoints(0);
            m_nScriptTimerA = 0;
            m_nState = 5;
        } else if (m_nState == 5) {
            m_nScriptTimerA += delta;
            if (m_nScriptTimerA >= 3001) {
                m_nState = 6;
            }
        } else if (m_nState == 6 && ((RadioMessage *) ((*messages)[5]))->isOver()) {
            Globals::status->field_64 = player->player->getHitpoints();
            Globals::status->field_5c = player->player->getShieldHP();
            Globals::status->field_60 = player->player->getArmorHP();
            Globals::status->field_68 = player->player->getGammaHP();
            Globals::status->nextCampaignMission(true);
            Globals::status->nextCampaignMission(true);
            Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
            Globals::switch_to_target_setting = 1;
            Level::initStreamOutPosition = 1;
            Globals::appManager->SetCurrentApplicationModule(2);
        }
        if (m_nState >= 2 && m_nState <= 4) {
            (*enemies)[0]->geometry->moveForward((float) delta);
        }
        m_bCinematicBreak = 1;
        return 1;
    } else if (Globals::status->getCurrentCampaignMission() == 95 || Globals::status->getCurrentCampaignMission() == 99
               || Globals::status->getCurrentCampaignMission() == 109
               || Globals::status->getCurrentCampaignMission() == 119
               || Globals::status->getCurrentCampaignMission() == 126
               || Globals::status->getCurrentCampaignMission() == 133
               || Globals::status->getCurrentCampaignMission() == 160
               || Globals::status->getCurrentCampaignMission() == 161) {
        // Chain A: short station-departure intro shared by several missions (0x138ea4).
        m_nScriptTimerA += delta;
        m_pCamera->translate((float) delta, 0.0f, (float) (-2 * delta));
        if (m_nState == 0) {
            if (m_nScriptTimerA >= 2000) {
                m_nState = 1;
            }
        } else if (m_nState == 1) {
            if (((RadioMessage *) ((*messages)[0]))->isOver()) {
                m_nState = 2;
                m_nScriptTimerA = 0;
            }
        }
        if (m_nState == 2 && m_nScriptTimerA >= 2000) {
            ((Mission *) (intptr_t) Globals::status->getCampaignMission())->setStatusValue(1);
        }
        return 1;
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
                    ((PlayerFighter *) (*wingmen)[i])->field_0x12d = 1;
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
            m_pLevel->getPlayer()->player->setVulnerable(true);
            m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11)
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
                        ((PlayerFighter *) (*wingmen)[i])->field_0x12d = 0;
                    }
                }
            }
        }
    }

    // --- pre-switch mission specials (disasm 0x13812a); run for all missions ---
    // Re-read the campaign mission (fresh call at 0x13812e; -Oz has no CSE).
    if (Globals::status->getCurrentCampaignMission() == 5) {
        // disasm 0x13813c: on the first frame, park enemies[0] ahead of the player and wake it.
        if (m_nState == 0) {
            Vector *scratch = &m_tempVec; // LevelScript tempVec @0x28
            *scratch = player->getPosition();
            (*enemies)[0]->setPosition(scratch->x + 5000.0f, scratch->y, scratch->z + 30000.0f);
            (*enemies)[0]->geometry->rotate(0.0f, 3.1415927f, 0.0f); // const @0x13853c = pi
            (*enemies)[0]->awake();
            m_nState = 1;
        }
    } else if (Globals::status->getCurrentCampaignMission() == 26 && Globals::status->inAlienOrbit()) {
        // disasm 0x1382dc: in alien orbit, once radio message #1 fires, spin up the exit wormhole.
        if (((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0) {
            m_nState = 1;
            Array<KIPlayer *> *landmarks = m_pLevel->getLandmarks();
            ((PlayerWormHole *) (*landmarks)[3])->reset(true);
        }
    } else if ((Globals::status->getCurrentCampaignMission() == 50 ||
                Globals::status->getCurrentCampaignMission() == 51) && messages != nullptr &&
               messages->count >= 3 && ((RadioMessage *) ((*messages)[2]))->isOver() && m_nState == 0) {
        // disasm 0x139ac2 (missions 50 @0x1383e4 and 51 @0x13876c share this body): the exit-wormhole
        // cutscene turns every non-wingman ship permanently hostile and floors the player's standing.
        for (unsigned int i = 0; i < enemies->count; ++i) {
            KIPlayer *kp = (*enemies)[i];
            if (!kp->isWingMan()) {
                kp->player->setAlwaysFriend(false);
                kp->player->setAlwaysEnemy(true);
                kp->player->turnEnemy();
            }
        }
        ((Standing *) (intptr_t) Globals::status->getStanding())->setStanding(0, 100);
        m_nState = 1; // 0x1381aa -> switch guard 0x13879e
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
        switch (cm) {
            case 158: {
                if (m_nState == 1) {
                    // Fly the marker ship in towards the station.
                    m_nScriptTimerA += delta;
                    float approach = (float) m_nScriptTimerA / -34000.0f + 1.0f;
                    float step = (float) delta;
                    m_pCamera->translate(step * 4.8f * approach, 0.0f, (float) -delta * 0.7f * approach);
                    AEGeometry *marker = m_pGeometry4;
                    Vector target{9000.0f, 0.0f, -13000.0f};
                    marker->setDirection(AbyssEngine::AEMath::VectorNormalize(target - marker->getPosition()),
                                         Vector{0.0f, 1.0f, 0.0f});
                    if (m_nScriptTimerA <= 29999) {
                        m_pGeometry4->moveForward(step * 2.5f);
                    }
                    if (m_nScriptTimerA > 25000) {
                        if (!(*enemies)[0]->isVisible()) {
                            (*enemies)[0]->setVisible(true);
                            (*enemies)[0]->setActive(true);
                        }
                    }
                    (*enemies)[0]->geometry->setPosition(m_pGeometry4->getPosition());
                    m_pCamera->setTarget((*enemies)[0]->geometry);
                    player->geometry->rotate(step / 2000.0f, step / 2000.0f, step / 2000.0f);
                    if (m_nScriptTimerA >= 20000) {
                        ((PlayerFighter *) (*enemies)[0])->setExhaustVisible(false);
                        if (m_nScriptTimerA >= 34000) {
                            m_nScriptTimerA = 0;
                            m_nScriptTimerB = 0;
                            ((PlayerFighter *) (*enemies)[0])->setExhaustVisible(true);
                            m_nState = 2;
                        }
                    }
                } else if (m_nState == 2) {
                    // Hold while the marker drifts closer, then hand control back to the player.
                    if (m_nScriptTimerA <= 2999) {
                        m_nScriptTimerA += delta;
                    }
                    m_nScriptTimerB += delta;
                    (*enemies)[0]->geometry->moveForward((float) m_nScriptTimerA * 0.1f);
                    if (m_nScriptTimerB >= 12001 && ((RadioMessage *) ((*messages)[2]))->isOver()) {
                        field_0x24 = 0;
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        player->setFreeze(false);
                        player->setVisible(true);
                        player->setSpeed(2.0f);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        player->player->setVulnerable(true);
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        m_nScriptTimerA = 0;
                        m_nScriptTimerB = 0;
                        ((PlayerFighter *) (*enemies)[0])->setCloakingPossible(true);
                        (*enemies)[0]->player->setAlwaysEnemy(true);
                        ((PlayerFighter *) (*enemies)[0])->setAIDisabled(false);
                        player->geometry->setDirection(
                                AbyssEngine::AEMath::VectorNormalize(
                                        -(player->getPosition() - (*enemies)[0]->getPosition())),
                                Vector{0.0f, 1.0f, 0.0f});
                        m_nState = 3;
                    }
                } else if (m_nState == 3) {
                    // Every 4s a dormant escort may join the fight; every 25s the marker toggles.
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB += delta;
                    if (m_nScriptTimerA >= 4001) {
                        m_nScriptTimerA = 0;
                        if (AbyssEngine::AEMath::VectorLength((*enemies)[0]->getPosition() - player->getPosition())
                            < 25000.0f && Globals::rnd->nextInt(100) >= 61) {
                            for (unsigned int i = 1; i <= 3; i++) {
                                if (!(*enemies)[i]->player->isActive()) {
                                    (*enemies)[i]->revive();
                                    (*enemies)[i]->setActive(true);
                                    (*enemies)[i]->setPosition((*enemies)[0]->getPosition());
                                    (*enemies)[i]->player->turnEnemy();
                                    break;
                                }
                            }
                        }
                    }
                    if (m_nScriptTimerB >= 25001) {
                        m_nScriptTimerB = 0;
                        ((PlayerFighter *) (*enemies)[0])->field_0x140 = !((PlayerFighter *) (*enemies)[0])->field_0x140;
                    }
                }
                break;
            }
            case 131: {
                // Opening fly-by: the camera rides along in front of the player's ship.
                if (m_nState == 1) {
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(
                            canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    Vector *tempVec = &m_tempVec;
                    Vector *scratch = &m_dirVec;
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
                } else if (m_nState == 2) {
                    // State 2 (0x13c8fc): keep driving the same cinematic camera creep as state 1
                    // (dir*delta*2.01 + right*delta*0.03 + up*delta*0.01 offset applied to the render
                    // camera's local matrix), then once the intro radio (message #0) is over run the
                    // standard cutscene-exit teardown and hand control back to the player.
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(
                            canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    Vector *tempVec = &m_tempVec;
                    Vector *scratch = &m_dirVec;
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
                    if (((RadioMessage *) ((*messages)[0]))->isOver()) {
                        player->player->setVulnerable(true);
                        player->setCollide(true);
                        player->setComputerControlled(false);
                        player->setTurretMode(false);
                        player->setFreeLookMode(false);
                        m_pCamera->enableFirstPersonCam(false);
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setLookAtCam(false);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nScriptTimerA = 0;
                        m_bCinematicBreak = 0;
                        m_nState = 3;
                    }
                }
                break;
            }
            case 64: {
                // The pirate fleet flies past the player and leaves along a far-away route.
                if (m_nState == 2) {
                    (*enemies)[0]->field_0x24 = 1;
                }
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[0]))->isTriggered()) {
                    player->freeze = 1;
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*enemies)[0]->geometry);
                    Vector camPos = (*enemies)[0]->getPosition();
                    camPos += (*enemies)[0]->geometry->getDirection() * 10000.0f;
                    camPos.z += 3000.0f;
                    camPos.y += 300.0f;
                    m_pCamera->setPosition(camPos);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 1;
                } else if (m_nState == 1) {
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_pCamera->translate(0.0f, (float) ((double) delta * 0.2), 0.0f);
                    if (((RadioMessage *) ((*messages)[2]))->isTriggered()) {
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        player->freeze = 0;
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            (*enemies)[i]->setRoute(nullptr);
                        }
                        m_nState = 2;
                    }
                } else if (m_nState == 2 && ((RadioMessage *) ((*messages)[5]))->isTriggered()) {
                    (*enemies)[0]->field_0x24 = 0;
                    int routePoints[3] = {500000, 500000, 500000};
                    Route *route = new Route(routePoints, 3);
                    for (unsigned int i = 1; i < enemies->count; i++) {
                        if (!(*enemies)[i]->isWingMan()) {
                            (*enemies)[i]->setSpeed(16.5f);
                            (*enemies)[i]->player->setEnemies(nullptr);
                            (*enemies)[i]->setRoute(route->clone());
                        }
                    }
                    m_nState = 3;
                } else if (m_nState == 3 && ((RadioMessage *) ((*messages)[6]))->isOver()) {
                    for (unsigned int i = 1; i < enemies->count; i++) {
                        (*enemies)[i]->setDead();
                    }
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
                        m_nScriptTime < 60001) {
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
                    m_nScriptTimerA = 0; // 64-bit script counter @0x90
                    break; // -> post-switch tail (0x144e36)
                }
                if (m_nState < 1) {
                    break; // negative state -> post-switch tail (0x144e36)
                }
                // states >= 1: accumulate the 64-bit script counter {m_nScriptTimerA:m_nScriptCounterA}.
                m_nScriptTimerA += delta;
                if (m_nState == 1) {
                    // Advance once the mission's production quota is at least half filled.
                    int statusValue = ((Mission *) Globals::status->getCampaignMission())->getStatusValue();
                    int amount = ((Mission *) Globals::status->getCampaignMission())->getProductionGoodAmount();
                    if (statusValue >= amount / 2) {
                        m_nState++;
                    }
                }
                if (m_nScriptTimerA < 75001) {
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
                m_nScriptTimerA = 0;
                break; // -> post-switch tail (0x144e36)
            }
            case 145: {
                // The fleet bombards the alien mothership.
                float step = (float) (delta << 1);
                for (int i = 0; i < 13; i++) {
                    (*enemies)[i]->geometry->moveForward(step);
                    if (i != 0 && m_nState >= 2) {
                        if (m_nState != 4 || m_nScriptTimerA <= 1000) {
                            (*enemies)[i]->player->shoot(0, delta, false);
                        }
                    }
                }
                if (m_nState == 0) {
                    for (int i = 0; i < 13; i++) {
                        (*enemies)[i]->setEnemies(nullptr);
                        (*enemies)[i]->player->setEnemy((*enemies)[14]->player);
                    }
                    m_pExplosion = new Explosion(0);
                    m_pExplosion->addFireStreaks();
                    m_pExplosion->setScaling(12.0f);
                    m_nState = 1;
                } else if (m_nState == 1 && ((RadioMessage *) ((*messages)[0]))->isOver()) {
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
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                    m_tempVec = (*enemies)[0]->geometry->getPosition();
                    m_tempVec += (*enemies)[0]->geometry->getDirection() * 18000.0f;
                    m_tempVec += (*enemies)[0]->geometry->getRightVector() * 3000.0f;
                    m_pCamera->setPosition(m_tempVec);
                    (*enemies)[0]->setVisible(true);
                    (*enemies)[0]->setActive(true);
                    m_nScriptTimerA = 0;
                    m_nState++;
                } else if (m_nState == 2) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 1001) {
                        (*enemies)[0]->player->shoot(0, delta, false);
                        m_pGeometry4 = new AEGeometry(Globals::Canvas);
                        m_tempVec = (*enemies)[0]->player->getPosition();
                        m_pGeometry4->setPosition(m_tempVec);
                        Gun *leadGun = (*(*(*enemies)[0]->player->guns)[0])[0];
                        m_tempVec = *reinterpret_cast<Vector *>(reinterpret_cast<char *>(leadGun) + 0x18);
                        m_pGeometry4->setDirection(m_tempVec, Vector{0.0f, 1.0f, 0.0f});
                        m_pGeometry4->setVisible(false);
                        m_pCamera->setTarget(m_pGeometry4);
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 3) {
                    Gun *leadGun = (*(*(*enemies)[0]->player->guns)[0])[0];
                    m_pGeometry4->moveForward(leadGun->pitchRate * (float) delta);
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 5001) {
                        m_matrix = (*enemies)[14]->geometry->getMatrix();
                        m_pLevel->field_74->systemSetMatrix(m_pLevel->field_54, &m_matrix);
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_54, true);
                        m_pExplosion->start(AbyssEngine::AEMath::MatrixGetPosition(m_matrix), Vector{0.0f, 0.0f, 0.0f});
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 4) {
                    m_nScriptTimerA += delta;
                    m_pExplosion->update((int) ((float) delta * 0.5f), nullptr);
                    if (m_nScriptTimerA >= 324) {
                        KIPlayer *wreck = (*enemies)[13];
                        KIPlayer *boss = (*enemies)[14];
                        wreck->geometry->setMatrix(boss->geometry->getMatrix());
                        wreck->setVisible(true);
                        wreck->setActive(true);
                        boss->setVisible(false);
                        boss->setActive(false);
                        boss->field_0x74 = 1;
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(wreck->geometry->transform))
                                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 5) {
                    m_nScriptTimerA += delta;
                    long long elapsed = m_nScriptTimerA;
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*enemies)[13]->geometry->transform))
                            ->Update((long long) ((float) delta * 0.3f), false);
                    for (int i = 0; i < 13; i++) {
                        AEGeometry *ship = (*enemies)[i]->geometry;
                        m_tempVec = ship->getDirection();
                        if (elapsed > 2000 && elapsed < 5000) {
                            m_tempVec -= ship->getRightVector() * ((float) elapsed / 100000.0f);
                        }
                        ship->setDirection(AbyssEngine::AEMath::VectorNormalize(m_tempVec), Vector{0.0f, 1.0f, 0.0f});
                        ship->moveForward((float) (elapsed * (long long) delta) * 0.001f);
                    }
                    m_pExplosion->update((int) ((float) delta * 0.5f), nullptr);
                    if (m_nScriptTimerA > 9999) {
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_54, false);
                        for (int i = 0; i < 13; i++) {
                            (*enemies)[i]->setActive(false);
                            (*enemies)[i]->setVisible(false);
                            (*enemies)[i]->setPosition(50000.0f, 50000.0f, 50000.0f);
                        }
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        player->player->setVulnerable(true);
                        player->setFreeze(false);
                        player->setVisible(true);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_pRadar->lockedEnemy = nullptr;
                        m_pRadar->dockNavPtr = nullptr;
                        m_bCinematicBreak = 0;
                        m_nState++;
                    }
                }
                break;
            }
            case 142: {
                // Collect the plot cargo from a sparking gas cloud while the freighter waits.
                if (m_nState <= 1) {
                    if (AbyssEngine::AEMath::VectorLength(
                                (*enemies)[0]->getPosition() - (*enemies)[0]->getRoute()->getWaypoint(0)->getPosition())
                        < 3000.0f) {
                        (*enemies)[0]->setSpeed(0.0f);
                    }
                }
                if (m_nState == 0 && player->getRoute() != 0
                    && (uint8_t) ((Route *) (intptr_t) player->getRoute())->getWaypoint(0)->state) {
                    m_nState = 1;
                } else if (m_nState == 1 && m_pLevel->getGasClouds() != nullptr) {
                    for (unsigned int i = 0; i < m_pLevel->getGasClouds()->count; i++) {
                        if (((PlayerGasCloud *) (*m_pLevel->getGasClouds())[i])->getSparks() != nullptr) {
                            m_nState = 2;
                        }
                    }
                } else if (m_nState == 2
                           && (Globals::status->getShip()->hasCargo(201, 1) || Globals::status->getShip()->hasCargo(202, 1)
                               || Globals::status->getShip()->hasCargo(203, 1)
                               || Globals::status->getShip()->hasCargo(204, 1)
                               || Globals::status->getShip()->hasCargo(205, 1))) {
                    (*enemies)[0]->setSpeed(2.1f);
                    m_nState = 3;
                }
                break;
            }
            case 144: {
                // Departure: follow the marker out while the cloaked escorts slip away.
                m_pGeometry4->moveForward((float) (delta * 5));
                m_bCinematicBreak = 1;
                m_pCamera->translate(0.0f, (float) delta * 0.05f, (float) delta * 0.28f);
                if (m_nState == 1) {
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_pCamera->setTarget(m_pGeometry4);
                    m_nState = 2;
                } else if (m_nState == 2) {
                    if (((RadioMessage *) ((*messages)[3]))->isOver()) {
                        m_nScriptTimerA = 0;
                        m_nState = 3;
                    }
                } else if (m_nState == 3) {
                    m_nScriptTimerA += delta;
                    for (unsigned int i = 1; i < enemies->count; i++) {
                        if ((*enemies)[i]->shipGroupFlag == 44) {
                            if (Globals::rnd->nextInt(100) <= 9 && !((PlayerFighter *) (*enemies)[i])->field_0x13c) {
                                ((PlayerFighter *) (*enemies)[i])->setCloakingPossible(true);
                                ((PlayerFighter *) (*enemies)[i])->cloak(20000, true);
                            }
                            (*enemies)[i]->geometry->moveForward((float) m_nScriptTimerA * 0.05f);
                        }
                    }
                    if (m_nScriptTimerA > 4000) {
                        (*enemies)[0]->geometry->moveForward((float) (m_nScriptTimerA - 4000) * 0.05f);
                        if (m_nScriptTimerA >= 7001) {
                            m_nState = 4;
                            Globals::status->nextCampaignMission(true);
                            Globals::status->setStation(Globals::status->getStation());
                            Globals::status->departStation(Globals::status->getStation());
                            if (Level::initStreamOutPositionAfterCutscene) {
                                Level::initStreamOutPositionAfterCutscene = 0;
                                Level::initStreamOutPosition = 1;
                            }
                            Globals::appManager->SetCurrentApplicationModule(2);
                        }
                    }
                }
                break;
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
                            ->setName(String("", false));
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
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
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
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0; // cinematicBreak_ (m_nFlags high byte @0x11) = 0
                        m_nState = 2;
                        (*m_pLevel->getEnemies())[0]->setActive(false);
                        (*m_pLevel->getEnemies())[0]->setToSleep();
                        (*m_pLevel->getEnemies())[0]->setPosition(0.0f, 0.0f, -5000000.0f); // vtable+0x48
                        (*m_pLevel->getEnemies())[0]->setVisible(false);
                    }
                }
                break;
            }
            case 73: { // 0x139834 (tbh on m_nState 0..3)
                switch (m_nState) {
                    case 0: {
                        // Proximity gate: once the player closes to within 49999 units of enemies[8].
                        Vector playerPos = player->getPosition();
                        Vector enemyPos = (*enemies)[8]->getPosition(); // vtable+0x28
                        if ((int) AbyssEngine::AEMath::VectorLength(playerPos - enemyPos) <= 49999) {
                            m_nState = 1;
                        }
                        break;
                    }
                    case 1:
                        // Wait for any escort (enemies[8+]) whose EMP points dropped below max.
                        for (unsigned int i = 8; i < enemies->count; ++i) {
                            if ((*enemies)[i]->field_0x40 != 0 &&
                                (*enemies)[i]->player->getEmpPoints() < (*enemies)[i]->player->getMaxEmpPoints()) {
                                m_nState = 2;
                                break;
                            }
                        }
                        break;
                    case 2:
                        // Once any EMP-disabled escort is found, stop the first live escort.
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
                    case 3:
                        // Once radio message #7 fires, scatter enemies[4..7] around the player.
                        if (((RadioMessage *) ((*messages)[7]))->isTriggered()) {
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
                        }
                        break;
                }
                break;
            }
            case 67: {
                // The smuggler freighter: stop it, scan it, then follow it through the pirates.
                if (((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0) {
                    m_nState = 1;
                    (*enemies)[10]->setSpeed(0.0f);
                    (*enemies)[10]->setEnemies(nullptr);
                    ((PlayerFighter *) (*enemies)[10])->setExhaustVisible(false);
                } else if (((RadioMessage *) ((*messages)[3]))->isOver() && m_nState == 1) {
                    (*enemies)[10]->setSpeed(3.0f);
                    ((PlayerFighter *) (*enemies)[10])->setExhaustVisible(true);
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*enemies)[10]->geometry);
                    m_pCamera->setPosition((*enemies)[10]->getPosition() + Vector{6000.0f, -200.0f, 1000.0f});
                    player->player->setVulnerable(false);
                    player->freeze = 1;
                    m_nState = 2;
                } else if (m_nState == 2) {
                    if (((RadioMessage *) ((*messages)[4]))->isOver()) {
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        player->player->setVulnerable(true);
                        player->freeze = 0;
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        (*enemies)[10]->setPosition(500000.0f, 500000.0f, 500000.0f);
                        (*enemies)[10]->setActive(false);
                        (*enemies)[10]->setVisible(false);
                        m_nState = 3;
                        m_bCinematicBreak = 0;
                    }
                } else if (((RadioMessage *) ((*messages)[5]))->isTriggered() && m_nState == 3) {
                    Vector origin = (*enemies)[0]->getPosition();
                    for (int i = 5; i != 9; i++) {
                        KIPlayer *pirate = (*enemies)[i];
                        int dx = Globals::rnd->nextInt(10000) + 15000;
                        int x = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dx;
                        int dy = Globals::rnd->nextInt(10000);
                        int dz = Globals::rnd->nextInt(10000) + 15000;
                        int z = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dz;
                        pirate->setPosition(origin + Vector{(float) x, (float) (dy - 5000), (float) z});
                    }
                    m_nState = 4;
                } else if (((RadioMessage *) ((*messages)[7]))->isTriggered() && m_nState == 4) {
                    Vector origin = (*enemies)[0]->getPosition();
                    (*enemies)[10]->setPosition(origin.x, origin.y, origin.z + 12000.0f);
                    (*enemies)[10]->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
                    (*enemies)[10]->setActive(true);
                    (*enemies)[10]->setVisible(true);
                    (*enemies)[10]->setSpeed(2.0f);
                    delete m_pLevel->objectivesB;
                    m_pLevel->objectivesB = nullptr;
                    int routePoints[3] = {0, 0, 800000};
                    Route *route = new Route(routePoints, 3);
                    (*enemies)[10]->setRoute(route);
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*enemies)[10]->geometry);
                    m_pCamera->setPosition((*enemies)[10]->getPosition() + Vector{6000.0f, -200.0f, 10000.0f});
                    player->player->setVulnerable(false);
                    player->freeze = 1;
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 5;
                } else if (((RadioMessage *) ((*messages)[9]))->isTriggered() && m_nState == 5) {
                    player->player->setVulnerable(true);
                    player->freeze = 0;
                    (*enemies)[10]->setSpeed(8.0f);
                    m_pCamera->setLookAtCam(false);
                    m_pCamera->setTarget(player->geometry);
                    player->setComputerControlled(false);
                    m_pHud->visible = 1;
                    m_pRadar->enabled = 1;
                    resetCamera(m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 6;
                    m_bCinematicBreak = 0;
                } else if (((RadioMessage *) ((*messages)[11]))->isOver() && m_nState == 6) {
                    (*enemies)[10]->setActive(false);
                    (*enemies)[10]->setVisible(false);
                    m_nState = 7;
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
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
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
                    m_pRadar->enabled = 1;
                    resetCamera(m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_bCinematicBreak = 0; // clear cinematicBreak_ (m_nFlags high byte @0x11)
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
            case 29: { // 0x139ca2
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
                    // Intro-approach cutscene (0x13e1f6): hand control back to the AI, hide the
                    // HUD/radar cinematic overlay, then reframe the camera to a fixed offset built
                    // from the player ship's own basis vectors (dir*25000 + right*1000 + up*1000 +
                    // pos, accumulated into the LevelScript scratch vector @0x28), and finally spawn
                    // the intro geometry (mesh 0x37d2) into m_pGeometry6, seat it on the player, target
                    // the camera on it and fire radio/sound event 14. FP immediates from the pool:
                    //   @0x13e3ec = 25000.0f (forward), @0x13e3f0 = 1000.0f (right/up).
                    m_nState = 1;
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->player->setVulnerable(false);
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->stopShooting(0);
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pCamera->setLookAtCam(true);
                    player->setComputerControlled(true);
                    m_pCamera->setPosition(player->geometry->getPosition());
                    Vector *frame = &m_tempVec; // LevelScript tempVec @0x28
                    *frame = player->geometry->getDirection();
                    *frame *= 25000.0f;
                    Vector tmp{0.0f, 0.0f, 0.0f};
                    tmp = player->geometry->getRightVector();
                    tmp *= 1000.0f;
                    *frame += tmp;
                    tmp = player->geometry->getUpVector();
                    tmp *= 1000.0f;
                    *frame += tmp;
                    *frame += player->geometry->getPosition();
                    m_pCamera->setPosition(frame->x, frame->y, frame->z);
                    AEGeometry *intro = new AEGeometry(0x37d2, Globals::Canvas, false);
                    m_pGeometry6 = intro;
                    m_pGeometry6->setPosition(player->geometry->getPosition());
                    m_pGeometry6->setDirection(player->geometry->getDirection(),
                                               Vector{0.0f, 1.0f, 0.0f});
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_pCamera->setTarget(m_pGeometry6);
                    Globals::sound->play(14, nullptr, nullptr, 0.0f);
                    Vector camPos = *m_pCamera->getPosition();
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(14, &camPos, &zero, false);
                } else if (((RadioMessage *) ((*messages)[2]))->isOver() && m_nState == 1) {
                    // Cutscene over: retarget the camera to the player, restore the HUD/radar and
                    // player controls, drop the intro geometry and spawn the follow-up objective.
                    m_nState = 2;
                    m_pCamera->setTarget(player->geometry);
                    m_pHud->visible = 1;
                    m_pRadar->enabled = 1;
                    m_bCinematicBreak = 0; // cinematicBreak_ (m_nFlags high byte @0x11) = 0
                    m_pCamera->setLookAtCam(false);
                    player->setComputerControlled(false);
                    player->player->setVulnerable(true);
                    if (m_pGeometry6 != nullptr) {
                        delete m_pGeometry6;
                    }
                    m_nScriptTime = 0;
                    m_nTimeLimit = 180000;
                    m_pGeometry6 = nullptr;
                    m_pLevel->objectivesA = new Objective(3, 180000, m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                }
                if (m_pGeometry6 != nullptr) {
                    m_pGeometry6->moveForward((float) (delta * 3));
                }
                break;
            }
            case 41: {
                // The freighter escape: escorts jump in, the freighter powers up and leaves.
                if (((RadioMessage *) ((*messages)[4]))->isTriggered() && m_nState == 0) {
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    player->freeze = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_bCinematicBreak = 1;
                    m_pCamera->setLookAtCam(true);
                    player->setComputerControlled(true);
                    player->player->setVulnerable(false);
                    for (int i = 1; i < 4; i++) {
                        (*enemies)[i]->revive();
                        (*enemies)[i]->setPosition((*enemies)[0]->getPosition());
                        (*enemies)[i]->player->setEnemy((*enemies)[0]->player);
                        (*enemies)[i]->field_0x38 = 0;
                        (*enemies)[i]->setState(1);
                    }
                    m_tempVec = Vector{-40000.0f, 500.0f, -30000.0f};
                    (*enemies)[1]->translate(m_tempVec);
                    (*enemies)[1]->geometry->setDirection(AbyssEngine::AEMath::VectorNormalize(-m_tempVec),
                                                          Vector{0.0f, 1.0f, 0.0f});
                    m_tempVec = Vector{-41000.0f, -200.0f, -31000.0f};
                    (*enemies)[2]->translate(m_tempVec);
                    (*enemies)[2]->geometry->setDirection(AbyssEngine::AEMath::VectorNormalize(-m_tempVec),
                                                          Vector{0.0f, 1.0f, 0.0f});
                    m_tempVec = Vector{-42000.0f, 100.0f, -32000.0f};
                    (*enemies)[3]->translate(m_tempVec);
                    (*enemies)[3]->geometry->setDirection(AbyssEngine::AEMath::VectorNormalize(-m_tempVec),
                                                          Vector{0.0f, 1.0f, 0.0f});
                    m_pCamera->setTarget((*enemies)[0]->geometry);
                    m_pCamera->setPosition((*enemies)[0]->getPosition());
                    m_pCamera->translate(-38000.0f, 0.0f, -30200.0f);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 1;
                } else if (((RadioMessage *) ((*messages)[5]))->isTriggered() && m_nState == 1) {
                    m_nState = 2;
                    m_pCamera->setRotationAroundTarget(false);
                    (*enemies)[0]->geometry->updateReferenceMatrix();
                    m_pLevel->field_74->enableSystemEmit(field_0xe0, true);
                    m_pLevel->field_74->enableSystemEmit(field_0xe4, true);
                    KIPlayer *freighter = (*m_pLevel->getEnemies())[0];
                    freighter->player->setHitpoints(9999999);
                    ((PlayerFixedObject *) freighter)->setMoving(false);
                    m_pCamera->setTarget(freighter->geometry);
                    m_pCamera->setPosition(freighter->getPosition());
                    m_pCamera->translate(-3000.0f, -2000.0f, 12000.0f);
                    Globals::sound->play(155, nullptr, nullptr, 0.0f);
                    (*enemies)[0]->player->StopEngineSound();
                    m_nScriptTimerA = 0;
                } else if (m_nState == 2) {
                    (*enemies)[0]->geometry->updateReferenceMatrix();
                    KIPlayer *freighter = (*m_pLevel->getEnemies())[0];
                    freighter->translate(Vector{0.0f, (float) -delta, (float) (2 * delta)});
                    freighter->geometry->rotate(0.0f, 0.0f, (float) delta * 3.0e-5f);
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 15001) {
                        m_nState = 3;
                    }
                } else if (m_nState == 3) {
                    (*enemies)[0]->geometry->updateReferenceMatrix();
                    KIPlayer *freighter = (*m_pLevel->getEnemies())[0];
                    freighter->setPosition(2006.0f, -31500.0f, -86720.0f);
                    freighter->geometry->setRotation(-0.4f, 0.0f, 1.8f);
                    ((PlayerFixedObject *) freighter)->setExhaustVisible(false);
                    m_pCamera->setPosition(freighter->getPosition());
                    m_pCamera->translate(3000.0f, 1000.0f, 2000.0f);
                    m_pCamera->useTargetsUpVector(false);
                    for (unsigned int i = 1; i < enemies->count; i++) {
                        KIPlayer *ship = (*enemies)[i];
                        if (ship != nullptr && ship->shipGroup == 9) {
                            ship->player->setEnemy(player->player);
                        }
                    }
                    m_nScriptTimerA = 0;
                    m_nState = 4;
                } else if (m_nState == 4) {
                    (*enemies)[0]->geometry->updateReferenceMatrix();
                    m_nScriptTimerA += delta;
                    m_pCamera->translate((float) delta, (float) delta, (float) (-2 * delta));
                    if (m_nScriptTimerA >= 15001) {
                        Globals::sound->stop(156);
                        m_nState = 5;
                        (*enemies)[0]->player->setHitpoints(100);
                        (*enemies)[0]->speed = 0.0f;
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        m_bCinematicBreak = 0;
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->useTargetsUpVector(true);
                        player->setComputerControlled(false);
                        player->player->setVulnerable(true);
                        player->freeze = 0;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                    }
                }
                break;
            }
            case 80: {
                // The capital ship destroys the pirate base, then the player is towed into the dock.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[0]))->isOver()) {
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    player->freeze = 1;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(m_pGeometry1);
                    m_tempVec = m_pGeometry1->getPosition();
                    m_tempVec += Vector{-20000.0f, 5000.0f, -35000.0f};
                    m_pCamera->setPosition(m_tempVec);
                    m_nScriptTimerA = 0;
                    m_nState = 1;
                    Globals::sound->play(1121, nullptr, nullptr, 0.0f);
                } else if (m_nState == 1) {
                    m_pCamera->translate((float) -delta, (float) delta, (float) (-12 * delta));
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 3001) {
                        m_nScriptTimerA = 0;
                        m_nState = 2;
                    }
                } else if (m_nState == 2) {
                    m_nScriptTimerA += delta;
                    m_pCamera->translate((float) -delta, (float) delta, (float) (-12 * delta));
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                            ->Update((long long) delta, true);
                    if (m_nScriptTimerA >= 2001) {
                        m_nScriptTimerA = 0;
                        m_nState = 6;
                        m_pGeometry1->setVisible(false);
                    }
                } else if (m_nState == 6) {
                    m_pGeometry6->setPosition(Vector{12487.0f, -11451.0f, 5958.0f});
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(m_pGeometry6);
                    m_tempVec = Vector{-28855.0f, -30075.0f, 63820.0f} / 1.7f;
                    m_pCamera->setPosition(m_tempVec);
                    Vector facing = m_pGeometry6->getPosition() - m_tempVec;
                    AEGeometry *frame = new AEGeometry(Globals::Canvas);
                    frame->setDirection(facing, Vector{0.0f, 1.0f, 0.0f});
                    frame->setPosition(m_tempVec);
                    Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(), frame->getMatrix());
                    delete frame;
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_pGeometry0->setVisible(true);
                    m_nScriptTimerA = 0;
                    m_nState = 7;
                } else if (m_nState == 7) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 801) {
                        m_nScriptTimerA = 0;
                        m_nState = 8;
                    }
                } else if (m_nState == 8) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 801) {
                        m_pExplosion->start(m_pGeometry6->getPosition(), Vector{0.0f, 0.0f, 0.0f});
                        Globals::sound->play(18, nullptr, nullptr, 0.0f);
                        m_pLevel->field_74->systemSetMatrix(m_pLevel->field_58, &m_pGeometry6->getMatrix());
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_58, true);
                        m_tempVec = m_pGeometry6->getPosition();
                        m_tempVec.x += 4500.0f;
                        m_tempVec.z += 1000.0f;
                        AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, m_tempVec.x, m_tempVec.y, m_tempVec.z);
                        m_pLevel->field_74->systemSetMatrix(m_pLevel->field_5c, &m_matrix);
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_5c, true);
                        m_nScriptTimerA = 0;
                        m_nState = 9;
                    }
                } else if (m_nState == 9) {
                    m_pExplosion->update(delta, nullptr);
                    m_tempVec.x += (float) (2 * delta);
                    AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, m_tempVec.x, m_tempVec.y, m_tempVec.z);
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 8001) {
                        AEGeometry *root = ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry;
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->transform))
                                ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->transform))
                                                 ->animationLength,
                                         false);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->childTransform))
                                ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                                                  root->childTransform))
                                                 ->animationLength,
                                         false);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->parentTransform))
                                ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                                                  root->parentTransform))
                                                 ->animationLength,
                                         false);
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        player->freeze = 0;
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_pGeometry0->setVisible(false);
                        m_nState = 10;
                        m_bCinematicBreak = 0;
                    }
                } else if (m_nState == 10 && ((RadioMessage *) ((*messages)[9]))->isOver()) {
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    player->freeze = 1;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(m_pGeometry1);
                    m_tempVec = m_pGeometry1->getPosition();
                    m_tempVec += Vector{20000.0f, 5000.0f, -20000.0f};
                    m_pCamera->setPosition(m_tempVec);
                    m_nScriptTimerA = 0;
                    m_nState = 11;
                } else if (m_nState == 11) {
                    m_nScriptTimerA += delta;
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f, (float) -delta * 4.0f);
                    if (m_nScriptTimerA >= 4001) {
                        m_tempVec = m_pGeometry1->getPosition();
                        m_tempVec += Vector{-4000.0f, 5000.0f, -6000.0f};
                        m_pExplosion->start(m_tempVec, Vector{0.0f, 0.0f, 0.0f});
                        m_nScriptTimerA = 0;
                        m_nState = 12;
                    }
                } else if (m_nState == 12) {
                    m_nScriptTimerA += delta;
                    m_pExplosion->update(delta, nullptr);
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f, (float) -delta * 4.0f);
                    float rumble = (float) m_nScriptTimerA / 8000.0f;
                    if (!(rumble < 1.0f)) {
                        rumble = 1.0f;
                    }
                    m_pCamera->setRumblePercentage(rumble, 100);
                    if (m_nScriptTimerA >= 8001) {
                        m_pGeometry5->setPosition(m_pGeometry1->getPosition() + Vector{0.0f, 0.0f, -10000.0f});
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                                ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                        Globals::sound->play(160, nullptr, nullptr, 0.0f);
                        m_nScriptTimerA = 0;
                        m_nState = 13;
                    }
                } else if (m_nState == 13) {
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f, (float) -delta * 4.0f);
                    m_nScriptTimerA += delta;
                    AEGeometry *dockModel = m_pGeometry5;
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    dockModel->setDirection(AbyssEngine::AEMath::MatrixGetDir(
                                                    *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()))),
                                            Vector{0.0f, 1.0f, 0.0f});
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry5->transform))
                            ->Update((long long) delta, false);
                    float rumble = (float) m_nScriptTimerA / 10000.0f;
                    if (!(rumble < 1.0f)) {
                        rumble = 1.0f;
                    }
                    m_pCamera->setRumblePercentage(1.0f - rumble, 100);
                    if (m_nScriptTimerA >= 2151) {
                        ((PlayerStation *) (*enemies)[0])->rootGeometry->setVisible(false);
                        if (m_nScriptTimerA >= 10001) {
                            m_pCamera->setRumblePercentage(0.0f, 0);
                            m_pCamera->setLookAtCam(false);
                            m_pCamera->setTarget(player->geometry);
                            player->setComputerControlled(false);
                            player->freeze = 0;
                            m_pHud->visible = 1;
                            m_pRadar->enabled = 1;
                            resetCamera(m_pLevel);
                            m_pLevel->lodManager->forceUpdate(delta, false);
                            m_bCinematicBreak = 0;
                            m_nScriptTimerA = 0;
                            m_nState = 14;
                        }
                    }
                }
                if (m_nState >= 7 && m_nState <= 9) {
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f, (float) -delta * 0.3f);
                    if (m_nState >= 8) {
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry0->transform))
                                ->Update((long long) delta, false);
                        AEGeometry *root = ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry;
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->transform))
                                ->Update((long long) delta, true);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->childTransform))
                                ->Update((long long) delta, true);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->parentTransform))
                                ->Update((long long) delta, true);
                    }
                }
                break;
            }
            case 56: {
                // Friendly ships hold fire until the player's armour is gone.
                if (m_nState == 0) {
                    if (enemies != nullptr) {
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->player->isAlwaysFriend()) {
                                (*enemies)[i]->player->setShootingEnabled(false);
                            }
                        }
                    }
                    m_nState = 1;
                } else if (m_nState == 1 && player->player->getArmorHP() <= 0) {
                    if (enemies != nullptr) {
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->player->isAlwaysFriend()) {
                                (*enemies)[i]->player->setShootingEnabled(true);
                            }
                        }
                    }
                    m_nState = 2;
                }
                break;
            }
            case 125: {
                // Pirate hideout: fly the route in, label the three docking ports, then ambush.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[1]))->isTriggered()) {
                    m_nState++;
                    int routePoints[3] = {-70000, 0, -130000};
                    Route *route = new Route(routePoints, 3);
                    m_pLevel->setPlayerRoute(route);
                    player->setRoute(route);
                    m_nScriptTimerA = 0;
                } else if (m_nState == 1 && player->getRoute() != 0
                           && (uint8_t) ((Route *) (intptr_t) player->getRoute())->getWaypoint(0)->state) {
                    m_nState = 2;
                    for (int i = 8; i <= 10; i++) {
                        PlayerFixedObject *port = (PlayerFixedObject *) (*enemies)[i];
                        port->setName(*Globals::gameText->getText(3212) + String(" ", false) + String(i - 7));
                        (*enemies)[i]->setActive(true);
                        (*enemies)[i]->field_0x74 = 0;
                    }
                }
                if (m_nState == 2) {
                    if (((RadioMessage *) ((*messages)[2]))->isOver()) {
                        int spread = 0;
                        for (int i = 2; i < 6; i++) {
                            (*enemies)[i]->awake();
                            KIPlayer *pirate = (*enemies)[i];
                            Vector playerPos = player->getPosition();
                            int sideX = Globals::rnd->nextInt(100);
                            int offsetY = Globals::rnd->nextInt(1000);
                            int offsetZ = Globals::rnd->nextInt(2000);
                            int sideZ = Globals::rnd->nextInt(100);
                            Vector offset{(float) ((sideX < 50 ? 1 : -1) * spread + 2000), (float) (offsetY + 2000),
                                          (float) (-57000 - (sideZ < 50 ? 1 : -1) * offsetZ)};
                            pirate->setPosition(playerPos + offset);
                            (*enemies)[i]->geometry->setDirection(
                                    AbyssEngine::AEMath::VectorNormalize(player->getPosition() - (*enemies)[i]->getPosition()),
                                    Vector{0.0f, 1.0f, 0.0f});
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
                            (*enemies)[i]->player->setEnemy(player->player);
                            spread += 2000;
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
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        m_pCamera->setLookAtCam(true);
                        player->setVisible(false);
                        player->setFreeze(true);
                        AEGeometry *leader = (*enemies)[4]->geometry;
                        m_pCamera->setTarget(leader);
                        m_tempVec = leader->getPosition();
                        m_tempVec += leader->getDirection() * 4000.0f;
                        m_tempVec += leader->getUpVector() * -500.0f;
                        m_tempVec += leader->getRightVector() * -500.0f;
                        m_pCamera->setPosition(m_tempVec.x, m_tempVec.y, m_tempVec.z);
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 3) {
                    float step = (float) (2 * delta);
                    for (int i = 2; i < 6; i++) {
                        (*enemies)[i]->geometry->moveForward(step);
                    }
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 7001) {
                        for (int i = 2; i < 6; i++) {
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                        }
                        m_nState = 4;
                        player->setVisible(true);
                        player->setFreeze(false);
                        m_pLevel->getPlayer()->setAutoPilot(nullptr);
                        if (!m_pLevel->getPlayer()->isDockedToDockingPoint()) {
                            m_pRadar->lockedEnemy = nullptr;
                            m_pRadar->dockNavPtr = nullptr;
                        }
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pLevel->getPlayer()->dockToDockingPoint(m_pRadar->lockedEnemy, m_pRadar);
                        player->geometry->setDirection(
                                AbyssEngine::AEMath::VectorNormalize((*enemies)[4]->getPosition() - player->getPosition()),
                                Vector{0.0f, 1.0f, 0.0f});
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
                        player->setComputerControlled(false);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                    }
                } else if (m_nState >= 4 && m_nState <= 6 && player->isDockedToDockingPoint() && player->hackingWon()
                           && player->getHackingGameDockIndex() >= 0) {
                    ((PlayerFixedObject *) m_pLevel->getDockingTarget(player->getHackingGameDockIndex()))->setDockingType(0);
                    ((PlayerFixedObject *) m_pLevel->getDockingTarget(player->getHackingGameDockIndex()))
                            ->setName(String("", false));
                    ((KIPlayer *) m_pLevel->getDockingTarget(player->getHackingGameDockIndex()))->field_0x74 = 1;
                    player->deleteHackingGame();
                    m_nState++;
                    if (m_nState == 6) {
                        for (int i = 6; i < 8; i++) {
                            (*enemies)[i]->awake();
                            KIPlayer *guard = (*enemies)[i];
                            guard->setPosition(player->getPosition()
                                               + Vector{(float) (i * 1000 + 19000), (float) (i * 50 + 2000), 25000.0f});
                        }
                    }
                }
                break;
            }
            case 92: {
                // The cloaked pirate trio attacks the station while the player is docked.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[2]))->isTriggered()) {
                    m_nState++;
                }
                if (m_nState == 1) {
                    if (Globals::status->field_178 == 0) {
                        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->setType(4);
                        m_nScriptTimerA = 0;
                        Vector ref = (*enemies)[3]->geometry->getPosition();
                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
                        (*enemies)[1]->setPosition(ref + Vector{-79000.0f, 4900.0f, 7500.0f});
                        (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
                        for (int i = 0; i < 3; i++) {
                            (*enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                            ((PlayerFighter *) (*enemies)[i])->field_0x128 = 0;
                            (*enemies)[i]->field_0x38 = 3;
                            (*enemies)[i]->awake();
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
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        AEGeometry *leader = (*enemies)[0]->geometry;
                        m_pCamera->setTarget(leader);
                        m_tempVec = leader->getPosition();
                        m_dirVec = leader->getDirection();
                        m_tempVec += m_dirVec * 7000.0f;
                        m_dirVec = leader->getRightVector();
                        m_tempVec += m_dirVec * 700.0f;
                        m_dirVec = leader->getUpVector();
                        m_tempVec += m_dirVec * 300.0f;
                        m_pCamera->setPosition(m_tempVec);
                        ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(0);
                        ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setName(String("", false));
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 2) {
                    for (int i = 0; i < 3; i++) {
                        float speed = (i == 0) ? 2.0f : (i == 1 ? 2.08f : 2.05f);
                        (*enemies)[i]->geometry->moveForward(speed * (float) delta);
                    }
                    if (((RadioMessage *) ((*messages)[5]))->isTriggered()) {
                        m_nScriptTimerA = 0;
                        m_nScriptTimerB = 0;
                        m_nState++;
                        AEGeometry *leader = (*enemies)[0]->geometry;
                        m_pCamera->setTarget(leader);
                        long long drift = m_nScriptTimerB;
                        m_tempVec = leader->getPosition();
                        m_dirVec = leader->getDirection();
                        m_tempVec += m_dirVec * (float) (drift + 3900);
                        m_dirVec = leader->getRightVector();
                        m_tempVec += m_dirVec * (float) (drift + 500);
                        m_dirVec = leader->getUpVector();
                        m_tempVec += m_dirVec * (400.0f + (float) drift * 0.004f);
                        m_pCamera->setPosition(m_tempVec);
                    }
                } else if (m_nState == 3) {
                    m_nScriptTimerB = (long long) ((float) delta * 0.05f + (float) m_nScriptTimerB);
                    AEGeometry *leader = (*enemies)[0]->geometry;
                    m_pCamera->setTarget(leader);
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                    m_dirVec = leader->getDirection();
                    m_tempVec += m_dirVec * ((float) delta * 1.7f);
                    m_dirVec = leader->getRightVector();
                    m_tempVec += m_dirVec * ((float) -delta * 0.03f);
                    m_dirVec = leader->getUpVector();
                    m_tempVec += m_dirVec * ((float) delta * 0.05f * 0.004f);
                    m_pCamera->setFixed(true);
                    m_pCamera->setLocal(AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec));
                    for (int i = 0; i < 3; i++) {
                        float speed = (i == 0) ? 2.0f : (i == 1 ? 2.3f : 2.2f);
                        (*enemies)[i]->geometry->moveForward(speed * (float) delta);
                    }
                    if (((RadioMessage *) ((*messages)[5]))->isOver()) {
                        for (int i = 0; i < 3; i++) {
                            ((PlayerFighter *) (*enemies)[i])->setCloakingPossible(true);
                            ((PlayerFighter *) (*enemies)[i])->cloak(18000, false);
                        }
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 4) {
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB = (long long) ((float) delta * 0.05f + (float) m_nScriptTimerB);
                    AEGeometry *leader = (*enemies)[0]->geometry;
                    m_pCamera->setTarget(leader);
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                    m_dirVec = leader->getDirection();
                    m_tempVec += m_dirVec * ((float) delta * 1.7f);
                    m_dirVec = leader->getRightVector();
                    m_tempVec += m_dirVec * ((float) -delta * 0.03f);
                    m_dirVec = leader->getUpVector();
                    m_tempVec += m_dirVec * ((float) delta * 0.05f * 0.004f);
                    m_pCamera->setFixed(true);
                    m_pCamera->setLocal(AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec));
                    for (int i = 0; i < 3; i++) {
                        float speed = (i == 0) ? 2.0f : (i == 1 ? 2.3f : 2.2f);
                        (*enemies)[i]->geometry->moveForward(speed * (float) delta);
                    }
                    if (m_nScriptTimerA > 2000) {
                        m_nState++;
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 5) {
                    m_nScriptTimerA += delta;
                    for (int i = 0; i < 3; i++) {
                        float speed = (i == 0) ? 6.0f : (i == 1 ? 4.6f : 4.4f);
                        (*enemies)[i]->geometry->moveForward(speed * (float) delta);
                    }
                    if (m_nScriptTimerA >= 2001) {
                        for (int i = 0; i < 3; i++) {
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                            ((PlayerFighter *) (*enemies)[i])->field_0x128 = 50000;
                            (*enemies)[i]->field_0x38 = 4;
                        }
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget(player->geometry);
                        bool docked = player->isDockedToDockingPoint();
                        m_pCamera->setLookAtCam(false);
                        if (docked) {
                            lookBehind();
                            m_pCamera->setRotationAroundTarget(true);
                            player->setFreeLookMode(true);
                            if (field_0xab) {
                                player->setTurretMode(true);
                            }
                        } else {
                            m_pCamera->setPosition(m_dirVec);
                        }
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 6) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 3001) {
                        m_nState = 7;
                    }
                } else if (m_nState == 7 && ((RadioMessage *) ((*messages)[8]))->isTriggered()) {
                    if (player->isDockedToDockingPoint()) {
                        m_pLevel->getPlayer()->setAutoPilot(nullptr);
                        m_pRadar->lockedEnemy = nullptr;
                        m_pRadar->dockNavPtr = nullptr;
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
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->player->setVulnerable(false);
                    player->setComputerControlled(true);
                    player->setCollide(false);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(player->geometry);
                    m_tempVec = player->geometry->getPosition();
                    m_dirVec = player->geometry->getDirection();
                    m_tempVec += m_dirVec * 7000.0f;
                    m_dirVec = player->geometry->getRightVector();
                    m_tempVec += m_dirVec * 700.0f;
                    m_pCamera->setPosition(m_tempVec);
                    m_nScriptTimerA = 0;
                    m_nState++;
                } else if (m_nState == 8) {
                    m_nScriptTimerA += delta;
                    (*enemies)[3]->geometry->moveForward((float) (2 * delta));
                    if (m_nScriptTimerA >= 8001) {
                        m_pCamera->setTarget((*enemies)[3]->geometry);
                        m_tempVec = (*enemies)[3]->geometry->getPosition();
                        m_dirVec = (*enemies)[3]->geometry->getDirection();
                        m_tempVec += m_dirVec * 10000.0f;
                        m_dirVec = (*enemies)[3]->geometry->getRightVector();
                        m_tempVec += m_dirVec * 4800.0f;
                        m_pCamera->setPosition(m_tempVec);
                        m_nState++;
                        m_nScriptTimerA = 0;
                        m_tempVec = Vector{1.0f, 1.0f, 1.0f};
                    }
                } else if (m_nState == 9 || m_nState == 10) {
                    Vector pos = (*enemies)[3]->geometry->getPosition();
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(2249, &pos, &zero, false);
                    m_nScriptTimerA += delta;
                    m_tempVec.x += (float) delta * 0.006f;
                    (*enemies)[3]->geometry->moveForward(m_tempVec.x * (float) delta);
                    if (m_nScriptTimerA >= 2001) {
                        if (m_nState == 9) {
                            m_nState = 10;
                        }
                        if (m_nScriptTimerA >= 7001) {
                            m_pCamera->setFixed(false);
                            m_pCamera->setTarget(player->geometry);
                            m_pCamera->setLookAtCam(false);
                            m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
                            m_pHud->visible = 1;
                            m_pRadar->enabled = 1;
                            resetCamera(m_pLevel);
                            m_pLevel->lodManager->forceUpdate(delta, false);
                            player->setComputerControlled(false);
                            m_bCinematicBreak = 0;
                            player->player->setVulnerable(true);
                            (*enemies)[3]->setActive(false);
                            (*enemies)[3]->setVisible(false);
                            m_nState++;
                        }
                    }
                } else if (m_nState == 11) {
                    m_tempVec.x += (float) delta * 0.02f;
                    (*enemies)[3]->geometry->moveForward(m_tempVec.x * (float) delta);
                }
                break;
            }
            case 157: {
                // Supernova finale: the alien station escapes, the sun collapses and the player is
                // thrown out to the next system. The escort ship slots depend on the device tier.
                int stationIndex = 12;
                int chaserIndex = 11;
                int fighterIndex = 10;
                if (!Globals::isRunningHDonWeakDevice) {
                    stationIndex = 23;
                    chaserIndex = 22;
                    fighterIndex = 21;
                }
                if (m_pGeometry2->isVisible()) {
                    m_pGeometry2->setPosition(((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry2->transform))
                            ->Update((long long) delta, true);
                }
                if (m_pGeometry3->isVisible()) {
                    m_pGeometry3->setPosition(((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry3->transform))
                            ->Update((long long) delta, true);
                }
                if (m_nState == 0) {
                    m_pGeometry4 = new AEGeometry(Globals::Canvas);
                    m_pGeometry4->setPosition(Vector{-120000.0f, 0.0f, 5000.0f});
                    m_pGeometry4->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
                    m_nState = 1;
                }
                if (m_nState == 1 && ((RadioMessage *) ((*messages)[0]))->isOver()) {
                    if (player->isInRocketControl()) {
                        resetCamera(m_pLevel);
                        player->setRocketControl(nullptr, nullptr);
                        player->killLiberator();
                        m_pCamera->setRumblePercentage(0.0f, 0);
                    }
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(m_pGeometry4);
                    m_pCamera->setPosition(m_pGeometry4->getPosition());
                    m_pCamera->translate(-10000.0f, 5000.0f, 25000.0f);
                    player->player->setVulnerable(false);
                    player->setFreeze(true);
                    m_nScriptTimerA = 0;
                    m_nState = 2;
                } else if (m_nState == 2) {
                    m_nScriptTimerA += delta;
                    m_pGeometry4->translate(0.0f, 0.0f, (float) (delta * 3));
                    m_pCamera->translate(0.0f, 0.0f, (float) delta);
                    if (m_nScriptTimerA >= 16001) {
                        m_nScriptTimerA = 0;
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            KIPlayer *ship = (*enemies)[i];
                            if (ship->shipGroupFlag == 49 || ship->shipGroupFlag == 44) {
                                ship->setVisible(true);
                                ship->setActive(true);
                            }
                        }
                        m_pGeometry4->setPosition((*enemies)[stationIndex]->getPosition());
                        m_tempVec = (*enemies)[fighterIndex]->getPosition();
                        m_tempVec += Vector{-1500.0f, -5000.0f, 2500.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_nState = 3;
                    }
                } else if (m_nState == 3) {
                    m_nScriptTimerA += delta;
                    float lift = (float) m_nScriptTimerA / -15000.0f + 1.0f;
                    if (lift < 0.0f) {
                        lift = 0.0f;
                    }
                    m_pCamera->translate(0.0f, lift * (float) delta, 0.0f);
                    if (m_nScriptTimerA >= 8001) {
                        m_nScriptTimerB = 0;
                        m_nState = 4;
                    }
                } else if (m_nState == 4) {
                    if (m_nScriptTimerB <= 6999) {
                        m_nScriptTimerB += delta;
                    }
                    m_nScriptTimerA += delta;
                    float lift = (float) m_nScriptTimerA / -15000.0f + 1.0f;
                    if (lift < 0.0f) {
                        lift = 0.0f;
                    }
                    m_pCamera->translate(0.0f, lift * (float) delta, 0.0f);
                    for (unsigned int i = 0; i < enemies->count; i++) {
                        KIPlayer *ship = (*enemies)[i];
                        if (ship->shipGroupFlag == 49 || ship->shipGroupFlag == 44) {
                            ship->geometry->moveForward((float) m_nScriptTimerB * 0.02f);
                        }
                    }
                    if (((RadioMessage *) ((*messages)[3]))->isOver() && m_nScriptTimerA >= 18001) {
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        player->setFreeze(false);
                        player->setVisible(true);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        player->player->setVulnerable(true);
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            KIPlayer *ship = (*enemies)[i];
                            if (ship->shipGroupFlag == 49 || ship->shipGroupFlag == 44) {
                                ship->setVisible(true);
                                ship->setActive(true);
                                ((PlayerFighter *) ship)->setAIDisabled(false);
                                Vector offset;
                                if (ship->shipGroupFlag == 44) {
                                    ((PlayerFighter *) ship)->setCloakingPossible(true);
                                    offset.y = (float) (Globals::rnd->nextInt(12000) - 6000);
                                    offset.z = (float) (Globals::rnd->nextInt(30000) - 87000);
                                    offset.x = 120000.0f;
                                } else {
                                    offset.y = (float) (Globals::rnd->nextInt(6000) - 3000);
                                    offset.z = (float) (Globals::rnd->nextInt(6000) - 7000);
                                    offset.x = 20000.0f;
                                }
                                ship->translate(offset);
                            }
                        }
                        m_nState = 5;
                    }
                } else if (m_nState == 5) {
                    int destroyed;
                    if (Globals::isRunningHDonWeakDevice) {
                        destroyed = 5;
                        for (int i = 6; i < 12; i++) {
                            destroyed += (*enemies)[i]->isDead();
                        }
                    } else {
                        destroyed = 0;
                        for (int i = 11; i < 22; i++) {
                            destroyed += (*enemies)[i]->isDead();
                        }
                    }
                    m_nScriptTimerB += delta;
                    if (m_nScriptTimerB >= 25001) {
                        m_nScriptTimerB = 0;
                        PlayerFighter *fighter = (PlayerFighter *) (*enemies)[fighterIndex];
                        fighter->field_0x140 = !fighter->field_0x140;
                    }
                    if ((destroyed >= 9 && m_nScriptTime > 200000)
                        || (*enemies)[fighterIndex]->player->getHitpoints()
                           < (*enemies)[fighterIndex]->player->getMaxHitpoints() / 4) {
                        if (player->isInRocketControl()) {
                            player->setRocketControl(nullptr, nullptr);
                            player->killLiberator();
                            m_pCamera->setRumblePercentage(0.0f, 0);
                        }
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        player->player->setVulnerable(false);
                        player->setFreeze(true);
                        m_pCamera->setLookAtCam(true);
                        KIPlayer *chaser = (*enemies)[chaserIndex];
                        m_pCamera->setTarget(chaser->geometry);
                        m_tempVec = (*enemies)[stationIndex]->getPosition();
                        m_tempVec += Vector{45000.0f, 2000.0f, 11000.0f};
                        m_pCamera->setPosition(m_tempVec);
                        chaser->setPosition(m_tempVec + Vector{15000.0f, -200.0f, 1000.0f});
                        chaser->geometry->setDirection(
                                AbyssEngine::AEMath::VectorNormalize((*enemies)[stationIndex]->getPosition()
                                                                     - chaser->getPosition()),
                                Vector{0.0f, 1.0f, 0.0f});
                        chaser->setVisible(true);
                        chaser->setActive(true);
                        (*enemies)[fighterIndex]->player->setVulnerable(false);
                        ((PlayerFighter *) (*enemies)[fighterIndex])->field_0x140 = 0;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nScriptTimerA = 0;
                        m_nState = 6;
                    }
                } else if (m_nState == 6) {
                    m_nScriptTimerA += delta;
                    KIPlayer *chaser = (*enemies)[chaserIndex];
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    if (chaser->getPosition().x > (*enemies)[stationIndex]->getPosition().x) {
                        chaser->geometry->moveForward((float) (delta * 3));
                    }
                    if (((RadioMessage *) ((*messages)[10]))->isTriggered()) {
                        chaser->setVisible(false);
                        chaser->setActive(false);
                        chaser->setPosition(50000.0f, 50000.0f, 50000.0f);
                        m_pCamera->setTarget(((PlayerStation *) (*enemies)[stationIndex])->rootGeometry);
                        m_nScriptTimerA = 0;
                        m_nState = 7;
                    }
                } else if (m_nState == 7 && ((RadioMessage *) ((*messages)[10]))->isOver()) {
                    PlayerStation *station = (PlayerStation *) (*enemies)[stationIndex];
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Vector pos = station->getPosition();
                    Globals::sound->updateEvent3DAttributes(2251, &pos, &zero, false);
                    if (m_nScriptTimerA <= 9999) {
                        m_nScriptTimerA += delta;
                    }
                    station->rootGeometry->moveForward((float) m_nScriptTimerA / 10000.0f * (float) (-10 * delta));
                    if (station->rootGeometry->getPosition().z > 100000.0f) {
                        KIPlayer *fighter = (*enemies)[fighterIndex];
                        fighter->setPosition(*m_pCamera->getPosition() + Vector{4000.0f, -600.0f, -2000.0f});
                        m_tempVec = AbyssEngine::AEMath::VectorNormalize(
                                (*enemies)[stationIndex]->getPosition() + Vector{0.0f, 0.0f, 40000.0f}
                                - fighter->getPosition());
                        fighter->geometry->setDirection(m_tempVec, Vector{0.0f, 1.0f, 0.0f});
                        ((PlayerFighter *) fighter)->setAIDisabled(true);
                        fighter->setVisible(true);
                        fighter->setActive(true);
                        player->setComputerControlled(true);
                        player->geometry->setDirection(m_tempVec, Vector{0.0f, 1.0f, 0.0f});
                        player->setSpeed(4.0f);
                        player->setFreeze(false);
                        player->setPosition(*m_pCamera->getPosition() + Vector{5000.0f, 300.0f, -14000.0f});
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_pCamera->setTarget(station->rootGeometry);
                        m_nScriptTimerA = 0;
                        m_nState = 8;
                    }
                } else if (m_nState == 8) {
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Vector pos = (*enemies)[stationIndex]->getPosition();
                    Globals::sound->updateEvent3DAttributes(2251, &pos, &zero, false);
                    ((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->moveForward((float) (-10 * delta));
                    KIPlayer *fighter = (*enemies)[fighterIndex];
                    fighter->geometry->moveForward((float) (4 * delta));
                    fighter->player->shoot(0, (long long) delta, false);
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 12001 && ((RadioMessage *) ((*messages)[12]))->isOver()) {
                        m_tempVec = (*enemies)[stationIndex]->getPosition();
                        m_pCamera->setPosition(m_tempVec + Vector{35000.0f, 0.0f, 70000.0f});
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        fighter->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
                        player->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
                        fighter->setVisible(false);
                        fighter->setActive(false);
                        player->setSpeed(10.0f);
                        m_nScriptTimerA = 0;
                        m_nScriptTimerB = 0;
                        m_nState = 9;
                    }
                } else if (m_nState == 9) {
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Vector pos = (*enemies)[stationIndex]->getPosition();
                    Globals::sound->updateEvent3DAttributes(2251, &pos, &zero, false);
                    KIPlayer *fighter = (*enemies)[fighterIndex];
                    ((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->moveForward((float) (-10 * delta));
                    fighter->geometry->moveForward((float) (2 * delta));
                    fighter->player->shoot(0, (long long) delta, false);
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB += delta;
                    if (m_nScriptTimerB < 9000 && m_nScriptTimerB + delta >= 9000) {
                        fighter->setPosition(*m_pCamera->getPosition() + Vector{4000.0f, -600.0f, -2000.0f});
                        fighter->setVisible(true);
                        fighter->setActive(true);
                    }
                    m_tempVec = AbyssEngine::AEMath::VectorNormalize((*enemies)[stationIndex]->getPosition()
                                                                     - fighter->getPosition());
                    fighter->geometry->setDirection(m_tempVec, Vector{0.0f, 1.0f, 0.0f});
                    if (((RadioMessage *) ((*messages)[13]))->isOver() && m_pExplosion == nullptr) {
                        m_pExplosion = new Explosion(0);
                        m_pExplosion->addFireStreaks();
                        m_pExplosion->setScaling(5.0f);
                        m_pExplosion->start((*enemies)[stationIndex]->getPosition() + Vector{-2000.0f, 1000.0f, -8000.0f},
                                            Vector{0.0f, 0.0f, 0.0f});
                        Globals::sound->play(2244, nullptr, nullptr, 0.0f);
                        m_nScriptTimerA = 0;
                        m_nFlashCountdown = 300;
                    }
                    if (m_nFlashCountdown > 0) {
                        m_nFlashCountdown -= delta;
                        if (m_nFlashCountdown <= 0) {
                            m_nFlashCountdown = 0;
                            m_pGeometry2->setPosition(
                                    ((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());
                            m_pGeometry2->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
                            m_pGeometry2->setVisible(true);
                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry2->transform))
                                    ->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
                        }
                    }
                    if (m_pExplosion != nullptr) {
                        m_pExplosion->update(delta, nullptr);
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA >= 6001) {
                            m_nScriptTimerA = -2000;
                            Globals::sound->play(2244, nullptr, nullptr, 0.0f);
                            m_pExplosion->start((*enemies)[stationIndex]->getPosition()
                                                + Vector{-6000.0f, 2000.0f, -10000.0f},
                                                Vector{0.0f, 0.0f, 0.0f});
                        }
                    }
                    if (((RadioMessage *) ((*messages)[14]))->isOver()) {
                        m_tempVec = (*enemies)[stationIndex]->getPosition();
                        m_pCamera->setPosition(m_tempVec + Vector{50000.0f, 0.0f, 70000.0f});
                        m_nScriptTimerA = 0;
                        m_nScriptTimerB = 0;
                        fighter->setVisible(false);
                        player->setVisible(false);
                        m_pGeometry3->setPosition(((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());
                        m_pGeometry3->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
                        m_pGeometry3->setVisible(true);
                        m_pGeometry2->setVisible(false);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry3->transform))
                                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                        m_nState = 10;
                    }
                } else if (m_nState == 10) {
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Vector pos = (*enemies)[stationIndex]->getPosition();
                    Globals::sound->updateEvent3DAttributes(2251, &pos, &zero, false);
                    m_nScriptTimerB += delta;
                    m_pCamera->setRumblePercentage(0.0f, 0);
                    if (m_nScriptTimerB > 2000 && m_nScriptTimerB < 9000 && m_nScriptTimerB + delta >= 9000) {
                        m_pExplosion->setScaling(10.0f);
                        m_pExplosion->start((*enemies)[stationIndex]->getPosition()
                                            - m_pLevel->getStarSystem()->getLightDirection() * 3000.0f,
                                            Vector{0.0f, 0.0f, 0.0f});
                        Globals::sound->play(2243, nullptr, nullptr, 0.0f);
                        m_pCamera->setRumblePercentage(100.0f, 30);
                    }
                    m_pExplosion->update(delta, nullptr);
                    m_pCamera->translate((float) (-2 * delta), 0.0f, (float) -delta * 0.33f);
                    if (((RadioMessage *) ((*messages)[15]))->isOver()) {
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA >= 3001) {
                            m_pGeometry1->setPosition(
                                    ((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());
                            m_pGeometry1->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                                    ->SetVisible(false);
                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                                    ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                            m_nScriptTimerA = 0;
                            m_nState++;
                        }
                    }
                } else if (m_nState == 11) {
                    m_nScriptTimerA += delta;
                    m_pCamera->translate((float) (-3 * delta), 0.0f, (float) -delta * 0.33f);
                    if (m_nScriptTimerA >= 6001) {
                        if (m_nScriptTimerA - delta <= 6000) {
                            Globals::sound->play(2247, nullptr, nullptr, 0.0f);
                        }
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                                ->SetVisible(true);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                                ->Update((long long) delta, true);
                        if (m_nScriptTimerA >= 10001) {
                            m_pGeometry4->setPosition((*enemies)[stationIndex]->getPosition());
                            m_pGeometry4->setDirection(m_pLevel->getStarSystem()->getLightDirection(),
                                                       Vector{0.0f, 1.0f, 0.0f});
                            m_pCamera->setTarget(m_pGeometry4);
                            m_nScriptTimerA = 0;
                            (*enemies)[fighterIndex]->setVisible(false);
                            (*enemies)[fighterIndex]->setActive(false);
                            m_nState++;
                        }
                    }
                } else if (m_nState == 12) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA < 3000) {
                        m_pCamera->translate((float) (-20 * delta), 0.0f, (float) (-20 * delta));
                    } else if (m_nScriptTimerA < 7000) {
                        m_pCamera->translate((float) (-3 * delta), 0.0f, (float) (-10 * delta));
                    }
                    if (m_nScriptTimerA < 5000) {
                        m_pGeometry4->moveForward((float) (35 * delta));
                    }
                    if (m_nScriptTimerA < 6000) {
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                                ->Update((long long) delta, true);
                    }
                    m_pCamera->setRumblePercentage(100.0f, (int) ((float) m_nScriptTimerA / 6500.0f * 30.0f));
                    if (m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000) {
                        m_pLevel->switchSkyboxForSupernovaReversal();
                    }
                    if (m_nScriptTimerA >= 5500 && m_nScriptTimerA - delta < 5500) {
                        m_pExplosion->setScaling(25.0f);
                        m_pExplosion->start((*enemies)[stationIndex]->getPosition() + Vector{0.0f, 0.0f, -12000.0f}
                                            - m_pLevel->getStarSystem()->getLightDirection() * 3000.0f,
                                            Vector{0.0f, 0.0f, 0.0f});
                    }
                    if (m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700) {
                        m_pGeometry1->setVisible(false);
                        (*enemies)[stationIndex]->setVisible(false);
                        Globals::layout->startFade(true, -1, 6600 - (int) m_nScriptTimerA);
                        Globals::sound->play(2248, nullptr, nullptr, 0.0f);
                    }
                    if (m_nScriptTimerA >= 5500) {
                        m_pExplosion->update(delta, nullptr);
                    }
                    if (m_nScriptTimerA > 5000 && m_nScriptTimerA < 6000) {
                        AEGeometry *planet = (*(Array<AEGeometry *> *) m_pLevel->getStarSystem()->getPlanets())[0];
                        if (m_nScriptTimerA < 5500) {
                            planet->setScaling(planet->getScaling() * 0.8f);
                        } else {
                            planet->setScaling(planet->getScaling() * 1.7f);
                        }
                    }
                    if (m_nScriptTimerA >= 6500) {
                        m_nState++;
                        Globals::status->nextCampaignMission(true);
                        Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(111));
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(111));
                        Globals::status->field_64 = player->player->getHitpoints();
                        Globals::status->field_5c = player->player->getShieldHP();
                        Globals::status->field_60 = player->player->getArmorHP();
                        Globals::status->field_68 = player->player->getGammaHP();
                        Globals::status->field_f4 = player->getCurrentSecondaryWeaponIndex();
                        player->player->setGammaHP(100);
                        Globals::switch_to_target_setting = 1;
                        Level::initStreamOutPosition = 1;
                        Globals::appManager->SetCurrentApplicationModule(2);
                    }
                }
                break;
            }
            case 40: {
                // Escort the transport to the wormhole; the pirates jump in behind it.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[3]))->isTriggered()) {
                    ((PlayerFixedObject *) (*enemies)[0])->setMoving(true);
                    KIPlayer *transport = (*enemies)[0];
                    transport->setPosition((float) m_pLevel->getFriendRoute()->getWaypoint(0)->x,
                                           (float) m_pLevel->getFriendRoute()->getWaypoint(0)->y,
                                           (float) m_pLevel->getFriendRoute()->getWaypoint(0)->z);
                    (*enemies)[0]->awake();
                    (*enemies)[0]->setVisible(true);
                    (*enemies)[0]->shipGroup = 1;
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->player->setVulnerable(false);
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setPosition((*enemies)[0]->getPosition());
                    m_pCamera->translate(-7000.0f, 500.0f, 17000.0f);
                    m_pCamera->setTarget((*enemies)[0]->geometry);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 1;
                } else if (m_nState == 1) {
                    m_pCamera->translate(0.0f, 0.0f, (float) (-2 * delta));
                    if (((RadioMessage *) ((*messages)[4]))->isOver()) {
                        m_pCamera->setTarget(player->geometry);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        m_bCinematicBreak = 0;
                        m_pCamera->setLookAtCam(false);
                        player->setComputerControlled(false);
                        player->player->setVulnerable(true);
                        m_nState = 2;
                    }
                } else if (m_nState == 2 && (*enemies)[0]->getPosition().z >= 90000.0f) {
                    m_nState = 3;
                    for (unsigned int i = enemies->count - 4; i < enemies->count; i++) {
                        KIPlayer *pirate = (*enemies)[i];
                        pirate->setPosition(
                                (*m_pLevel->getLandmarks())[3]->getPosition().x - 10000.0f
                                + (float) Globals::rnd->nextInt(20000),
                                (*m_pLevel->getLandmarks())[3]->getPosition().y - 10000.0f
                                + (float) Globals::rnd->nextInt(20000),
                                (*m_pLevel->getLandmarks())[3]->getPosition().z - 10000.0f
                                + (float) Globals::rnd->nextInt(20000));
                        (*enemies)[i]->awake();
                        (*enemies)[i]->awake();
                    }
                } else if (m_nState == 3
                           && (*enemies)[0]->getPosition().z >= (*m_pLevel->getLandmarks())[3]->getPosition().z) {
                    m_nState = 4;
                } else if (m_nState == 4) {
                    KIPlayer *transport = (*enemies)[0];
                    ((PlayerFixedObject *) transport)->moveForward(
                            (int) (transport->getPosition().z - 200000.0f - (float) delta));
                    if ((*enemies)[0]->getPosition().z > 500000.0f) {
                        (*enemies)[0]->setPosition(0.0f, 0.0f, -200000.0f);
                        (*enemies)[0]->setActive(false);
                        m_nState = 5;
                    }
                }
                break;
            }
            case 102: {
                // The Valkyrie escort battle: the alien fleet arrives, a wing of fighters engages.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[0]))->isTriggered()) {
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
                    player->setFreeze(true);
                    player->setVisible(false);
                    player->player->setVulnerable(false);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget((*enemies)[0]->geometry);
                    m_pCamera->setPosition((*enemies)[0]->getPosition() + Vector{9000.0f, -7000.0f, 40000.0f});
                    Globals::sound->stop(Globals::sound->currentMusicEvent);
                    Globals::sound->play(2240, nullptr, nullptr, 0.0f);
                    m_nScriptTimerA = 0;
                    m_nState++;
                } else if (m_nState == 1) {
                    m_pCamera->translate((float) delta * 0.6f, (float) delta * 0.8f, (float) -delta * 2.5f);
                    if (((RadioMessage *) ((*messages)[2]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 2001) {
                        m_pLevel->getPlayer()->resetGunDelay();
                        player->setFreeze(false);
                        player->setVisible(true);
                        player->player->setVulnerable(true);
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        m_nScriptTimerA = 0;
                        Globals::sound->stop(Globals::sound->currentMusicEvent);
                        Globals::sound->play(2241, nullptr, nullptr, 0.0f);
                        m_nState++;
                    }
                } else if (m_nState == 2) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 30001) {
                        Vector playerPos = player->getPosition();
                        int leader = -1;
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            KIPlayer *ship = (*enemies)[i];
                            if (ship->shipGroup == 10) {
                                ship->awake();
                                if (leader == -1) {
                                    leader = i;
                                }
                                if (i == (unsigned int) leader) {
                                    KIPlayer *fighter = (*enemies)[i];
                                    fighter->setPosition((*enemies)[0]->getPosition() + Vector{-82000.0f, 2000.0f, 0.0f});
                                    ((PlayerFighter *) (*enemies)[i])->cloak(1000, true);
                                } else if (i == (unsigned int) (leader + 1)) {
                                    KIPlayer *fighter = (*enemies)[i];
                                    fighter->setPosition((*enemies)[leader]->getPosition() + Vector{-2000.0f, -800.0f, -2000.0f});
                                    ((PlayerFighter *) (*enemies)[i])->cloak(1000, true);
                                } else {
                                    KIPlayer *fighter = (*enemies)[i];
                                    int dx = Globals::rnd->nextInt(10000) + 35000;
                                    int x = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dx;
                                    int dy = Globals::rnd->nextInt(10000);
                                    int dz = Globals::rnd->nextInt(10000) + 35000;
                                    int z = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dz;
                                    fighter->setPosition(playerPos + Vector{(float) x, (float) (dy - 5000), (float) z});
                                }
                                (*enemies)[i]->player->setEnemies(nullptr);
                                int target = Globals::rnd->nextInt(5);
                                (*enemies)[i]->player->setEnemy(target == 4 ? player->player : (*enemies)[target + 2]->player);
                                (*enemies)[i]->awake();
                                (*enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                                ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
                            } else if (ship->shipGroupFlag == 51) {
                                Array<Player *> *targets = new Array<Player *>();
                                ArrayAdd((*enemies)[0]->player, *targets);
                                ArrayAdd((*enemies)[1]->player, *targets);
                                (*enemies)[i]->player->setEnemies(targets);
                            }
                        }
                        player->setTurretMode(false);
                        player->setFreeze(true);
                        player->setVisible(false);
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
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        AEGeometry *leaderShip = (*enemies)[leader]->geometry;
                        m_pCamera->setTarget(leaderShip);
                        m_tempVec = leaderShip->getPosition();
                        m_dirVec = leaderShip->getDirection();
                        m_tempVec += m_dirVec * 4500.0f;
                        m_dirVec = leaderShip->getRightVector();
                        m_tempVec += m_dirVec * 600.0f;
                        m_dirVec = leaderShip->getUpVector();
                        m_tempVec += m_dirVec * 400.0f;
                        m_pCamera->setPosition(m_tempVec);
                        m_nScriptTimerA = 0;
                        m_nState = 3;
                    }
                } else if (m_nState == 3) {
                    float step = (float) delta;
                    float drift = step * 0.05f;
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB = (long long) (drift + (float) m_nScriptTimerB);
                    float leaderSpeed = (float) ((double) delta * 2.3);
                    float wingSpeed = (float) ((double) delta * 2.2);
                    int leader = -1;
                    for (unsigned int i = 0; i < enemies->count; i++) {
                        KIPlayer *ship = (*enemies)[i];
                        if (ship->shipGroup == 10) {
                            if (leader == -1) {
                                leader = i;
                            }
                            if (i == (unsigned int) leader) {
                                ship->geometry->moveForward(leaderSpeed);
                            } else {
                                ship->geometry->moveForward(wingSpeed);
                            }
                        }
                    }
                    AEGeometry *leaderShip = (*enemies)[leader]->geometry;
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                    m_dirVec = leaderShip->getDirection();
                    m_tempVec += m_dirVec * (step * 2.04f);
                    m_dirVec = leaderShip->getRightVector();
                    m_tempVec += m_dirVec * (step * 0.04f);
                    m_dirVec = leaderShip->getUpVector();
                    m_tempVec += m_dirVec * (drift * 0.004f);
                    m_pCamera->setFixed(true);
                    AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec);
                    m_pCamera->setLocal(local);
                    if (m_nScriptTimerA >= 10001) {
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->shipGroup == 10) {
                                ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                            }
                        }
                        player->setFreeze(false);
                        player->setVisible(true);
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        m_nState++;
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 4) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA > 60000) {
                        Vector playerPos = player->getPosition();
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->shipGroup == 10 && (*enemies)[i]->isDead()) {
                                (*enemies)[i]->revive();
                                (*enemies)[i]->awake();
                                KIPlayer *fighter = (*enemies)[i];
                                int dx = Globals::rnd->nextInt(10000) + 35000;
                                int x = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dx;
                                int dy = Globals::rnd->nextInt(10000);
                                int dz = Globals::rnd->nextInt(10000) + 35000;
                                int z = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dz;
                                fighter->setPosition(playerPos + Vector{(float) x, (float) (dy - 5000), (float) z});
                            }
                        }
                        m_nScriptTimerA = 0;
                    }
                    for (unsigned int i = 0; i < enemies->count; i++) {
                        if ((*enemies)[i]->shipGroup == 10 && (*enemies)[i]->player->getEnemy(0)->isDead()) {
                            int target = -1;
                            for (unsigned int k = 2; k <= 5 && target == -1; k++) {
                                if (!(*enemies)[k]->isDead()) {
                                    target = k;
                                }
                            }
                            (*enemies)[i]->player->setEnemy(target == -1 ? player->player : (*enemies)[target]->player);
                        }
                    }
                    if (((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() < 10) {
                        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->setType(4);
                        m_nState++;
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
                        player->setFreeze(true);
                        player->setVisible(false);
                        player->player->setVulnerable(false);
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        m_pCamera->setLookAtCam(true);
                        m_pCamera->setTarget((*enemies)[0]->geometry);
                        m_pCamera->setPosition((*enemies)[0]->getPosition() + Vector{9000.0f, -7000.0f, 40000.0f});
                        m_nScriptTimerA = 0;
                        for (int k = 2; k != 10; k++) {
                            (*enemies)[k]->setPosition(1000000.0f, 1000000.0f, 1000000.0f);
                            (*enemies)[k]->setActive(false);
                            (*enemies)[k]->setVisible(false);
                        }
                    }
                } else if (m_nState == 5) {
                    (*enemies)[0]->geometry->moveForward((float) (6 * delta));
                    m_pCamera->translate((float) delta * 0.4f, (float) delta * 0.1f, (float) -delta * 1.8f);
                    m_nScriptTimerA += delta;
                    Vector pos = (*enemies)[0]->geometry->getPosition();
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(2250, &pos, &zero, false);
                    if (m_nScriptTimerA >= 8001) {
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 6) {
                    Vector pos = (*enemies)[0]->geometry->getPosition();
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(2250, &pos, &zero, false);
                    m_nScriptTimerA += delta;
                    (*enemies)[0]->geometry->moveForward((float) (200 * delta));
                    if (m_nScriptTimerA >= 4001) {
                        m_pLevel->getPlayer()->resetGunDelay();
                        player->setFreeze(false);
                        player->setVisible(true);
                        player->player->setVulnerable(true);
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setPosition(m_dirVec);
                        m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                }
                break;
            }
            case 154: {
                // The pirate ambush at the asteroid station: a long camera ride along the player ship.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[0]))->isTriggered()) {
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
                    if (field_0xaa) {
                        player->setAutoTurret(false);
                    }
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget((*m_pLevel->getEnemies())[2]->geometry);
                    m_tempVec = (*m_pLevel->getEnemies())[2]->geometry->getPosition();
                    m_tempVec += Vector{300.0f, 300.0f, 5800.0f};
                    m_pCamera->setPosition(m_tempVec);
                    ((PlayerFighter *) (*enemies)[0])->setAIDisabled(true);
                    for (unsigned int i = 0; i < enemies->count; i++) {
                        if ((*enemies)[i]->shipGroup == 9) {
                            AEGeometry *ship = (*enemies)[i]->geometry;
                            ship->setDirection(AbyssEngine::AEMath::VectorNormalize(
                                                       m_pLevel->getPlayer()->getPosition() - (*enemies)[i]->getPosition()),
                                               Vector{0.0f, 1.0f, 0.0f});
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
                        }
                    }
                    (*enemies)[1]->field_0x70 = 1;
                    m_nScriptTimerA = 0;
                    m_nState++;
                    m_pLevel->lodManager->forceUpdate(delta, false);
                } else if (m_nState == 1) {
                    player->setSpeed(0.0f);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    float step = (float) delta * 0.5f;
                    for (unsigned int i = 0; i < enemies->count; i++) {
                        if ((*enemies)[i]->shipGroup == 9) {
                            (*enemies)[i]->geometry->moveForward(step);
                        }
                    }
                    if (((RadioMessage *) ((*messages)[1]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 1501) {
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->shipGroup == 9) {
                                (*enemies)[i]->setActive(false);
                                (*enemies)[i]->setVisible(false);
                            }
                        }
                        m_pCamera->setTarget(player->geometry);
                        m_tempVec = player->getPosition();
                        m_tempVec += Vector{-350.0f, 400.0f, -1500.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_pCamera->setFixed(false);
                        m_nState++;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 2) {
                    player->setSpeed(0.0f);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    float step = (float) delta * 0.25f;
                    (*enemies)[0]->geometry->moveForward(step);
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                    m_dirVec = player->GetDirVector();
                    m_tempVec += m_dirVec * (step * 0.008f);
                    m_dirVec = player->geometry->getRightVector();
                    m_tempVec += m_dirVec * (step * -0.05f);
                    m_dirVec = player->GetUpVector();
                    m_tempVec += m_dirVec * (step * 0.0051f);
                    m_pCamera->setFixed(true);
                    AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec);
                    m_pCamera->setLocal(local);
                    if (((RadioMessage *) ((*messages)[2]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 1001) {
                        player->setSpeed(0.0f);
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget((*m_pLevel->getEnemies())[1]->geometry);
                        m_tempVec = (*m_pLevel->getEnemies())[1]->geometry->getPosition();
                        m_tempVec += Vector{0.0f, 4000.0f, 26000.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_nScriptTimerA = 0;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nState++;
                    }
                } else if (m_nState == 3) {
                    if (((RadioMessage *) ((*messages)[3]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.1f, (float) delta * 0.5f);
                    if (m_nScriptTimerA >= 1001) {
                        player->setSpeed(0.0f);
                        m_pCamera->setTarget(player->geometry);
                        m_tempVec = player->getPosition();
                        m_tempVec += Vector{-350.0f, 400.0f, -1500.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_pCamera->setFixed(false);
                        m_nState++;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 4) {
                    float step = (float) delta * 0.25f;
                    (*enemies)[0]->geometry->moveForward(step);
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                    m_dirVec = player->GetDirVector();
                    m_tempVec += m_dirVec * (step * 0.008f);
                    m_dirVec = player->geometry->getRightVector();
                    m_tempVec += m_dirVec * (step * -0.05f);
                    m_dirVec = player->GetUpVector();
                    m_tempVec += m_dirVec * (step * 0.005f);
                    m_pCamera->setFixed(true);
                    AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec);
                    m_pCamera->setLocal(local);
                    if (((RadioMessage *) ((*messages)[4]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 1001) {
                        player->setSpeed(0.0f);
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget((*m_pLevel->getEnemies())[1]->geometry);
                        m_tempVec = (*m_pLevel->getEnemies())[1]->geometry->getPosition();
                        m_tempVec += Vector{0.0f, 4000.0f, 42000.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 5) {
                    m_pCamera->translate((float) delta, 0.0f, (float) delta * 0.1f);
                    if (((RadioMessage *) ((*messages)[5]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 1001) {
                        player->setSpeed(0.02f);
                        m_pCamera->setTarget(player->geometry);
                        m_tempVec = player->getPosition();
                        m_tempVec += Vector{150.0f, 400.0f, -1500.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_pCamera->setFixed(false);
                        m_nState++;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        (*enemies)[0]->setPosition(Vector{-10000.0f, 0.0f, 22000.0f});
                        ((PlayerFighter *) (*enemies)[0])->field_0x13e = 0;
                        ((PlayerFighter *) (*enemies)[0])->setAIDisabled(false);
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->shipGroup == 9) {
                                (*enemies)[i]->setActive(false);
                                (*enemies)[i]->player->setVulnerable(true);
                            }
                        }
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 6) {
                    float step = (float) delta * 0.25f;
                    (*enemies)[0]->geometry->moveForward(step);
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                    m_dirVec = player->GetDirVector();
                    m_tempVec += m_dirVec * (step * 0.008f);
                    m_dirVec = player->geometry->getRightVector();
                    m_tempVec += m_dirVec * (step * 0.05f);
                    m_dirVec = player->GetUpVector();
                    m_tempVec += m_dirVec * (step * 0.005f);
                    m_pCamera->setFixed(true);
                    AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec);
                    m_pCamera->setLocal(local);
                    if (((RadioMessage *) ((*messages)[6]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 1001) {
                        player->setSpeed(0.0f);
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                        m_tempVec = (*m_pLevel->getEnemies())[0]->geometry->getPosition();
                        m_tempVec += Vector{200.0f, 300.0f, 1800.0f};
                        m_pCamera->setPosition(m_tempVec);
                        m_nScriptTimerA = 0;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_nState++;
                    }
                } else if (m_nState == 7) {
                    if ((*enemies)[0]->isDocked()) {
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 8) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 2001) {
                        m_nState = 9;
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 9) {
                    if (((RadioMessage *) ((*messages)[7]))->isOver()) {
                        m_nScriptTimerA += delta;
                    }
                    if (m_nScriptTimerA >= 2001) {
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            KIPlayer *ship = (*enemies)[i];
                            if (ship->shipGroup == 9) {
                                ship->field_0x25 = 1;
                                ship->setActive(true);
                                (*enemies)[i]->setVisible(true);
                                ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                                (*enemies)[i]->player->setEnemy(i % 3 == 0 ? (*enemies)[0]->player : player->player);
                            }
                        }
                        (*m_pLevel->getEnemies())[1]->shipGroup = 8;
                        (*m_pLevel->getEnemies())[1]->field_0x74 = 0;
                        (*m_pLevel->getEnemies())[1]->player->setAlwaysFriend(false);
                        (*m_pLevel->getEnemies())[1]->player->setAlwaysEnemy(true);
                        (*m_pLevel->getEnemies())[0]->player->setEnemies(nullptr);
                        (*m_pLevel->getEnemies())[0]->player->setVulnerable(true);
                        m_pCamera->setFixed(false);
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setFreeze(false);
                        player->setSpeed(2.0f);
                        player->setComputerControlled(false);
                        player->player->setVulnerable(true);
                        player->setPosition(0.0f, 0.0f, 90000.0f);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        if (field_0xaa) {
                            player->setAutoTurret(true);
                        }
                        m_nScriptTimerA = 0;
                        m_nState++;
                    }
                } else if (m_nState == 10) {
                    if (((RadioMessage *) ((*messages)[8]))->isOver() && m_nTimeLimit <= 0) {
                        m_nScriptTime = 0;
                        m_nTimeLimit = 91000;
                        m_pLevel->objectivesB = new Objective(3, 91000, m_pLevel);
                    }
                    if (player->isDockedToDockingPoint() && player->hackingWon()) {
                        m_nState = 11;
                        delete m_pLevel->objectivesB;
                        m_pLevel->objectivesB = nullptr;
                        for (unsigned int i = 0; i < enemies->count; i++) {
                            if ((*enemies)[i]->shipGroup == 9) {
                                (*enemies)[i]->player->setEnemies(nullptr);
                            }
                        }
                        m_nTimeLimit = 0;
                    }
                }
                break;
            }
            case 24: { // 0x13a930
                if (((RadioMessage *) ((*messages)[3]))->isTriggered() && m_nState == 0) {
                    // 0x13e97e: radio message #3 -- lock the ship down, aim the camera behind it and
                    // pre-position the exit wormhole ahead of the player, then clear everyone's enemy list.
                    player->stopShooting(0);
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_pCamera->setLookAtCam(false);
                    player->player->setVulnerable(false);
                    player->geometry->setRotation(0.0f, 1.5707964f, 0.0f); // yaw = pi/2 @0x13ed8c
                    player->setComputerControlled(true);
                    player->player->removeAllGuns();
                    m_pCamera->setTarget(player->geometry);
                    Vector camPos = player->geometry->getPosition();
                    m_pCamera->setPosition(camPos);
                    m_pCamera->translate(10500.0f, 700.0f, 1000.0f); // pool @0x13ed90/94/98
                    Vector *ahead = &m_tempVec;
                    Vector *dir = &m_dirVec;
                    *ahead = player->geometry->getPosition();
                    *dir = player->geometry->getDirection();
                    *dir *= 45000.0f; // @0x13ed9c
                    *ahead += *dir;
                    (*m_pLevel->getLandmarks())[3]->setPosition(ahead->x, ahead->y, ahead->z);
                    for (unsigned int j = 0; j < m_pLevel->getEnemies()->count; ++j) {
                        (*m_pLevel->getEnemies())[j]->player->setEnemies(nullptr);
                    }
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_bCinematicBreak = 1; // field_0x11 (cinematic break) = 1
                    m_nState = 1; // shared tail 0x1438d6 -> post-switch tail (0x144e36)
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
                    m_nState = 2;
                } else if (m_nState == 2) {
                    // state-2 (0x13c690): while the wormhole spins up, keep its 3D sound event anchored
                    // to the camera, rumble the view and slowly roll the player ship. FP pool:
                    //   @0x13c8d4 = 50.0f (rumble percentage), @0x13c54c = 5000.0f (roll divisor).
                    Vector camPos = *m_pCamera->getPosition();
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(0x22, &camPos, &zero, false);
                    m_pCamera->setRumblePercentage(50.0f, 3);
                    m_nScriptTimerA += delta; // 64-bit @0x90
                    float roll = (float) delta / 5000.0f;
                    player->rotate(roll, roll, roll); // 0x13c6f4
                }
                break;
            }
            case 14: {
                // The player's ship is wrecked by the exploding fighter and towed away.
                if (((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0) {
                    m_nState = 1;
                    (*m_pLevel->getEnemies())[0]->player->setHitpoints(0);
                    (*m_pLevel->getEnemies())[0]->setDead();
                    m_pLevel->field_8c->systemSetMatrix(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x130,
                                                        &player->geometry->getReferenceMatrix());
                    m_pLevel->field_8c->enableSystemEmit(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x130, true);
                    m_pLevel->field_8c->enableSystemRender(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x130, true);
                    m_pLevel->field_8c->systemSetMatrix(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x134,
                                                        &player->geometry->getReferenceMatrix());
                    m_pLevel->field_8c->enableSystemEmit(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x134, true);
                    m_pLevel->field_8c->enableSystemRender(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x134, true);
                    m_pLevel->field_8c->systemSetMatrix(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x138,
                                                        &player->geometry->getReferenceMatrix());
                    m_pLevel->field_8c->enableSystemEmit(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x138, true);
                    m_pLevel->field_8c->enableSystemRender(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x138, true);
                    Globals::sound->play(15, nullptr, nullptr, 0.0f);
                } else if (((RadioMessage *) ((*messages)[1]))->isOver() && m_nState == 1) {
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Vector pos = player->getPosition();
                    Globals::sound->updateEvent3DAttributes(15, &pos, &zero, false);
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    m_bCinematicBreak = 1;
                    player->stopShooting(0);
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    m_pLevel->flashScreen(3);
                    player->setComputerControlled(true);
                    player->setSpeed(0.0f);
                    player->player->removeAllGuns();
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(player->geometry);
                    m_pCamera->setPosition(player->getPosition());
                    m_pCamera->translate(1000.0f, 700.0f, 1000.0f);
                    for (unsigned int i = 0; i < m_pLevel->getEnemies()->count; i++) {
                        if ((*m_pLevel->getEnemies())[i]->shipGroup == 8) {
                            (*m_pLevel->getEnemies())[i]->player->setHitpoints(0);
                            (*m_pLevel->getEnemies())[i]->setDead();
                        }
                    }
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 2;
                } else if (m_nState == 2) {
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->geometry->updateReferenceMatrix();
                    float step = (float) delta;
                    player->geometry->rotate(0.0f, step / 10000.0f, 0.0f);
                    m_pCamera->translate(0.0f, 0.0f, step);
                    if (((RadioMessage *) ((*messages)[3]))->isOver()) {
                        player->setVisible(false);
                        m_pLevel->field_80->enableSystemEmit(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x130, false);
                        m_pLevel->field_80->enableSystemEmit(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x134, false);
                        m_pLevel->field_80->enableSystemEmit(((PlayerFighter *) (*m_pLevel->getEnemies())[0])->field_0x138, false);
                        KIPlayer *tug = (*m_pLevel->getEnemies())[m_pLevel->getEnemies()->count - 1];
                        ((PlayerFixedObject *) tug)->setMoving(true);
                        m_pCamera->setTarget(tug->geometry);
                        m_pCamera->setPosition(tug->getPosition());
                        m_pCamera->translate(-10000.0f, -200.0f, 5000.0f);
                        m_nState = 3;
                        m_pLevel->lodManager->forceUpdate(delta, false);
                    }
                } else if (((RadioMessage *) ((*messages)[4]))->isOver() && m_nState == 3) {
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 4;
                    m_nScriptTimerA = m_nScriptTime;
                } else if (m_nState == 4 && m_nScriptTime >= m_nScriptTimerA + 4000) {
                    Globals::status->nextCampaignMission(true);
                    Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(98));
                    Globals::switch_to_target_setting = 0;
                    Globals::appManager->SetCurrentApplicationModule(5);
                    Level::initStreamOutPosition = 0;
                    m_nState = 5;
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
            case 42: {
                // Leaving the alien system through the wormhole.
                if (messages != nullptr && m_nState == 5 && ((RadioMessage *) ((*messages)[7]))->isOver()) {
                    Globals::sound->play(153, nullptr, nullptr, 0.0f);
                    m_nScriptTime = 0;
                    delete m_pLevel->objectivesB;
                    m_pLevel->objectivesB = nullptr;
                    delete m_pLevel->objectivesA;
                    m_pLevel->objectivesA = nullptr;
                    (*m_pLevel->getLandmarks())[3]->setPosition(25000.0f, 20000.0f, -55000.0f);
                    ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->open();
                    m_nScriptTimerA = 0;
                    m_nState = 6;
                } else if (m_nState == 0 && !Globals::status->inAlienOrbit()) {
                    ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
                    m_nState = 1;
                } else if (m_nState == 6) {
                    Vector pos = player->getPosition();
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(153, &pos, &zero, false);
                    m_nScriptTimerA += delta;
                    m_pCamera->setRumblePercentage(0.5f, 30);
                    Globals::sound->setParamValue(1, 153, 0.5f);
                    if (player->isInWormhole()) {
                        player->setTurretMode(false);
                        resetCamera(m_pLevel);
                        player->setFreeLookMode(false);
                        m_pCamera->enableFirstPersonCam(false);
                        player->hideShipForFirstPersonCameraView(false);
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        m_bCinematicBreak = 1;
                        m_pCamera->setLookAtCam(true);
                        player->setComputerControlled(true);
                        player->setVisible(false);
                        player->player->setVulnerable(false);
                        m_pCamera->setPosition(player->getPosition());
                        m_pCamera->setTarget(((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry);
                        m_tempVec = 5000.0f * AbyssEngine::AEMath::VectorNormalize(player->getPosition());
                        m_pCamera->translate(m_tempVec.x, m_tempVec.y, m_tempVec.z);
                        ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->freeMissionLock();
                        ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
                        m_bRenderParticles = 1;
                        Globals::sound->play(154, nullptr, nullptr, 0.0f);
                        m_nScriptTimerA = 0;
                        m_nState = 7;
                    }
                } else if (m_nState == 7) {
                    m_pCamera->setRumblePercentage(100.0f, 30);
                    m_pCamera->translate(0.0f, 0.0f, (float) (-18 * delta));
                    if (m_bRenderParticles) {
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom0->transform))
                                ->Update((long long) delta, false);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom1->transform))
                                ->Update((long long) delta, false);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom2->transform))
                                ->Update((long long) delta, false);
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Vector dir = AbyssEngine::AEMath::MatrixGetDir(
                                *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent())));
                        m_pParticleGeom1->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
                        m_pParticleGeom2->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
                        m_pParticleGeom1->setPosition(dir * 10000.0f);
                        if (((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom0->transform))
                                    ->currentTime >= 4001) {
                            ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->setVisible(false);
                        }
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA >= 15001) {
                            m_bRenderParticles = 0;
                            Globals::layout->startFade(true, 255, 4000);
                            m_nScriptTimerA = 0;
                        }
                    } else {
                        m_nScriptTimerA += delta;
                        if (!Globals::layout->isFading() || m_nScriptTimerA >= 10001) {
                            Globals::status->field_64 = player->player->getHitpoints();
                            Globals::status->field_5c = player->player->getShieldHP();
                            Globals::status->field_60 = player->player->getArmorHP();
                            Globals::status->field_68 = player->player->getGammaHP();
                            Globals::status->field_f4 = player->getCurrentSecondaryWeaponIndex();
                            Globals::status->setStation(
                                    (Station *) (intptr_t) Globals::galaxy->getStation(Globals::status->field_84));
                            Globals::status->departStation(Globals::status->getStation());
                            Level::initStreamOutPosition = 1;
                            Level::comingFromAlienWorld = 1;
                            Globals::switch_to_target_setting = 1;
                            Level::programmedStation = nullptr;
                            Globals::appManager->SetCurrentApplicationModule(2);
                            field_0x12 = 0x101;
                            return m_bCinematicBreak != 0;
                        }
                    }
                }
                break;
            }
            case 91: {
                // Escorting the defector freighter to the station, then flying away with it.
                if (m_nState == 0 && ((RadioMessage *) ((*messages)[3]))->isTriggered()) {
                    m_nScriptTimerB = 0;
                    m_nState = 1;
                    (*enemies)[0]->setActive(true);
                    ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))
                            ->setName(String(*Globals::gameText->getText(3211), false));
                } else if (m_nState == 1 && player->isDockedToDockingPoint()) {
                    m_nState = 2;
                } else if (m_nState == 2 && ((RadioMessage *) ((*messages)[5]))->isOver()) {
                    ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(2);
                    if (player->isDockedToDockingPoint()) {
                        player->setDockingState(2);
                    }
                    m_nState = 3;
                } else if (m_nState == 3 && Globals::status->field_178 >= 10) {
                    m_nState = 4;
                    delete m_pLevel->objectivesB;
                    m_pLevel->objectivesB = nullptr;
                } else if (m_nState == 4 && ((RadioMessage *) ((*messages)[6]))->isOver()
                           && !m_pLevel->getPlayer()->isDockedToDockingPoint()) {
                    m_nScriptTimerA = 0;
                    m_nState = 5;
                } else if (m_nState == 5) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 3001) {
                        m_nScriptTimerA = 0;
                        m_pLevel->getPlayer()->setAutoPilot(nullptr);
                        m_pRadar->lockedEnemy = nullptr;
                        m_pRadar->dockNavPtr = nullptr;
                        if (player->isInRocketControl()) {
                            resetCamera(m_pLevel);
                            player->setRocketControl(nullptr, nullptr);
                            player->killLiberator();
                            m_pCamera->setRumblePercentage(0.0f, 0);
                        }
                        player->setTurretMode(false);
                        resetCamera(m_pLevel);
                        player->setFreeLookMode(false);
                        m_pCamera->enableFirstPersonCam(false);
                        player->hideShipForFirstPersonCameraView(false);
                        player->stopShooting(0);
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        player->setComputerControlled(true);
                        player->setCollide(false);
                        m_pCamera->setLookAtCam(true);
                        m_pCamera->setTarget(player->geometry);
                        m_tempVec = player->geometry->getPosition();
                        AEGeometry *ship = player->geometry;
                        ship->setDirection(AbyssEngine::AEMath::VectorNormalize(
                                                   m_pLevel->getPlayer()->getPosition()
                                                   - ((KIPlayer *) m_pLevel->getDockingTarget(0))->geometry->getPosition()),
                                           Vector{0.0f, 1.0f, 0.0f});
                        m_dirVec = player->geometry->getDirection();
                        m_tempVec += m_dirVec * 10000.0f;
                        m_dirVec = player->geometry->getRightVector();
                        m_tempVec += m_dirVec * 700.0f;
                        m_pCamera->setPosition(m_tempVec);
                        (*enemies)[0]->player->setHitpoints(0);
                        m_nState = 6;
                    }
                } else if (m_nState == 6) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 2001) {
                        int speedBits = player->getSpeed();
                        if (reinterpret_cast<float &>(speedBits) < 100.0f) {
                            speedBits = player->getSpeed();
                            player->setSpeed(reinterpret_cast<float &>(speedBits) * 1.05f);
                        }
                        if (m_nScriptTimerA >= 8001) {
                            Globals::status->nextCampaignMission(true);
                            Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(113));
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(113));
                            Level::initStreamOutPosition = 1;
                            Globals::status->field_64 = player->player->getHitpoints();
                            Globals::status->field_5c = player->player->getShieldHP();
                            Globals::status->field_60 = player->player->getArmorHP();
                            Globals::status->field_68 = player->player->getGammaHP();
                            Globals::appManager->SetCurrentApplicationModule(2);
                        }
                    }
                }
                Vector pos = (*enemies)[0]->geometry->getPosition();
                Vector zero{0.0f, 0.0f, 0.0f};
                Globals::sound->updateEvent3DAttributes(2281, &pos, &zero, false);
                if (Globals::status->field_178 > 7 && m_nScriptTime <= 179999) {
                    if (m_nScriptTimerB == 0) {
                        Globals::sound->play(2282, nullptr, nullptr, 0.0f);
                        m_nScriptTimerB = 1;
                    }
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*enemies)[0]->geometry->transform))
                            ->Update((long long) delta, true);
                } else {
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                             (*enemies)[0]->geometry->childTransform))
                            ->Update((long long) delta, true);
                }
                break;
            }
            case 94: {
                // Ambush at the station: two fighter pairs drop in while the player is docked.
                switch (m_nState) {
                    case 0:
                        if (Globals::status->field_178 > 6 || m_nScriptTime >= 60001) {
                            for (int i = 0; i != 2; i++) {
                                (*enemies)[i]->setPosition((float) (50000 + 3000 * i), 1000.0f, 140000.0f);
                                (*enemies)[i]->awake();
                                (*enemies)[i]->setVisible(true);
                                (*enemies)[i]->geometry->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
                                ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
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
                            m_pCamera->enableFirstPersonCam(false);
                            player->hideShipForFirstPersonCameraView(false);
                            player->stopShooting(0);
                            m_bCinematicBreak = 1;
                            m_pHud->visible = 0;
                            m_pRadar->enabled = 0;
                            m_pCamera->setLookAtCam(true);
                            AEGeometry *leader = (*enemies)[0]->geometry;
                            m_pCamera->setTarget(leader);
                            m_tempVec = leader->getPosition();
                            m_dirVec = leader->getDirection();
                            m_tempVec += m_dirVec * 8000.0f;
                            m_dirVec = leader->getRightVector();
                            m_tempVec += m_dirVec * 1200.0f;
                            m_dirVec = leader->getUpVector();
                            m_tempVec += m_dirVec * -300.0f;
                            m_pCamera->setPosition(m_tempVec);
                            m_nScriptTimerA = 0;
                            m_nState++;
                        }
                        break;
                    case 1:
                        m_nScriptTimerA += delta;
                        for (int i = 0; i != 2; i++) {
                            (*enemies)[i]->geometry->moveForward((float) (3 * delta));
                        }
                        if (m_nScriptTimerA >= 6001) {
                            for (int i = 0; i != 2; i++) {
                                ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                            }
                            m_pLevel->getPlayer()->resetGunDelay();
                            m_pCamera->setLookAtCam(false);
                            m_pCamera->setTarget(player->geometry);
                            bool docked = player->isDockedToDockingPoint();
                            m_pCamera->setLookAtCam(false);
                            if (docked) {
                                lookBehind();
                                m_pCamera->setRotationAroundTarget(true);
                                player->setFreeLookMode(true);
                                if (field_0xab) {
                                    player->setTurretMode(true);
                                }
                            } else {
                                m_pCamera->setPosition(m_dirVec);
                            }
                            m_pHud->visible = 1;
                            m_pRadar->enabled = 1;
                            m_pLevel->lodManager->forceUpdate(delta, false);
                            m_bCinematicBreak = 0;
                            m_nScriptTimerA = 0;
                            Globals::sound->stop(Globals::sound->currentMusicEvent);
                            m_nState++;
                        }
                        break;
                    case 2:
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA > 100000) {
                            m_nScriptTimerA = 0;
                            int x = -40000;
                            for (int i = 2; i != 4; i++) {
                                KIPlayer *ship = (*enemies)[i];
                                ship->setPosition(player->getPosition() + Vector{(float) x, 3000.0f, 20000.0f});
                                (*enemies)[i]->awake();
                                (*enemies)[i]->setVisible(true);
                                x -= 3000;
                            }
                            m_nState++;
                        }
                        break;
                    case 3:
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA > 100000) {
                            m_nScriptTimerA = 0;
                            int x = -40000;
                            for (int i = 4; i != 6; i++) {
                                KIPlayer *ship = (*enemies)[i];
                                ship->setPosition(player->getPosition() + Vector{(float) x, 3000.0f, 20000.0f});
                                (*enemies)[i]->awake();
                                (*enemies)[i]->setVisible(true);
                                x -= 3000;
                            }
                            m_nState++;
                        }
                        break;
                }
                break;
            }
            case 105: { // 0x13aea4
                if (m_nState <= 2) {
                    float step = (float) (delta << 1);
                    for (int i = 0; i != 2; ++i) {
                        (*enemies)[i]->geometry->moveForward(step);
                    }
                }
                if (m_nState <= 4) {
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                }
                if (m_nState == 1) {
                    m_nScriptTimerA += delta;
                    Vector drift = (player->geometry->getDirection() * 6.08f
                                    + player->geometry->getRightVector() * 0.2f) * 0.4f * (float) delta;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    if (m_nScriptTimerA >= 13001) {
                        m_nState = 2;
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 2) {
                    Vector drift = (player->geometry->getDirection() * 6.08f
                                    + player->geometry->getRightVector() * 0.2f) * 0.4f * (float) delta;
                    m_pCamera->translate(drift.x * 0.9f, drift.y * 0.9f, drift.z * 0.9f);
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
                        m_nScriptTimerA = 0;
                        m_nState = m_nState + 1; // 2 -> 3
                    }
                } else if (m_nState == 3) {
                    m_nScriptTimerA += delta;
                    Vector drift = (player->geometry->getDirection() * 6.08f
                                    + player->geometry->getRightVector() * 0.2f) * 0.4f * (float) delta;
                    m_pCamera->translate(drift.x * 0.4f, drift.y * 0.4f, drift.z * 0.4f);
                    for (int i = 0; i < 2; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->field_0x12c = 1;
                    }
                    if (m_nScriptTimerA > 3000) {
                        m_nScriptTimerA = 0;
                        m_nState = m_nState + 1; // 3 -> 4
                    }
                } else if (m_nState == 4) {
                    m_nScriptTimerA += delta;
                    Vector drift = (player->geometry->getDirection() * 6.08f
                                    + player->geometry->getRightVector() * 0.2f) * 0.4f * (float) delta;
                    m_pCamera->translate(drift.x * 0.1f, drift.y * 0.1f, drift.z * 0.1f);
                    for (int i = 0; i < 2; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->field_0x12c = 1;
                    }
                    if (m_nScriptTimerA > 3000) {
                        m_pLevel->getPlayer()->resetGunDelay();
                        player->setComputerControlled(false);
                        m_pCamera->setLookAtCam(false);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1; // radar restore flag @0x48
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0; // clear cinematicBreak_ (m_nFlags high byte @0x11)
                        for (int i = 0; i < 2; ++i) {
                            (*enemies)[i]->field_0x74 = 1;
                        }
                        m_nScriptTimerA = 0;
                        m_nState = m_nState + 1; // 4 -> 5
                    }
                } else if (m_nState == 5) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 10001) {
                        for (int i = 0; i < 2; ++i) {
                            (*enemies)[i]->setActive(false);
                            (*enemies)[i]->setSpeed(0.0f); // KIPlayer vtable+0x1c
                        }
                        m_nState = m_nState + 1; // 5 -> 6
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 6) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 24001) {
                        m_nScriptTimerA = 0;
                        Vector lightDir = AbyssEngine::AEMath::VectorNormalize(
                                m_pLevel->getStarSystem()->getLightDirection());
                        Vector playerPos = player->geometry->getPosition();
                        for (int i = 2; i < 5; ++i) {
                            (*enemies)[i]->awake();
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
                            ((PlayerFighter *) (*enemies)[i])->cloak(1000, true);
                        }
                        (*enemies)[2]->setPosition(playerPos.x + lightDir.x * 100000.0f, 0.0f,
                                                   playerPos.z + lightDir.z * 100000.0f);
                        (*enemies)[2]->geometry->setDirection(-lightDir, Vector{0.0f, 1.0f, 0.0f});
                        {
                            AEGeometry *ref = (*enemies)[2]->geometry;
                            Vector pos2 = (*enemies)[2]->getPosition();
                            (*enemies)[3]->setPosition(pos2 + ref->getRightVector() * 2200.0f +
                                                       ref->getUpVector() * -200.0f +
                                                       ref->getDirection() * -2200.0f);
                            (*enemies)[3]->geometry->setDirection(-lightDir, Vector{0.0f, 1.0f, 0.0f});
                            pos2 = (*enemies)[2]->getPosition();
                            (*enemies)[4]->setPosition(pos2 + ref->getRightVector() * -2200.0f +
                                                       ref->getUpVector() * -200.0f +
                                                       ref->getDirection() * -2200.0f);
                            (*enemies)[4]->geometry->setDirection(-lightDir, Vector{0.0f, 1.0f, 0.0f});
                        }
                        for (int i = 2; i < 5; ++i) {
                            (*enemies)[i]->geometry->translate(
                                    (*enemies)[i]->geometry->getRightVector() * -30000.0f);
                        }
                        player->setTurretMode(false);
                        player->setFreeze(true);
                        player->setVisible(false);
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
                        m_bCinematicBreak = 1; // cinematicBreak_ high byte @0x11 = 1
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        m_pCamera->setTarget((*enemies)[2]->geometry);
                        AEGeometry *ref2 = (*enemies)[2]->geometry;
                        Vector *camPos = &m_tempVec; // tempVec @0x28
                        Vector *tmp = &m_dirVec;
                        *camPos = ref2->getPosition();
                        *tmp = ref2->getDirection();
                        *camPos += *tmp * 6800.0f;
                        *tmp = ref2->getRightVector();
                        *camPos += *tmp * 1200.0f;
                        *tmp = ref2->getUpVector();
                        *camPos += *tmp * 400.0f;
                        m_pCamera->setPosition(*camPos);
                        m_nScriptTimerA = 0;
                        m_nScriptTimerB = 0;
                        m_nState = m_nState + 1; // 6 -> 7 (via 0x144e12)
                    }
                } else if (m_nState == 7) {
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB =
                            (long long) ((float) delta * 0.05f +
                                         (float) m_nScriptTimerB);
                    (*enemies)[2]->geometry->moveForward((float) ((double) delta * 2.5));
                    (*enemies)[3]->geometry->moveForward((float) ((double) delta * 2.4));
                    (*enemies)[4]->geometry->moveForward((float) ((double) delta * 2.3));
                    {
                        AEGeometry *geo = (*enemies)[2]->geometry;
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *acc = &m_tempVec;
                        Vector *tmp = &m_dirVec;
                        *acc = AbyssEngine::AEMath::MatrixGetPosition(local);
                        *tmp = geo->getDirection();
                        *acc += *tmp * ((float) delta * 1.96f);
                        *tmp = geo->getRightVector();
                        *acc += *tmp * ((float) delta * 0.03f);
                        *tmp = geo->getUpVector();
                        *acc += *tmp * ((float) delta * 0.05f * 0.004f);
                        m_pCamera->setFixed(true);
                        m_pCamera->setLocal(AbyssEngine::AEMath::MatrixSetTranslation(local, *acc));
                    }
                    if (m_nScriptTimerA >= 10001) {
                        for (int i = 2; i < 5; ++i) {
                            ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
                        }
                        player->setFreeze(false);
                        player->setVisible(true);
                        m_pCamera->setFixed(false);
                        m_pCamera->setTarget(player->geometry);
                        m_pCamera->setLookAtCam(false);
                        Vector playerPos = player->getPosition();
                        Vector playerDir = player->GetDirVector();
                        m_pCamera->setPosition(playerPos - playerDir * 2400.0f);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0; // clear cinematicBreak_ (byte @0x11 = 0)
                        m_nState = m_nState + 1; // 7 -> 8
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 8) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 60001) {
                        m_nState = 9;
                    }
                } else if (m_nState == 9) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 110001) {
                        m_nScriptTimerA = 0;
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
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 10 && ((RadioMessage *) ((*messages)[7]))->isOver()) {
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
                    player->player->setVulnerable(false);
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;   // radar+0x48
                    player->stopShooting(0);
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pCamera->setLookAtCam(true);
                    player->setComputerControlled(true);
                    m_pCamera->setPosition(player->geometry->getPosition());
                    Vector lightDir = AbyssEngine::AEMath::VectorNormalize(
                            m_pLevel->getStarSystem()->getLightDirection());
                    player->geometry->setDirection(lightDir, Vector{0.0f, 1.0f, 0.0f});
                    Vector *camPos = &m_tempVec; // LevelScript tempVec @0x28
                    *camPos = player->geometry->getDirection();
                    *camPos *= 8000.0f;                                        // @0x142e1c
                    *camPos += player->geometry->getRightVector() * 1000.0f;   // @0x142e20
                    *camPos += player->geometry->getUpVector() * 1000.0f;
                    *camPos += player->geometry->getPosition();
                    m_pCamera->setPosition(camPos->x, camPos->y, camPos->z);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nScriptTimerA = 0;
                    m_nState = m_nState + 1; // 10 -> 11
                } else if (m_nState == 11) {
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 2501) {
                        if (m_pGeometry6 == nullptr) {
                            AEGeometry *marker = new AEGeometry(0x37a7, Globals::Canvas, false);
                            m_pGeometry6 = marker;
                            marker->setPosition(player->geometry->getPosition());
                            marker->setDirection(player->geometry->getDirection(),
                                                 Vector{0.0f, 1.0f, 0.0f});
                            m_pCamera->setTarget(marker);
                            Globals::sound->play(14, nullptr, nullptr, 0.0f);
                            Vector camPos = *m_pCamera->getPosition();
                            Vector zero{0.0f, 0.0f, 0.0f};
                            Globals::sound->updateEvent3DAttributes(14, &camPos, &zero, false);
                        }
                        if (m_nScriptTimerA >= 9001) {
                            if (m_nScriptTimerA - delta <= 9000) {
                                Globals::sound->play(2248, nullptr, nullptr, 0.0f);
                            }
                            Array<AEGeometry *> *planets = reinterpret_cast<Array<AEGeometry *> *>(
                                    m_pLevel->getStarSystem()->getPlanets());
                            AEGeometry *planet0 = (*planets)[0];
                            planet0->setScaling(planet0->getScaling() * 0.95f);
                            if (m_nScriptTimerA >= 10001) {
                                Globals::layout->startFade(true, -1, 500);
                                m_nState = m_nState + 1; // 11 -> 12
                                m_nScriptTimerA = 0;
                            }
                        }
                    }
                } else if (m_nState == 12) {
                    Array<AEGeometry *> *planets = reinterpret_cast<Array<AEGeometry *> *>(
                            m_pLevel->getStarSystem()->getPlanets());
                    AEGeometry *planet0 = (*planets)[0];
                    planet0->setScaling(planet0->getScaling() * 4.0f); // 4.0 @0x144d44
                    m_pCamera->setRumblePercentage(50.0f, 10);         // 50.0 @0x145018
                    for (unsigned int i = 0; i < enemies->count; ++i) {
                        KIPlayer *mover = (*enemies)[i];
                        if (mover != nullptr && mover->shipGroup == 10) {
                            mover->initPush(player->getPosition(), 100000);
                        }
                    }
                    if (!Globals::layout->isFading()) {
                        Globals::layout->startFade(false, -1, 2000); // 0x144da2
                        m_pCamera->setTarget(player->geometry);
                        Vector dir = player->geometry->getDirection();
                        player->geometry->setDirection(-dir, Vector{0.0f, 1.0f, 0.0f});
                        if (m_pGeometry6 != nullptr) {
                            delete m_pGeometry6; // 0x144df6
                        }
                        m_nScriptTimerA = 0;
                        m_pGeometry6 = nullptr;
                        m_pLevel->getStarSystem()->switchSunForSupernovaExpansion(); // 0x144e0e
                        m_nState = m_nState + 1; // 12 -> 13
                    }
                } else if (m_nState == 13) {
                    m_nScriptTimerA += delta;
                    float spin = (float) delta / 800.0f; // @0x144c34
                    player->rotate(spin, spin, spin);
                    player->geometry->moveForward((float) (delta * 12)); // 0x144c6a
                    m_pCamera->setRumblePercentage(50.0f, 10); // 50.0 @0x145018
                    if (m_nScriptTimerA >= 6001) {
                        Globals::status->field_64 = player->player->getHitpoints(); // hullHp @0x64
                        Globals::status->field_5c = player->player->getShieldHP(); // @0x5c
                        Globals::status->field_60 = player->player->getArmorHP();  // @0x60
                        Globals::status->field_68 = player->player->getGammaHP();  // @0x68
                        Globals::status->nextCampaignMission(true);
                        Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(0x6f));
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(0x6f));
                        Globals::switch_to_target_setting = 1;
                        Level::initStreamOutPosition = 1;
                        Globals::appManager->SetCurrentApplicationModule(2); // 0x144d1e
                    }
                }
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
            case 16: {
                // Wormhole tutorial: show the target, open the wormhole and send the enemies through.
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                    Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                    camPos += Vector{6000.0f, 4000.0f, 47500.0f};
                    m_pCamera->setPosition(camPos);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 1;
                } else if (m_nState == 1) {
                    m_pCamera->translate((float) -delta, 0.0f, (float) (-3 * delta));
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    if (((RadioMessage *) ((*messages)[1]))->isOver()) {
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        m_pHud->visible = 1;
                        m_pRadar->enabled = 1;
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_bCinematicBreak = 0;
                        m_nState = 2;
                    }
                } else if (m_nState == 2 && m_nScriptTime >= 20001) {
                    m_nState = 3;
                    ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
                } else if (((RadioMessage *) ((*messages)[2]))->isTriggered() && m_nState == 3) {
                    m_nState = 4;
                    ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->open();
                    (*m_pLevel->getLandmarks())[3]->setVisible(true);
                    player->setTurretMode(false);
                    resetCamera(m_pLevel);
                    player->setFreeLookMode(false);
                    m_pCamera->enableFirstPersonCam(false);
                    player->hideShipForFirstPersonCameraView(false);
                    player->stopShooting(0);
                    m_bCinematicBreak = 1;
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
                    player->setComputerControlled(true);
                    player->freeze = 1;
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setActive(true);
                    m_pCamera->setTarget((*m_pLevel->getLandmarks())[3]->geometry);
                    Vector camPos = (*m_pLevel->getLandmarks())[3]->getPosition();
                    camPos += Vector{-9000.0f, -4000.0f, -30000.0f};
                    m_pCamera->setPosition(camPos);
                    camPos = (*m_pLevel->getLandmarks())[3]->getPosition();
                    int routePoints[3] = {(int) camPos.x, (int) camPos.y, (int) (camPos.z + 30000.0f)};
                    Route *route = new Route(routePoints, 3);
                    camPos.z += -15000.0f;
                    for (unsigned int i = 0; i < m_pLevel->getEnemies()->count; i++) {
                        KIPlayer *ship = (*m_pLevel->getEnemies())[i];
                        if (ship != nullptr && ship->shipGroup == 9) {
                            ship->setPosition(camPos.x, camPos.y - 2000.0f + (float) Globals::rnd->nextInt(4000),
                                              camPos.z - 2000.0f + (float) Globals::rnd->nextInt(4000));
                            ship->geometry->setDirection(
                                    AbyssEngine::AEMath::VectorNormalize((*m_pLevel->getLandmarks())[3]->getPosition()
                                                                         - ship->getPosition()),
                                    Vector{0.0f, 1.0f, 0.0f});
                            ship->setRoute(route->clone());
                            ship->player->setEnemies(nullptr);
                        }
                    }
                    m_pLevel->lodManager->forceUpdate(delta, false);
                } else if (m_nState == 4) {
                    m_pCamera->translate((float) ((double) delta * 0.5), 0.0f, (float) ((double) delta * 0.2));
                    if (((RadioMessage *) ((*messages)[3]))->isOver()) {
                        m_nState = 5;
                        for (unsigned int i = 0; i < m_pLevel->getEnemies()->count; i++) {
                            KIPlayer *ship = (*m_pLevel->getEnemies())[i];
                            if (ship != nullptr && ship->shipGroup == 9) {
                                ship->setVisible(false);
                                ship->setDead();
                            }
                        }
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
                    }
                } else if (((RadioMessage *) ((*messages)[4]))->isOver() && m_nState == 5) {
                    m_nState = 6;
                    player->freeze = 0;
                    m_pCamera->setLookAtCam(false);
                    m_pCamera->setTarget(player->geometry);
                    player->setComputerControlled(false);
                    m_pHud->visible = 1;
                    m_pRadar->enabled = 1;
                    resetCamera(m_pLevel);
                    m_bCinematicBreak = 0;
                    m_pLevel->lodManager->forceUpdate(delta, false);
                }
                break;
            }
            default:
                break; // -> post-switch tail (0x144e36)
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
    return m_bCinematicBreak != 0;
}

void LevelScript::lookBehind() {
    m_pCamera->setTargetOffset(Vector{0.0f, 0.0f, -950.0f});
    m_pCamera->setCamOffset(Vector{0.0f, 600.0f, 2230.0f});
}

LevelScript::LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera)
    : m_tempVec(), m_camVec(), m_dirVec() {
    m_pHud = hud;
    m_pRadar = radar;
    m_pCamera = camera;
    m_pLevel = level;
    hud->drawTitleImage(false);
    m_nState = 0;
    m_bStartSequence = 1;
    m_bStartSequenceOver = 0;
    field_0x10 = 0;
    m_bCinematicBreak = 1;
    m_nTimeLimit = level->getTimeLimit();
    m_nScriptTime = 0;
    field_0x24 = 0;
    field_0x8c = 0;
    m_nScriptTimerA = 0;
    m_nScriptTimerB = 0;
    m_nFlashCountdown = 0;
    m_bRenderParticles = 0;
    field_0xa9 = 0;
    m_pParticleGeom0 = nullptr;
    m_pParticleGeom1 = nullptr;
    m_pParticleGeom2 = nullptr;
    m_pGeometry0 = nullptr;
    m_pGeometry1 = nullptr;
    m_pGeometry2 = nullptr;
    m_pGeometry3 = nullptr;
    m_pExplosion = nullptr;
    m_pGeometry4 = nullptr;
    m_pGeometry5 = nullptr;
    m_pGeometry6 = nullptr;
    field_0x12 = 0;
    camera->setLookAtCam(true);

    if (level->getPlayer() == nullptr)
        m_bStartSequence = 0;
    else
        level->getPlayer()->player->setVulnerable(false);
    level->getPlayer()->setCollide(false);


    if (Globals::status->getCurrentCampaignMission() == 0) {
        m_nScriptTimerA = 0;
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->player->setVulnerable(true);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setPosition(Vector{-1000.0f, -500.0f, -40000.0f});
        level->getPlayer()->setPosition(0.0f, 0.0f, -60000.0f);
        level->getPlayer()->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
        for (int i = 0; i < 3; i++) {
            (*level->getEnemies())[i]->setToSleep();
            (*level->getEnemies())[i]->geometry->setVisible(false);
            ((PlayerFighter *) (*level->getEnemies())[i])->field_0x128 = 0;
            (*level->getEnemies())[i]->player->enemyFlagsLo = 0;
        }
        level->lodManager->forceUpdate(30, false);
        m_pGeometry5 = new AEGeometry(15027, Globals::Canvas, false);
    } else if (Globals::status->getCurrentCampaignMission() == 1) {
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setRotation(0.462f, 0.462f, 1.5339f);
        level->getPlayer()->setFreeze(true);
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->setPosition(0.0f, 0.0f, 0.0f);
        level->getPlayer()->setExhaustVisible(false);
        level->getPlayer()->StopEngineSound();
        (*level->getEnemies())[0]->StopEngineSound();
        camera->setPosition(1500.0f, 1600.0f, -3000.0f);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setTarget(level->getPlayer()->geometry);
        Globals::layout->startFade(false, 255, 5000);
    } else if (Globals::status->getCurrentCampaignMission() == 81 && Globals::status->inAlienOrbit()) {
        m_nScriptTimerA = 0;
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->freeze = 1;
        level->getPlayer()->player->setVulnerable(false);
        level->getPlayer()->setVisible(false);
        Globals::sound->stop(level->getPlayer()->field_0x1c);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setPosition(Vector{-20000.0f, 800.0f, 120000.0f});
        level->getPlayer()->setPosition(0.0f, 0.0f, 0.0f);
        ((PlayerStation *) (*level->getEnemies())[0])->rootGeometry->setVisible(false);
        level->lodManager->forceUpdate(30, false);
        m_pGeometry5 = new AEGeometry(15027, Globals::Canvas, false);
        m_pGeometry5->setScaling(30.0f, 30.0f, 30.0f);
    } else if (Globals::status->getCurrentCampaignMission() == 78) {
        camera->setPosition(2000.0f, -1500.0f, 16000.0f);
        field_0x24 = 0;
        m_bStartSequence = 0;
        m_bStartSequenceOver = 1;
        level->getPlayer()->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
        m_nState = 1;
        m_bCinematicBreak = 1;
        m_pGeometry5 = new AEGeometry(15027, Globals::Canvas, false);
        m_pGeometry5->setScaling(20.0f, 20.0f, 20.0f);
    } else if (Globals::status->getCurrentCampaignMission() == 89) {
        m_nScriptTimerA = 0;
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->freeze = 1;
        level->getPlayer()->player->setVulnerable(false);
        level->getPlayer()->setVisible(false);
        Globals::sound->stop(level->getPlayer()->field_0x1c);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        m_pGeometry4 = new AEGeometry(Globals::Canvas);
        m_pGeometry4->setPosition((*level->getEnemies())[1]->getPosition());
        m_pGeometry4->setDirection(-level->getStarSystem()->getLightDirection(), Vector{0.0f, 1.0f, 0.0f});
        m_pGeometry4->moveForward(-100000.0f);
        m_pGeometry4->translate(m_pGeometry4->getRightVector() * 200000.0f);
        camera->setTarget(m_pGeometry4);
        camera->setPosition((*level->getEnemies())[1]->getPosition());
        camera->translate(10000.0f, 1500.0f, -20000.0f);
        (*level->getEnemies())[0]->geometry->setPosition(
                Vector{-25000.0f, 800.0f, 120000.0f} + 1000.0f * level->getStarSystem()->getLightDirection());
        (*level->getEnemies())[0]->geometry->setDirection(level->getStarSystem()->getLightDirection(),
                                                         Vector{0.0f, 1.0f, 0.0f});
        (*level->getEnemies())[0]->geometry->translate(Vector{3000.0f, 0.0f, 3000.0f});
        m_pGeometry6 = new AEGeometry(14229, Globals::Canvas, false);
        m_pGeometry6->setScaling(1.0f, 1.0f, 500000.0f);
        m_nState = 1;
        level->lodManager->forceUpdate(30, false);
    } else if (Globals::status->getCurrentCampaignMission() == 105 && Globals::status->inSupernovaOrbit()) {
        m_bCinematicBreak = 1;
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->setCollide(false);
        m_bStartSequence = 0;
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setTarget(level->getPlayer()->geometry);
        camera->setPosition((*level->getEnemies())[1]->getPosition());
        camera->translate(5000.0f, 1500.0f, -20000.0f);
        camera->setPosition(level->getPlayer()->getPosition() + level->getPlayer()->GetDirVector() * -2400.0f +
                            level->getPlayer()->GetUpVector() * 500.0f);
        level->getPlayer()->player->setVulnerable(true);
        if (!Level::doInstantJump)
            setAutoPilotToProgrammedStation();
        m_nState = 1;
        m_nScriptTimerA = 0;
    } else if (Globals::status->getCurrentCampaignMission() == 106) {
        level->getPlayer()->setPosition(300000.0f, 0.0f, 300000.0f);
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->freeze = 1;
        level->getPlayer()->player->setVulnerable(false);
        Vector lightDir = VectorNormalize(level->getStarSystem()->getLightDirection());
        level->getPlayer()->geometry->setDirection(lightDir, Vector{0.0f, 1.0f, 0.0f});
        Globals::sound->stop(level->getPlayer()->field_0x1c);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setPosition(level->getPlayer()->getPosition() + Vector{2200.0f, 1300.0f, -4000.0f});
        m_bCinematicBreak = 1;
        Globals::layout->startFade(false, -1, 8000);
        m_nState = 1;
    } else if (Globals::status->getCurrentCampaignMission() == 144) {
        m_pGeometry4 = new AEGeometry(Globals::Canvas);
        m_pGeometry4->setPosition(30000.0f, 10000.0f, 160000.0f);
        m_pGeometry4->setDirection(Vector{1.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
        m_bCinematicBreak = 1;
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->setCollide(false);
        level->getPlayer()->setActive(false);
        level->getPlayer()->setVisible(false);
        level->getPlayer()->setFreeze(true);
        m_bStartSequence = 0;
        m_bStartSequenceOver = 1;
        Globals::sound->stop(level->getPlayer()->field_0x1c);
        camera->setLookAtCam(true);
        camera->setTarget(m_pGeometry4);
        m_tempVec = (*level->getEnemies())[0]->getPosition();
        m_tempVec += Vector{-4600.0f, 1000.0f, -4300.0f};
        camera->setPosition(m_tempVec);
        if (!Level::doInstantJump)
            setAutoPilotToProgrammedStation();
        m_nState = 1;
        m_nScriptTimerA = 0;
    } else if (Globals::status->getCurrentCampaignMission() == 131 && !Globals::status->inAlienOrbit() &&
               ((Station *) Globals::status->getStation())->getIndex() == 112) {
        m_bCinematicBreak = 1;
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->setCollide(false);
        level->getPlayer()->setPosition(-65000.0f, 0.0f, 95000.0f);
        level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                   Vector{0.0f, 1.0f, 0.0f});
        m_bStartSequence = 0;
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setTarget(level->getPlayer()->geometry);
        camera->setPosition(level->getPlayer()->getPosition() - level->getPlayer()->GetDirVector() * 2400.0f +
                            level->getPlayer()->GetUpVector() * 500.0f -
                            level->getPlayer()->geometry->getRightVector() * 600.0f);
        camera->update(10);
        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
        Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
        m_tempVec = MatrixGetPosition(local);
        camera->setFixed(true);
        MatrixSetTranslation(local, m_tempVec);
        camera->setLocal(local);
        if (!Level::doInstantJump)
            setAutoPilotToProgrammedStation();
        m_nState = 1;
        m_nScriptTimerA = 0;
    } else if (Globals::status->getCurrentCampaignMission() == 158 && !Globals::status->inAlienOrbit() &&
               ((Station *) Globals::status->getStation())->getIndex() == 111) {
        m_pGeometry4 = new AEGeometry(Globals::Canvas);
        m_pGeometry4->setDirection(Vector{1.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
        m_bStartSequence = 0;
        m_bStartSequenceOver = 1;
        level->getPlayer()->setPosition(-50000.0f, 0.0f, 15000.0f);
        hud->visible = 0;
        radar->enabled = 0;
        level->getPlayer()->setFreeze(true);
        level->getPlayer()->player->setVulnerable(false);
        Vector lightDir = VectorNormalize(level->getStarSystem()->getLightDirection());
        level->getPlayer()->geometry->setDirection(lightDir, Vector{0.0f, 1.0f, 0.0f});
        Globals::sound->stop(level->getPlayer()->field_0x1c);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setPosition(level->getPlayer()->getPosition() + Vector{-28000.0f, 500.0f, 8500.0f});
        m_pGeometry4->setPosition(*camera->getPosition());
        m_pGeometry4->setDirection(VectorNormalize(lightDir), Vector{0.0f, 1.0f, 0.0f});
        m_pGeometry4->moveForward(50000.0f);
        camera->setTarget(m_pGeometry4);
        m_bCinematicBreak = 1;
        Globals::layout->startFade(false, -1, 8000);
        Array<KIPlayer *> *enemies = level->getEnemies();
        (*enemies)[0]->player->setEnemies(nullptr);
        (*enemies)[0]->player->addEnemy(level->getPlayer()->player);
        for (int i = 1; i < 4; i++) {
            (*enemies)[i]->player->setEnemies(nullptr);
            (*enemies)[i]->player->setEnemy(level->getPlayer()->player);
            Gun *gun = (*(*(*enemies)[i]->player->guns)[0])[0];
            gun->damage = (int) ((float) gun->damage * 0.3f);
            if (level->field_b0 != nullptr) {
                for (unsigned j = 0; j < level->field_b0->size(); j++) {
                    (*enemies)[i]->player->addEnemy((*level->field_b0)[j]->player);
                    (*level->field_b0)[j]->player->addEnemy((*enemies)[i]->player);
                }
            }
        }
        (*enemies)[0]->setActive(false);
        (*enemies)[0]->setVisible(false);
        level->getPlayer()->setPosition(m_pGeometry4->getPosition());
        level->getPlayer()->geometry->setDirection(
                VectorNormalize(*camera->getPosition() - m_pGeometry4->getPosition() + Vector{20000.0f, 0.0f, 0.0f}),
                Vector{0.0f, 1.0f, 0.0f});
        level->getPlayer()->geometry->moveForward(30000.0f);
        level->getPlayer()->setSpeed(0.1f);
        m_nState = 1;
    } else {
        unsigned char streamOut = Level::initStreamOutPosition;
        int range = streamOut == 0 ? 2000 : 500;
        m_tempVec.x = (float) (Globals::rnd->nextInt(range) + 500);
        m_tempVec.y = (float) (Globals::rnd->nextInt(range) + 500);
        m_tempVec.z = streamOut == 0 ? 9000.0f : 7000.0f;
        if (Globals::rnd->nextInt(2) == 0)
            m_tempVec.x = -m_tempVec.x;
        if (Globals::rnd->nextInt(2) == 0)
            m_tempVec.y = -m_tempVec.y;

        if (Globals::status->getCurrentCampaignMission() == 48) {
            level->getPlayer()->setPosition(0.0f, 0.0f, 55000.0f);
            Vector dir = level->getPlayer()->geometry->getDirection();
            dir.x += 0.00001f;
            level->getPlayer()->geometry->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
            level->getPlayer()->setAutoPilot((*level->getLandmarks())[0]);
        } else if (Globals::status->getCurrentCampaignMission() == 65 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(0.0f, 0.0f, 170000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
            Vector pos = level->getPlayer()->getPosition();
            pos.x -= 3000.0f;
            (*level->getEnemies())[0]->setPosition(pos.x, pos.y, pos.z);
            (*level->getEnemies())[0]->geometry->setRotation(Vector{0.0f, 3.1415927f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 87 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(0.0f, 0.0f, 210000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 91 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(0.0f, 0.0f, -130000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 92 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(0.0f, 0.0f, 200000.0f);
            level->getPlayer()->geometry->setDirection(
                    VectorNormalize((*level->getEnemies())[3]->getPosition() - level->getPlayer()->getPosition()),
                    Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 102 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(90000.0f, 6000.0f, 150000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 123 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(0.0f, 0.0f, 180000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if ((Globals::status->getCurrentCampaignMission() > 84 ||
                    Globals::status->getCurrentCampaignMission() == 135) &&
                   !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() == 103) {
            level->getPlayer()->setPosition(70000.0f, 0.0f, 100000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 114 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(0.0f, 0.0f, -120000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 137 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(20000.0f, 0.0f, 150000.0f);
            level->getPlayer()->geometry->setDirection(VectorNormalize(-level->getPlayer()->getPosition()),
                                                       Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 139 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(20000.0f, 0.0f, 190000.0f);
            level->getPlayer()->geometry->setDirection(
                    VectorNormalize((*level->getEnemies())[1]->getPosition() - level->getPlayer()->getPosition()),
                    Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->getCurrentCampaignMission() == 142 && !Globals::status->inAlienOrbit() &&
                   ((Station *) Globals::status->getStation())->getIndex() ==
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            level->getPlayer()->setPosition(60000.0f, 0.0f, 120000.0f);
            level->getPlayer()->geometry->setDirection(
                    VectorNormalize(((Route *) (intptr_t) level->getPlayer()->getRoute())->getWaypoint(0)->getPosition() -
                                    level->getPlayer()->getPosition()),
                    Vector{0.0f, 1.0f, 0.0f});
            (*level->getEnemies())[0]->setPosition(level->getPlayer()->getPosition() +
                                                   level->getPlayer()->geometry->getRightVector() * 2100.0f +
                                                   level->getPlayer()->geometry->getDirection() * 2000.0f);
            (*level->getEnemies())[0]->geometry->setDirection(level->getPlayer()->GetDirVector(),
                                                             Vector{0.0f, 1.0f, 0.0f});
        } else if (Globals::status->inAlienOrbit() && Globals::status->getCurrentCampaignMission() == 154) {
            level->getPlayer()->setPosition(0.0f, 0.0f, 140000.0f);
            (*level->getEnemies())[0]->setPosition(level->getPlayer()->getPosition() +
                                                   level->getPlayer()->geometry->getRightVector() * 2600.0f +
                                                   level->getPlayer()->geometry->getDirection() * 4000.0f);
            (*level->getEnemies())[0]->geometry->setDirection(level->getPlayer()->GetDirVector(),
                                                             Vector{0.0f, 1.0f, 0.0f});
            (*level->getEnemies())[0]->player->setEnemy((*level->getEnemies())[1]->player);
            for (unsigned i = 0; i < level->getEnemies()->size(); i++) {
                if ((*level->getEnemies())[i]->field_0x3e)
                    (*level->getEnemies())[i]->player->setEnemies(nullptr);
                if ((*level->getEnemies())[i]->shipGroup == 9)
                    (*level->getEnemies())[i]->field_0x25 = 0;
            }
        }

        Matrix playerMatrix = level->getPlayer()->geometry->getMatrix();
        m_camVec = MatrixRotateVector(playerMatrix, m_tempVec);
        m_camVec += level->getPlayer()->geometry->getPosition();
        camera->setPosition(m_camVec);
        bool inAlienOrbit = Globals::status->inAlienOrbit();
        if (inAlienOrbit || Level::comingFromAlienWorld) {
            if (Globals::status->getCurrentCampaignMission() <= 42) {
                m_tempVec = level->getPlayer()->getPosition();
                m_dirVec = level->getPlayer()->geometry->getDirection();
                m_dirVec *= 10000.0f;
                m_tempVec -= m_dirVec;
                ((PlayerWormHole *) (*level->getLandmarks())[3])->reset(true);
                (*level->getLandmarks())[3]->setPosition(m_tempVec.x, m_tempVec.y, m_tempVec.z);
                (*level->getLandmarks())[3]->setVisible(true);
            }
            Level::comingFromAlienWorld = 0;
        }
    }

    if (Globals::status->getCurrentCampaignMission() == 80) {
        m_pGeometry0 = new AEGeometry(14370, Globals::Canvas, false);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry0->transform))->boundingRadius =
                1000000.0f;
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry0->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        m_pGeometry0->setVisible(false);
        m_pGeometry0->setRotation(Vector{0.0f, 3.1415927f, 0.0f});
        AEGeometry *stationRoot = ((PlayerStation *) (*level->getLandmarks())[0])->rootGeometry;
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(stationRoot->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(stationRoot->childTransform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(stationRoot->parentTransform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                 ((PlayerStation *) (*level->getLandmarks())[0])->meshTransform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
        m_pExplosion = new Explosion(0);
        m_pExplosion->addFireStreaks();
        m_pExplosion->setScaling(3.0f);
        m_pGeometry6 = new AEGeometry(Globals::Canvas);
        m_pGeometry6->setVisible(false);
        m_pGeometry1 = new AEGeometry(14366, Globals::Canvas, false);
        m_pGeometry1->setPosition(((PlayerStation *) (*level->getEnemies())[0])->rootGeometry->getPosition());
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        m_pGeometry5 = new AEGeometry(15027, Globals::Canvas, false);
        m_pGeometry5->setScaling(20.0f, 20.0f, 20.0f);
    }

    if (Globals::status->getCurrentCampaignMission() == 41 && Globals::status->inAlienOrbit()) {
        field_0xe0 = level->field_74->addSystem(&(*level->getEnemies())[0]->geometry->getReferenceMatrix(),
                                               (ParticleSettings::ParticleSet) 40, false);
        level->field_74->enableSystemEmit(field_0xe0, false);
        field_0xe4 = level->field_74->addSystem(&(*level->getEnemies())[0]->geometry->getReferenceMatrix(),
                                               (ParticleSettings::ParticleSet) 41, false);
        level->field_74->enableSystemEmit(field_0xe4, false);
        level->lodManager->forceUpdate(30, false);
        m_pParticleGeom0 = new AEGeometry(14285, Globals::Canvas, false);
        m_pParticleGeom1 = new AEGeometry(14286, Globals::Canvas, false);
        m_pParticleGeom2 = new AEGeometry(14287, Globals::Canvas, false);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom0->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom1->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGeom2->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
    } else if (Globals::status->getCurrentCampaignMission() == 157 && !Globals::status->inAlienOrbit() &&
               ((Station *) Globals::status->getStation())->getIndex() ==
                       ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
        m_pGeometry1 = new AEGeometry(19061, Globals::Canvas, false);
        AEGeometry *part0 = new AEGeometry(19062, Globals::Canvas, false);
        AEGeometry *part1 = new AEGeometry(19063, Globals::Canvas, false);
        m_pGeometry1->addChild(part0->transform);
        m_pGeometry1->addChild(part1->transform);
        m_pGeometry2 = new AEGeometry(19095, Globals::Canvas, false);
        m_pGeometry3 = new AEGeometry(19097, Globals::Canvas, false);
        AEGeometry *part2 = new AEGeometry(19096, Globals::Canvas, false);
        AEGeometry *part3 = new AEGeometry(19098, Globals::Canvas, false);
        m_pGeometry2->addChild(part2->transform);
        m_pGeometry3->addChild(part3->transform);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))->SetVisible(false);
        m_pGeometry2->setVisible(false);
        m_pGeometry3->setVisible(false);
        delete part0;
        delete part1;
        delete part2;
        delete part3;
    }

    Globals::layout->enableFillScreen(false);

    if (!Globals::status->inAlienOrbit() && Globals::status->getCampaignMission() != 0 &&
        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 163 &&
        Globals::status->field_90 != nullptr) {
        for (unsigned i = 0; i < Globals::status->field_90->size(); i++) {
            if ((*Globals::status->field_90)[i] == ((Station *) Globals::status->getStation())->getIndex()) {
                field_0xa9 = 1;
                break;
            }
        }
    }

    if (Globals::status->getCurrentCampaignMission() == 95 || Globals::status->getCurrentCampaignMission() == 99 ||
        Globals::status->getCurrentCampaignMission() == 109 || Globals::status->getCurrentCampaignMission() == 119 ||
        Globals::status->getCurrentCampaignMission() == 126 || Globals::status->getCurrentCampaignMission() == 133 ||
        Globals::status->getCurrentCampaignMission() == 160 || Globals::status->getCurrentCampaignMission() == 161) {
        hud->visible = 0;
        radar->enabled = 0;
        m_bCinematicBreak = 1;
        level->getPlayer()->setComputerControlled(true);
        level->getPlayer()->freeze = 1;
        level->getPlayer()->player->setVulnerable(false);
        level->getPlayer()->setVisible(false);
        Globals::sound->stop(level->getPlayer()->field_0x1c);
        m_bStartSequenceOver = 1;
        camera->setLookAtCam(true);
        camera->setPosition(Vector{-15000.0f, 800.0f, 75000.0f});
        level->getPlayer()->setPosition(0.0f, 0.0f, 0.0f);
        level->lodManager->forceUpdate(30, false);
        m_nScriptTimerA = 0;
    }
}

int LevelScript::getEvent() {
    return m_nState;
}

void LevelScript::setEvent(int event) {
    m_nState = event;
}
