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

int LevelScript::process(int delta) {
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
                        m_nScriptTimerA += delta;
                        m_pExplosion->update((int) ((float) delta * 0.5f), (TargetFollowCamera *) nullptr);
                        if (m_nScriptTimerA < 324) {
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
                        m_nScriptTimerA = 0;
                        m_nState++; // 4 -> 5 (shared set-state tail 0x1438d6)
                        break; // -> post-switch tail (0x144e36)
                    }
                    if (m_nState == 5) {
                        // State 5 (0x13f6c6): the boss wreck's Transform animation plays out
                        // (Transform::Update(delta*0.3)); meanwhile the 13 fighters are fanned outward --
                        // each is re-aimed along its own facing skewed by its right vector (scaled by
                        // timerA/100000, but only while 2001 <= timerA <= 4999) and pushed forward by
                        // timerA*delta*0.001. Keep the wreck explosion rumbling ((int)(delta*0.5)). Once
                        // 10s elapse, banish every fighter to (50000,50000,50000), silence the boss's
                        // particle emitter and run the standard cutscene-exit teardown (advance state 5->6
                        // via the shared exit tail). FP pool: 0.3 @0x13fa88, 100000 @0x13fa8c,
                        // 0.001 @0x13fa90, 50000 @0x13fa94.
                        m_nScriptTimerA += delta; // 0x13f6c6
                        long long timerA = m_nScriptTimerA;
                        // Advance the wreck's Transform animation (0x13f6da).
                        AbyssEngine::Transform *anim = (AbyssEngine::Transform *)
                            Globals::Canvas->TransformGetTransform((*enemies)[13]->geometry->transform);
                        anim->Update((long long) ((float) delta * 0.3f), false); // 0x13f718
                        // Fan the 13 fighters outward (0x13f732).
                        Vector *scratch = &m_tempVec; // this+0x28
                        for (int i = 0; i < 13; ++i) {
                            AEGeometry *geom = (*enemies)[i]->geometry;
                            *scratch = geom->getDirection(); // 0x13f746
                            if ((unsigned long long) (timerA - 2001) <= 2998) { // 0x13f74a..0x13f764
                                *scratch += geom->getRightVector() *
                                            ((float) timerA / 100000.0f); // 0x13f77c..0x13f798
                            }
                            geom->setDirection(AbyssEngine::AEMath::VectorNormalize(*scratch),
                                               Vector{0.0f, 1.0f, 0.0f}); // 0x13f7c0
                            geom->moveForward((float) (timerA * (long long) delta) * 0.001f); // 0x13f7f2
                        }
                        m_pExplosion->update((int) ((float) delta * 0.5f),
                                             (TargetFollowCamera *) nullptr); // 0x13f81a
                        if (m_nScriptTimerA <= 9999) { // 0x13f82e
                            break; // -> post-switch tail (0x144e36)
                        }
                        // 10s up: silence the boss emitter (0x13f83e) and banish the fighters (0x13f84a).
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_54, false);
                        for (int i = 0; i < 13; ++i) {
                            (*enemies)[i]->setActive(false);  // 0x13f854
                            (*enemies)[i]->setVisible(false); // 0x13f862
                            (*enemies)[i]->setPosition(50000.0f, 50000.0f, 50000.0f); // 0x13f878 vtable#72
                        }
                        // Standard cutscene-exit teardown (0x13f880).
                        m_pLevel->getPlayer()->resetGunDelay();
                        m_pCamera->setLookAtCam(false);
                        m_pCamera->setTarget(player->geometry);
                        player->setComputerControlled(false);
                        player->player->setVulnerable(true);
                        player->setFreeze(false);
                        player->setVisible(true);
                        m_pHud->visible = 1;      // m_pHud byte @0x1 = 1
                        m_pRadar->enabled = 1; // m_pRadar byte @0x48 = 1
                        resetCamera(m_pLevel);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        m_pRadar->lockedEnemy = nullptr; // 0x13f8f2 strd zeroes m_pRadar+4/+8
                        m_pRadar->dockNavPtr = nullptr;
                        m_bCinematicBreak = 0; // 0x13f8fa cinematic break high byte = 0
                        m_nState++; // 5 -> 6 (shared exit tail 0x14500e increments m_nState)
                        break;
                    }
                    if (m_nState == 2) {
                        // State 2 (0x13cae0): once 1s has elapsed, have the lead escort keep firing, spawn
                        // a fresh tracking marker (m_pGeometry4) seated at the lead escort's muzzle, aim it
                        // along the escort's first gun's fire direction, hide it and point the camera at it,
                        // then reset the timer and advance to state 3. The aim vector is the Vector at
                        // Gun+0x18 (the escort's first gun; unnamed pad field in our Gun model). The
                        // getPosition here is Player's vtable slot 0x28 on enemies[0]->player.
                        m_nScriptTimerA += delta; // 0x13cae6
                        if (m_nScriptTimerA < 1001) { // 0x13caf0 gate
                            break; // -> post-switch tail (0x144e36)
                        }
                        (*enemies)[0]->player->shoot(0, delta, false); // 0x13cb18
                        m_pGeometry4 = new AEGeometry(Globals::Canvas); // 0x13cb1e..0x13cb36 (0xc0 bytes)
                        Vector *scratch = &m_tempVec; // this+0x28
                        // getPosition == Player vtable slot 0x28 on the lead escort's Player (0x13cb42).
                        *scratch = (*enemies)[0]->player->getPosition();
                        m_pGeometry4->setPosition(*scratch); // 0x13cb4c
                        // Aim direction = the 12-byte fire-direction Vector at Gun+0x18 of the lead escort's
                        // first gun (0x13cb50..0x13cb66). Our Gun model splits this region into two
                        // upstream-dropped 4-byte fields (_pad_0x18) plus field_0x20 (which the ctor reuses as a
                        // VecArray base), so no single named member spans 0x18..0x24; read it by offset as the
                        // original does. enemies[0]->player->guns is Array<Array<Gun*>*>.
                        Gun *leadGun = (*(*(*enemies)[0]->player->guns)[0])[0];
                        *scratch = *reinterpret_cast<Vector *>(reinterpret_cast<char *>(leadGun) + 0x18);
                        m_pGeometry4->setDirection(*scratch, Vector{0.0f, 1.0f, 0.0f}); // 0x13cb82
                        m_pGeometry4->setVisible(false);   // 0x13cb8c
                        m_pCamera->setTarget(m_pGeometry4); // 0x13cb96
                        m_nScriptTimerA = 0; // 0x13cb9a
                        m_nState++; // 2 -> 3 (shared set-state tail 0x143d0a)
                        break; // -> post-switch tail (0x144e36)
                    }
                    if (m_nState == 3) {
                        // State 3 (0x13f53c): every frame advance the tracking marker along its facing at a
                        // speed of Gun+0x50 (== pitchRate) * delta, taken from the lead escort's first gun.
                        // Once 5s elapse, snapshot the boss[14]'s matrix into the particle system, enable
                        // its emitter, detonate the boss explosion at that matrix position, reset the timer
                        // and advance to state 4.
                        Gun *leadGun = (*(*(*enemies)[0]->player->guns)[0])[0]; // 0x13f542..0x13f556
                        m_pGeometry4->moveForward(leadGun->pitchRate * (float) delta); // 0x13f558..0x13f56a
                        m_nScriptTimerA += delta; // 0x13f576
                        if (m_nScriptTimerA < 5001) { // 0x13f580 gate
                            break; // -> post-switch tail (0x144e36)
                        }
                        m_matrix = (*enemies)[14]->geometry->getMatrix(); // 0x13f58e..0x13f5a4
                        m_pLevel->field_74->systemSetMatrix(m_pLevel->field_54, &m_matrix); // 0x13f5b0
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_54, true);     // 0x13f5bc
                        Vector burstPos = AbyssEngine::AEMath::MatrixGetPosition(m_matrix); // 0x13f5cc
                        m_pExplosion->start(burstPos, Vector{0.0f, 0.0f, 0.0f}); // 0x13f5de
                        m_nScriptTimerA = 0; // 0x13f5e4
                        m_nState++; // 3 -> 4 (shared set-state tail 0x143d0a)
                        break; // -> post-switch tail (0x144e36)
                    }
                    break; // -> post-switch tail (0x144e36)
                }
                // state 1 (0x1393fe): once the intro radio (message #0) is over, run the standard
                // cutscene-exit teardown and reframe the camera ahead of enemies[0] along its own basis.
                // FP immediates: dir scale @0x13970c = 18000.0, right scale @0x139710 = 3000.0.
                if (!((RadioMessage *) ((*messages)[0]))->isOver()) {
                    // 0x13caca: intro radio still playing -> fall to the post-switch tail.
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
                m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->enabled = 0; // disasm stores m_pRadar+0x48 (same field the siblings exit zero)
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                Vector *camPos = &m_tempVec; // LevelScript tempVec @0x28
                *camPos = (*enemies)[0]->geometry->getPosition();
                *camPos += (*enemies)[0]->geometry->getDirection() * 18000.0f;
                *camPos += (*enemies)[0]->geometry->getRightVector() * 3000.0f;
                m_pCamera->setPosition(*camPos);
                (*enemies)[0]->setVisible(true);
                (*enemies)[0]->setActive(true);
                m_nScriptTimerA = 0; // 64-bit script counter @0x90
                m_nState++; // shared tail 0x13953c increments m_nState
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
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;
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
                    m_pRadar->enabled = 1;
                    resetCamera(m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    (*enemies)[10]->setPosition(500000.0f, 500000.0f, 500000.0f); // vtable+0x48 [slot 18]
                    (*enemies)[10]->setActive(false);
                    (*enemies)[10]->setVisible(false);
                    m_nState = 3;
                    m_bCinematicBreak = 0; // clear cinematicBreak_ (m_nFlags high byte @0x11)
                } else {
                    // State-3 (0x13da68): once radio message #5 fires, scatter the mid-wave escorts
                    // (enemies[5..8]) to random positions around enemies[0] -- magnitude 15000..24999
                    // on ±X/±Z, -5000..4999 on Y (Globals::rnd->nextInt) -- then advance to state 4
                    // (shared tail 0x13e80c -> 0x1438d4). No FP immediates: the components are integer
                    // arithmetic converted with vcvt.f32.s32. No awake() call here (unlike case 66/11).
                    if (m_nState == 3 && ((RadioMessage *) ((*messages)[5]))->isTriggered()) {
                        Vector scatterOrigin = (*enemies)[0]->getPosition();
                        for (int i = 5; i != 9; ++i) {
                            int magX = 15000 + Globals::rnd->nextInt(10000);
                            int signX = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
                            int y = Globals::rnd->nextInt(10000) - 5000;
                            int magZ = 15000 + Globals::rnd->nextInt(10000);
                            int signZ = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
                            Vector offset{(float) (signX * magX), (float) y, (float) (signZ * magZ)};
                            (*enemies)[i]->setPosition(scatterOrigin + offset); // vtable+0x44
                        }
                        m_nState = 4;
                        break;
                    }
                    // State-4 (0x13eeaa): mid-wave escort (enemies[10]) formation-up and camera hand-off.
                    // Reached whenever we are not in the state-3 scatter path: gate on radio message #7
                    // being triggered while m_nState == 4. Seat the lead escort above enemies[0], face it
                    // +Z, wake it, spin it up to speed 2, swap the mission objective for a fresh straight
                    // outbound route, then run the standard cutscene-exit teardown and lock the camera onto
                    // enemies[10] before advancing to state 5.
                    if (!((RadioMessage *) ((*messages)[7]))->isTriggered() || m_nState != 4) {
                        break; // -> post-switch tail (0x13faee -> 0x144e36)
                    }
                    // Seat enemies[10] just above enemies[0] (getPosition slot 10; z += 11.71875 pool
                    // @0x13f1c4), then face it straight +Z with +Y up (0x13eec6..0x13ef0c).
                    Vector formPos = (*enemies)[0]->getPosition();          // vtable+0x28 [slot 10]
                    (*enemies)[10]->setPosition(formPos.x, formPos.y, formPos.z + 11.71875f); // vtable+0x48 [slot 18]
                    (*enemies)[10]->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
                    (*enemies)[10]->setActive(true);                        // 0x13ef16
                    (*enemies)[10]->setVisible(true);                       // 0x13ef1e
                    (*enemies)[10]->setSpeed(2.0f); // KIPlayer vtable+0x1c [slot 7], 2.0f @0x13ef2c
                    // Swap the mission objective (m_pLevel->objectivesB @Level+0x2c) for a fresh route:
                    // free the old Objective, then build Route({0, 0, 800000}, 3) from the .rodata table
                    // @0x202768 and store the Route pointer back into objectivesB (0x13ef32..0x13ef4c).
                    delete m_pLevel->objectivesB;
                    int routePts[3] = {0, 0, 800000}; // .rodata @0x202768
                    Route *route = new Route(routePts, 3);
                    m_pLevel->objectivesB = reinterpret_cast<Objective *>(route);
                    (*enemies)[10]->setRoute(route);                        // 0x13ef72
                    // Standard cutscene-exit teardown (0x13ef76..): hand control back to the AI, hide the
                    // HUD/radar cinematic overlay and lock the camera onto enemies[10].
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
                    m_pCamera->setTarget((*enemies)[10]->geometry);
                    // reframe: enemies[10]->getPosition() + {6000, -200, 10000} (pool @0x13f1cc..0x13f1d4)
                    Vector camPos = (*enemies)[10]->getPosition() + Vector{6000.0f, -200.0f, 10000.0f};
                    m_pCamera->setPosition(camPos);
                    player->player->setVulnerable(false);
                    player->freeze = 1; // PlayerEgo+0x24
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_nState = 5; // -> post-switch tail 0x13999a
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
                    // -> shared geometry-6 advance tail (0x144e18)
                    break;
                }
                if (((RadioMessage *) ((*messages)[2]))->isOver() && m_nState == 1) {
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
                    // -> shared tail 0x13e3bc
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
            case 80: { // 0x139e34
                if (m_nState == 12) {
                    // State 12 @0x14421c: per-frame docking-approach choreography. Advance the frame
                    // timer, keep the wreck explosion rumbling, drift the follow camera, ramp the rumble
                    // up over 8s, and once 8s elapse seat the docking model ahead of the intro geometry,
                    // kick its Transform animation, play the docking sting and advance to state 13.
                    m_nScriptTimerA += delta;
                    m_pExplosion->update(delta, (TargetFollowCamera *) nullptr);
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f,
                                         -(float) delta * 4.0f); // 0.5,0.2 @0x14446c,-4.0
                    float rumble =
                            (float) m_nScriptTimerA / 8000.0f; // @0x144470
                    if (!(rumble < 1.0f)) {
                        rumble = 1.0f;
                    }
                    m_pCamera->setRumblePercentage(rumble, 100);
                    if (m_nScriptTimerA >= 8001) {
                        Vector dockPos = m_pGeometry1->getPosition() +
                                         Vector{0.0f, 0.0f, -10000.0f}; // z @0x144474
                        m_pGeometry5->setPosition(dockPos);
                        AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(m_pGeometry5->transform);
                        t->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                        t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(m_pGeometry5->transform);
                        t->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                        Globals::sound->play(160, nullptr, nullptr, 0.0f);
                        m_nScriptTimerA = 0;
                        m_nState = 13;
                    }
                    break;
                }
                if (m_nState == 13) {
                    // State 13 @0x1440a0: the docking model has seated; hold the camera on it while the
                    // rumble winds down. Keep drifting the camera, aim the docking model down the camera's
                    // current facing and step its animation, ramp the rumble back to zero, then at 8s hide
                    // the wreck and at 100s hand control back to the player and reframe onto the ship.
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f,
                                         -(float) delta * 4.0f); // 0.5,0.2 @0x1440a8,143d50, -4.0
                    m_nScriptTimerA += delta;
                    AEGeometry *dockModel = *reinterpret_cast<AEGeometry **>(&m_dirVec.z);
                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                    Vector camDir = AbyssEngine::AEMath::MatrixGetDir(
                            *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent())));
                    dockModel->setDirection(camDir, Vector{0.0f, 1.0f, 0.0f});
                    AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                            Globals::Canvas->TransformGetTransform(dockModel->transform);
                    t->Update((long long) delta, false);
                    float rumble =
                            (float) m_nScriptTimerA / 10000.0f; // @0x144468
                    if (!(rumble < 1.0f)) {
                        rumble = 1.0f;
                    }
                    m_pCamera->setRumblePercentage(1.0f - rumble, 100);
                    if (m_nScriptTimerA < 2151) {
                        break;
                    }
                    // 0x1441a4: hide the escort station's root marker (field @0x140 == rootGeometry).
                    ((PlayerStation *) (*enemies)[0])->rootGeometry->setVisible(false);
                    if (m_nScriptTimerA < 10001) {
                        break;
                    }
                    m_pCamera->setRumblePercentage(0.0f, 0);
                    m_pCamera->setLookAtCam(false);
                    m_pCamera->setTarget(player->geometry);
                    player->setComputerControlled(false);
                    player->freeze = 0;
                    m_pHud->visible = 1;
                    m_pRadar->enabled = 1;
                    resetCamera(m_pLevel);
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_bCinematicBreak = 0; // field_0x11 (cinematic break) = 0
                    m_nScriptTimerA = 0;
                    m_nState = 14;
                    break;
                }
                if (m_nState != 0) {
                    // Cold substates dispatched by the tbh @0x13e630 (index = m_nState - 1, 10 entries).
                    // States 3/4/5 fall through to the default (shared camera epilogue 0x143c06);
                    // states 6..10 (aim/objective-spawn/animation-stepping chains) remain deferred.
                    if (m_nState == 1) {
                        // State 1 (0x13e650): drift the follow camera and, after ~3s on the 64-bit
                        // script timer, advance to state 2.
                        m_pCamera->translate((float) delta, -(float) delta,
                                             -12.0f * (float) delta); // 0x13e650: delta, -delta, -12*delta
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA >= 3001) {
                            m_nScriptTimerA = 0;
                            m_nState = 2;
                        }
                        break; // -> shared tail (0x14433e)
                    }
                    if (m_nState == 2) {
                        // State 2 (0x1414d6): keep drifting the follow camera and step m_pGeometry1's
                        // Transform animation; after ~2s, hide m_pGeometry1 and advance to state 6.
                        m_pCamera->translate((float) delta, -(float) delta,
                                             -12.0f * (float) delta); // 0x1414d6: delta, -delta, -12*delta
                        m_nScriptTimerA += delta;
                        AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(m_pGeometry1->transform);
                        t->Update((long long) delta, true);
                        if (m_nScriptTimerA >= 2001) {
                            m_nScriptTimerA = 0;
                            m_nState = 6;
                            m_pGeometry1->setVisible(false); // this+0xbc
                        }
                        break; // -> shared tail (0x14433e)
                    }
                    if (m_nState == 6) {
                        // State 6 (0x14156a): one-shot camera reframe onto the escort (m_pGeometry6).
                        // Seat the escort at a fixed world spot, latch the follow camera onto it in
                        // look-at mode, seat the camera 1/1.7 of the way along a fixed offset, then
                        // frame the camera to look from the escort back toward that seat by borrowing
                        // a throwaway AEGeometry's oriented matrix. Finally reveal the hero geometry,
                        // reset the timer and advance to state 7 in a single frame (no timer gate).
                        // NB: no Transform +0xf8 fn-ptr chain here -- state 6 uses only named virtual
                        // calls; the deep animation-step chain lives elsewhere (states 9/10).
                        m_pGeometry6->setPosition(Vector{12487.0f, -11451.0f, 5958.0f}); // 0x14156a
                        m_pCamera->setLookAtCam(true);       // 0x141582
                        m_pCamera->setTarget(m_pGeometry6);  // 0x14158a
                        Vector *camSeat = &m_tempVec; // LevelScript tempVec @0x28
                        *camSeat = Vector{-28855.0f, -30075.0f, 63820.0f} / 1.7f;  // 0x1415a8/0x1415b4
                        m_pCamera->setPosition(*camSeat);    // 0x1415bc
                        Vector facing = m_pGeometry6->getPosition() - *camSeat; // 0x1415c8/0x1415d2
                        AEGeometry *frame = new AEGeometry(Globals::Canvas); // 0x1415d8 (0xc0 bytes)
                        frame->setDirection(facing, Vector{0.0f, 1.0f, 0.0f}); // 0x141604
                        frame->setPosition(*camSeat);        // 0x14160c
                        Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(),
                                                        frame->getMatrix()); // 0x141614..0x141626
                        delete frame;                        // 0x14162a/0x141630
                        m_pLevel->lodManager->forceUpdate(delta, false); // 0x14163e
                        m_pGeometry0->setVisible(true);      // 0x141648 (this+0xb8)
                        m_nScriptTimerA = 0; // 0x14164e
                        m_nState = 7;                        // 0x141652
                        break; // 0x141654 -> shared tail (0x14433e)
                    }
                    if (m_nState == 7) {
                        // State 7 (0x141658): pure dwell. Advance the 64-bit script timer and, once
                        // ~801ms elapse, reset it and advance to state 8. No camera/geometry work here.
                        m_nScriptTimerA += delta; // 0x15165e..0x15166c
                        if (m_nScriptTimerA >= 801) { // 0x321 @0x15165a
                            m_nScriptTimerA = 0; // 0x15167c
                            m_nState = 8;                                       // 0x151684
                        }
                        break; // -> shared tail (0x14433e)
                    }
                    if (m_nState == 8) {
                        // State 8 (0x1416ec): one-shot fire-up of the escort's engine effects. After the
                        // same ~801ms dwell gate, detonate a "start" burst at the escort (m_pGeometry6),
                        // play radio sting 18, then wire two particle-emitter systems (level handles
                        // field_58 / field_5c on the shared ParticleSystemManager field_74) onto the
                        // escort's matrix (system field_58) and onto an offset copy in m_matrix
                        // (system field_5c, +4500 in X / +1000 in Z), enabling emit on both. Then reset
                        // the timer and advance to state 9. Gated once, runs in a single frame.
                        m_nScriptTimerA += delta; // 0x1516f2..0x151700
                        if (m_nScriptTimerA < 801) { // 0x321 @0x1516ee
                            break; // -> shared tail (0x14433e)
                        }
                        Vector escortPos = m_pGeometry6->getPosition();  // 0x15171e
                        m_pExplosion->start(escortPos, Vector{0.0f, 0.0f, 0.0f}); // 0x151734
                        Globals::sound->play(18, nullptr, nullptr, 0.0f); // 0x15174a (event 0x12)
                        // System field_58: track the escort's live matrix, enable its emitter (0x151764/0x151770).
                        m_pLevel->field_74->systemSetMatrix(m_pLevel->field_58, &m_pGeometry6->getMatrix());
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_58, true);
                        // System field_5c: seat at the escort's position offset by {+4500, 0, +1000} via
                        // m_matrix, then enable its emitter (0x1517b4..0x1517d4). tempVec == this+0x28.
                        Vector *tempVec = &m_tempVec; // 0x151780
                        *tempVec = m_pGeometry6->getPosition();          // 0x151786
                        tempVec->x += 4500.0f;                           // 4500 @0x151ae0
                        tempVec->z += 1000.0f;                           // 1000 @0x1516a4
                        AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, tempVec->x, tempVec->y, tempVec->z); // 0x1517bc
                        m_pLevel->field_74->systemSetMatrix(m_pLevel->field_5c, &m_matrix); // 0x1517c8
                        m_pLevel->field_74->enableSystemEmit(m_pLevel->field_5c, true);     // 0x1517d4
                        m_nScriptTimerA = 0; // 0x1517da
                        m_nState = 9;                                      // 0x1517de
                        break; // 0x1517e0 -> shared tail (0x14433e)
                    }
                    if (m_nState == 9) {
                        // State 9 (0x14133a): the escort engines have fired; hold on the escort while
                        // the follow camera drifts sideways (tempVec.x += 2*delta seated into m_matrix)
                        // and the wreck explosion keeps rumbling. After ~8s snap every landmark's
                        // transform animation to its end, unlatch the look-at camera, hand control back
                        // to the player, hide the intro geometry (m_pGeometry0), clear the cinematic
                        // break and advance to state 10.
                        m_pExplosion->update(delta, (TargetFollowCamera *) nullptr); // 0x141344
                        // Drift the camera-path seat: tempVec.x += 2*delta, re-seat m_matrix translation.
                        Vector *tempVec = &m_tempVec; // this+0x28
                        tempVec->x += (float) (delta * 2); // 0x14134c: sl<<1 -> +2*delta @0x14136c
                        AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, tempVec->x, tempVec->y,
                                                                 tempVec->z); // 0x141378
                        m_nScriptTimerA += delta; // 0x14137c
                        if (m_nScriptTimerA < 8001) { // 0x1f41 @0x141380
                            break; // -> shared tail (0x14433e)
                        }
                        // 0x14139a: snap three of the primary landmark's sub-transforms to their end.
                        // landmarks[0]'s [+0x140] field is PlayerStation::rootGeometry; for each of its
                        // three transform ids we call TransformGetTransform(id)->Update(t->animationLength,
                        // false), i.e. fast-forward the animation to completion.
                        // 0x1413a2-0x14140c: unrolled as transform (+0xc) / childTransform (+0x14) /
                        // parentTransform (+0x10), in that emitted order.
                        {
                            AEGeometry *root =
                                    ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry;
                            AbyssEngine::Transform *t;
                            t = (AbyssEngine::Transform *)
                                    Globals::Canvas->TransformGetTransform(root->transform); // 0x1413b6
                            t->Update((long long) t->animationLength, false);                // 0x1413d6
                            t = (AbyssEngine::Transform *)
                                    Globals::Canvas->TransformGetTransform(root->childTransform); // 0x1413d8
                            t->Update((long long) t->animationLength, false);                     // 0x1413f0
                            t = (AbyssEngine::Transform *)
                                    Globals::Canvas->TransformGetTransform(root->parentTransform); // 0x1413f2
                            t->Update((long long) t->animationLength, false);                      // 0x14140a
                        }
                        m_pCamera->setLookAtCam(false);       // 0x14140c
                        m_pCamera->setTarget(player->geometry); // 0x141416 ([sp+84]+8)
                        player->setComputerControlled(false); // 0x141420
                        player->freeze = 0;                   // 0x141428 (player+0x24)
                        m_pHud->visible = 1;                  // 0x14142c ([this+0xd0]+1)
                        m_pRadar->enabled = 1;             // 0x141434 ([this+0xd4]+0x48)
                        resetCamera(m_pLevel);                // 0x14143c
                        m_pLevel->lodManager->forceUpdate(delta, false); // 0x141444
                        m_pGeometry0->setVisible(false);      // 0x141450 (this+0xb8)
                        m_nState = 10;                        // 0x14145a
                        m_bCinematicBreak = 0; // 0x14145e (cinematic break = 0)
                        break; // 0x141462 -> shared tail (0x14433e)
                    }
                    if (m_nState == 10) {
                        // State 10 (0x1417e4): the escort has departed. Once radio message #9 finishes,
                        // tear down the whole cutscene -- release turret/free-look/first-person locks,
                        // stop shooting, re-enter the cinematic break, hide the HUD/radar, freeze the ship
                        // under AI control and frame the follow camera behind the intro geometry
                        // (m_pGeometry1), seated at its position plus a fixed offset. Reset the timer and
                        // advance to state 11.
                        if (!((RadioMessage *) ((*messages)[9]))->isOver()) { // [fp+4]+0x24 @0x1417e4
                            break; // 0x1417f0 -> shared tail (0x143c02)
                        }
                        player->setTurretMode(false);         // 0x1417f4
                        resetCamera(m_pLevel);                // 0x141802
                        player->setFreeLookMode(false);       // 0x14180c
                        m_pCamera->enableFirstPersonCam(false); // 0x141814
                        player->hideShipForFirstPersonCameraView(false); // 0x14181c
                        player->stopShooting(0);              // 0x141824
                        m_bCinematicBreak = 1; // 0x141832 (cinematic break = 1)
                        m_pHud->visible = 0;                  // 0x141836 ([this+0xd0]+1)
                        m_pRadar->enabled = 0;             // 0x14183e ([this+0xd4]+0x48)
                        player->setComputerControlled(true);  // 0x141842
                        player->freeze = 1;                   // 0x141848 (player+0x24)
                        m_pCamera->setLookAtCam(true);        // 0x14184e
                        m_pCamera->setTarget(m_pGeometry1);   // 0x141856 (this+0xbc)
                        Vector *tempVec = &m_tempVec; // this+0x28
                        *tempVec = m_pGeometry1->getPosition(); // 0x141864
                        *tempVec += Vector{5000.0f, 20000.0f, -20000.0f}; // 0x141876..0x141886
                        m_pCamera->setPosition(*tempVec);     // 0x14188a
                        m_nScriptTimerA = 0; // 0x141892
                        m_nState = 11;                        // 0x141896
                        break; // 0x141898 -> shared tail (0x14433c)
                    }
                    // states 3/4/5 route to the default epilogue (0x143c06).
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
                m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->enabled = 0;
                player->setComputerControlled(true);
                player->freeze = 1; // PlayerEgo+0x24 (byte flag): freeze the ship for the cutscene
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget(m_pGeometry1);
                Vector *camPos = &m_tempVec; // LevelScript tempVec @0x28
                *camPos = m_pGeometry1->getPosition();
                *camPos += Vector{-20000.0f, 5000.0f, -35000.0f};
                m_pCamera->setPosition(*camPos);
                m_nScriptTimerA = 0;
                m_nState = 1;
                Globals::sound->play(1121, nullptr, nullptr, 0.0f); // 0x139ef2/0x139f0c
                // shared tail 0x14433e: the intro path (and the state 10/12 set-state paths that
                // fall in via 0x14433c) route here. Only the escort-hold states 7/8/9 do per-frame
                // camera/geometry work; every other state (including the intro's freshly-set state 1)
                // falls straight through to the post-switch tail (0x144e36).
                if (m_nState >= 7 && m_nState <= 9) { // 0x14433e: (unsigned)(m_nState-7) <= 2
                    // 0x14434a: drift the follow camera down its fixed escort-hold path.
                    m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f,
                                         -(float) delta * 0.3f); // 0.5,0.2,0.3 @0x14446c,0x144480
                    if (m_nState >= 8) { // 0x14438c: geometry/loop work only for states 8/9
                        // 0x1443a2: step the intro geometry's own transform animation.
                        AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(m_pGeometry0->transform); // this+0xb8
                        t->Update((long long) delta, false); // 0x1443c8
                        // 0x1443ca: step the primary landmark's three sub-transforms every frame.
                        // landmarks[0]'s [+0x140] field is PlayerStation::rootGeometry; emitted order
                        // is transform (+0xc) / childTransform (+0x14) / parentTransform (+0x10), true.
                        AEGeometry *root =
                                ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry;
                        t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(root->transform); // 0x1443dc
                        t->Update((long long) delta, true); // 0x1443ec
                        t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(root->childTransform); // 0x1443ee
                        t->Update((long long) delta, true); // 0x1443fe
                        t = (AbyssEngine::Transform *)
                                Globals::Canvas->TransformGetTransform(root->parentTransform); // 0x144400
                        t->Update((long long) delta, true); // 0x144410
                    }
                }
                // -> post-switch tail (0x144e36)
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
                    m_nScriptTimerA = 0;

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
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;

                    AEGeometry *geo = (*enemies)[0]->geometry;
                    m_pCamera->setTarget(geo);
                    Vector *acc = &m_tempVec; // LevelScript tempVec @0x28
                    Vector *tmp = &m_dirVec; // LevelScript scratch vec @0x40
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
                    m_nScriptTimerA = 0;
                    m_nScriptTimerB = 0;
                    m_nState++; // 2 -> 3
                    AEGeometry *geo = (*enemies)[0]->geometry;
                    m_pCamera->setTarget(geo);
                    Vector *acc = &m_tempVec; // LevelScript tempVec @0x28
                    Vector *tmp = &m_dirVec; // LevelScript scratch vec @0x40
                    long long ctrB = m_nScriptTimerB;
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
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 3001) {
                        m_nState = 7;
                    }
                } else if (m_nState == 3) {
                    // State 3 (0x13fcde): advance script-timer-B by delta*0.05, reframe the follow
                    // camera by driving its PaintCanvas local matrix to chase enemies[0]'s basis,
                    // creep the three fighters (2.0/2.3/2.2 * delta), and once radio message #5 is
                    // over enable cloaking on them and advance. FP pool @0x13fef0..0x13ff14:
                    // 0.004/400/0.05/1.7/0.03; move speeds @0x13ff14/0x13ff18 = 2.2/2.3.
                    m_nScriptTimerB =
                            (long long) ((float) delta * 0.05f +
                                         (float) m_nScriptTimerB);
                    {
                        // Reframe: snapshot the camera's PaintCanvas local matrix, rebuild its
                        // translation from enemies[0]'s basis, and push it back (setLocal).
                        AEGeometry *geo = (*enemies)[0]->geometry;
                        m_pCamera->setTarget(geo);
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *acc = &m_tempVec;
                        Vector *tmp = &m_dirVec;
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
                    m_nScriptTimerA = 0;
                    m_nState++;
                } else if (m_nState == 4) {
                    // State 4 (0x13ff1c): like state 3 but also advances script-timer-A by delta;
                    // identical camera reframe + fighter creep. Advances to state 5 once timer-A
                    // passes 2000.
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB =
                            (long long) ((float) delta * 0.05f +
                                         (float) m_nScriptTimerB);
                    {
                        // Reframe: snapshot the camera's PaintCanvas local matrix, rebuild its
                        // translation from enemies[0]'s basis, and push it back (setLocal).
                        AEGeometry *geo = (*enemies)[0]->geometry;
                        m_pCamera->setTarget(geo);
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *acc = &m_tempVec;
                        Vector *tmp = &m_dirVec;
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
                    if (m_nScriptTimerA > 2000) {
                        m_nState++;
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 5) {
                    // State 5 (0x14011a): advance timer-A by delta, creep the fighters faster
                    // (6.0/4.6/4.4 * delta, pool @0x140248), then once timer-A passes 2000 disable
                    // the fighters' AI, park them (field_0x128=50000, field_0x38=4), unfix the
                    // camera onto the player and hand control back through the shared look-behind
                    // tail (0x144fd4/0x144fe2).
                    m_nScriptTimerA += delta;
                    for (int i = 0; i < 3; ++i) {
                        float sp = (i == 0) ? 6.0f : (i == 1 ? 4.6f : 4.4f);
                        (*enemies)[i]->geometry->moveForward(sp * (float) delta);
                    }
                    if (m_nScriptTimerA < 2001) {
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
                        m_pCamera->setPosition(*&m_dirVec);
                    }
                    // shared look-behind tail (0x144fe2): re-show HUD/radar, force a LOD update,
                    // clear the cinematic-break flag, reset timer-A and advance the state.
                    m_pHud->visible = 1;
                    m_pRadar->enabled = 1;   // radar+0x48
                    m_pLevel->lodManager->forceUpdate(delta, false);
                    m_bCinematicBreak = 0;           // clear cinematicBreak_ (m_nFlags high byte @0x11)
                    m_nScriptTimerA = 0;
                    m_nState++;
                } else if (m_nState == 7) {
                    // State 7 (0x140250): finish the docking cutscene once radio message #8 fires.
                    // Dock the player, run the full cutscene-exit teardown, reframe the follow
                    // camera behind the player (dir*7000 + right*700, pool @0x1405fc/0x140600),
                    // then advance. If the radio hasn't fired the original falls to sub-tail
                    // 0x1435ec (still DEFERRED).
                    if (!((RadioMessage *) ((*messages)[8]))->isTriggered()) {
                        // 0x1435ec dispatch: re-reads m_nState (9/10 -> 0x143704, 11 -> 0x144416,
                        // 8 -> 0x143606, anything else -> post-switch tail). When reached from
                        // state 7 (m_nState==7) it lands on the tail. States 8/9/10 are handled by
                        // the else-if branches below; only state 11 remains DEFERRED (see below).
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
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                    m_pHud->visible = 0;
                    m_pRadar->enabled = 0;             // radar+0x48
                    player->player->setVulnerable(false);
                    player->setComputerControlled(true);
                    player->setCollide(false);
                    m_pCamera->setLookAtCam(true);
                    m_pCamera->setTarget(player->geometry);
                    Vector *acc = &m_tempVec;
                    Vector *tmp = &m_dirVec;
                    *acc = player->geometry->getPosition();
                    *tmp = player->geometry->getDirection();
                    *acc += *tmp * 7000.0f;
                    *tmp = player->geometry->getRightVector();
                    *acc += *tmp * 700.0f;
                    m_pCamera->setPosition(*acc);
                    m_nScriptTimerA = 0;
                    m_nState++;
                } else if (m_nState == 8) {
                    // State 8 (0x143606): drift the reference escort ship (enemies[3]) forward while
                    // the script timer ramps; once it passes 8001 ticks reframe the follow camera on
                    // enemies[3]'s basis (dir*10000 + right*4800), bump the state, reset the timer and
                    // re-seat the camera-path accumulator (field_0x28) to {1,1,1}.
                    m_nScriptTimerA += delta; // ldr.w r0,[r6,#144]! (0x143614)
                    (*enemies)[3]->geometry->moveForward((float) (2 * delta)); // 0x143630
                    if (m_nScriptTimerA < 8001) { // movw #0x1f41 (0x143638)
                        break; // -> post-switch tail 0x144e36
                    }
                    m_pCamera->setTarget((*enemies)[3]->geometry); // 0x143656
                    Vector *acc = &m_tempVec; // this+0x28 (sl)
                    Vector *tmp = &m_dirVec; // this+0x40 (r5)
                    *acc = (*enemies)[3]->geometry->getPosition();  // 0x143666
                    *tmp = (*enemies)[3]->geometry->getDirection(); // 0x143686
                    *acc += *tmp * 10000.0f; // pool 0x143950 = 0x461c4000 (0x14369e)
                    *tmp = (*enemies)[3]->geometry->getRightVector(); // 0x1436b6
                    *acc += *tmp * 4800.0f;  // pool 0x143954 = 0x45960000 (0x1436ca)
                    m_pCamera->setPosition(*acc); // 0x1436dc
                    m_nState++;                   // [fp+0x1c]++ = 9 (0x1436e0)
                    m_nScriptTimerA = 0; // strd 0,0 [r6] (0x1436ee)
                    *acc = Vector{1.0f, 1.0f, 1.0f}; // 0x1436f6..0x1436fe re-seat accumulator
                } else if (m_nState == 9 || m_nState == 10) {
                    // States 9/10 (0x143704): keep FMOD event 2249 pinned to enemies[3] and drift the
                    // ship forward with an accelerating creep (tempVec.x += delta*0.006 each tick).
                    // At 2001 ticks promote state 9 -> 10; at 7001 ticks unlatch the fixed camera,
                    // reframe it behind the player (dir*2400), restore HUD/radar, hand control back,
                    // clear the cinematic break, wake the player, park enemies[3] and advance.
                    Vector pos = (*enemies)[3]->geometry->getPosition(); // 0x143710
                    Vector zero{0.0f, 0.0f, 0.0f};
                    Globals::sound->updateEvent3DAttributes(2249, &pos, &zero, false); // 0x8c9 (0x14372e)
                    Vector *acc = &m_tempVec; // this+0x28
                    m_nScriptTimerA += delta; // ldrd/strd [r4+0x90] (0x143740)
                    acc->x += (float) delta * 0.006f; // vmla s4,s0,s2 pool 0x14395c = 0x3bc49ba6 (0x143754)
                    (*enemies)[3]->geometry->moveForward(acc->x * (float) delta); // s0=s4*s0 (0x14376c)
                    if (m_nScriptTimerA < 2001) { // movw #0x7d1 (0x143774)
                        break; // -> post-switch tail 0x144e36
                    }
                    if (m_nState == 9) { // 0x143786
                        m_nState = 10;   // 0x14378a moveq
                    }
                    if (m_nScriptTimerA < 7001) { // movw #0x1b59 (0x143790)
                        break; // -> post-switch tail 0x144e36
                    }
                    m_pCamera->setFixed(false); // 0x1437a4
                    m_pCamera->setTarget(player->geometry); // [sp+0x54]+8 (0x1437b2)
                    m_pCamera->setLookAtCam(false); // 0x1437ba
                    Vector camPos = player->getPosition(); // sret sp+0x164 (0x1437ca)
                    Vector dir = player->GetDirVector();   // sret sp+0x100 (0x1437d4)
                    Vector reframe = camPos - dir * 2400.0f; // pool 0x143960 = 0x45160000 (0x1437e2..0x1437f0)
                    m_pCamera->setPosition(reframe); // 0x1437f8
                    m_pHud->visible = 1;             // [r5+0xd0]+1 (0x143802)
                    m_pRadar->enabled = 1;        // [r5+0xd4]+0x48 (0x143808)
                    resetCamera(m_pLevel);           // 0x14380e
                    m_pLevel->lodManager->forceUpdate(delta, false); // 0x14381c
                    player->setComputerControlled(false); // 0x143824
                    m_bCinematicBreak = 0; // strb 0 [r5+0x11] clear cinematicBreak (0x14382c)
                    player->player->setVulnerable(true); // [r9]->setVulnerable(1) (0x143832)
                    ((KIPlayer *) (*enemies)[3])->setActive(false);  // 0x14383e
                    ((KIPlayer *) (*enemies)[3])->setVisible(false); // 0x143846
                    m_nState++; // 0x13953c: [r5+0x1c]+1 -> stored at 0x13edde
                } else if (m_nState == 11) {
                    // State 11 (0x144416): drift the reference escort ship (enemies[3]) forward with
                    // an accelerating creep (tempVec.x += delta*0.02 each tick), scaled by delta.
                    // No timer gate -- a pure per-frame drift that falls straight into the shared
                    // moveForward call site (0x144e32).
                    Vector *acc = &m_tempVec; // this+0x28
                    acc->x += (float) delta * 0.02f; // vmla s4,s0,s2 pool 0x144490 = 0x3ca3d70a (0x144428)
                    (*enemies)[3]->geometry->moveForward(acc->x * (float) delta); // s0=s4*s0 (0x144e32)
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
            case 102: { // 0x13a658
                if (m_nState != 0) {
                    // 0x13e816: cold substate (m_nState != 0). 6-way tbh on (m_nState-1),
                    // table @0x13e822 (base 0x13e822): prong targets & classification.
                    if (m_nState == 1) {
                        // state 1 @0x13e880: slew the follow-camera and, once the mission radio message
                        // (messages[2]) is over, advance the 64-bit script timer by delta. Both paths then
                        // fall into the shared 2001-tick gate epilogue (DEFERRED 0x143b18).
                        //   receiver [sp+0x58]=this -> [+0x14]=m_pCamera; args s6/s0/s2 = delta*0.6/0.8/-2.5
                        //   (pool 0.6=0x3f19999a @0x13e878, 0.8=0x3f4ccccd @0x13e87c, 2.5=vmov.f32 #4).
                        m_pCamera->translate((float) delta * 0.6f, (float) delta * 0.8f, -(float) delta * 2.5f);
                        // isOver source [fp+4]=messages->data, [+8]=elem 2 (fp==messages, cf. case head 0x13a662).
                        if (((RadioMessage *) ((*messages)[2]))->isOver()) {
                            m_nScriptTimerA += delta; // strd @+0x90 (0x13e8d0)
                        }
                        // Shared cold-state epilogue (0x143b18 -> 0x143b1e, reached by fall-through from
                        // the cold states 1..6). Wait until the 64-bit script timer passes 2001 ticks
                        // (movw #2001; subs/sbcs -> blt exit), then hand control back to the player and
                        // tear the cutscene framing down: re-enable the player ship, drop the look-at cam,
                        // reframe the fixed camera 2400 units behind the player along its facing, restore
                        // the HUD/radar, clear the cinematic-break flag, stop the current music, kick off
                        // event 2241, reset the script timer and advance the state.
                        if (m_nScriptTimerA >= 2001) { // 0x143b1e
                            m_pLevel->getPlayer()->resetGunDelay();                   // 0x143b34/38
                            player->setFreeze(false);                                 // 0x143b46
                            player->setVisible(true);                                 // 0x143b4e
                            player->player->setVulnerable(true);                      // 0x143b56
                            m_pCamera->setLookAtCam(false);                           // 0x143b60
                            m_pCamera->setTarget(player->geometry);                   // [r5+8]=player->geometry, 0x143b6a
                            // reframe: player->getPosition() - player->GetDirVector() * 2400.0f (pool @0x143d48)
                            m_pCamera->setPosition(player->getPosition()
                                                   - player->GetDirVector() * 2400.0f); // 0x143b7a..0x143ba4
                            m_pHud->visible = 1;                                      // [fp+0xd0]+1 = 1, 0x143bae
                            m_pRadar->enabled = 1;                                 // [fp+0xd4]+0x48 = 1, 0x143bb4
                            resetCamera(m_pLevel);                                    // 0x143bbe
                            m_pLevel->lodManager->forceUpdate(delta, false);          // 0x143bcc
                            m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11), 0x143bd2
                            m_nScriptTimerA = 0;                                      // strd 0,0 [r4], 0x143bd8
                            Globals::sound->stop(Globals::sound->currentMusicEvent);  // 0x143be2
                            Globals::sound->play(2241, nullptr, nullptr, 0.0f);       // movw #0x8c1, 0x143bf4
                            m_nState = m_nState + 1; // [fp+0x1c] += 1 -> post-switch tail 0x13999a, 0x143bfc
                        }
                        break;
                    }
                    if (m_nState == 2) {
                        // state 2 @0x140b12 (tbh table @0x13e822[1]=0x1178; verified: sequence with
                        // s3 0x140628 / s4 0x1408ea / s5 0x140a50 / s6 0x140ed8). "FP geometry body."
                        // Advance the 64-bit script timer by delta; hold until it passes 30001 ticks
                        // (movw #0x7531; subs/sbcs -> blt exit). Once past, re-stage the shipGroup==10
                        // escort fighters into a new formation around the player, hand them to the AI,
                        // reframe the fixed camera on the lead escort, and jump straight to state 3.
                        m_nScriptTimerA += delta; // strd @+0x90 (0x140b28)
                        if (m_nScriptTimerA < 30001) { // 0x140b2c
                            break; // -> post-switch tail 0x144e36
                        }
                        Vector playerPos = player->getPosition(); // sp+0x170, 0x140b3a (unused except below)
                        (void) playerPos;
                        // Scratch vectors reuse the sibling LevelScript temp/scratch slots (cf. case 94).
                        Vector *acc = &m_tempVec;   // this+0x28 tempVec
                        Vector *scratch = &m_dirVec; // this+0x40 scratch
                        int firstIdx = -1; // r8: index of the first shipGroup==10 escort
                        for (unsigned int i = 0; i < enemies->count; ++i) {      // loop 0x140d54
                            KIPlayer *e = (*enemies)[i];
                            if (e->shipGroup == 10) {                            // [+0x28]==10, 0x140b56
                                e->awake();                                      // vtable+0xc, 0x140b60
                                if (firstIdx == -1) {                            // 0x140b62
                                    firstIdx = (int) i;
                                }
                                if ((int) i == firstIdx || (int) i == firstIdx + 1) {
                                    // Lead escort (i==firstIdx, 0x140b6e): drop it far to the player's
                                    //   left. pool: x=-81920.3125 @0x140ea8, y=2000.0 @0x140ea4, z=0.0.
                                    // Second escort (i==firstIdx+1, 0x140be6): seat it behind/below the
                                    //   lead escort. pool: x=-2000.0 @0x140eb0, y=-800.0, z=-2000.0.
                                    // Both share the operator+ / setPosition / cloak code at 0x140bfc.
                                    Vector pos;
                                    if ((int) i == firstIdx) {
                                        pos = e->getPosition()                     // vtable+0x28, 0x140b7e
                                              + Vector{-81920.3125f, 2000.0f, 0.0f}; // 0x140bfc
                                    } else {
                                        pos = (*enemies)[firstIdx]->getPosition()  // vtable+0x28, 0x140bea
                                              + Vector{-2000.0f, -800.0f, -2000.0f}; // 0x140bfc
                                    }
                                    e->setPosition(pos);                           // vtable+0x44, 0x140c06
                                    ((PlayerFighter *) e)->cloak(1000, true);      // 0x140c1e
                                } else {
                                    // Remaining escorts (0x140c24): scatter to a random position around the
                                    // player. x/z = (nextInt(2)==0 ? 1 : -1) * (35000 + nextInt(10000)),
                                    //   y = nextInt(10000) - 5000; base = playerPos (arg1 = sp+0x170).
                                    int magX = 35000 + Globals::rnd->nextInt(10000);      // 0x140c32
                                    int signX = (Globals::rnd->nextInt(2) == 0) ? 1 : -1; // 0x140c48
                                    int y = Globals::rnd->nextInt(10000) - 5000;          // 0x140c62/0x140c9a
                                    int magZ = 35000 + Globals::rnd->nextInt(10000);      // 0x140c6e
                                    int signZ = (Globals::rnd->nextInt(2) == 0) ? 1 : -1; // 0x140c88
                                    Vector offset{(float) (signX * magX), (float) y, (float) (signZ * magZ)};
                                    e->setPosition(playerPos + offset);            // 0x140cc6 / 0x140cd2
                                }
                                // Common escort tail (0x140cd4..0x140d4c): drop AI enemies, re-aim, disable AI.
                                e->player->setEnemies(nullptr);                    // [elem+4], 0x140ce0
                                int pick = Globals::rnd->nextInt(5);               // 0x140cee
                                Player *foe = (pick == 4)
                                                  ? player->player                 // [sp+0x54]->player, 0x140d02
                                                  : (*enemies)[pick + 2]->player;  // data[pick+2]->player, 0x140d08
                                e->player->setEnemy(foe);                          // [elem+4], 0x140d10
                                e->awake();                                        // vtable+0xc, 0x140d22
                                e->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f},  // sp+0x118, 0x140d40
                                                          Vector{0.0f, 1.0f, 0.0f}); // sp+0x10c
                                ((PlayerFighter *) e)->setAIDisabled(true);        // 0x140d4c
                            } else if (e->shipGroupFlag == 51) {                  // [+0x7c]==51, 0x140b90/92
                                // shipGroup==51 branch (0x140b98): build a 2-element enemy list from the
                                // first two escorts' Players and hand it to this escort. No delete (0x140bce).
                                Array<Player *> *foes = new Array<Player *>();       // _Znwj(12) + ctor, 0x140ba0
                                ArrayAdd((*enemies)[0]->player, *foes);              // data[0]->player, 0x140bb0
                                ArrayAdd((*enemies)[1]->player, *foes);              // data[1]->player, 0x140bbc
                                e->player->setEnemies(foes);                        // [elem+4], 0x140bca
                            }
                        }
                        // Cutscene teardown + camera reframe on the lead escort (0x140d5e..0x140e8e).
                        player->setTurretMode(false);                            // 0x140d66
                        player->setFreeze(true);                                 // 0x140d72
                        player->setVisible(false);                               // 0x140d7a
                        resetCamera(m_pLevel);                                   // 0x140d84
                        if (player->isInRocketControl()) {                       // 0x140d8a
                            player->setRocketControl(nullptr, nullptr);          // 0x140d96
                            player->killLiberator();                             // 0x140d9c
                            m_pCamera->setRumblePercentage(0.0f, 0);             // 0x140da6
                        }
                        player->setFreeLookMode(false);                          // 0x140dae
                        m_pCamera->setLookAtCam(true);                           // 0x140db6
                        m_pCamera->enableFirstPersonCam(false);                  // 0x140dbe
                        player->hideShipForFirstPersonCameraView(false);         // 0x140dc6
                        player->stopShooting(0);                                 // 0x140dce
                        m_bCinematicBreak = 1; // cinematicBreak_ = 1 (byte @0x11), 0x140dd6
                        m_pHud->visible = 0;                                     // [fp+0xd0]+1 = 0, 0x140dda
                        m_pRadar->enabled = 0;                                // [fp+0xd4]+0x48 = 0, 0x140de0
                        AEGeometry *leadGeo = (*enemies)[firstIdx]->parentGeometry; // [elem+8], 0x140dee
                        m_pCamera->setTarget(leadGeo);                           // 0x140df2
                        *acc = leadGeo->getPosition();                           // op= 0x140e0e
                        *scratch = leadGeo->getDirection();                      // op= 0x140e26
                        *acc += *scratch * 4500.0f;                              // op*/op+= (pool @0x140ec8)
                        *scratch = leadGeo->getRightVector();                    // op= 0x140e54
                        *acc += *scratch * 600.0f;                               // pool @0x140ed0
                        *scratch = leadGeo->getUpVector();                       // op= 0x140e76
                        *acc += *scratch * 400.0f;                               // pool @0x140ed4
                        m_pCamera->setPosition(*acc);                            // 0x140e8e
                        m_nScriptTimerA = 0;      // strd 0,0 @+0x90, 0x140e96
                        m_nState = 3; // tail 0x142f2e (movs r0,#3) -> 0x144c0c str m_nState -> 0x144e36
                        break;
                    }
                    if (m_nState == 3) {
                        // state 3 @0x140628 (tbh table @0x13e822[2]=0x0f03 -> 0x140628; verified). "Heavy FP
                        // geometry advance." Advance the two 64-bit script counters, creep the shipGroup==10
                        // escorts forward at a depth-dependent rate, and drive the fixed follow-camera along
                        // the lead escort's basis; once the 64-bit script timer passes 10001 ticks, hand the
                        // escorts back to the AI, re-enable the player ship, reframe the camera and advance.
                        //
                        // 0x14062a..0x140690: m_nScriptTimerA(64) += delta (0x14065e/0x14066e), and
                        //   m_nScriptTimerB(64) = (long long)((float)delta*0.05f + (float)m_nScriptTimerB(64))
                        //   (l2f @0x14064e, f2lz @0x140682). s2=0.05f @0x140610.
                        m_nScriptTimerA += delta;
                        m_nScriptTimerB = (long long) (
                                (float) delta * 0.05f
                                + (float) m_nScriptTimerB);
                        // Per-frame creep of the shipGroup==10 escorts (loop 0x14069c..0x1406cc). The first
                        // matching escort advances at delta*2.3 (d17=2.3 @0x140620, s22); the rest at
                        // delta*2.2 (d16=2.2 @0x140618, s20). The double multiplies are the exact FP source.
                        float leadStep = (float) ((double) delta * 2.3);   // vmul.f64 d17, cvt s22
                        float restStep = (float) ((double) delta * 2.2);   // vmul.f64 d16, cvt s20
                        int firstIdx = -1; // r4
                        for (unsigned int i = 0; i < enemies->count; ++i) {
                            KIPlayer *e = (*enemies)[i];
                            if (e->shipGroup != 10) { // [elem+0x28]==10, 0x1406a0
                                continue;
                            }
                            AEGeometry *geo = e->parentGeometry; // [elem+8], 0x1406a6
                            if (firstIdx == -1) {
                                firstIdx = (int) i;
                            }
                            if ((int) i == firstIdx) {
                                geo->moveForward(leadStep); // 0x1406b4
                            } else {
                                geo->moveForward(restStep); // 0x1406be
                            }
                        }
                        // Drive the fixed follow-camera along the lead escort's basis (0x1406ce..0x14080c).
                        // Take the render camera's current local matrix, offset its translation along the
                        // lead escort geometry's dir/right/up basis, then fix + re-apply the matrix.
                        //   dir  * delta*2.04  (s0=2.04f @0x140a24)
                        //   right* delta*0.04  (s0=0.04f @0x140a30)
                        //   up   * (delta*0.05f * 0.004f) (s16 reused, s0=0.004f @0x140a34)
                        AEGeometry *leadGeo = (*enemies)[firstIdx]->parentGeometry; // [elem+8], 0x1406d8
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *tempVec = &m_tempVec;   // this+0x28
                        Vector *scratch = &m_dirVec;   // this+0x40
                        *tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                        *scratch = leadGeo->getDirection();
                        *tempVec += *scratch * ((float) delta * 2.04f);
                        *scratch = leadGeo->getRightVector();
                        *tempVec += *scratch * ((float) delta * 0.04f);
                        *scratch = leadGeo->getUpVector();
                        *tempVec += *scratch * ((float) ((float) delta * 0.05f) * 0.004f);
                        m_pCamera->setFixed(true);
                        AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec);
                        m_pCamera->setLocal(local);
                        // Hold until the 64-bit script timer passes 10001 ticks (movw #0x2711 @0x140812).
                        if (m_nScriptTimerA < 10001) { // 0x14081a
                            break; // -> post-switch tail 0x144e36
                        }
                        // Cutscene teardown + reframe (0x140824..0x1408e6). Re-enable AI on the shipGroup==10
                        // escorts, drop the player out of the cutscene framing, reset the camera, seat it
                        // 2400 units behind the player along its facing, restore HUD/radar, clear the
                        // cinematic-break flag, advance the state and reset the script timer.
                        for (unsigned int i = 0; i < enemies->count; ++i) {
                            KIPlayer *e = (*enemies)[i];
                            if (e->shipGroup == 10) { // [elem+0x28]==10, 0x140830
                                ((PlayerFighter *) e)->setAIDisabled(false); // 0x140838
                            }
                        }
                        player->setFreeze(false);            // 0x14084e
                        player->setVisible(true);            // 0x140856
                        m_pCamera->setFixed(false);          // 0x140864
                        m_pCamera->setTarget(player->geometry); // [player+8], 0x14086c
                        m_pCamera->setLookAtCam(false);      // 0x140874
                        // reframe: player->getPosition() - player->GetDirVector() * 2400.0f (pool @0x140a38)
                        m_pCamera->setPosition(player->getPosition()
                                               - player->GetDirVector() * 2400.0f); // 0x140884..0x1408ae
                        m_pHud->visible = 1;                 // [fp+0xd0]+1 = 1, 0x1408b6
                        m_pRadar->enabled = 1;            // [fp+0xd4]+0x48 = 1, 0x1408be
                        resetCamera(m_pLevel);               // 0x1408c6
                        m_pLevel->lodManager->forceUpdate(delta, false); // 0x1408d2
                        m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11), 0x1408d8
                        m_nState = m_nState + 1;             // [fp+0x1c] += 1, 0x1408dc/0x1408de
                        m_nScriptTimerA = 0; // strd 0,0 @+0x90, 0x1408e2
                        break; // -> post-switch tail 0x144e36 (0x1408e6)
                    }
                    if (m_nState == 4) {
                        // state 4 @0x1408ea (tbh table @0x13e822[3]=0x1064 -> 0x1408ea; verified).
                        // Per-frame: advance the 64-bit script timer by delta (strd @+0x90, 0x1408ea).
                        // Every 60000-tick (0xea60 @0x140902) cycle, respawn the shipGroup==10 escorts:
                        // any dead one is revived, woken, and re-scattered to a random spot around the
                        // player; the timer then resets to 0. Then, unconditionally each frame, retarget
                        // every dead escort's AI onto the first surviving escort (or the player), and once
                        // the mission's status value passes 10 finish the wave -- mark the mission solved
                        // (setType(4)), hand control back to the player, reframe the camera on the lead
                        // escort and park/deactivate the remaining escorts far away.
                        m_nScriptTimerA += delta; // 0x1408ee..0x1408fe
                        if (m_nScriptTimerA > 60000) { // 0x140902..0x14090c
                            Vector playerPos = player->getPosition(); // sp+0x170, 0x140918
                            for (unsigned int i = 0; i < enemies->count; ++i) { // loop 0x140934..0x140a0a
                                KIPlayer *e = (*enemies)[i];
                                if (e->shipGroup != 10) { // [elem+0x28]==10, 0x14093a
                                    continue;
                                }
                                if (!e->isDead()) { // 0x140940
                                    continue;
                                }
                                e->revive();  // vtable+0x18, 0x140950
                                e->awake();   // vtable+0xc, 0x14095e
                                // Scatter to a random spot around the player (0x140960..0x1409ee):
                                //   x/z = (nextInt(2)==0 ? 1 : -1) * (35000 + nextInt(10000)),
                                //   y = nextInt(10000) - 5000. Base = playerPos.
                                int magX = 35000 + Globals::rnd->nextInt(10000);      // 0x140974/0x14096e
                                int signX = (Globals::rnd->nextInt(2) == 0) ? 1 : -1; // 0x140980
                                int y = Globals::rnd->nextInt(10000) - 5000;          // 0x14099a/0x1409c0
                                int magZ = 35000 + Globals::rnd->nextInt(10000);      // 0x1409a8
                                int signZ = (Globals::rnd->nextInt(2) == 0) ? 1 : -1; // 0x1409b4
                                Vector offset{(float) (signX * magX), (float) y, (float) (signZ * magZ)};
                                e->setPosition(playerPos + offset); // vtable+0x44, 0x1409f2/0x140a00
                            }
                            m_nScriptTimerA = 0; // strd 0,0 @+0x90, 0x140a0c
                        }
                        // Unconditional every frame (0x143974): retarget each dead escort's AI onto the
                        // first surviving escort among enemies[2..5], or onto the player if none survive.
                        for (unsigned int i = 0; i < enemies->count; ++i) { // 0x143978..0x1439de
                            KIPlayer *e = (*enemies)[i];
                            if (e->shipGroup != 10) { // [elem+0x28]==10, 0x14397e
                                continue;
                            }
                            if (!e->player->getEnemy(0)->isDead()) { // 0x143988/0x14398c
                                continue;
                            }
                            int alive = -1; // r1: index of first surviving escort in [2..5]
                            for (int j = 2; j <= 5; ++j) { // 0x14399a..0x1439ba
                                if (!(*enemies)[j]->isDead()) {
                                    alive = j;
                                    break;
                                }
                            }
                            Player *foe = (alive < 0) ? player->player               // [sp+0x54]->player, 0x1439ce
                                                      : (*enemies)[alive]->player;    // data[alive]->player, 0x1439c6
                            e->player->setEnemy(foe); // 0x1439d2
                        }
                        // Wave-complete gate (0x1439e0): only finish once the mission status value passes 10.
                        if (((Mission *) Globals::status->getCampaignMission())->getStatusValue() >= 10) {
                            break; // -> post-switch tail 0x144e36
                        }
                        ((Mission *) Globals::status->getCampaignMission())->setType(4); // 0x1439fc
                        m_nState = m_nState + 1; // [this+0x1c] += 1, 0x143a0a..0x143a0e
                        // Cutscene teardown: drop the player out of the cutscene framing (0x143a10..0x143a70).
                        player->setTurretMode(false);       // 0x143a12
                        resetCamera(m_pLevel);              // 0x143a1a
                        if (player->isInRocketControl()) {  // 0x143a20
                            player->setRocketControl(nullptr, nullptr); // 0x143a2c
                            player->killLiberator();        // 0x143a32
                            m_pCamera->setRumblePercentage(0.0f, 0); // 0x143a3c
                        }
                        player->setFreeLookMode(false);     // 0x143a44
                        m_pCamera->enableFirstPersonCam(false); // 0x143a4c
                        player->hideShipForFirstPersonCameraView(false); // 0x143a54
                        player->stopShooting(0);            // 0x143a5c
                        player->setFreeze(true);            // 0x143a68
                        player->setVisible(false);          // 0x143a70
                        player->player->setVulnerable(false); // [player+0]=player->player, 0x143a78
                        m_bCinematicBreak = 1; // cinematicBreak_ = 1 (byte @0x11), 0x143a82
                        m_pHud->visible = 0;                // [fp+0xd0]+1 = 0, 0x143a86
                        m_pRadar->enabled = 0;           // [fp+0xd4]+0x48 = 0, 0x143a8e
                        m_pCamera->setLookAtCam(false);     // 0x143a94
                        // Reframe the camera on the lead escort (enemies[0]): its parentGeometry becomes
                        // the target, and the camera is seated at enemies[0]->getPosition() + {-7000, 9000,
                        // 40000} (pool @0x143d38/d3c/d40). 0x143a98..0x143ad4.
                        m_pCamera->setTarget((*enemies)[0]->parentGeometry); // [elem+8], 0x143aa0
                        m_pCamera->setPosition((*enemies)[0]->getPosition()  // vtable+0x28, 0x143ab4
                                               + Vector{-7000.0f, 9000.0f, 40000.0f}); // 0x143aca
                        m_nScriptTimerA = 0; // strd 0,0 @+0x90, 0x143ada
                        // Park + deactivate the remaining escorts enemies[2..9] far away (0x143ae4..0x143b12).
                        for (int i = 2; i != 10; ++i) {
                            (*enemies)[i]->setPosition(1000000.0f, 1000000.0f, 1000000.0f); // vtable+0x48, 0x143af4
                            (*enemies)[i]->setActive(false);  // 0x143afe
                            (*enemies)[i]->setVisible(false); // 0x143b0a
                        }
                        break; // -> post-switch tail 0x144e36 (0x143b14)
                    }
                    if (m_nState == 5) {
                        // state 5 @0x140a50 (tbh table @0x13e822[4]=0x1117 -> 0x140a50; verified).
                        // Per-frame: creep the lead escort forward, drift the fixed follow-camera along
                        // it, keep the 3D audio event pinned to the escort, advance the 64-bit script
                        // timer; once it passes 8001 ticks reset the timer and advance the state.
                        //
                        // 0x140a50..0x140a6e: enemies[0]->geometry->moveForward((float)(delta*6)).
                        //   r8=delta; r0 = delta + delta*2 = delta*3, <<1 -> delta*6 (add.w r0,r8,r8,lsl#1;
                        //   lsls r0,#1); vcvt s32->f32; receiver [enemies+4][0]->[+8]=geometry.
                        (*enemies)[0]->geometry->moveForward((float) (delta * 6));
                        // 0x140a72..0x140aae: m_pCamera->translate(delta*0.4f, delta*0.1f, (float)-delta*1.8f).
                        //   s0=(float)delta, s2=(float)-delta; pool s4=0.4f @0x140a44 (0x3ecccccd),
                        //   s6=0.1f @0x140a48 (0x3dcccccd), s8=1.8f @0x140a4c (0x3fe66666).
                        m_pCamera->translate((float) delta * 0.4f, (float) delta * 0.1f,
                                             -(float) delta * 1.8f);
                        // 0x140ab2..0x140ac6: m_nScriptTimerA(64) += delta (ldr.w r0,[r5,#144]!; adc asr#31; strd).
                        m_nScriptTimerA += delta;
                        // 0x140aca..0x140af0: pin FMOD event 2250 (0x8ca) to the lead escort's live position.
                        //   pos = enemies[0]->geometry->getPosition() (sret sp+0x170), velocity = {0,0,0}
                        //   (strd 0,0 @sp#280 / str 0 @sp#288), bool last = false (str 0 @sp#0).
                        Vector enemyPos = (*enemies)[0]->geometry->getPosition();
                        Vector zero{0.0f, 0.0f, 0.0f};
                        Globals::sound->updateEvent3DAttributes(2250, &enemyPos, &zero, false);
                        // 0x140af4..0x140b02: hold until the 64-bit script timer passes 8001 (movw #0x1f41).
                        if (m_nScriptTimerA < 8001) {
                            break; // -> post-switch tail 0x144e36
                        }
                        // 0x140b06..0x140b0e: reset the timer and advance the state.
                        m_nScriptTimerA = 0; // strd 0,0 [r5]
                        m_nState = m_nState + 1; // [r4+28]+1 -> 0x144c0c str m_nState -> 0x144e36
                        break;
                    }
                    if (m_nState == 6) {
                        // state 6 @0x140ed8 (tbh table @0x13e822[5]=0x135b -> 0x140ed8; verified).
                        // LAST cold substate: creep the lead escort straight forward, keep the 3D audio
                        // event pinned to it, advance the 64-bit script timer; once it passes 4001 ticks
                        // hand control back to the player, tear the cutscene framing down and reframe the
                        // fixed follow-camera 100 units behind the player, then advance the state.
                        //
                        // 0x140ed8..0x140ee6: pin FMOD event 2250 (0x8ca) to the lead escort's live pos.
                        //   pos = enemies[0]->geometry->getPosition() (sret sp+0x170); receiver
                        //   [enemies+4][0]->[+8]=geometry. velocity = {0,0,0} (strd 0,0 @sp#280 /
                        //   str 0 @sp#288), bool last = false (str 0 @sp#0). GOT _ZN7Globals5soundE @0x210168.
                        Vector enemyPos = (*enemies)[0]->geometry->getPosition();
                        Vector zero{0.0f, 0.0f, 0.0f};
                        Globals::sound->updateEvent3DAttributes(2250, &enemyPos, &zero, false);
                        // 0x140f0c..0x140f36: m_nScriptTimerA(64) += delta (ldr.w r0,[r4,#144]!; adc asr#31; strd).
                        //   s0 = (float)(delta*200) computed in parallel for the moveForward below (mul #0xc8).
                        m_nScriptTimerA += delta;
                        // 0x140f38..0x140f40: enemies[0]->geometry->moveForward((float)(delta*200)).
                        (*enemies)[0]->geometry->moveForward((float) (delta * 200));
                        // 0x140f42..0x140f50: hold until the 64-bit script timer passes 4001 (movw #0xfa1).
                        if (m_nScriptTimerA < 4001) {
                            break; // -> post-switch tail 0x144e36
                        }
                        // Cutscene teardown + camera reframe (0x140f54..0x140ffa).
                        m_pLevel->getPlayer()->resetGunDelay();  // [r5+0x18]=m_pLevel, 0x140f54/5a
                        player->setFreeze(false);                // [sp+0x54]=player, 0x140f66
                        player->setVisible(true);                // 0x140f6e
                        player->player->setVulnerable(true);     // [player+0]=player->player, 0x140f78
                        m_pCamera->setLookAtCam(false);          // [r5+0x14]=m_pCamera, 0x140f80
                        m_pCamera->setTarget(player->geometry);  // [player+8]=player->geometry, 0x140f8a
                        m_pCamera->setPosition(
                                *&m_dirVec); // &this+0x40 scratch, 0x140f94
                        // reframe: player->getPosition() - player->GetDirVector() * 100.0f (pool @0x141270).
                        m_pCamera->setPosition(player->getPosition()
                                               - player->GetDirVector() * 100.0f); // 0x140f98..0x140fce
                        m_pHud->visible = 1;                     // [r5+0xd0]+1 = 1, 0x140fd2/d8
                        m_pRadar->enabled = 1;                // [r5+0xd4]+0x48 = 1, 0x140fda/de
                        resetCamera(m_pLevel);                   // [r5+0x18]=m_pLevel, 0x140fe6
                        m_pLevel->lodManager->forceUpdate(delta, false); // 0x140ff2
                        m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11), 0x140ff6
                        m_nScriptTimerA = 0; // strd 0,0 @+0x90, 0x140ffa
                        m_nState++; // shared tail 0x13953c increments m_nState -> 0x13edde
                        break; // -> shared set-state tail (0x140ffe -> 0x13953c)
                    }
                    break;
                }
                // m_nState==0 entry (0x13a662): gate on messages[0]->isTriggered(). When false the
                // beq targets 0x13e812, whose `subs #1; cmp #5; bhi` (0u-1 > 5) falls to the 0x144e36
                // tail -> a plain break with no side effects.
                if (!((RadioMessage *) ((*messages)[0]))->isTriggered()) {
                    break; // 0x13a66e beq 0x13e812 -> 0x144e36
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
                m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->enabled = 0;
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget((*enemies)[0]->geometry);
                Vector camPos = (*enemies)[0]->getPosition() + Vector{9000.0f, -7000.0f, 40000.0f};
                m_pCamera->setPosition(camPos);
                // 0x13a744..0x13a74e: stop the current music (GOT _ZN7Globals5soundE, identical idiom
                // to the shared epilogue @0x143bd0). r4=Globals::sound, r0=[r4]=currentMusicEvent, r1=[r0].
                Globals::sound->stop(Globals::sound->currentMusicEvent); // 0x13a74e
                // 0x13a752..0x13a760: play(0x8c0=2240, nullptr, nullptr, 0.0f) (r8==0 pushed as pitch).
                Globals::sound->play(2240, nullptr, nullptr, 0.0f);      // 0x13a760
                m_nScriptTimerA = 0;
                m_nState = 1; // shared tail 0x143d0c increments m_nState (0 -> 1) -> post-switch tail
                break;
            }
            case 154: { // 0x13a76e
                if (m_nState != 0) {
                    // Cold-substate dispatch @0x13e8e4: a 10-way tbh on (m_nState-1). States 1..10
                    // each drive one beat of the post-cutscene escort/dock sequence. States >10 fall
                    // straight through to the post-switch tail (0x144e36). Simple beats are decoded
                    // here; the matrix-aim and long dwell-tail beats are left DEFERRED.
                    //
                    // ========================================================================
                    // CANONICAL DWELL-TAIL PATTERN (proven, reusable across case 154 states
                    // 1/2/3/5/6/9 and the analogous per-state escort beats).
                    //
                    // Each cold substate first (optionally) runs a camera-creep / target beat,
                    // then -- only when its radio message isOver() -- bumps the 64-bit primary
                    // script timer m_nScriptTimerA:m_nScriptCounterA @this+0x90 by delta. If the
                    // radio is NOT over, control falls straight through to the post-switch tail
                    // (0x144e36) -- the `break` below. Once the radio is over, the accumulated
                    // timer is compared against a per-state threshold; when it is reached the
                    // DWELL TAIL fires:
                    //
                    //     if (radio.isOver()) {
                    //         m_nScriptTimerA += delta;
                    //         if (m_nScriptTimerA >= <thr>) {
                    //             <teardown beat: camera retarget to enemies[k], player
                    //              setSpeed(0)/setFixed(false), LOD forceUpdate, ...>   // DEFERRED
                    //             m_nScriptTimerA = 0;   // strd 0
                    //             m_nState = m_nState + 1;                              // advance
                    //         }
                    //     }
                    //
                    // ARM shape of the gate (all five load timerA via `ldr rX,[base,#144]!`,
                    // i.e. base := this+0x90, then compare the 64-bit value against the movw
                    // threshold with subs/sbcs + blt to 0x144e36):
                    //     ldr.w r0,[base,#144]!   ; base = &m_nScriptTimerA
                    //     ldr   r1,[base,#4]      ; high word = m_nScriptCounterA
                    //     movw  r2,#<thr>
                    //     subs  r0,r0,r2 / sbcs r0,r1,#0 / blt.w 144e36   ; if (timer < thr) break
                    // The tail either advances m_nState inline (`str r,[this,#0x1c]` after
                    // `ldrd .. [this,#0x18]; adds r,#1`) or funnels through the shared
                    // continuation at 0x144a08 which does the same `[this+0x1c] += 1`. Every
                    // tail resets the 64-bit timer to 0 (`strd r,r,[&m_nScriptTimerA]`).
                    //
                    // PER-STATE THRESHOLD TABLE (radio message index -> threshold -> advance):
                    //   s1  msg#1  thr 1501  gate 0x14444c  advance inline (0x144510)  (E0 @0x13e908)
                    //   s2  msg#2  thr 1001  gate 0x14452e  advance inline (0x144648)  (E1 @0x142f34)
                    //   s3  msg#3  thr 1001  gate 0x1445de  advance via 0x144834->0x144a08 (E2 @0x1430d6)
                    //   s5  msg#5  thr 1001  gate 0x1446e0  advance inline (0x144746)  (E4 @0x1432c2)
                    //   s6  msg#6  thr 1001  gate 0x1447b8  advance via 0x144834->0x144a08 (E5 @0x14330e)
                    //   s9  msg#7  thr 2001  gate 0x14484e  hostile-wave arm loop -> 0x144a08 (E8 @0x1434f2)
                    //
                    // The teardown BODIES (camera retarget to enemies[k], per-ship KIPlayer/
                    // Player toggles, hostile-wave arming) are left DEFERRED at their addresses;
                    // the proven threshold gate + timer reset + state advance are implemented.
                    // ========================================================================
                    if (m_nState == 1) { // prong E0 @0x13e908
                        // Halt the player and force an LOD refresh, then nudge every shipGroup==9
                        // escort forward by delta*0.5 units. Once radio message #1 is over the primary
                        // script timer starts running (into the DEFERRED 0x14444c dwell tail).
                        player->setSpeed(0.0f);
                        m_pLevel->lodManager->forceUpdate(delta, false);
                        float step = (float) delta * 0.5f; // @0x13e926
                        for (unsigned int i = 0; i < enemies->count; ++i) {
                            if ((*enemies)[i]->shipGroup == 9) {
                                (*enemies)[i]->geometry->moveForward(step);
                            }
                        }
                        if (((RadioMessage *) ((*messages)[1]))->isOver()) {
                            m_nScriptTimerA += delta; // 0x13e96a
                            // DWELL TAIL 0x14444c: dwell 1501 ticks, then teardown + advance.
                            if (m_nScriptTimerA >= 1501) {
                                // 0x14445a: deactivate every shipGroup==9 escort (setActive(false),
                                // setVisible(false)), then reframe the camera onto the player and
                                // refresh LOD.
                                for (unsigned int i = 0; i < enemies->count; ++i) {
                                    if ((*enemies)[i]->shipGroup == 9) {
                                        (*enemies)[i]->setActive(false);  // 0x1444a2
                                        (*enemies)[i]->setVisible(false); // 0x1444b0
                                    }
                                }
                                m_pCamera->setTarget(player->geometry);   // 0x1444c4
                                Vector camPos = player->getPosition();    // 0x1444ce
                                camPos += Vector{-350.0f, 400.0f, -1500.0f}; // 0x1444de pool
                                m_pCamera->setPosition(camPos);           // 0x1444f8
                                m_pCamera->setFixed(false);               // 0x144504
                                m_pLevel->lodManager->forceUpdate(delta, false); // 0x144518
                                m_nScriptTimerA = 0; // 0x14451c strd 0
                                m_nState = m_nState + 1; // 0x144510 advance
                            }
                        }
                        // DEFERRED 0x144444: radio-not-over -> shared tail.
                        break;
                    }
                    if (m_nState == 2) { // prong E1 @0x142f34
                        // Halt the player, refresh LOD, and creep the dock target (enemies[0]) forward
                        // by delta*0.25. Then run the CANONICAL cinematic camera-aim chain (see the
                        // write-up above case 131 state 2): take the render camera's current local
                        // matrix, offset its translation along the player's dir/right/up basis
                        // (dir*step*0.008 + right*step*-0.05 + up*step*0.0051, with step = delta*0.25),
                        // fix the follow camera and re-drive it with the retranslated matrix. Once radio
                        // message #2 is over, advance the 64-bit script counter by delta.
                        player->setSpeed(0.0f);                              // 0x142f3c
                        m_pLevel->lodManager->forceUpdate(delta, false);     // 0x142f4e
                        float step = (float) delta * 0.25f;                  // 0x142f64
                        (*enemies)[0]->geometry->moveForward(step);          // 0x142f6e
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *tempVec = &m_tempVec;
                        Vector *scratch = &m_dirVec;
                        *tempVec = AbyssEngine::AEMath::MatrixGetPosition(local); // 0x142f9a
                        *scratch = player->GetDirVector();                   // 0x142fb6
                        *tempVec += *scratch * (step * 0.008f);              // 0x142fce
                        *scratch = player->geometry->getRightVector();       // 0x142ff6
                        *tempVec += *scratch * (step * -0.05f);              // 0x143008
                        *scratch = player->GetUpVector();                    // 0x143020
                        *tempVec += *scratch * (step * 0.0051f);            // 0x143032
                        m_pCamera->setFixed(true);                           // 0x14304a
                        AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec); // 0x14305a
                        m_pCamera->setLocal(local);                          // 0x1430a6
                        if (((RadioMessage *) ((*messages)[2]))->isOver()) { // 0x1430b0
                            m_nScriptTimerA += delta; // 0x1430ce
                            // DWELL TAIL 0x14452e: dwell 1001 ticks, then teardown + advance.
                            if (m_nScriptTimerA >= 1001) {
                                // 0x14453c: halt the player, unfix the camera, then reframe onto the
                                // lead escort getEnemies()[0]->geometry and refresh LOD (advance runs
                                // through the shared continuation 0x144834 -> 0x144a08).
                                player->setSpeed(0.0f);                    // 0x144544
                                m_pCamera->setFixed(false);                // 0x14454e
                                AEGeometry *geom0 = (*m_pLevel->getEnemies())[0]->geometry; // 0x144556
                                m_pCamera->setTarget(geom0);               // 0x144562
                                Vector camPos = (*m_pLevel->getEnemies())[0]->geometry->getPosition(); // 0x144578
                                camPos += Vector{0.0f, 4000.0f, 26000.0f}; // 0x144586 pool
                                m_pCamera->setPosition(camPos);            // 0x14459e
                                m_nScriptTimerA = 0; // 0x1445a2 strd 0
                                m_pLevel->lodManager->forceUpdate(delta, false); // 0x144834
                                m_nState = m_nState + 1; // 0x144a08 shared advance
                            }
                        }
                        // DEFERRED 0x144524: radio-not-over -> shared dwell tail.
                        break;
                    }
                    if (m_nState == 3) { // prong E2 @0x1430d6
                        if (((RadioMessage *) ((*messages)[3]))->isOver()) {
                            m_nScriptTimerA += delta; // 0x1430ec
                            // DWELL TAIL 0x1445de: dwell 1001 ticks, then teardown + advance.
                            if (m_nScriptTimerA >= 1001) {
                                // 0x1445ec: halt the player, reframe the camera onto the player itself
                                // at a fixed offset, unfix it, refresh LOD, then advance inline.
                                player->setSpeed(0.0f);                    // 0x1445f6
                                m_pCamera->setTarget(player->geometry);    // 0x144600
                                Vector camPos = player->getPosition();     // 0x14460c
                                camPos += Vector{-350.0f, 400.0f, -1500.0f}; // 0x14461c pool
                                m_pCamera->setPosition(camPos);            // 0x144634
                                m_pCamera->setFixed(false);                // 0x14463c
                                m_nState = m_nState + 1; // 0x144648 advance (inline)
                                m_pLevel->lodManager->forceUpdate(delta, false); // 0x14464e
                                m_nScriptTimerA = 0; // 0x144652 strd 0
                            }
                        }
                        // DEFERRED 0x1445aa/0x1445b2: shared dwell tail.
                        break;
                    }
                    if (m_nState == 4) { // prong E3 @0x143138
                        // Idiom A (camera local-matrix creep), sibling of state 2. Creep the dock
                        // target (enemies[0]) forward by step = delta*0.25, then offset the render
                        // camera's local matrix translation along the player's dir/right/up basis
                        // (dir*step*0.008 + right*step*-0.05 + up*step*0.005). Once radio message #4
                        // is over, advance the 64-bit script timer by delta.
                        float step = (float) delta * 0.25f;                  // s0=0.25 @0x15313a
                        (*enemies)[0]->geometry->moveForward(step);          // 0x153156
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *tempVec = &m_tempVec;
                        Vector *scratch = &m_dirVec;
                        *tempVec = AbyssEngine::AEMath::MatrixGetPosition(local); // 0x153198
                        *scratch = player->GetDirVector();                   // 0x1531a6
                        *tempVec += *scratch * (step * 0.008f);              // k_dir 0x3c03126f @0x153124
                        *scratch = player->geometry->getRightVector();       // 0x1531e6
                        *tempVec += *scratch * (step * -0.05f);             // k_right 0xbd4ccccd @0x153130
                        *scratch = player->GetUpVector();                    // 0x153208
                        *tempVec += *scratch * (step * 0.005f);             // k_up 0x3ba3d70a @0x153530
                        m_pCamera->setFixed(true);                           // 0x15323a
                        AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec); // 0x15324a
                        m_pCamera->setLocal(local);                          // 0x153296
                        if (((RadioMessage *) ((*messages)[4]))->isOver()) { // 0x1532a0
                            m_nScriptTimerA += delta; // 0x1532b4
                        }
                        // DEFERRED 0x154658: radio-not-over -> shared dwell tail.
                        break;
                    }
                    if (m_nState == 5) { // prong E4 @0x1432c2
                        // Ease the follow camera in (translate by delta * pool@0x143534) then, once
                        // radio message #5 is over, run the primary timer into the DEFERRED tail.
                        // DEFERRED 0x1432c2: camera translate FP arg pool @0x143534.
                        if (((RadioMessage *) ((*messages)[5]))->isOver()) {
                            m_nScriptTimerA += delta; // 0x1432f8
                            // DWELL TAIL 0x1446e0: dwell 1001 ticks, then teardown + advance.
                            if (m_nScriptTimerA >= 1001) {
                                // 0x1446ee: crawl the player to a slow drift, reframe onto the player,
                                // refresh LOD, then seat the dock target (enemies[0]) at its parking
                                // position, re-enable its AI, and light up every shipGroup==9 escort
                                // (setActive(false) + Player::setVulnerable(true)).
                                player->setSpeed(0.02f);                   // 0x1446f4 pool
                                m_pCamera->setTarget(player->geometry);    // 0x1446fe
                                Vector camPos = player->getPosition();     // 0x144708
                                camPos += Vector{150.0f, 400.0f, -1500.0f}; // 0x144718 pool
                                m_pCamera->setPosition(camPos);            // 0x144730
                                m_pCamera->setFixed(false);                // 0x14473a
                                m_nState = m_nState + 1; // 0x144746 advance (inline)
                                m_pLevel->lodManager->forceUpdate(delta, false); // 0x14474c
                                Vector parkPos{-10000.0f, 0.0f, 22000.0f}; // 0x144750 pool
                                (*enemies)[0]->setPosition(parkPos);       // 0x144768 vtable+0x44
                                ((PlayerFighter *) (*enemies)[0])->field_0x13e = 0; // 0x144770
                                ((PlayerFighter *) (*enemies)[0])->setAIDisabled(false); // 0x144774
                                for (unsigned int i = 0; i < enemies->count; ++i) {
                                    if ((*enemies)[i]->shipGroup == 9) {
                                        (*enemies)[i]->setActive(false);        // 0x144788
                                        (*enemies)[i]->player->setVulnerable(true); // 0x144798
                                    }
                                }
                                m_nScriptTimerA = 0; // 0x1447a8 strd 0
                            }
                        }
                        // DEFERRED 0x1446d8/0x1446e0: 1001-tick dwell tail.
                        break;
                    }
                    if (m_nState == 6) { // prong E5 @0x14330e
                        // Idiom A (camera local-matrix creep), sibling of states 2 & 4. Same creep as
                        // state 4 but with a POSITIVE right scalar: dir*step*0.008 + right*step*0.05 +
                        // up*step*0.005, step = delta*0.25. Once radio message #6 is over, advance the
                        // 64-bit script timer by delta.
                        float step = (float) delta * 0.25f;                  // s0=0.25 @0x153310
                        (*enemies)[0]->geometry->moveForward(step);          // 0x15332c
                        AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                        Matrix local = *reinterpret_cast<Matrix *>(
                                canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                        Vector *tempVec = &m_tempVec;
                        Vector *scratch = &m_dirVec;
                        *tempVec = AbyssEngine::AEMath::MatrixGetPosition(local); // 0x15336e
                        *scratch = player->GetDirVector();                   // 0x15337c
                        *tempVec += *scratch * (step * 0.008f);              // k_dir 0x3c03126f @0x153124
                        *scratch = player->geometry->getRightVector();       // 0x1533bc
                        *tempVec += *scratch * (step * 0.05f);              // k_right 0x3d4ccccd @0x153548
                        *scratch = player->GetUpVector();                    // 0x1533de
                        *tempVec += *scratch * (step * 0.005f);             // k_up 0x3ba3d70a @0x153530
                        m_pCamera->setFixed(true);                           // 0x153410
                        AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec); // 0x153420
                        m_pCamera->setLocal(local);                          // 0x15346c
                        if (((RadioMessage *) ((*messages)[6]))->isOver()) { // 0x153476
                            m_nScriptTimerA += delta; // 0x15348c
                            // DWELL TAIL 0x1447b8: dwell 1001 ticks, then teardown + advance.
                            if (m_nScriptTimerA >= 1001) {
                                // 0x1447c6: halt the player, unfix the camera, reframe onto the lead
                                // escort getEnemies()[0]->geometry at a fixed offset, then refresh LOD
                                // and advance through the shared continuation 0x144834 -> 0x144a08.
                                player->setSpeed(0.0f);                    // 0x1447ce
                                m_pCamera->setFixed(false);                // 0x1447d8
                                AEGeometry *geom0 = (*m_pLevel->getEnemies())[0]->geometry; // 0x1447e0
                                m_pCamera->setTarget(geom0);               // 0x1447ec
                                Vector camPos = (*m_pLevel->getEnemies())[0]->geometry->getPosition(); // 0x144802
                                camPos += Vector{200.0f, 300.0f, 1800.0f}; // 0x144810 pool
                                m_pCamera->setPosition(camPos);            // 0x14482a
                                m_nScriptTimerA = 0; // 0x14482e strd 0
                                m_pLevel->lodManager->forceUpdate(delta, false); // 0x144834
                                m_nState = m_nState + 1; // 0x144a08 shared advance
                            }
                        }
                        // DEFERRED 0x1547ae: radio-not-over -> shared dwell tail.
                        break;
                    }
                    if (m_nState == 7) { // prong E6 @0x14349c
                        // Wait for the lead escort enemies[0] to dock; when it does, reset the primary
                        // script timer and advance to state 8. Otherwise fall through to the tail.
                        if ((*enemies)[0]->isDocked()) {
                            m_nScriptTimerA = 0; // 0x1434b4
                            m_nState = m_nState + 1; // 7 -> 8
                        }
                        break; // -> post-switch tail (0x144e36)
                    }
                    if (m_nState == 8) { // prong E7 @0x1434c0
                        // Dwell for 2001ms on the primary script timer, then jump to state 9 and rearm.
                        m_nScriptTimerA += delta; // 0x1434c4
                        if (m_nScriptTimerA >= 2001) {
                            m_nState = 9;
                            m_nScriptTimerA = 0;
                        }
                        break; // -> post-switch tail (0x144e36)
                    }
                    if (m_nState == 9) { // prong E8 @0x1434f2
                        if (((RadioMessage *) ((*messages)[7]))->isOver()) {
                            m_nScriptTimerA += delta; // 0x143508
                            // DWELL TAIL 0x14485c: dwell 2001 ticks, then arm the hostile escort
                            // wave and hand control back to the player. This is the largest tail.
                            if (m_nScriptTimerA >= 2001) {
                                // Arm loop 0x14486a..0x1448fe: iterate every escort with
                                // shipGroup==9; reactivate it and point its Player at a target.
                                // The target alternates: enemies with index i%3==0 hunt the lead
                                // escort enemies[0], the rest hunt the player's own ship. (r5 =
                                // 3*(i/3), and the target is chosen by `r5 == i`.)
                                for (unsigned int i = 0; i < enemies->count; ++i) { // 0x14486a
                                    KIPlayer *elem = (*enemies)[i];
                                    if (elem->shipGroup != 9) { // [elem+0x28]==9, 0x144870
                                        continue;
                                    }
                                    elem->field_0x25 = 1;         // 0x14487a strb 1 -> [elem+0x25]
                                    elem->setActive(true);        // 0x144882
                                    elem->setVisible(true);       // 0x14488e
                                    ((PlayerFighter *) elem)->setAIDisabled(false); // 0x14489a
                                    Player *target = (i % 3 == 0)          // 0x1448a0 cmp r5(=3*(i/3)),i
                                                         ? (*enemies)[0]->player   // 0x1448aa
                                                         : player->player;         // 0x1448e8 [sp+0x54]
                                    elem->player->setEnemy(target); // 0x1448ec
                                }
                                // Boss escort enemies[1]: promote to hostile shipGroup 8 and clear
                                // its wreck-latch flag (0x14490c..0x154948, r6 = Level::getEnemies).
                                (*m_pLevel->getEnemies())[1]->shipGroup = 8;  // 0x154912 str 8 -> [+0x28]
                                (*m_pLevel->getEnemies())[1]->field_0x74 = 0; // 0x154920 strb 0 -> [+0x74]
                                (*m_pLevel->getEnemies())[1]->player->setAlwaysFriend(false); // 0x154932
                                (*m_pLevel->getEnemies())[1]->player->setAlwaysEnemy(true);   // 0x154948
                                // Lead escort enemies[0]: clear its enemy list and make it vulnerable.
                                (*m_pLevel->getEnemies())[0]->player->setEnemies(nullptr); // 0x15495a
                                (*m_pLevel->getEnemies())[0]->player->setVulnerable(true);  // 0x15496c
                                // Camera / player-control teardown (0x144970..0x1449c6): unfix the
                                // follow camera, reset the player's gun delay, re-target the camera
                                // onto the player's ship and hand control back (unfreeze, cruise
                                // speed, player-driven, vulnerable, respawn at z=90000).
                                m_pCamera->setFixed(false);          // 0x144976
                                m_pLevel->getPlayer()->resetGunDelay(); // 0x144982
                                m_pCamera->setLookAtCam(false);      // 0x14498c
                                m_pCamera->setTarget(player->geometry); // 0x144998 [player+8]
                                player->setFreeze(false);            // 0x1449a0
                                player->setSpeed(2.0f);              // 0x1449aa 0x40000000
                                player->setComputerControlled(false); // 0x1449b2
                                player->player->setVulnerable(true); // 0x1449ba
                                player->setPosition(0.0f, 0.0f, 90000.0f); // 0x1449c6 z=0x47afc800
                                m_pHud->visible = 1;                 // 0x1449ce strb 1 -> [m_pHud+1]
                                m_pRadar->enabled = 1;            // 0x1449d6 strb 1 -> [m_pRadar+0x48]
                                resetCamera(m_pLevel);               // 0x1449e0
                                m_pLevel->lodManager->forceUpdate(delta, false); // 0x1449ee
                                m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11), 0x1449f6
                                if (field_0xaa != 0) {               // 0x1449f2 ldrb [+0xaa]; cbz
                                    player->setAutoTurret(true);     // 0x144a00
                                }
                                m_nScriptTimerA = 0; // 0x144a04 strd 0
                                m_nState = m_nState + 1; // 0x144a08 shared advance
                            }
                        }
                        // 0x14484e radio-not-over / timer-not-elapsed -> shared tail.
                        break;
                    }
                    if (m_nState == 10) { // prong E9 @0x14354c
                        // Final beat of the docking/hacking sequence. Once radio message #8 is over
                        // and the mission timer has not been armed yet (m_nTimeLimit <= 0), spawn the
                        // dock/hack objective (a type-3 Objective valued 91000) into the level's
                        // objectivesB slot. Then, every frame, wait for the player to actually dock at
                        // the docking point AND for the hack minigame to be won; when both hold, advance
                        // to state 11, tear down the objective, disarm every shipGroup==9 escort's
                        // enemy list, and clear the mission timer.
                        if (((RadioMessage *) ((*messages)[8]))->isOver()) { // 0x14354c
                            if (m_nTimeLimit <= 0) { // 0x14355e (m_nTimeLimit @this+0, cmp >0 skips)
                                m_nScriptTime = 0;      // 0x143568 strd 0,0,[this+8]
                                m_nTimeLimit = 91000; // 0x143570 str 0x16378 -> [this+0]
                                // new Objective(type=3, value=91000, m_pLevel) -> objectivesB, 0x143580
                                m_pLevel->objectivesB = new Objective(3, 91000, m_pLevel); // 0x143588
                            }
                        }
                        if (!player->isDockedToDockingPoint()) { // 0x14358c
                            break; // -> post-switch tail (0x144e36)
                        }
                        if (!player->hackingWon()) { // 0x143598
                            break; // -> post-switch tail (0x144e36)
                        }
                        m_nState = 11; // 0x1435a8 str 11 -> [this+0x1c]
                        delete m_pLevel->objectivesB; // 0x1435b0 ~Objective + operator delete
                        m_pLevel->objectivesB = nullptr; // 0x1435c4
                        for (unsigned int i = 0; i < enemies->count; ++i) { // 0x1435de
                            KIPlayer *elem = (*enemies)[i];
                            if (elem->shipGroup == 9) { // [elem+0x28]==9, 0x1435d0
                                elem->player->setEnemies(nullptr); // 0x1435d8
                            }
                        }
                        m_nTimeLimit = 0; // 0x1435e4 str 0 -> [this+0]
                        break; // -> post-switch tail (0x144e36)
                    }
                    break; // states >10 -> post-switch tail (0x144e36)
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
                m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                m_pHud->visible = 0;
                m_pRadar->enabled = 0;
                m_pCamera->setLookAtCam(true);
                m_pCamera->setTarget((*m_pLevel->getEnemies())[2]->geometry);
                Vector *camPos = &m_tempVec; // LevelScript tempVec @0x28
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
                m_nState = m_nState + 1; // 0 -> 1
                m_pLevel->lodManager->forceUpdate(delta, false);
                // -> post-switch tail (0x144e36)
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
            case 91: { // 0x13ac42
                if (m_nState != 0) {
                    // Cold substates (0x13ebe8). States 1 and 2 handle the docking hand-off; the
                    // deeper docking/objective states (3..6) remain deferred.
                    if (m_nState == 1) {
                        // State 1 (0x13ebec): once the player ship is latched onto the docking
                        // point, advance to state 2.
                        if (player->isDockedToDockingPoint()) {
                            m_nState = 2;
                            break;
                        }
                        break;
                    }
                    if (m_nState == 2) {
                        // State 2 (0x13ec02): wait for radio message #5 to finish, then flag the
                        // docking target and, if the player is still docked, push it into docking
                        // state 2 before advancing to state 3.
                        if (((RadioMessage *) ((*messages)[5]))->isOver()) {
                            ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(2);
                            if (player->isDockedToDockingPoint()) {
                                player->setDockingState(2);
                            }
                            m_nState = 3;
                        }
                        break;
                    }
                    // Cold docking substates 3..6 (dispatch @0x13f312/0x13f36e).
                    if (m_nState == 3) {
                        // State 3 (0x13f31e): once the mission counter has advanced far enough
                        // (Globals::status->field_178 >= 10), tear down objective B and arm state 4.
                        if (Globals::status->field_178 >= 10) {
                            m_nState = 4;
                            Objective *obj = m_pLevel->objectivesB; // [m_pLevel,#0x2c]
                            if (obj != nullptr) {
                                delete obj;
                            }
                            m_pLevel->objectivesB = nullptr;
                        }
                        break; // -> tail (0x1410e2)
                    }
                    if (m_nState == 4) {
                        // State 4 (0x13f350): once radio message #6 finishes and the player has
                        // undocked, reset the 64-bit script timer and advance to state 5.
                        if (((RadioMessage *) ((*messages)[6]))->isOver() &&
                            !player->isDockedToDockingPoint()) {
                            m_nScriptTimerA = 0; // strd 0 -> this+0x90
                            m_nState = 5;
                        }
                        break; // -> tail (0x1410e2)
                    }
                    if (m_nState == 5) {
                        // State 5 (0x13f37e): hold for ~3s on the 64-bit script timer, then run the
                        // full docking-cutscene teardown -- clear autopilot, drop the radar locks,
                        // exit rocket/turret/free-look/first-person modes, stop shooting, re-seat the
                        // HUD/radar/camera, hand control back to the AI, and re-aim the docked ship on
                        // the station before advancing to state 6.
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA < 3001) {
                            break; // -> tail (0x1410e2)
                        }
                        m_nScriptTimerA = 0;
                        m_pLevel->getPlayer()->setAutoPilot(nullptr); // 0x13f3b0
                        m_pRadar->lockedEnemy = nullptr;  // strd 0 -> m_pRadar+4
                        m_pRadar->dockNavPtr = nullptr;   // strd 0 -> m_pRadar+8
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
                        m_pHud->visible = 0;                             // strb 0 -> m_pHud+1
                        m_bCinematicBreak = 1; // cinematicBreak_ = 1 (@0x11)
                        m_pRadar->enabled = 0;                       // strb 0 -> m_pRadar+0x48
                        player->setComputerControlled(true);
                        player->setCollide(false);
                        m_pCamera->setLookAtCam(true);
                        m_pCamera->setTarget(player->geometry);
                        // Re-aim the docked ship at the docking target and reframe the camera
                        // ahead of it (0x13f452..0x13f51e). Receivers resolved from asm: r9 =
                        // (*enemies)[0] (KIPlayer*), [r9+8] = ->geometry (AEGeometry*, saved to
                        // [sp,#0x50]); r4 = this (LevelScript), [r4+20] = m_pCamera. The aim
                        // direction is (playerPos - dockingTarget[0]->getPosition()) normalized;
                        // the fp GOT thunk is Vector::operator= (copy). FP pool @0x13f5f4 = 10000.0f
                        // (direction scale), @0x13f5f8 = 700.0f (right-vector scale).
                        AEGeometry *dockedGeom = (*enemies)[0]->geometry; // [r9+8], saved [sp,#0x50]
                        dockedGeom->getPosition();                        // 0x13f45a (into scratch)
                        Vector *camPos = &m_tempVec; // this+0x28
                        *camPos = dockedGeom->getPosition();              // 0x13f46e: *camPos = geom pos
                        Vector aimDir = AbyssEngine::AEMath::VectorNormalize(
                                m_pLevel->getPlayer()->getPosition()
                                - ((KIPlayer *) m_pLevel->getDockingTarget(0))
                                          ->geometry->getPosition()); // 0x13f47c..0x13f4b0, [dock+8]=geometry
                        dockedGeom->setDirection(aimDir, Vector{0.0f, 1.0f, 0.0f}); // 0x13f4c6
                        *camPos += dockedGeom->getDirection() * 10000.0f; // 0x13f4d2..0x13f4f0
                        *camPos += dockedGeom->getRightVector() * 700.0f; // 0x13f4fc..0x13f516
                        m_pCamera->setPosition(*camPos);                  // 0x13f51e
                        (*enemies)[0]->player->setHitpoints(0); // 0x13f530: enemies[0]->player
                        m_nState = 6;
                        break; // -> tail (0x1410e2)
                    }
                    if (m_nState == 6) {
                        // State 6 (0x141002): wait ~2s on the 64-bit script timer while ramping the
                        // player ship up to cruise speed; then advance the campaign mission, hop the
                        // status to station #113, snapshot the player's hull/shield/armor/energy into
                        // Status, and switch the application to the docking (menu) module.
                        m_nScriptTimerA += delta;
                        if (m_nScriptTimerA < 2001) {
                            break; // -> tail (0x1410e2)
                        }
                        // Speed ramp (0x141022): PlayerEgo::getSpeed() returns the speed's float
                        // bit-pattern packed into an int; while the ship is below cruise (100.0f)
                        // nudge it up 5% each frame.  getSpeed() is invoked twice (compare, then
                        // scale) exactly as the original.
                        int speedBits = player->getSpeed();                 // 0x141024
                        if (reinterpret_cast<float &>(speedBits) < 100.0f) { // vcmpe s2,100.0 @0x141030
                            float scaled = reinterpret_cast<float &>(
                                    (speedBits = player->getSpeed())) * 1.05f; // vmul 1.05 @0x14104c
                            player->setSpeed(scaled);                        // 0x141054
                        }
                        if (m_nScriptTimerA >= 8001) {
                            Globals::status->nextCampaignMission(true);
                            Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(113)); // 0x141082
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(113));
                            Level::initStreamOutPosition = 1; // strb 1 -> Level::initStreamOutPosition
                            Globals::status->field_64 = player->player->getHitpoints(); // hullHp
                            Globals::status->field_5c = player->player->getShieldHP();   // shieldHp
                            Globals::status->field_60 = player->player->getArmorHP();    // armorHp
                            Globals::status->field_68 = player->player->getGammaHP();    // energy
                            Globals::appManager->SetCurrentApplicationModule(2); // 0x1410de
                        }
                        break; // -> tail (0x1410e2)
                    }
                    break;
                }
                if (!((RadioMessage *) ((*messages)[3]))->isTriggered()) {
                    // DEFERRED 0x13ebe4 -> tail.
                    break;
                }
                // Docking approved: reset timers, activate the docking target and give it its
                // localized name.
                m_nScriptTimerB = 0;
                m_nState = 1;
                (*enemies)[0]->setActive(true);
                PlayerFixedObject *dockTarget = (PlayerFixedObject *) m_pLevel->getDockingTarget(0);
                String dockName(*Globals::gameText->getText(3211), false);
                dockTarget->setName(dockName);
                // -> tail (1410e2)
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
                // Pre-dispatch head (runs for every state): while still early in the cutscene, creep
                // the two intro ships forward, and keep the cinematic-break flag latched.
                if (m_nState <= 2) {
                    float step = (float) (delta << 1);
                    for (int i = 0; i != 2; ++i) {
                        (*enemies)[i]->geometry->moveForward(step);
                    }
                }
                if (m_nState <= 4) {
                    m_bCinematicBreak = 1; // cinematicBreak_ (m_nFlags high byte @0x11) = 1
                }
                // 10-way tbh sub-dispatch on (m_nState-1) for states 1..10 (0x13aee4; table @0x13aef0):
                //   s1 0x13af3c, s2 0x141e78, s3 0x142062, s4 0x142138, s5 0x142272, s6 0x1422cc,
                //   s7 0x1426c4, s8 0x1429dc, s9 0x142a04, s10 0x142bf2. States 0 and 14+ fall straight
                //   to the shared 0x144e18 tail; states 11/12/13 divert to their own bodies (0x143eee/
                //   0x144d24/0x144c38, still DEFERRED) via the range check at 0x143edc.
                if (m_nState == 11) {
                    // State 11 (0x143eee): stage the jump-out marker. After 2.5s spawn a marker
                    // AEGeometry (mesh 0x37a7) into m_pGeometry6, seat it on the player and target the
                    // camera on it, and fire radio/sound event 14. At 9s (edge-triggered) fire sound
                    // event 2248 and start inflating the destination planet (getPlanets()[0] scaled by
                    // 0.95/frame @0x144098). At 10s start the fade-out and hand off to state 12. All
                    // exits run the shared geometry-6 advance (0x144e18).
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
                    if (m_pGeometry6 != nullptr) {
                        m_pGeometry6->moveForward((float) (delta * 13));
                    }
                    break;
                }
                if (m_nState == 12) {
                    // State 12 (0x144d24): inflate the destination planet 4x and hold the rumble, drag
                    // any type-10 objects onto the player, then once the fade has cleared start the
                    // outbound fade, spin the player to face back, drop the jump marker and flip the sun
                    // to its supernova (advancing to state 13).
                    Array<AEGeometry *> *planets = reinterpret_cast<Array<AEGeometry *> *>(
                            m_pLevel->getStarSystem()->getPlanets());
                    AEGeometry *planet0 = (*planets)[0];
                    planet0->setScaling(planet0->getScaling() * 4.0f); // 4.0 @0x144d44
                    m_pCamera->setRumblePercentage(50.0f, 10);         // 50.0 @0x145018
                    // 0x144d6a: drag every ship-group-10 object onto the player. vtable+0x2c ==
                    //   KIPlayer::initPush(const Vector&, int radius); radius 100000 @0x14502c.
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
                    break;
                }
                if (m_nState == 13) {
                    // State 13 (0x144c38): rotate/creep the player as a final shudder, hold the rumble,
                    // and after 6s save the player's HP into Status, roll the campaign forward, hand the
                    // player to station 0x6f and jump to the results module.
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
                        // Arm the scene-switch flags (both set to 1) before handing control to the
                        // application module: request the target-setting switch and the stream-out
                        // reposition. (0x144d14 word store; 0x144d1a byte store.)
                        Globals::switch_to_target_setting = 1;
                        Level::initStreamOutPosition = 1;
                        Globals::appManager->SetCurrentApplicationModule(2); // 0x144d1e
                    }
                    break;
                }
                if (m_nState == 1) {
                    // State 1 (0x13af3c): drift the camera along the player's basis; after 13s -> s2.
                    // FP: @0x13b1d4 = 6.08f (dir weight), @0x13b1d8 = 0.2f (right weight),
                    // @0x13b1dc = 0.4f (drift scale).
                    m_nScriptTimerA += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    if (m_nScriptTimerA >= 13001) {
                        m_nState = 2;
                        m_nScriptTimerA = 0;
                    }
                } else if (m_nState == 2) {
                    // State 2 (0x141e78): keep drifting the camera (0.9x the s1 rate) and, once radio
                    // message #1 finishes, split the two escorts onto mirrored fly-away routes derived
                    // from the player's own basis (route point magnitude 900000, see 0x141f24). FP pool
                    // @0x142018..0x142024 = 6.08/0.2/0.4/0.9; route consts @0x142028..0x142030 =
                    // 900000/-900000/-90000.
                    m_nScriptTimerA += delta;
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
                        m_nScriptTimerA = 0;
                        m_nState = m_nState + 1; // 2 -> 3
                    }
                } else if (m_nState == 3) {
                    // State 3 (0x142062): counter += delta, slow camera drift (0.4x), keep both escorts
                    // AI-tagged, and after 3s advance to state 4 (via the shared 0x142266 tail).
                    m_nScriptTimerA += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta * 0.4f;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
                    for (int i = 0; i < 2; ++i) {
                        ((PlayerFighter *) (*enemies)[i])->field_0x12c = 1;
                    }
                    if (m_nScriptTimerA > 3000) {
                        m_nScriptTimerA = 0;
                        m_nState = m_nState + 1; // 3 -> 4
                    }
                } else if (m_nState == 4) {
                    // State 4 (0x142138): counter += delta, very slow drift (0.1x); after 3s hand control
                    // back to the player and tear the cutscene framing down (restore HUD/radar, clear the
                    // cinematic-break flag, unhide the escort exhausts), then advance to state 5.
                    m_nScriptTimerA += delta;
                    Vector dir = player->geometry->getDirection();
                    Vector right = player->geometry->getRightVector();
                    Vector drift = (dir * 6.08f + right * 0.2f) * 0.4f * (float) delta * 0.1f;
                    m_pCamera->translate(drift.x, drift.y, drift.z);
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
                    // State 5 (0x142272): after 10s, deactivate both escorts (setActive(false) +
                    // setSpeed(0)) and advance to state 6.
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
                    // State 6 (0x1422cc): once the 64-bit counter passes 24s, re-stage the escort
                    // formation. Cloak escorts 2..4, teleport escort 2 out along the sun's light
                    // direction (offset 100000) and hang escorts 3/4 in a fixed diamond relative to
                    // escort 2's basis, then run the cutscene teardown and reframe the fixed camera
                    // onto escort 2. FP pool: @0x141ff0 = 100000, @0x142768 = 2200, @0x142770 = -200,
                    // @0x142774 = -2200, @0x142778 = -30000 (right nudge), @0x142780 = 6800,
                    // @0x142788 = 1200, @0x14278c = 400.
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA < 24001) {
                        break; // -> shared 0x144e18 geometry tail
                    }
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
                    // cutscene teardown (0x142582): freeze/hide the player under AI, tear down
                    // rocket/turret/free-look control and the HUD/radar.
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
                    // Reframe (0x142618): camPos = escort2 pos + dir*6800 + right*1200 + up*400.
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
                } else if (m_nState == 7) {
                    // State 7 (0x1426c4): keep the fixed camera chasing the escort formation by driving
                    // the PaintCanvas camera-local matrix off escort 2's basis, creep escorts 2/3/4
                    // forward, and once script-timer-A passes 10s hand control back to the player.
                    // FP pool: @0x142790 = 0.05 (timer-B rate), escort creep 2.5 / @0x142798 = 2.4 /
                    // @0x1427a0 = 2.3, @0x142a70 = 1.96 (dir), @0x142a7c = 0.03 (right),
                    // @0x142a80 = 0.004 (up), @0x142a84 = 2400 (final camera pull-back).
                    m_nScriptTimerA += delta;
                    m_nScriptTimerB =
                            (long long) ((float) delta * 0.05f +
                                         (float) m_nScriptTimerB);
                    (*enemies)[2]->geometry->moveForward((float) ((double) delta * 2.5));
                    (*enemies)[3]->geometry->moveForward((float) ((double) delta * 2.4));
                    (*enemies)[4]->geometry->moveForward((float) ((double) delta * 2.3));
                    {
                        // Reframe: snapshot the camera's PaintCanvas local matrix, rebuild its
                        // translation from escort 2's basis, and push it back (setLocal).
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
                    // State 8 (0x1429dc): pure timer -- after 60s advance to state 9.
                    m_nScriptTimerA += delta;
                    if (m_nScriptTimerA >= 60001) {
                        m_nState = 9;
                    }
                } else if (m_nState == 9) {
                    // State 9 (0x142a04): every ~110s scatter the dead escorts (enemies[2..4]) as debris
                    // along the sun direction, then once the player's route waypoint goes live hand back
                    // control. RNG scatter @0x142a94: base +/- Globals::rnd->nextInt(1000) offsets, per
                    // ship stagger 10000/15000/20000 (step 5000), sun offset 100000 (@0x142a90), right
                    // nudge -30000 (@0x142e10).
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
                } else if (m_nState == 10) {
                    // State 10 (0x142bf2): once radio message #7 is over, run the full cutscene-exit
                    // teardown and reframe the camera behind the (re-oriented) player, then advance to
                    // state 11. If the radio has not finished yet the original diverts to the shared
                    // state-11/12/13 range check (0x143ed8), which for m_nState==10 just falls through
                    // to the shared 0x144e18 geometry tail -- so we simply do nothing that frame.
                    // FP pool: @0x142e1c = 8000.0f (dir weight), @0x142e20 = 1000.0f (right/up weight).
                    if (((RadioMessage *) ((*messages)[7]))->isOver()) {
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
                    }
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
