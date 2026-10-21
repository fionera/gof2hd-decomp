#include "game/core/RadioMessage.h"
#include "game/core/Radio.h"
#include "engine/core/AEString.h"
#include "engine/core/Array.h"
#include "game/mission/Objective.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Radar.h"
#include "game/world/Level.h"
#include "game/world/LevelScript.h"
#include "game/world/Route.h"

RadioMessage::RadioMessage(int textID, int imageID, int conditionType, int conditionValue,
                           int targetCount) {
    this->targetIndices = nullptr;
    this->radio = nullptr;
    this->objective = nullptr;
    this->textID = textID;
    this->imageID = imageID;
    this->conditionType = conditionType;
    this->conditionValue = conditionValue;

    int *values = new int[targetCount];
    this->targetIndices = values;
    for (int i = 0; i < targetCount; ++i) {
        values[i] = conditionValue + i;
    }

    this->targetCount = targetCount;
    this->triggeredFlag = 0;
    this->over = 0;
    this->lastRouteIndex = 0;
}

RadioMessage::RadioMessage(int textID, int imageID, int conditionType, int conditionValue) {
    this->targetIndices = nullptr;
    this->radio = nullptr;
    this->objective = nullptr;
    this->textID = textID;
    this->imageID = imageID;
    this->conditionType = conditionType;
    this->conditionValue = conditionValue;

    this->targetIndices = new int[1];
    this->targetIndices[0] = conditionValue;
    this->targetCount = 1;

    this->triggeredFlag = 0;
    this->over = 0;
    this->lastRouteIndex = 0;
}

RadioMessage::RadioMessage(int textID, int imageID, Objective *objective) {
    this->targetIndices = nullptr;
    this->radio = nullptr;
    this->objective = objective;
    this->textID = textID;
    this->imageID = imageID;
    this->conditionType = 0xb;
    this->triggeredFlag = 0;
    this->over = 0;
    this->lastRouteIndex = 0;
}

RadioMessage::~RadioMessage() {
    delete[] this->targetIndices;
    this->targetIndices = nullptr;
}

void RadioMessage::finish() {
    this->over = 1;
}

bool RadioMessage::isOver() {
    return this->over;
}

void RadioMessage::reset() {
    this->lastRouteIndex = 0;
    this->triggeredFlag = 0;
    this->over = 0;
}

void RadioMessage::trigger() {
    this->triggeredFlag = 1;
}

uint8_t RadioMessage::isTriggered() {
    return this->triggeredFlag;
}

void RadioMessage::setRadio(Radio *radio) {
    this->radio = radio;
}

int RadioMessage::getTextID() {
    return this->textID;
}

int RadioMessage::getImageID() {
    return this->imageID;
}

int RadioMessage::getSoundID() {
    return 0;
}

