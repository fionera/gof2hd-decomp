#include "game/world/Level.h"


#include "game/mission/Mission.h"
#include "game/ship/Ship.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/mission/Achievements.h"
#include "game/ui/Hud.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Gun.h"
#include "game/ship/PlayerTurret.h"
#include "game/world/Waypoint.h"
#include "game/world/SolarSystem.h"
#include "game/world/Route.h"
#include "game/mission/Objective.h"
#include "game/mission/Item.h"
#include "engine/core/GameText.h"
#include "game/world/Station.h"
#include "engine/render/LODManager.h"
#include "game/world/Wanted.h"
#include "game/mission/Status.h"
#include "game/ship/KIPlayer.h"
#include "game/weapons/ObjectGun.h"
#include "game/core/RadioMessage.h"
#include "game/core/Radio.h"
#include "game/world/RadioStageEntry.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/world/StarSystem.h"
#include "engine/core/AbyssEngine.h"
#include "engine/core/AERandom.h"
#include "engine/core/ApplicationManager.h"
#include "engine/file/FileRead.h"
#include "game/mission/Generator.h"
#include "engine/math/AEMath.h"
#include "engine/math/BoundingSphere.h"
#include "engine/math/BoundingAAB.h"
#include "game/core/Globals.h"
#include "game/ship/PlayerAsteroid.h"
#include "game/ship/PlayerFighter.h"
#include "game/ship/PlayerGasCloud.h"
#include "game/ship/PlayerStatic.h"
#include "game/ship/PlayerJunk.h"
#include "game/weapons/RocketGun.h"
#include "game/world/Galaxy.h"

#include "game/ship/PlayerFixedObject.h"
#include "game/ship/PlayerStation.h"
#include "game/ship/PlayerJumpgate.h"
#include "game/ship/PlayerWormHole.h"
#include <new>
#include "game/ship/Agent.h"
#include "engine/render/Engine.h"
#include "engine/render/ParticleSettingsRef.h"
#include "game/weapons/Radar.h"
#include "game/weapons/BeamGun.h"
#include "game/weapons/BombGun.h"
#include "game/weapons/MineGun.h"
#include "game/weapons/SentryGun.h"
#include "engine/math/Transform.h"
#include "engine/render/Mesh.h"
#include "game/core/GameSettings.h"
#include "game/mission/DifficultyRecord.h"
#include <cstdint>
#include <cstddef>

// Hangar ship spots per scene race ({x, y, z}).
static const int kHangarSpotsTerran[2][3] = {{0, 0, 2891}, {0, 0, 5781}};
static const int kHangarSpotsVossk[9][3] = {{-3715, 0, 661},     {-6983, 36, 2548},   {-9408, 36, 5438},
                                            {-9408, 36, 16301},  {-6983, 36, 19191},  {-3715, 36, 21078},
                                            {-9408, 5359, 16301}, {-6983, 5359, 19191}, {-3715, 5359, 21078}};