int RadioMessage::triggered(int64_t time, PlayerEgo *ego, LevelScript *script) {
    if (this->triggeredFlag)
        return 0;

    Array<Player *> *enemies;
    switch (this->conditionType) {
        case 0: // the ego reached the given waypoint of its route
            if (ego->getRoute() == 0)
                goto reject;
            {
                bool reached = false;
                if (((Route *) (intptr_t) ego->getRoute())->getCurrent() > this->lastRouteIndex)
                    reached = this->lastRouteIndex == this->conditionValue;
                this->lastRouteIndex = ((Route *) (intptr_t) ego->getRoute())->getCurrent();
                this->triggeredFlag = reached;
            }
            break;
        case 1: // one of the targets died
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if ((*enemies)[this->targetIndices[i]]->isDead()) {
                    goto fire;
                }
            }
            goto reject;
        case 2: // one of the friendly targets died
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if ((*enemies)[this->targetIndices[i]]->carriesFriendCargoFlag &&
                    (*enemies)[this->targetIndices[i]]->isDead()) {
                    goto fire;
                }
            }
            goto reject;
        case 3:
            this->triggeredFlag = ego->level->getEnemiesLeft() < 1;
            break;
        case 4:
            this->triggeredFlag = ego->level->getFriendsLeft() < 1;
            break;
        case 5:
            this->triggeredFlag = time >= this->conditionValue;
            break;
        case 6:
            this->triggeredFlag = this->radio->getMessage(this->conditionValue)->triggeredFlag;
            break;
        case 8: // one of the targets became active
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if (!(*enemies)[this->targetIndices[i]]->isAsteroid() &&
                    (*enemies)[this->targetIndices[i]]->isActive()) {
                    goto fire;
                }
            }
            goto reject;
        case 9: // all targets died
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if (!(*enemies)[this->targetIndices[i]]->isDead())
                    goto reject;
            }
            goto fire;
        case 10: // one of the friendly targets became active
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if (!(*enemies)[this->targetIndices[i]]->isAsteroid() &&
                    (*enemies)[this->targetIndices[i]]->carriesFriendCargoFlag &&
                    (*enemies)[this->targetIndices[i]]->isActive()) {
                    goto fire;
                }
            }
            goto reject;
        case 11:
            this->triggeredFlag = this->objective->achieved((int) time);
            break;
        case 12: // one of the targets is below half hull
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if ((*enemies)[this->targetIndices[i]]->getHitpoints() <
                    (*enemies)[this->targetIndices[i]]->getMaxHitpoints() / 2) {
                    goto fire;
                }
            }
            goto reject;
        case 14:
            this->triggeredFlag =
                    (*ego->player->getEnemies())[this->conditionValue]->getKIPlayer()->lostMissionCrateToEgo;
            break;
        case 15: // any ship died
            enemies = ego->player->getEnemies();
            for (unsigned int i = 0; i < enemies->size(); ++i) {
                if (!(*enemies)[i]->isAsteroid() && (*enemies)[i]->isDead()) {
                    goto fire;
                }
            }
            goto reject;
        case 16: // any non-friendly ship is active
            enemies = ego->player->getEnemies();
            for (unsigned int i = 0; i < enemies->size(); ++i) {
                if (!(*enemies)[i]->isAsteroid() && (*enemies)[i]->isActive() &&
                    !(*enemies)[i]->isAlwaysFriend()) {
                    goto fire;
                }
            }
            goto reject;
        case 17:
            enemies = ego->player->getEnemies();
            for (unsigned int i = 0; i < enemies->size(); ++i) {
                if ((int) i != this->conditionValue && !(*enemies)[i]->isAsteroid() && (*enemies)[i]->isDead())
                    goto fire;
            }
            goto fire;
        case 18:
            if ((*ego->player->getEnemies())[this->conditionValue]->getKIPlayer()->lostMissionCrateToEgo)
                goto fire;
                this->triggeredFlag =
                        (*ego->player->getEnemies())[this->conditionValue]->getKIPlayer()->field_0x6a;
            break;
        case 19: // one of the targets is below a quarter hull
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if ((*enemies)[this->targetIndices[i]]->getHitpoints() <
                    (*enemies)[this->targetIndices[i]]->getMaxHitpoints() / 4) {
                    goto fire;
                }
            }
            goto reject;
        case 20: { // the given number of ships died
            enemies = ego->player->getEnemies();
            int dead = 0;
            for (unsigned int i = 0; i < enemies->size(); ++i) {
                if (!(*enemies)[i]->isAsteroid())
                    dead += (*enemies)[i]->isDead();
                if (dead >= this->conditionValue)
                    goto fire;
            }
            goto reject;
        }
        case 21:
            this->triggeredFlag = (*ego->player->getEnemies())[this->conditionValue]->getKIPlayer()->field_0x24;
            break;
        case 22:
            this->triggeredFlag = ego->level->field_1c >= this->conditionValue;
            break;
        case 23:
            this->triggeredFlag = ego->field_0x14->stationLocked();
            break;
        case 24:
            if ((*ego->player->getEnemies())[this->conditionValue]->isActive())
                goto reject;
            this->triggeredFlag =
                        !(*ego->player->getEnemies())[this->conditionValue]->isDead() & (time >= 60000);
            break;
        case 25: { // passed the first waypoint with the given number of ships alive
            if (ego->getRoute() == 0)
                goto reject;
            int current = ((Route *) (intptr_t) ego->getRoute())->getCurrent();
            int last = this->lastRouteIndex;
            this->lastRouteIndex = ((Route *) (intptr_t) ego->getRoute())->getCurrent();
            enemies = ego->player->getEnemies();
            int alive = 0;
            for (unsigned int i = 0; i < enemies->size(); ++i) {
                if (!(*enemies)[i]->isAsteroid())
                    alive += !(*enemies)[i]->isDead();
            }
            this->triggeredFlag = (alive >= this->conditionValue) & ((current > last) & (last == 0));
            break;
        }
        case 26: { // the first ship is within 5000 units of the given depth
            Player *ship = (*ego->player->getEnemies())[0];
            if (!ship->isActive() || ship->isDead())
                goto reject;
            Vector position = ship->getPosition();
            if (position.z - (float) this->conditionValue > 0.0f)
                this->triggeredFlag = ship->getPosition().z - (float) this->conditionValue < 5000.0f;
            else
                this->triggeredFlag = ship->getPosition().z - (float) this->conditionValue > -5000.0f;
            break;
        }
        case 27:
            this->triggeredFlag = script->getEvent() == this->conditionValue;
            break;
        case 28:
            this->triggeredFlag = ego->player->getArmorHP() < 1;
            break;
        case 30: { // the given number of the escorted ships died
            enemies = ego->player->getEnemies();
            int dead = 0;
            for (int i = 2; i != 6; ++i) {
                if (!(*enemies)[i]->isAsteroid())
                    dead += (*enemies)[i]->isDead();
            }
            this->triggeredFlag = dead == this->conditionValue;
            break;
        }
        case 31: // one of the targets is below three quarters hull
            enemies = ego->player->getEnemies();
            for (int i = 0; i < this->targetCount; ++i) {
                if ((*enemies)[this->targetIndices[i]]->getHitpoints() <
                    (*enemies)[this->targetIndices[i]]->getMaxHitpoints() / 4 * 3) {
                    goto fire;
                }
            }
            goto reject;
        default:
            goto reject;
    }
    if (!this->triggeredFlag)
        return 0;
    this->radio->setCurrentMessage(this);
    return 1;

fire:
    this->triggeredFlag = 1;
    this->radio->setCurrentMessage(this);
    return 1;
reject:
    this->triggeredFlag = 0;
    return 0;
}