static const int kHangarSpotsNivelian[3][3] = {{-4096, 0, 0}, {0, 0, 4096}, {-4096, 0, 4096}};
static const int kHangarSpotsVoid[1][3] = {{4096, 0, 0}};
static const int kHangarSpotsKhador[3][3] = {{-1961, 0, 8512}, {-1961, 0, 5562}, {-1961, 0, 2610}};
static const int kHangarSpotsDefault[2][3] = {{4096, 0, 0}, {4096, 0, 4096}};
void Level::createScene() {
    static const int kBarChildCounts[4] = {11, 0, 10, 12};
    static const int kBarChildMeshes[4] = {14380, -1, 14403, 14391};
    static const int kBarCounterMeshes[4] = {14281, 14282, 14283, 14284};
    static const int kShipHangarHeights[64] = {
        160, 150, 220, 310, 160, 180, 140, 170, 170, 110, 250, 205, 240, 250, 250, 250,
        170, 170, 190, 460, 170, 210, 140, 170, 160, 190, 170, 240, 220, 280, 200, 200,
        140, 160, 180, 150, 310, 390, 480, 200, 300, 300, 170, 200, 170, 370, 370, 300,
        440, 340, 170, 470, 170, 170, 170, 170, 200, 170, 250, 150, 170, 250, 270, 170};
    static const int kHangarMeshes[10][4] = {
        {14376, 14302, 14301, -1}, {14377, 14304, -1, -1}, {14378, 14308, 14307, -1}, {14379, 14310, 14264, -1},
        {-1, -1, -1, -1},          {-1, -1, -1, -1},       {-1, -1, -1, -1},          {14362, 14358, 14360, 14359},
        {14354, 14355, 14357, 14356}, {-1, -1, -1, -1}};
    static const int kHangarCapacity[10] = {2, 9, 2, 3, 0, 0, 0, 3, 0, 0};
    static const int kBarSeats[4][7][3] = {
        {{-2325, 0, -964}, {-1023, 212, 745}, {-1098, 212, 427}, {-1164, 212, -227}, {-1979, 0, -108},
         {-2556, 0, 646}, {-1439, 212, -860}},
        {{-694, 102, -271}, {914, 102, 453}, {-680, 102, 1734}, {-417, 102, 1454}, {462, 102, -101},
         {285, 102, 913}, {-680, 102, 155}},
        {{-1279, 0, 264}, {-1220, 0, -192}, {-1849, 0, -1214}, {-875, 0, -686}, {48, 0, -911},
         {-457, 0, -2195}, {-794, 0, -1788}},
        {{1542, -10, -642}, {1357, -10, -1986}, {1924, -10, -1377}, {-194, -10, -2542}, {979, -10, -202},
         {1047, -10, 220}, {2319, -10, -498}}};
    static const int kBarMeshes[10][3] = {{14025, 14026, 14027}, {14028, 14029, -1}, {14031, 14032, 14033},
                                          {14034, 14035, 14036}, {-1, -1, -1},       {-1, -1, -1},
                                          {-1, -1, -1},          {-1, -1, -1},       {-1, -1, -1},
                                          {-1, -1, -1}};
    static const int kAgentMeshes[8] = {14725, 14726, 14723, 14725, 14722, 14725, 14720, 14721};

    if (this->enemies != nullptr) {
        ArrayReleaseClasses(*this->enemies);
        delete this->enemies;
    }
    this->enemies = nullptr;
    
    if (this->missionPtr == 23) {
        {
            int sceneRace;
            {
                if (Globals::status->getStation()->getIndex() == 101)
                    sceneRace = 8;
                else if (Globals::status->getStation()->getIndex() == 100)
                    sceneRace = 7;
                else
                    sceneRace = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            }
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            int shipIndex = Globals::status->getShip()->getIndex();
            (*this->enemies)[0] = this->createShip(Globals::status->getShip()->getRace(), 0, shipIndex, nullptr,
                                                   this->missionPtr != 23, false);
            (*this->enemies)[0]->setPosition(0.0f, (float) kShipHangarHeights[shipIndex], 0.0f);
            ((PlayerFighter *) (*this->enemies)[0])->removeTrail();
            ((PlayerFighter *) (*this->enemies)[0])->setExhaustVisible(false);
            (*this->enemies)[0]->setToSleep();
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            bool plainHangar = sceneRace == 1 || sceneRace > 3;
            for (int k = 0; k < 4; k++) {
                int mesh = kHangarMeshes[sceneRace][k];
                if (mesh < 0)
                    continue;
                AEGeometry *g = new AEGeometry(mesh, Globals::Canvas, false);
                g->setRotation(Vector{0.0f, 3.1415927f, 0.0f});
                PlayerStatic *hangarPart = new PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                if (!plainHangar) {
                    for (int j = 0; j < kBarChildCounts[sceneRace]; j++) {
                        AEGeometry *child = new AEGeometry(kBarChildMeshes[sceneRace] + j, Globals::Canvas, false);
                        g->addChild(child->transform);
                        delete child;
                    }
                }
                ArrayAdd((KIPlayer *) hangarPart, *this->enemies);
            }
            bool ownHangar = false;
            if (Globals::status->getStation()->getIndex() == 108)
                ownHangar = Globals::status->field_114 == 3;
            int capacity = kHangarCapacity[sceneRace];
            int shipCount = Globals::rnd->nextInt(capacity + 1);
            if (ownHangar) {
                Array<Ship *> *ships = Globals::status->voidStation->getShips();
                shipCount = ships != nullptr ? (capacity <= ships->size() ? capacity : ships->size()) : 0;
            }
            unsigned char *spotUsed = new unsigned char[capacity];
            AbyssEngine::AERandom *rnd = Globals::rnd;
            for (int i = 0; i < capacity; i++)
                spotUsed[i] = 0;
            const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];
            for (int i = 0; i < shipCount; i++) {
                int race = sceneRace;
                if (rnd->nextInt(100) <= 29) {
                    race = rnd->nextInt(4);
                    if (Globals::rnd->nextInt(100) < 30)
                        race = 8;
                }
                int type = Globals::globals->getRandomEnemyFighter(race);
                if (Globals::status->getStation()->getIndex() == 100) {
                    int pick = Globals::rnd->nextInt(3);
                    type = pick == 0 ? 37 : pick == 1 ? 38 : 40;
                }
                if (ownHangar)
                    type = (*Globals::status->voidStation->getShips())[i]->getIndex();
                KIPlayer *ship = this->createShip(0, 0, type, nullptr, false, false);
                int spot = Globals::rnd->nextInt(capacity);
                for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
                    spot = rnd->nextInt(capacity);
                spotUsed[spot] = 1;
                const int *table;
                switch (sceneRace) {
                    case 0:
                        table = kHangarSpotsTerran[0];
                        break;
                    case 1:
                        table = kHangarSpotsVossk[0];
                        break;
                    case 3:
                        table = kHangarSpotsNivelian[0];
                        break;
                    case 8:
                        table = kHangarSpotsVoid[0];
                        break;
                    default:
                        table = spots;
                        break;
                }
                ship->setPosition((float) table[spot * 3], (float) (table[spot * 3 + 1] + kShipHangarHeights[type]),
                                  (float) table[spot * 3 + 2]);
                ship->player->setAlwaysFriend(true);
                ship->setToSleep();
                ship->geometry->setRotation(0.0f, (float) rnd->nextInt(300) / 100.0f, 0.0f);
                ((PlayerFighter *) ship)->setExhaustVisible(false);
                ArrayAdd(ship, *this->enemies);
            }
            delete[] spotUsed;
            return;
        }

    }

    else if (this->missionPtr == 4) {
        {
            int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            int staticCount = race == 1 ? 2 : 3;
            Array<Agent *> *agents = Globals::status->getStation()->getAgents();
            unsigned char *seatUsed = new unsigned char[7];
            if (agents != nullptr) {
                int agentCount = agents->size();
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(agentCount * 3 + staticCount, *this->enemies);
                for (int i = 0; i < 7; i++)
                    seatUsed[i] = 0;
                for (int i = 0; i < agentCount; i++) {
                    int look = (*agents)[i]->getRace();
                    if (look == 3)
                        look = ((*agents)[i]->getImageParts() != nullptr && (*agents)[i]->getImageParts()[0] == 2) ? 2 : 3;
                    int mesh = kAgentMeshes[look];
                    if ((*agents)[i]->getRace() == 0 && !(*agents)[i]->isMale())
                        mesh = 14724;
                    int seat;
                    do {
                        seat = Globals::rnd->nextInt(7);
                    } while (seatUsed[seat] != 0);
                    float x = (float) kBarSeats[race][seat][0];
                    float y = (float) kBarSeats[race][seat][1];
                    float z = (float) kBarSeats[race][seat][2];
                    seatUsed[seat] = 1;
                    AEGeometry *agentGeometry = new AEGeometry(mesh, Globals::Canvas, false);
                    (*this->enemies)[i] = new PlayerStatic(-1, agentGeometry, x, y, z);
                    AEGeometry *counterGeometry = new AEGeometry(kBarCounterMeshes[race], Globals::Canvas, false);
                    (*this->enemies)[agentCount + i] = new PlayerStatic(-1, counterGeometry, x, y, z);
                    AEGeometry *markerGeometry = new AEGeometry(14348, Globals::Canvas, false);
                    y += 20.0f;
                    (*this->enemies)[agentCount * 2 + i] = new PlayerStatic(-1, markerGeometry, x, y, z);
                }
            } else {
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(3, *this->enemies);
            }
            for (int k = 0; k < staticCount; k++) {
                AEGeometry *g = new AEGeometry(kBarMeshes[race][k], Globals::Canvas, false);
                (*this->enemies)[this->enemies->size() - staticCount + k] = new PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            }
            delete[] seatUsed;
            return;
        }

    }

    else if (this->missionPtr == 2) {
            this->createPlayer();
            Globals::status->setMission(Mission::empty);
            this->createMission();
            if (Globals::status->getCurrentCampaignMission() == 43) {
                AEGeometry *shield = new AEGeometry(14288, Globals::Canvas, false);
                ArrayAdd((KIPlayer *) new PlayerStatic(-1, shield, 0.0f, 0.0f, 0.0f), *this->enemies);
                AEGeometry *beam = new AEGeometry(14289, Globals::Canvas, false);
                ArrayAdd((KIPlayer *) new PlayerStatic(-1, beam, 0.0f, 0.0f, 0.0f), *this->enemies);
            }
            return;
    }

}
