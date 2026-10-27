#include "engine/audio/FModSound.h"
#include "game/ui/Hud.h"
#include "game/weapons/Radar.h"
#include "game/mission/Mission.h"
#include "game/mission/Item.h"
#include "engine/render/Sprite.h"
#include "engine/core/GameText.h"
#include "game/core/Globals.h"
#include "game/core/Radio.h"
#include "game/world/LevelScript.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "game/ui/TouchButton.h"
#include "game/ui/ListItem.h"
#include "game/mission/Status.h"
#include "game/ship/Ship.h"
#include "game/world/Level.h"
#include "game/ship/Player.h"
#include "engine/render/PaintCanvas.h"
#include "game/ui/Layout.h"


// lint: void_ptr (external symbol; param types are mangling-load-bearing)


// lint: void_ptr (external symbol; param types are mangling-load-bearing)



static unsigned int g_Hud_heImportantMask = 0;

// Reused as the zero-pad string; the original reloads the pointer inside the pad loop.
extern const char *ndkEntrance;

void drawControlsInterface(long long t0, long long t1, PlayerEgo *ego, bool letterbox,
                           unsigned int x, unsigned int y) {
    (void) t0;
    (void) t1;
    (void) ego;
    (void) letterbox;
    (void) x;
    (void) y;
}

void Hud::enableFireForTutorial(bool value) {
    this->fireForTutorial = value;
}

void Hud::setVisible(bool value) {
    this->visible = value;
}

void Hud::drawCredits() {
}

void Hud::drawBigNumber(int x, int y, int value, bool flag) {
    (void) x;
    (void) y;
    (void) value;
    (void) flag;
}

int Hud::hudAction(int action, Level *lvl, Radar *radar) {
    (void) action;
    (void) lvl;
    (void) radar;
    return 0;
}

void Hud::setTimeExtender(bool p1, bool p2, bool p3, bool p4) {
    this->field_0x0 = p1;
    this->field_0x280 = p3;
    this->field_0x281 = p4;
    if (p2 && p3) {
        this->timeExtenderDuration = 0x7d0;
        this->timeExtenderTimer = 0x50;
    }
}

void Hud::playerHit() {
    this->shieldHitFlash = 1;
    this->field_0x46c = 0;
}

void Hud::addToEventQueue(ListItem *item) {
    for (unsigned int i = 1; i < this->eventQueue->size(); i++) {
        if ((*this->eventQueue)[i] == 0) {
            (*this->eventQueue)[i] = item;
            this->eventQueueDirty = 1;
            return;
        }
    }
}

unsigned int Hud::firePressed() {
    return (reinterpret_cast<const unsigned char &>(this->touchFlags) >> 4) & 1;
}

void Hud::resetAnalogStick() {
    this->lockBracketX = this->reticleX;
    this->lockBracketY = this->reticleY;
}

float Hud::getAnalogY() {
    float num = (float) ((int) this->lockBracketY - (int) this->reticleY);
    float den = (float) this->analogStickRadius;
    return num / den;
}

bool Hud::cargoFull() {
    return this->cargoFullFlag;
}

unsigned int Hud::touchEnd(unsigned int a, unsigned int b, void *key) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    unsigned int ret = 0;
    int i = 0;
    for (; i != 0x19; i = i + 1) {
        if ((*this->keyArray)[i] == key) {
            ret = (unsigned int) this->elementBits[i];
            this->touchFlags = this->touchFlags & ~ret;
            (*this->keyArray)[i] = 0;
            this->elementBits[i] = 0;
        }
    }
    if (this->quickMenuOpen != 0) {
        Array<TouchButton *> *btns = this->menuButtons;
        if (btns != 0) {
            for (unsigned int j = 0; j < btns->size(); j = j + 1) {
                (*btns)[j]->OnTouchEnd((int) a, (int) b);
                btns = this->menuButtons;
            }
        }
    }
    return ret;
}

void Hud::releaseAllKeys() {
    this->touchFlags = 0;
    for (int i = 0; i != 0x19; i++) {
        Array<void *> *p = this->keyArray; // lint: void_ptr (mirrors keyArray's opaque touch-handle element type)
        if (p != 0) {
            if ((*p)[i] != 0)
                (*p)[i] = 0;
        }
        int *q = this->elementBits;
        q[i] = 0;
    }
    this->field_0x288 = 0;
}

void Hud::closeHudMenu() {
    if (this->menuButtons != 0) {
        ArrayReleaseClasses(*this->menuButtons);
        delete this->menuButtons;
        this->menuButtons = 0;
    }
    this->quickMenuOpen = 0;
}

float Hud::getAnalogX() {
    float num = (float) ((int) this->lockBracketX - (int) this->reticleX);
    float den = (float) this->analogStickRadius;
    return num / den;
}

void Hud::setAutofireEnabled(bool value) {
    this->autofireEnabled = value;
}

bool Hud::isHackingGameActive() {
    return this->hackingGameActive;
}

void Hud::setHackingGameActive(bool value) {
    this->hackingGameActive = value;
}

void Hud::setJumpMapSelected(bool value) {
    this->jumpMapSelectedFlag = value;
}

bool Hud::jumpMapSelected() {
    return this->jumpMapSelectedFlag;
}

static Level **g_Hud_level = nullptr;

void Hud::draw(long long frameTime, long long timeLeft, PlayerEgo *ego, bool letterbox, unsigned int secondaryKind,
               unsigned int fireImageIndex) {
    (void) letterbox;
    int dt = (int) frameTime;
    bool mining = ego->isMining();
    if (!mining && this->eventQueueDirty && !this->hackingGameActive) {
        this->updateQueue(dt);
        this->drawEventQueue();
    }
    if (Globals::iPad && (this->steerOption != *reinterpret_cast<int *>(&Globals::options[84]) ||
                          this->fireOption != *reinterpret_cast<int *>(&Globals::options[88]))) {
        Globals::globals->setCoordsSteer(*reinterpret_cast<int *>(&Globals::options[84]),
                                         Globals::Canvas->GetImage2DWidth(this->image_0x31c),
                                         Globals::Canvas->GetImage2DWidth(this->image_0x310),
                                         Globals::Canvas->GetImage2DWidth(this->fuelGaugeBarImage), this->field_0x3f8,
                                         this->field_0x3fa, this->field_0x42c, this->field_0x42e, this->reticleX,
                                         this->reticleY, this->field_0x410, this->field_0x412, this->field_0x404,
                                         this->field_0x406);
        this->lockBracketX = this->reticleX;
        this->lockBracketY = this->reticleY;
        Globals::globals->setCoordsFire(*reinterpret_cast<int *>(&Globals::options[88]),
                                        Globals::Canvas->GetImage2DWidth(this->image_0x4), this->image_0x4,
                                        this->image_0x8, this->image_0x348, this->field_0xc, this->field_0xe,
                                        this->field_0x3e4, this->field_0x3e6, this->field_0x416, this->field_0x418,
                                        this->field_0x3f2, this->field_0x3f4, this->field_0x3ec, this->field_0x3ee,
                                        this->field_0x3fe, this->field_0x400);
        this->steerOption = *reinterpret_cast<int *>(&Globals::options[84]);
        this->fireOption = *reinterpret_cast<int *>(&Globals::options[88]);
    }

    unsigned int savedColor = Globals::Canvas->GetColor();
    if (this->shieldHitFlash) {
        this->field_0x46c += dt;
        if (this->field_0x46c > 500) {
            this->field_0x46c = 0;
            this->shieldHitFlash = 0;
        }
    }
    if (this->field_0x470 >= 1)
        this->field_0x470 -= dt;

    // Boost / cloak ready flashes.
    if (ego->getBoostRate() == 1.0f && !this->field_0x474) {
        this->field_0x474 = 1;
        this->field_0x484 = 2000;
        this->field_0x488 = 80;
        int textY = this->field_0x4d4;
        if (this->field_0x48c >= 0)
            this->field_0x47c = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) + textY;
        else
            this->field_0x47c = textY;
    } else {
        this->field_0x474 = ego->getBoostRate() == 1.0f;
    }
    if (this->hasCloak) {
        if (!this->field_0x476 && !ego->isCloaked() && !ego->isRechargingCloak()) {
            this->field_0x476 = 1;
            this->field_0x498 = 2000;
            this->field_0x49c = 80;
        } else {
            this->field_0x476 = !ego->isCloaked() && !ego->isRechargingCloak();
        }
    }

    // Shield / armour / gamma bars.
    int dividerOffset = Globals::layout->field_0x1e8;
    Globals::Canvas->SetColor(0xffffffff);
    int pad_ = this->field_0x470;
    int armorY;
    int armorBarY;
    if (this->hasShieldBar) {
        Globals::Canvas->DrawImage2D(ego->player->getShieldHP() >= 2 && this->shieldHitFlash ? this->shieldFrameHitImage
                                                                                               : this->shieldFrameImage,
                                     this->field_0x43c, this->field_0x442);
        Globals::Canvas->DrawImage2D(this->lockBracketImage, this->field_0x43e, this->field_0x442 + dividerOffset);
        Globals::Canvas->DrawImage2D(this->shieldBarBgImage, this->field_0x440, this->field_0x44a);
        int shieldW = (int) ((float) ego->player->getShieldDamageRate() * 0.01f * (float) this->field_0x446);
        Globals::Canvas->DrawRegion2D(this->shieldBarFillImage, 0, 0, shieldW, this->field_0x44c, 0.0f, 0, 0,
                                      this->field_0x440, this->field_0x44a);
        armorBarY = this->field_0x448;
        armorY = this->field_0x444;
    } else {
        armorBarY = this->field_0x44a;
        armorY = this->field_0x442;
    }
    Globals::Canvas->DrawImage2D(ego->player->getArmorHP() < 1 ? this->armorFrameLowImage : this->armorFrameImage,
                                 this->field_0x43c, armorY);
    Globals::Canvas->DrawImage2D(this->lockBracketImage, this->field_0x43e, dividerOffset + armorY);
    Globals::Canvas->DrawImage2D(this->armorBarBgImage, this->field_0x440, armorBarY);
    int hullW = (int) ((float) ego->getHullDamageRate() * 0.01f * (float) this->field_0x446);
    Globals::Canvas->DrawRegion2D(this->armorBarFillImage, 0, 0, hullW, this->field_0x44c, 0.0f, 0, 0,
                                  this->field_0x440, armorBarY);
    if (this->hasArmorRegen) {
        int regenW = (int) ((float) ego->player->getArmorDamageRate() * 0.01f * (float) this->field_0x446);
        Globals::Canvas->DrawRegion2D(this->armorRegenFillImage, 0, 0, regenW, this->field_0x44c, 0.0f, 0, 0,
                                      this->field_0x440, armorBarY);
    }
    if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                    Globals::status->getCurrentCampaignMission()) > 0.0f) {
        int gammaY = this->field_0x444 - this->field_0x442 + this->field_0x444;
        Globals::Canvas->DrawImage2D(this->image_0x2d0, this->field_0x43c, gammaY);
        Globals::Canvas->DrawImage2D(this->lockBracketImage, this->field_0x43e, gammaY + dividerOffset);
        int gammaBarY = this->field_0x448 - this->field_0x44a + this->field_0x448;
        Globals::Canvas->DrawImage2D(this->image_0x2cc, this->field_0x440, gammaBarY);
        int gammaW = (int) ((float) ego->player->getGammaHP() / 100.0f * (float) this->field_0x446);
        Globals::Canvas->DrawRegion2D(this->barDividerImage, 0, 0, gammaW, this->field_0x44c, 0.0f, 0, 0,
                                      this->field_0x440, gammaBarY);
    }

    if (ego->isInRocketControl()) {
        if ((this->touchFlags & 8) || (this->field_0x48c >= 1 && this->field_0x490 <= 0)) {
            Globals::Canvas->DrawImage2D(this->reticleImage, this->field_0x3ec, this->field_0x3ee);
            if (this->field_0x48c >= 1)
                this->field_0x490 = 80;
        } else {
            Globals::Canvas->DrawImage2D(this->missionBannerImage, this->field_0x3ec, this->field_0x3ee);
        }
        if ((Globals::options[17] && !ego->isAutoPilot() && !ego->isDockingToAsteroid() &&
             !ego->isDockingToDockingPoint() && !this->hackingGameActive &&
             (!ego->isDockedToDockingPoint() || ego->isInTurretMode())) ||
            (Globals::options[17] && ego->isInTurretMode()))
            Globals::Canvas->SetColor(0xffffffff);
        else
            Globals::Canvas->SetColor(255, 255, 255, 50);
        Globals::Canvas->DrawImage2D(this->image_0x31c, this->field_0x42c, this->field_0x42e);
        if (Globals::options[17] && (this->touchFlags & 0x20)) {
            Globals::Canvas->DrawImage2D(this->image_0x304, this->lockBracketX, this->lockBracketY, 0x11, 0x44);
        } else {
            this->lockBracketX = this->reticleX;
            this->lockBracketY = this->reticleY;
            Globals::Canvas->DrawImage2D(this->image_0x308, this->reticleX, this->reticleY, 0x11, 0x44);
        }
        Globals::Canvas->SetColor(savedColor);
        return;
    }

    // Damage direction indicators.
    unsigned int sideImage = ego->getShieldDamageRate() < 1 ? this->image_0x364 : this->image_0x36c;
    unsigned int frontImage = ego->getShieldDamageRate() < 1 ? this->image_0x360 : this->image_0x368;
    int hitFlags = ego->field_0x20;
    if (hitFlags & 1)
        this->field_0x4ac = 300;
    if (hitFlags & 2)
        this->field_0x4b0 = 300;
    if (hitFlags & 0x24)
        this->field_0x4b8 = 300;
    if (hitFlags & 0x18)
        this->field_0x4b4 = 300;
    if (this->field_0x4ac >= 1) {
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) (this->field_0x4ac * 255 / 300));
        Globals::Canvas->DrawImage2D(sideImage, (Globals::w >> 1) - ego->field_0x14->imageWidth, Globals::h >> 1,
                                     Globals::Canvas->GetImage2DWidth(sideImage),
                                     Globals::Canvas->GetImage2DHeight(sideImage), 0x11, 0x41, 1);
        this->field_0x4ac -= dt;
    }
    if (this->field_0x4b0 >= 1) {
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) (this->field_0x4b0 * 255 / 300));
        Globals::Canvas->DrawImage2D(sideImage, (Globals::w >> 1) - ego->field_0x14->imageWidth, Globals::h >> 1, 0x12,
                                     0x42);
        this->field_0x4b0 -= dt;
    }
    if (this->field_0x4b4 >= 1) {
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) (this->field_0x4b4 * 255 / 300));
        Globals::Canvas->DrawImage2D(frontImage, Globals::w >> 1, (Globals::h >> 1) - ego->field_0x14->imageHeight,
                                     0x11, 0x14);
        this->field_0x4b4 -= dt;
    }
    if (this->field_0x4b8 >= 1) {
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) (this->field_0x4b8 * 255 / 300));
        Globals::Canvas->DrawImage2D(frontImage, Globals::w >> 1, (Globals::h >> 1) - ego->field_0x14->imageHeight,
                                     Globals::Canvas->GetImage2DWidth(frontImage),
                                     Globals::Canvas->GetImage2DHeight(frontImage), 0x21, 0x24, 2);
        this->field_0x4b8 -= dt;
    }

    if ((Globals::options[17] && !ego->isAutoPilot() && !ego->isDockingToAsteroid() &&
         !ego->isDockingToDockingPoint() && !this->hackingGameActive &&
             (!ego->isDockedToDockingPoint() || ego->isInTurretMode())) ||
        (Globals::options[17] && ego->isInTurretMode()))
        Globals::Canvas->SetColor(0xffffffff);
    else
        Globals::Canvas->SetColor(255, 255, 255, 50);
    Globals::Canvas->DrawImage2D(this->image_0x31c, this->field_0x42c, this->field_0x42e);
    if (Globals::options[17] && (this->touchFlags & 0x20)) {
        Globals::Canvas->DrawImage2D(this->image_0x304, this->lockBracketX, this->lockBracketY, 0x11, 0x44);
    } else {
        this->lockBracketX = this->reticleX;
        this->lockBracketY = this->reticleY;
        Globals::Canvas->DrawImage2D(this->image_0x308, this->reticleX, this->reticleY, 0x11, 0x44);
    }
    Globals::Canvas->SetColor(0xffffffff);

    // Mission status panel (top right).
    if (Globals::status->getMission() != 0 && Globals::status->getMission()->getType() == 12) {
        String kills = String(ego->level->killCountB) + String(" : ") + String(ego->level->killCountA);
        Globals::Canvas->DrawImage2D(this->image_0x324, this->field_0x438 - Globals::layout->field_0x1ec, this->field_0x43a);
        if (ego->hasVolatileGoods()) {
            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x340);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x340);
            Globals::Canvas->DrawRegion2D(
                this->image_0x340, 0, 0,
                (int) ((ego->getVolatileForce() > 1.0f ? 1.0f : ego->getVolatileForce()) * (float) barW), barH, 0.0f,
                0, 0, this->field_0x438 - Globals::layout->field_0x1ec, this->field_0x43a);
        }
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, kills,
                                    this->field_0x438 + Globals::layout->field_0x1fc, this->field_0x43a + 5, false);
    } else if (timeLeft > 0 && Globals::status->getCurrentCampaignMission() != 42) {
        String time;
        Globals::globals->longToTimeString(timeLeft, time);
        Globals::Canvas->DrawImage2D(this->image_0x320, this->field_0x438, this->field_0x43a);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, time,
                                    this->field_0x438 + Globals::layout->field_0x204, this->field_0x43a + 5, false);
    } else if (Globals::status->getMission() != 0 && Globals::status->getMission()->getType() == 184) {
        bool passengers;
        if (Globals::status->getCurrentCampaignMission() == 102 && !Globals::status->inAlienOrbit() &&
            Globals::status->getStation()->getIndex() == 113)
            passengers = false;
        else
            passengers = !(Globals::status->getCurrentCampaignMission() == 139 && !Globals::status->inAlienOrbit() &&
                           Globals::status->getStation()->getIndex() == 131);
        String count = String(Globals::status->field_178) + String(" / ") +
                       String(Globals::status->getShip()->getMaxPassengers());
        if (passengers)
            Globals::Canvas->DrawImage2D(this->image_0x334, this->field_0x438 - Globals::layout->field_0x1f0,
                                         this->field_0x43a);
        else
            Globals::Canvas->DrawImage2D(this->image_0x324, this->field_0x438 - Globals::layout->field_0x1ec,
                                         this->field_0x43a);
        if (ego->hasVolatileGoods()) {
            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x340);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x340);
            Globals::Canvas->DrawRegion2D(
                this->image_0x340, 0, 0,
                (int) ((ego->getVolatileForce() > 1.0f ? 1.0f : ego->getVolatileForce()) * (float) barW), barH, 0.0f,
                0, 0, this->field_0x438 - Globals::layout->field_0x1ec, this->field_0x43a);
        }
        if (passengers) {
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, count,
                                        this->field_0x438 + Globals::layout->field_0x200, this->field_0x43a + 5, false);
        } else {
            String load = String(Globals::status->getShip()->getCurrentLoad()) + String(" / ") +
                          String(Globals::status->getShip()->getMaxLoad()) + String("t");
            int shift = 0;
            if (Globals::status->getShip()->getCurrentLoad() >= 101)
                shift = -2 * Globals::layout->field_0x2c_rowHeight;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, load,
                                        shift + this->field_0x438 - Globals::layout->field_0x208,
                                        this->field_0x43a + 5, false);
        }
        String status(Globals::status->getMission()->getStatusValue());
        Globals::Canvas->DrawImage2D(this->image_0x344, this->field_0x438 - Globals::layout->field_0x1f0,
                                     Globals::layout->field_0x1f4 + this->field_0x43a +
                                         Globals::Canvas->GetImage2DHeight(this->image_0x334));
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, status,
                                    Globals::layout->field_0x200 + this->field_0x438,
                                    this->field_0x43a + Globals::layout->field_0x1f4 + Globals::layout->field_0x1f8 * 2 +
                                        Globals::Canvas->GetImage2DHeight(this->image_0x334),
                                    false);
    } else if (Globals::status->getMission() != 0 && Globals::status->getMission()->getType() == 174) {
        int amount = Globals::status->getShip()->getCargo(Globals::status->getMission()->getProductionGoodIndex()) != 0
                         ? Globals::status->getShip()
                               ->getCargo(Globals::status->getMission()->getProductionGoodIndex())
                               ->getAmount()
                         : 0;
        String cargo = amount + String(" / ") + String(amount + Globals::status->getShip()->getFreeSpace());
        Globals::Canvas->DrawImage2D(this->image_0x338, this->field_0x438 - Globals::layout->field_0x1f0, this->field_0x43a);
        if (ego->hasVolatileGoods()) {
            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x340);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x340);
            Globals::Canvas->DrawRegion2D(
                this->image_0x340, 0, 0,
                (int) ((ego->getVolatileForce() > 1.0f ? 1.0f : ego->getVolatileForce()) * (float) barW), barH, 0.0f,
                0, 0, this->field_0x438 - Globals::layout->field_0x1ec, this->field_0x43a);
        }
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, cargo,
                                    this->field_0x438 + Globals::layout->field_0x200, this->field_0x43a + 4, false);
        String remaining(Globals::status->getMission()->getProductionGoodAmount() -
                         Globals::status->getMission()->getStatusValue());
        Globals::Canvas->DrawImage2D(this->image_0x33c, this->field_0x438 - Globals::layout->field_0x1f0,
                                     Globals::layout->field_0x1f4 + this->field_0x43a +
                                         Globals::Canvas->GetImage2DHeight(this->image_0x334));
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, remaining,
                                    Globals::layout->field_0x200 + this->field_0x438,
                                    this->field_0x43a + Globals::layout->field_0x1f8 + Globals::layout->field_0x1f4 +
                                        Globals::Canvas->GetImage2DHeight(this->image_0x334),
                                    false);
    } else {
        String load = String(Globals::status->getShip()->getCurrentLoad()) + String(" / ") +
                      String(Globals::status->getShip()->getMaxLoad()) + String("t");
        int shift = 0;
        if (Globals::status->getShip()->getCurrentLoad() >= 101)
            shift = -2 * Globals::layout->field_0x2c_rowHeight;
        Globals::Canvas->DrawImage2D(this->image_0x324, this->field_0x438 - Globals::layout->field_0x1ec, this->field_0x43a);
        if (ego->hasVolatileGoods()) {
            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x340);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x340);
            Globals::Canvas->DrawRegion2D(
                this->image_0x340, 0, 0,
                (int) ((ego->getVolatileForce() > 1.0f ? 1.0f : ego->getVolatileForce()) * (float) barW), barH, 0.0f,
                0, 0, this->field_0x438 - Globals::layout->field_0x1ec, this->field_0x43a);
        }
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, load,
                                    this->field_0x438 + shift - Globals::layout->field_0x208, this->field_0x43a + 5,
                                    false);
    }

    // Pause / map buttons.
    if (Globals::mouseCursorActivated || this->quickMenuOpen)
        Globals::Canvas->SetColor(0xffffff00);
    if (Globals::iPad)
        Globals::Canvas->DrawImage2D(this->image_0x348, this->field_0xc, this->field_0xe);
    else
        Globals::Canvas->DrawImage2D(this->image_0x348, Globals::w, Globals::h, 0x11, 0x22);
    if (this->hackingGameActive && !ego->isInTurretMode()) {
        if (this->touchFlags & 0x200)
            Globals::Canvas->DrawImage2D(this->image_0x3a4, this->field_0x454, this->field_0x456);
        else
            Globals::Canvas->DrawImage2D(this->image_0x3a8, this->field_0x454, this->field_0x456);
        if (this->touchFlags & 0x400)
            Globals::Canvas->DrawImage2D(this->image_0x3a4, this->field_0x458, this->field_0x45a);
        else
            Globals::Canvas->DrawImage2D(this->image_0x3a8, this->field_0x458, this->field_0x45a);
    }
    if (mining || this->hackingGameActive || ego->isDockedToDockingPoint() || ego->isLandingOrTakingOff())
        Globals::Canvas->SetColor(0xffffff2f);
    if (!Globals::status->inAlienOrbit() ||
        (Globals::status->inAlienOrbit() && Globals::status->getCurrentCampaignMission() == 154 &&
         ego->level->getNumDockingTargets() >= 1)) {
        if (Globals::status->getCurrentCampaignMission() >= 2 &&
            !(Globals::status->getMission() != 0 && Globals::status->getMission()->getType() == 183))
        {
            if (this->touchFlags & 0x40)
                Globals::Canvas->DrawImage2D(this->image_0x30c, this->field_0x3f8, this->field_0x3fa);
            else
                Globals::Canvas->DrawImage2D(this->image_0x310, this->field_0x3f8, this->field_0x3fa);
        }
    }

    if (pad_ < 0)
        Globals::Canvas->SetColor(savedColor);
    // Autopilot button.
    if (!ego->radioRef->isShowingMessage()) {
        if ((!ego->isAutoPilot() && !ego->isDockingToAsteroid() &&
             (!ego->isDockingToDockingPoint() || ego->isLandingOrTakingOff()) && !this->hackingGameActive) ||
            ego->field_0x14->field_0x54 || ego->aboutToReachAutoTarget()) {
            if (this->field_0x0) {
                if (this->timeExtenderDuration >= 0) {
                    this->timeExtenderTimer -= dt;
                    this->timeExtenderDuration -= dt;
                }
                unsigned int color = Globals::Canvas->GetColor();
                if (!this->field_0x280)
                    Globals::Canvas->SetColor(255, 255, 255, 55);
                Globals::Canvas->DrawImage2D(this->image_0x398, this->field_0x404 + this->field_0x450, this->field_0x406);
                if ((this->touchFlags & 0x100) ||
                    (this->timeExtenderDuration >= 1 && this->timeExtenderTimer < 1) || this->field_0x281) {
                    if (this->timeExtenderDuration >= 1)
                        this->timeExtenderTimer = 80;
                    Globals::Canvas->DrawImage2D(this->image_0x39c, this->field_0x404, this->field_0x406);
                } else {
                    Globals::Canvas->DrawImage2D(this->image_0x3a0, this->field_0x404, this->field_0x406);
                }
                Globals::Canvas->SetColor(color);
            }
        } else {
            Globals::Canvas->DrawImage2D(this->image_0x398, this->field_0x404 + this->field_0x450, this->field_0x406);
            if (Globals::mouseCursorActivated)
                Globals::Canvas->SetColor(0xffffffff);
            if (this->touchFlags & 0x100)
                Globals::Canvas->DrawImage2D(this->image_0x390, this->field_0x404, this->field_0x406);
            else
                Globals::Canvas->DrawImage2D(this->image_0x394, this->field_0x404, this->field_0x406);
            if (Globals::mouseCursorActivated || this->quickMenuOpen)
                Globals::Canvas->SetColor(0xffffff00);
        }
    }
    if ((ego->isAutoPilot() || ego->isInDockingProcedure()) && !ego->isDockedToDockingPoint() &&
        !ego->isLandingOrTakingOff()) {
        Globals::Canvas->SetColor(0xffffffff);
        Globals::Canvas->DrawImage2D(this->image_0x30c, this->field_0x3f8, this->field_0x3fa);
    }
    Globals::Canvas->SetColor(0xffffffff);
    if (Globals::mouseCursorActivated || this->quickMenuOpen)
        Globals::Canvas->SetColor(0xffffff00);

    // Fire button and secondary-weapon selector.
    if (this->touchFlags & 0x80)
        Globals::Canvas->DrawImage2D(this->fireButtonPressedImages[fireImageIndex], this->field_0x3f2, this->field_0x3f4);
    else
        Globals::Canvas->DrawImage2D(this->fireButtonImages[fireImageIndex], this->field_0x3f2, this->field_0x3f4);
    if (this->secondaryLabelTimerSeed == -1) {
        this->secondaryLabelTimerSeed = secondaryKind;
    } else if (this->secondaryLabelTimerSeed != (int) secondaryKind) {
        this->secondaryLabelTimerSeed = secondaryKind;
        this->secondaryLabelTimer = dt;
        switch (secondaryKind) {
        case 0:
            this->field_0x51c = *Globals::gameText->getText(217);
            break;
        case 1:
            this->field_0x51c = *Globals::gameText->getText(218);
            break;
        case 2:
            this->field_0x51c = *Globals::gameText->getText(219);
            break;
        case 3:
            this->field_0x51c = *Globals::gameText->getText(220);
            break;
        }
    }
    if (ego->hasAutoTurret()) {
        if (ego->autoTurretIsEnabled() || (this->touchFlags & 0x20000000))
            Globals::Canvas->DrawImage2D(this->image_0x314, this->field_0x3fe, this->field_0x400);
        else
            Globals::Canvas->DrawImage2D(this->image_0x318, this->field_0x3fe, this->field_0x400);
    } else if (this->secondaryLabelTimer >= 1) {
        int labelH = Globals::Canvas->GetImage2DHeight(this->image_0x354);
        int labelGap = Globals::layout->field_0x20c;
        int textGap = Globals::layout->field_0x210;
        int canvasW = Globals::Canvas->GetWidth();
        int alpha = (int) ((float) this->secondaryLabelTimer / 2000.0f * 255.0f);
        if (alpha > 255)
            alpha = 254 - alpha;
        unsigned int color = Globals::Canvas->GetColor();
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) alpha);
        int labelY = this->field_0x418 - labelH - labelGap;
        Globals::Canvas->DrawImage2D(this->image_0x354, this->field_0x3ec, labelY);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, this->field_0x51c,
            this->field_0x3ec +
                (canvasW - this->field_0x3ec -
                 Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->field_0x51c)) /
                    2,
            labelY + textGap, false);
        Globals::Canvas->SetColor(color);
        this->secondaryLabelTimer += dt;
        if (this->secondaryLabelTimer > 4000)
            this->secondaryLabelTimer = 0;
    }
    if (Globals::mouseCursorActivated || this->quickMenuOpen)
        Globals::Canvas->SetColor(0xffffff00);

    // Quick-menu button.
    if (!this->quickMenuEmpty) {
        if ((this->touchFlags & 4) || (this->field_0x498 >= 1 && this->field_0x49c <= 0)) {
            Globals::Canvas->DrawImage2D(this->image_0x34c, this->field_0x416, this->field_0x418);
            if (this->field_0x498 >= 1)
                this->field_0x49c = 80;
        } else {
            Globals::Canvas->DrawImage2D(this->image_0x350, this->field_0x416, this->field_0x418);
        }
    }

    // Secondary weapon button and ammo label.
    if ((this->currentSecondaryWeapon != 0 && !ego->isInTurretMode() && this->currentSecondaryWeapon->getAmount() > 0) ||
        ego->level->field_69) {
        if ((this->touchFlags & 8) || (this->field_0x48c >= 1 && this->field_0x490 <= 0)) {
            Globals::Canvas->DrawImage2D(this->reticleImage, this->field_0x3ec, this->field_0x3ee);
            if (this->field_0x48c >= 1)
                this->field_0x490 = 80;
        } else {
            Globals::Canvas->DrawImage2D(this->missionBannerImage, this->field_0x3ec, this->field_0x3ee);
        }
        if (Globals::mouseCursorActivated)
            Globals::Canvas->SetColor(0xffffffff);
        Globals::Canvas->DrawImage2D(this->image_0x358, Globals::w >> 1, Globals::h, 0x11, 0x24);
        String label = *Globals::gameText->getText(this->currentSecondaryWeapon->getIndex() + 1274) +
                       String(String(" (") + String(this->currentSecondaryWeapon->getAmount()) + String(")"), false);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, label,
            (Globals::w >> 1) - (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, label) >> 1),
            Globals::h - Globals::layout->field_0x4 + Globals::layout->field_0x214, false);
        if (Globals::mouseCursorActivated || this->quickMenuOpen)
            Globals::Canvas->SetColor(0xffffff00);
    }
    if (this->hasCloak && this->field_0x498 >= 0) {
        this->field_0x498 -= dt;
        this->field_0x49c -= dt;
    }

    // Boost button.
    if (this->hasBoostButton) {
        if (this->field_0x484 >= 0) {
            this->field_0x484 -= dt;
            this->field_0x488 -= dt;
        }
        Globals::Canvas->SetColor(255, 255, 255,
                                  (unsigned char) ((ego->boosting() ? 0
                                                                    : (ego->getBoostRate() < 1.0f
                                                                           ? (int) (ego->getBoostRate() * 75.0f)
                                                                           : 200)) +
                                                   55));
        if (Globals::mouseCursorActivated || mining || this->hackingGameActive || ego->isDockedToDockingPoint())
            Globals::Canvas->SetColor(ego->getBoostRate() < 1.0f ? 0xffffff2f : 0xffffff00);
        if ((this->touchFlags & 2) || (this->field_0x484 >= 1 && this->field_0x488 < 1)) {
            if (this->field_0x484 >= 1) {
                this->field_0x488 = 80;
                if (Globals::mouseCursorActivated)
                    Globals::Canvas->SetColor(0xffffffff);
            }
            Globals::Canvas->DrawImage2D(this->fuelGaugeIconImage, this->field_0x410, this->field_0x412);
        } else {
            Globals::Canvas->DrawImage2D(this->fuelGaugeBarImage, this->field_0x410, this->field_0x412);
        }
    }
    Globals::Canvas->SetColor(0xffffffff);
    if (Globals::mouseCursorActivated || this->quickMenuOpen)
        Globals::Canvas->SetColor(0xffffff00);

    // Primary fire button and lock marker.
    {
        unsigned int fireImage =
            (this->touchFlags & 0x10) || this->fireForTutorial ? this->orbitMarkerIdleImage : this->autoTurretOnImage;
        if (Globals::iPad)
            Globals::Canvas->DrawImage2D(fireImage, this->field_0x3e4, this->field_0x3e6, 0x11, 0x44);
        else
            Globals::Canvas->DrawImage2D(fireImage, this->field_0x3e4, this->field_0x3e6);
    }
    if (!ego->isDockingToAsteroid() && !ego->isMining() && !ego->isDockingToStream() && !ego->isInDockingProcedure() &&
        !ego->isDockingToDockingPoint() && !ego->isInTurretMode()) {
        Radar *radar = ego->field_0x14;
        if (radar->field_0x14 || radar->lockedAsteroid || radar->lockedStation ||
            (radar->lockedEnemy != 0 && radar->lockedEnemy->field_0x70 && radar->lockedEnemy->field_0x75)) {
            if (Globals::iPad)
                Globals::Canvas->DrawImage2D(this->autoTurretOffImage, this->field_0x3e4 + this->field_0x3ea,
                                             this->field_0x3e6 + this->field_0x3ea, 0x11, 0x44);
            else
                Globals::Canvas->DrawImage2D(this->autoTurretOffImage, this->field_0x3e4 + this->field_0x3ea,
                                             this->field_0x3e6 + this->field_0x3ea);
        }
    }

    // Jump drive / cloak / docking transfer progress bars.
    if (this->field_0x277 || this->field_0x276 || this->weaponSelectState) {
        int barOffset = 0;
        if (this->weaponSelectState && ego->isDockedToDockingPoint() && ego->getHitpoints() >= 1) {
            int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            int textH = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
            int transferred = ego->getDockTransferedAmount();
            int total = ego->getDockTotalAmount();
            float progress = (float) transferred / (float) total;
            if (this->field_0x279)
                progress = 1.0f - progress;
            barOffset = (int) ((float) textH + (float) frameH * 2.5f);
            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            this->field_0x468 += dt;
            float fadeIn = (float) this->field_0x468 / 1000.0f;
            float fade = 1.0f;
            if (fadeIn < 1.0f)
                fade = fadeIn;
            int top = this->field_0x3e2 * 2;
            int barYOffset = Globals::layout->field_0x218;
            Globals::Canvas->SetColor((int) (fade * 255.0f) - 256);
            Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, top, 0x11, 0x14);
            Globals::Canvas->DrawRegion2D(this->image_0x38c, 0, 0, (int) (progress * (float) barW), barH, 0.0f, 0, 0,
                                          (Globals::w >> 1) - barW / 2, barYOffset + top);
            int textW = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
            float textY = (float) top + (float) barH * 2.5f;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, (Globals::w >> 1) - (textW >> 1),
                                        (int) textY, false);
            if (this->field_0x27a && Globals::status->getMission() != 0 &&
                Globals::status->getMission()->getType() != 168)
                Globals::Canvas->DrawImage2D(
                    this->image_0x380,
                    (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1) +
                        (Globals::w >> 1),
                    (int) (textY + (float) (Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2)),
                    0x11, 0x41);
            if (Globals::status->getMission() != 0 && Globals::status->getMission()->getType() == 174)
                Globals::Canvas->DrawImage2D(
                    this->image_0x384,
                    (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1) +
                        (Globals::w >> 1),
                    (int) (textY + (float) (Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2)),
                    0x11, 0x41);
        }
        float rate;
        bool show = true;
        if (this->field_0x277)
            rate = ego->getDriveChargeRate();
        else if (this->field_0x276)
            rate = ego->getCloakRate();
        else
            show = false;
        if (show) {
            float progress = 1.0f;
            if (rate * 1.05f < 1.0f)
                progress = (this->field_0x277 ? ego->getDriveChargeRate() : ego->getCloakRate()) * 1.05f;
            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            this->field_0x464 += dt;
            float fade = (float) this->field_0x464 / 1000.0f;
            int top = this->field_0x3e2;
            int barYOffset = Globals::layout->field_0x218;
            Globals::Canvas->SetColor(fade < 1.0f ? (int) (fade * 255.0f) - 256 : 0xffffffff);
            int y = barOffset + top * 2;
            Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, y, 0x11, 0x14);
            float filled = progress * (float) halfW;
            Globals::Canvas->DrawRegion2D(this->image_0x37c, (int) ((float) halfW - filled), 0, (int) (filled + filled),
                                          barH, 0.0f, 0, 0, (int) ((float) (Globals::w >> 1) - filled), y + barYOffset);
            int textW = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, (Globals::w >> 1) - (textW >> 1),
                                        (int) ((float) y + (float) barH * 2.5f), false);
        }
    }

    // First-flight steering hint.
    if (!ego->isMining() && !Globals::hints[17] && Globals::status->getCurrentCampaignMission() == 2 &&
        ego->levelScript->m_nScriptTime > 12000 && !ego->isDockingToAsteroid() && !ego->isDockedToAsteroid()) {
        this->field_0x4c4 += dt;
        if (this->field_0x4c4 > 2000)
            this->field_0x4c4 = 0;
        int alpha = (int) ((float) this->field_0x4c4 / 1000.0f * 255.0f);
        if (alpha > 255)
            alpha = 255 - alpha;
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) alpha);
        String hint(*Globals::gameText->getText(618), false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, hint,
                                    Globals::w / 2 -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, hint) / 2,
                                    Globals::layout->field_0x2c_rowHeight + this->field_0x3e2, false);
        Globals::Canvas->SetColor(0xffffffff);
    }
    Globals::Canvas->SetColor(savedColor);
}

void Hud::updateQueue(int dt) {
    this->eventQueueTimer += dt;
    if (this->eventQueueTimer > 4000) {
        this->eventQueueTimer = 0;
        if ((*this->eventQueue)[0] != 0)
            delete (*this->eventQueue)[0];
        (*this->eventQueue)[0] = 0;
        for (unsigned int i = 0; i + 1 < this->eventQueue->size(); i++)
            (*this->eventQueue)[i] = (*this->eventQueue)[i + 1];
        if ((*this->eventQueue)[1] == 0)
            this->eventQueueDirty = 0;
        this->eventQueuePaused = 0;
    } else if (this->eventQueuePaused == 0 && this->eventQueueTimer > 2000) {
        this->eventQueuePaused = 1;
    }
}




// Security-level text colours (r, g, b) indexed by SolarSystem::getSecurityLevel().
static const int g_Hud_securityColors[4][3] = {
    {255, 42, 0},
    {255, 108, 0},
    {237, 237, 0},
    {0, 237, 0},
};

void Hud::drawOrbitInformation() {
    if (Globals::status->inAlienOrbit() == 0) {
        Globals::Canvas->SetColor(0xffffffff);
        int x = Globals::Canvas->GetImage2DWidth(this->factionLogoImage);
        x += Globals::layout->orbitInfoTextX;
        // hasNoOwner() is declared uint32_t but only ever returns 0/1.  Calling it through a
        // bool-returning member pointer lets clang fold `!= 1` to a zero test (orig: cbnz).
        bool (SolarSystem::*hasNoOwner)() = (bool (SolarSystem::*)())&SolarSystem::hasNoOwner;
        if ((((SolarSystem *) (intptr_t) Globals::status->getSystem())->*hasNoOwner)() != 1)
            Globals::Canvas->DrawImage2D(this->factionLogoImage, 3, 3);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, Globals::status->getStation()->getName(), x,
                                    Globals::layout->orbitInfoStationY, false);
        Globals::Canvas->SetColor(0x777777ff);
        if (Globals::status->getCurrentCampaignMission() >= 16) {
            int security = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getSecurityLevel();
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 26 &&
                Globals::status->field_114 > 1)
                security = 3;
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font,
                String(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getName(), false) + String(" ") +
                    *Globals::gameText->getText(137),
                x, Globals::layout->orbitInfoSystemY, false);
            Globals::Canvas->SetColor(g_Hud_securityColors[security][0], g_Hud_securityColors[security][1],
                                      g_Hud_securityColors[security][2], 255);
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                        *Globals::gameText->getText(security + 402), x,
                                        Globals::layout->orbitInfoSecurityY, false);
        }
    }
}

unsigned int Hud::touchMove(unsigned int a, unsigned int b, void *key) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    unsigned int i = 0;
    for (; i <= 0x18; i = i + 1) {
        if ((*this->keyArray)[i] == key && this->elementBits[i] == 0x20)
            goto found;
    }

    return touchBegin(a, b, key);
found:
    int dx = (int) a - (int) this->reticleX;
    int dy = (int) b - (int) this->reticleY;
    float f = (float) (dy * dy + dx * dx);
    float r = Globals::globals->sqrt(f);
    int denom = this->analogStickRadius;
    int len = (int) r;
    if (denom < len) {
        short s = (short) (denom * dx / len);
        short base = this->reticleY;
        this->lockBracketX = s + this->reticleX;
        s = (short) (denom * dy / len);
        this->lockBracketY = s + base;
    } else {
        this->lockBracketX = (short) a;
        this->lockBracketY = (short) b;
    }
    return 0x20;
}




unsigned int Hud::touchedElement(unsigned int x, unsigned int y) {
    if (this->quickMenuOpen != 0 && this->menuButtons != 0) {
        for (unsigned int i = 0; i < this->menuButtons->size(); i++) {
            if ((*this->menuButtons)[i]->OnTouchBegin(x, y) != 0)
                return (unsigned int) (*this->menuButtons)[i]->id;
        }
        return 0;
    }
    if (Globals::iPad) {
        if (this->field_0x40c <= y && y <= this->field_0x40c + this->field_0x4d8 && this->field_0x40a <= x &&
            x <= this->field_0x40a + this->field_0x4d8)
            return 1;
        if (this->hasBoostButton != 0 && this->field_0x412 <= y && y <= this->field_0x412 + this->field_0x4d8 &&
            this->field_0x410 <= x && x <= this->field_0x410 + this->field_0x4d8)
            return 2;
        if (this->field_0x3fa <= y && y <= this->field_0x3fa + this->field_0x4d8 && this->field_0x3f8 <= x &&
            x <= this->field_0x3f8 + this->field_0x4d8)
            return 0x40;
        if (this->field_0x406 <= y && y <= this->field_0x406 + this->field_0x4d8 && this->field_0x404 <= x &&
            x <= this->field_0x404 + this->field_0x4d8)
            return 0x100;
        if (this->reticleX - this->field_0x4d8 <= x && x <= this->reticleX + this->field_0x4d8 &&
            this->reticleY - this->field_0x4dc <= y && y <= this->reticleY + this->field_0x4dc)
            return 0x20;
        if (this->field_0x3f2 <= x && x <= this->field_0x3f2 + this->field_0x4d8 && this->field_0x3f4 <= y &&
            y <= this->field_0x3f4 + this->field_0x4d8) {
            this->field_0x470 = 1000;
            return 0x80;
        }
        if (this->field_0x3ec - (this->field_0x4d8 >> 1) <= x && x <= this->field_0x3ec + this->field_0x4d8 &&
            this->field_0x3ee <= y && y <= this->field_0x3ee + this->field_0x4d8)
            return 8;
        if (this->field_0x3e4 - (this->field_0x4dc >> 1) <= x && x <= this->field_0x3e4 + (this->field_0x4dc >> 1) &&
            this->field_0x3e6 - (this->field_0x4dc >> 1) <= y && y <= this->field_0x3e6 + (this->field_0x4dc >> 1))
            return 0x10;
        if (this->quickMenuEmpty == 0 && this->field_0x416 <= x && x <= this->field_0x416 + this->field_0x41a &&
            this->field_0x418 <= y && y <= this->field_0x418 + this->field_0x41c)
            return 4;
        if (this->field_0x400 <= y && y <= this->field_0x400 + this->field_0x4d8 && this->field_0x3fe <= x &&
            x <= this->field_0x3fe + this->field_0x4d8)
            return 0x20000000;
        if (this->hackingGameActive != 0) {
            if (this->field_0x456 <= y && y <= this->field_0x456 + this->field_0x4d8 && this->field_0x454 <= x &&
                x <= this->field_0x454 + this->field_0x4d8)
                return 0x200;
            if (this->field_0x45a <= y && y <= this->field_0x45a + this->field_0x4d8 && this->field_0x458 <= x &&
                x <= this->field_0x458 + this->field_0x4d8)
                return 0x400;
            if (this->field_0x460 <= y && y <= this->field_0x460 + this->field_0x4d8 && this->field_0x45e <= x &&
                x <= this->field_0x45e + this->field_0x4d8)
                return 0x800;
        }
        return 0;
    }
    if (this->hackingGameActive != 0) {
        if (this->field_0x456 <= y && y <= this->field_0x456 + this->field_0x4d8 && this->field_0x454 <= x &&
            x <= this->field_0x454 + this->field_0x4d8)
            return 0x200;
        if (this->field_0x45a <= y && y <= this->field_0x45a + this->field_0x4d8 && this->field_0x458 <= x &&
            x <= this->field_0x458 + this->field_0x4d8)
            return 0x400;
        if (this->field_0x460 <= y && y <= this->field_0x460 + this->field_0x4d8 && this->field_0x45e <= x &&
            x <= this->field_0x45e + this->field_0x4d8)
            return 0x800;
    }
    if (y < (unsigned int) (Globals::h >> 2)) {
        if (this->field_0x40c <= y && y <= this->field_0x40c + this->field_0x4d8 && this->field_0x40a <= x &&
            x <= this->field_0x40a + this->field_0x4d8)
            return 1;
        return 0;
    }
    if (x < (unsigned int) (Globals::w >> 1)) {
        if (this->hasBoostButton != 0 && this->field_0x412 <= y && y <= this->field_0x412 + this->field_0x4d8 &&
            this->field_0x410 - this->field_0x4d8 <= x && x <= this->field_0x410 + this->field_0x4d8)
            return 2;
        if (this->field_0x3fa <= y && y <= this->field_0x3fa + this->field_0x4d8 && this->field_0x3f8 <= x &&
            x <= this->field_0x3f8 + this->field_0x4d8)
            return 0x40;
        if (this->field_0x406 <= y && y <= this->field_0x406 + this->field_0x4d8 && this->field_0x404 <= x &&
            x <= this->field_0x404 + this->field_0x4d8)
            return 0x100;
        if (this->reticleX - this->field_0x4d8 <= x && x <= this->reticleX + this->field_0x4d8 &&
            this->reticleY - this->field_0x4dc <= y && y <= this->reticleY + this->field_0x4dc)
            return 0x20;
        return 0;
    }
    if (this->field_0x3f2 <= x && x <= this->field_0x3f2 + this->field_0x4d8 && this->field_0x3f4 <= y &&
        y <= this->field_0x3f4 + this->field_0x4d8) {
        this->field_0x470 = 1000;
        return 0x80;
    }
    if (this->field_0x3ec - (this->field_0x4d8 >> 1) <= x && x <= this->field_0x3ec + this->field_0x4d8 &&
        this->field_0x3ee <= y && y <= this->field_0x3ee + this->field_0x4d8)
        return 8;
    if (this->field_0x3e4 <= x && x <= this->field_0x3e4 + this->field_0x4dc && this->field_0x3e6 <= y &&
        y <= this->field_0x3e6 + this->field_0x4dc)
        return 0x10;
    if (this->quickMenuEmpty == 0 && this->field_0x416 <= x && x <= this->field_0x416 + this->field_0x41a &&
        this->field_0x418 <= y && y <= this->field_0x418 + this->field_0x41c)
        return 4;
    if (this->field_0x400 <= y && y <= this->field_0x400 + this->field_0x4d8 && this->field_0x3fe <= x &&
        x <= this->field_0x3fe + this->field_0x4d8)
        return 0x20000000;
    return 0;
}

Hud::Hud() {
    init();
}


static const char g_Hud_ccHashX[1] = {0};
static const char g_Hud_ccHashN[1] = {0};
static const char g_Hud_ccUnit[1] = {0};
static const char g_Hud_ccUnit2[1] = {0};

void Hud::catchCargo(int itemId, int count, bool cargoFull, bool missionDelivery, bool extender,
                     bool slotMode, bool aggregate) {
    this->cargoFullFlag = cargoFull;
    this->field_0x1d0 = 0;
    ListItem *item;
    if (missionDelivery) {
        this->field_0x1f4 = *Globals::gameText->getText(537);
        this->field_0x1f4 = Globals::status->replaceHash(
            String(this->field_0x1f4, false),
            String(*Globals::gameText->getText(Globals::status->getMission()->getType() == 3 ? 1390 : 1391), false),
            String("#N"));
        this->field_0x1f4 = Globals::status->replaceHash(String(this->field_0x1f4, false), String(1), String("#Q"));
        item = new ListItem(new String(this->field_0x1f4, false));
        item->itemId = itemId;
        this->addToEventQueue(item);
        return;
    } else {
        if (cargoFull) {
            this->field_0x1f4 = *Globals::gameText->getText(322);
            this->addToEventQueue(new ListItem(new String(this->field_0x1f4, false), 1));
            return;
        }
        if (count < 1)
            return;
        if (aggregate && this->eventQueueDirty) {
            String text = String(String(this->cargoAggregateCount) + String("t "), false) +
                          *Globals::gameText->getText(itemId + 1274);
            int index = this->sameHudEventAsBeforeAggregate(String(text, false));
            if (index >= 0) {
                this->eventQueueTimer = 2000;
                this->cargoAggregateCount += count;
                *(*this->eventQueue)[index]->name = String(String(this->cargoAggregateCount) + String("t "), false) +
                                                    *Globals::gameText->getText(itemId + 1274);
                return;
            }
        }
        this->cargoAggregateCount = count;
        this->field_0x1f4 = String(String(count) + String("t "), false) + *Globals::gameText->getText(itemId + 1274);
        item = new ListItem(new String(this->field_0x1f4, false));
        item->itemId = itemId;
        if (!slotMode || extender)
            item->buttonKind = 2;
        if (slotMode)
            item->selectable = 1;
    }
    this->addToEventQueue(item);
}

void Hud::drawEventString(String text, bool rightAlign) {
    PaintCanvas *canvas = Globals::Canvas;
    unsigned int font = (unsigned int) (uintptr_t) Globals::font;
    if (this->letterbox != 0) {
        int x;
        if (rightAlign)
            x = this->field_0x4f0 + 1;
        else
            x = Globals::w - 1 - this->field_0x4f0 - canvas->GetTextWidth(font, text);
        canvas->DrawString(font, text, x, this->field_0x164 - 1, false);
    } else {
        int baseX = this->field_0x160;
        int margin = this->field_0x4e8;
        int x;
        if (rightAlign)
            x = -3 - margin;
        else
            x = margin + 3 - canvas->GetTextWidth(font, text);
        canvas->DrawString(font, text, x + baseX, this->field_0x164 - 1, false);
    }
}

void Hud::setCurrentSecondaryWeapon(Item *item) {
    this->currentSecondaryWeapon = item;
    this->updateSecondaryWeaponString();
}

int Hud::sameHudEventAsBeforeAggregate(String str) {
    for (unsigned int i = this->eventQueue->size() - 1; (int) i > 0; i--) {
        ListItem *e = (*this->eventQueue)[i];
        if (e != 0 && ((String *) e->name)->Compare(str) == 0)
            return (int) i;
    }
    return -1;
}




static const char g_Hud_swSep[1] = {0};
static const char g_Hud_swEnd[1] = {0};

void Hud::updateSecondaryWeaponString() {
    if (this->currentSecondaryWeapon == 0)
        return;
    this->field_0x3b4 = *Globals::gameText->getText(this->currentSecondaryWeapon->getIndex() + 0x4fa) + String(" (") +
                        String(this->currentSecondaryWeapon->getAmount()) + String(")");
    this->secondaryLabelX = (Globals::w >> 1) -
                            (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->field_0x3b4) >> 1);
}





void Hud::drawEventQueue() {
    unsigned char targetMode = Radar::drawTarget;
    int barY = targetMode != 0 ? this->field_0x3e2 : 0;
    int alpha = (int) ((float) this->eventQueueTimer / 2000.0f * 255.0f);
    if (alpha > 255)
        alpha = 510 - alpha;
    int bannerY = Globals::layout->field_0x1e4;
    float bannerScale = Globals::layout->eventBannerScale;
    int y = bannerY + barY;
    Globals::Canvas->SetColor(0xff, 0xff, 0xff, (unsigned char) alpha);
    int textOffset = (int) ((targetMode == 0 ? -2.0f : -1.0f) * bannerScale);
    Globals::Canvas->DrawImage2D(this->image_0x354, this->field_0x3e0,
                                 (Radar::drawTarget != 0 ? this->field_0x3e2 : 0) - Globals::layout->field_0x1e4);
    ListItem *item = (*this->eventQueue)[1];
    if (item != 0) {
        if (item->buttonKind == 1)
            Globals::Canvas->SetColor(0xff, 0x2a, 0, (unsigned char) alpha);
        else if (item->buttonKind == 2)
            Globals::Canvas->SetColor(0, 0xed, 0, (unsigned char) alpha);
        else if (item->buttonKind == 3)
            Globals::Canvas->SetColor(0xff, 0x80, 0, (unsigned char) alpha);
        else
            Globals::Canvas->SetColor(0xff, 0xff, 0xff, (unsigned char) alpha);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *item->name,
                                    (Globals::w >> 1) -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, *item->name) / 2,
                                    textOffset + y, false);
    }
    Globals::Canvas->SetColor(0xffffffff);
}

unsigned int Hud::touchBegin(unsigned int a, unsigned int b, void *key) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    unsigned int e = touchedElement(a, b);
    if (e != 0) {
        for (int j = 0; j < 25; j++) {
            if ((*this->keyArray)[j] == key) {
                unsigned int v = (unsigned int) this->elementBits[j];
                if (e == v)
                    this->touchFlags = this->touchFlags | e;
                else {
                    this->touchFlags &= ~v;
                    this->touchFlags |= e;
                }
                this->elementBits[j] = e;
                return this->touchFlags;
            }
        }
        for (int j = 0; j < 25; j++) {
            if ((*this->keyArray)[j] == 0) {
                (*this->keyArray)[j] = key;
                this->elementBits[j] = e;
                this->touchFlags = e | this->touchFlags;
                break;
            }
        }
    } else {
        for (int i = 0; i != 0x19; i = i + 1) {
            if ((*this->keyArray)[i] == key) {
                this->touchFlags = this->touchFlags & ~(unsigned int) this->elementBits[i];
                this->elementBits[i] = 0;
                (*this->keyArray)[i] = 0;
            }
        }
    }
    return this->touchFlags;
}

bool Hud::sameHudEventAsBefore(String str) {
    for (unsigned int i = this->eventQueue->size() - 1; (int) i > 0; i--) {
        ListItem *e = (*this->eventQueue)[i];
        if (e != 0 && ((String *) e->name)->Compare(str) == 0)
            return true;
    }
    return false;
}



static int **g_Hud_initOutX = nullptr;

static int **g_Hud_initOutY = nullptr;

static const int g_Hud_raceBadge[4] = {1190, 1187, 1189, 1188};
static const char g_Hud_initMsg[1] = {0};

void Hud::init() {
    Globals::Canvas->Image2DCreate(1196, this->shieldFrameImage);
    Globals::Canvas->Image2DCreate(1197, this->shieldFrameHitImage);
    Globals::Canvas->Image2DCreate(1198, this->shieldBarBgImage);
    Globals::Canvas->Image2DCreate(1199, this->shieldBarFillImage);
    Globals::Canvas->Image2DCreate(1194, this->armorFrameImage);
    Globals::Canvas->Image2DCreate(1195, this->armorFrameLowImage);
    Globals::Canvas->Image2DCreate(1191, this->armorBarBgImage);
    Globals::Canvas->Image2DCreate(1192, this->armorRegenFillImage);
    Globals::Canvas->Image2DCreate(1316, this->armorBarFillImage);
    Globals::Canvas->Image2DCreate(8025, this->image_0x2d0);
    Globals::Canvas->Image2DCreate(8026, this->image_0x2cc);
    Globals::Canvas->Image2DCreate(8027, this->barDividerImage);
    Globals::Canvas->Image2DCreate(1193, this->lockBracketImage);
    Globals::Canvas->Image2DCreate(1211, this->image_0x34c);
    Globals::Canvas->Image2DCreate(1210, this->image_0x350);
    if (Globals::iPad != 0) {
        Globals::Canvas->Image2DCreate(1222, this->image_0x4);
        Globals::Canvas->Image2DCreate(1706, this->image_0x8);
        this->image_0x348 = this->image_0x4;
    } else {
        Globals::Canvas->Image2DCreate(1222, this->image_0x348);
    }
    Globals::Canvas->Image2DCreate(1205, this->orbitMarkerIdleImage);
    Globals::Canvas->Image2DCreate(1204, this->autoTurretOnImage);
    Globals::Canvas->Image2DCreate(1334, this->autoTurretOffImage);
    Globals::Canvas->Image2DCreate(1213, this->reticleImage);
    Globals::Canvas->Image2DCreate(1212, this->missionBannerImage);
    Globals::Canvas->Image2DCreate(1209, this->pauseButtonPressedImage);
    Globals::Canvas->Image2DCreate(1208, this->pauseButtonImage);
    Globals::Canvas->Image2DCreate(1203, this->fuelGaugeIconImage);
    Globals::Canvas->Image2DCreate(1202, this->fuelGaugeBarImage);
    Globals::Canvas->Image2DCreate(1201, this->image_0x30c);
    Globals::Canvas->Image2DCreate(1200, this->image_0x310);
    Globals::Canvas->Image2DCreate(1207, this->image_0x304);
    Globals::Canvas->Image2DCreate(1206, this->image_0x308);
    Globals::Canvas->Image2DCreate(1217, this->image_0x31c);
    Globals::Canvas->Image2DCreate(1221, this->image_0x320);
    Globals::Canvas->Image2DCreate(1312, this->image_0x324);
    Globals::Canvas->Image2DCreate(1219, this->image_0x354);
    Globals::Canvas->Image2DCreate(1218, this->image_0x358);
    Globals::Canvas->Image2DCreate(1231, this->quickMenuTopImage);
    Globals::Canvas->Image2DCreate(1233, this->quickMenuMiddleImage);
    Globals::Canvas->Image2DCreate(1232, this->quickMenuBottomImage);
    Globals::Canvas->Image2DCreate(1335, this->image_0x370);
    Globals::Canvas->Image2DCreate(1336, this->image_0x374);
    Globals::Canvas->Image2DCreate(1337, this->image_0x37c);
    Globals::Canvas->Image2DCreate(1338, this->image_0x378);
    Globals::Canvas->Image2DCreate(1338, this->image_0x388);
    Globals::Canvas->Image2DCreate(8001, this->image_0x38c);
    Globals::Canvas->Image2DCreate(1317, this->image_0x360);
    Globals::Canvas->Image2DCreate(1318, this->image_0x364);
    Globals::Canvas->Image2DCreate(1323, this->image_0x368);
    Globals::Canvas->Image2DCreate(1324, this->image_0x36c);
    Globals::Canvas->Image2DCreate(1320, this->fireButtonImages[0]);
    Globals::Canvas->Image2DCreate(1319, this->fireButtonPressedImages[0]);
    Globals::Canvas->Image2DCreate(1257, this->fireButtonImages[1]);
    Globals::Canvas->Image2DCreate(1258, this->fireButtonPressedImages[1]);
    Globals::Canvas->Image2DCreate(1214, this->fireButtonImages[2]);
    Globals::Canvas->Image2DCreate(1215, this->fireButtonPressedImages[2]);
    Globals::Canvas->Image2DCreate(1322, this->fireButtonImages[3]);
    Globals::Canvas->Image2DCreate(1321, this->fireButtonPressedImages[3]);
    Globals::Canvas->Image2DCreate(1344, this->image_0x390);
    Globals::Canvas->Image2DCreate(1345, this->image_0x394);
    Globals::Canvas->Image2DCreate(1343, this->image_0x398);
    Globals::Canvas->Image2DCreate(1346, this->image_0x39c);
    Globals::Canvas->Image2DCreate(1347, this->image_0x3a0);
    Globals::Canvas->Image2DCreate(1350, this->image_0x314);
    Globals::Canvas->Image2DCreate(1351, this->image_0x318);
    Globals::Canvas->Image2DCreate(8024, this->image_0x3a4);
    Globals::Canvas->Image2DCreate(8023, this->image_0x3a8);
    Globals::Canvas->Image2DCreate(1201, this->image_0x3ac);
    Globals::Canvas->Image2DCreate(1200, this->image_0x3b0);
    Globals::Canvas->Image2DCreate(8003, this->image_0x334);
    Globals::Canvas->Image2DCreate(8002, this->image_0x344);
    Globals::Canvas->Image2DCreate(8000, this->image_0x380);
    Globals::Canvas->Image2DCreate(8033, this->image_0x338);
    Globals::Canvas->Image2DCreate(8032, this->image_0x33c);
    Globals::Canvas->Image2DCreate(8031, this->image_0x384);
    Globals::Canvas->Image2DCreate(8028, this->image_0x340);

    this->field_0x4a2 = 0;
    this->field_0x4ac = 0;
    this->field_0x4b4 = 0;
    this->field_0x4b0 = 0;
    this->field_0x4a3 = 0;
    this->field_0x4a1 = 0;
    this->field_0x4b8 = 0;
    this->field_0x4a4 = 0;
    this->field_0x4d8 = Globals::layout->field_0x130;
    this->field_0x4dc = Globals::layout->field_0x134;
    this->analogStickRadius = Globals::layout->field_0x138;
    this->field_0x4d4 = Globals::layout->field_0x12c;
    this->field_0x4e4 = Globals::layout->field_0x13c;
    this->field_0x4ec = Globals::layout->field_0x144;
    this->field_0x4e8 = Globals::layout->field_0x140;
    this->field_0x4f0 = Globals::layout->field_0x148;
    this->field_0x434 = Globals::w - Globals::layout->field_0x14c;
    this->field_0x436 = Globals::h - this->field_0x4d4 - Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) - Globals::layout->field_0x150;
    this->field_0x3f0 = Globals::Canvas->GetImage2DWidth(this->reticleImage);
    this->field_0x3e4 = Globals::w - Globals::layout->field_0x154 - Globals::Canvas->GetImage2DWidth(this->autoTurretOnImage);
    this->field_0x3e6 = Globals::h - Globals::layout->field_0x158 - Globals::Canvas->GetImage2DWidth(this->autoTurretOnImage);
    this->field_0x3ec = Globals::w - Globals::layout->field_0x15c - this->field_0x3f0;
    this->field_0x3ee = Globals::h - Globals::layout->field_0x160 - this->field_0x3f0;
    this->field_0x3ea = Globals::layout->field_0x164;
    this->field_0x3e0 = Globals::w / 2 - Globals::Canvas->GetImage2DWidth(this->image_0x354) / 2;
    this->field_0x3e2 = Globals::layout->field_0x168;
    this->field_0x3f6 = Globals::Canvas->GetImage2DWidth(this->fireButtonImages[0]);
    this->field_0x3f2 = Globals::w - this->field_0x3f6 - Globals::layout->field_0x16c;
    this->field_0x3f4 = Globals::h - Globals::layout->field_0x170 - Globals::Canvas->GetImage2DHeight(this->fireButtonImages[0]);
    this->field_0x41a = Globals::Canvas->GetImage2DWidth(this->image_0x34c);
    this->field_0x41c = Globals::layout->field_0x174;
    this->field_0x416 = Globals::w - Globals::layout->field_0x178 - this->field_0x41a;
    this->field_0x418 = Globals::h - Globals::layout->field_0x17c - Globals::Canvas->GetImage2DHeight(this->image_0x34c);
    this->field_0x3fc = Globals::Canvas->GetImage2DWidth(this->image_0x310);
    this->field_0x3f8 = Globals::layout->field_0x180;
    this->field_0x3fa = Globals::h - Globals::layout->field_0x184 - this->field_0x3fc;
    this->field_0x402 = Globals::Canvas->GetImage2DWidth(this->image_0x310);
    this->field_0x3fe = Globals::w - Globals::layout->field_0x180 - this->field_0x3fc;
    this->field_0x400 = Globals::h - Globals::layout->field_0x184 - this->field_0x3fc;
    this->field_0x40e = Globals::Canvas->GetImage2DWidth(this->pauseButtonPressedImage);
    this->field_0x40a = Globals::w - this->field_0x40e - Globals::layout->field_0x194;
    this->field_0x40c = Globals::layout->field_0x198;
    this->field_0x438 = Globals::w - Globals::Canvas->GetImage2DWidth(this->image_0x320) - Globals::layout->field_0x19c;
    this->field_0x43a = Globals::layout->field_0x1a0;
    this->field_0x430 = Globals::Canvas->GetImage2DWidth(this->image_0x31c);
    this->field_0x45c = Globals::Canvas->GetImage2DWidth(this->image_0x3a4);
    this->field_0x454 = Globals::w / 2 - this->field_0x45c / 2 - Globals::layout->helpWindowX;
    this->field_0x458 = Globals::layout->helpWindowX + Globals::w / 2 - this->field_0x45c / 2;
    this->field_0x45a = Globals::h / 2 - Globals::layout->helpWindowY;
    this->field_0x45e = Globals::w / 2 - this->field_0x45c / 2;
    this->field_0x456 = Globals::h / 2 - Globals::layout->helpWindowY;
    this->field_0x460 = this->field_0x3ee;
    this->field_0x42c = Globals::layout->field_0x1a4;
    this->field_0x42e = Globals::h - Globals::layout->field_0x1a8 - Globals::Canvas->GetImage2DHeight(this->image_0x31c);
    this->field_0x422 = Globals::Canvas->GetImage2DWidth(this->image_0x304);
    this->reticleX = this->field_0x42c + this->field_0x430 / 2;
    this->lockBracketX = this->field_0x42c + this->field_0x430 / 2;
    this->reticleY = this->field_0x42e + this->field_0x430 / 2;
    this->lockBracketY = this->field_0x42e + this->field_0x430 / 2;
    this->field_0x414 = Globals::Canvas->GetImage2DWidth(this->fuelGaugeIconImage);
    this->field_0x410 = Globals::layout->field_0x1ac;
    this->field_0x412 = Globals::h - Globals::layout->field_0x1b0 - this->field_0x414;
    this->field_0x408 = Globals::Canvas->GetImage2DWidth(this->image_0x394);
    this->field_0x404 = Globals::layout->field_0x188;
    this->field_0x406 = Globals::h - Globals::layout->field_0x18c - this->field_0x408;
    this->field_0x450 = Globals::layout->field_0x190;

    if (Globals::iPad != 0) {
        this->field_0x3c4 = Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(this->quickMenuTopImage);
        this->menuOriginY = this->field_0x418 - Globals::layout->field_0x2c_rowHeight - 6 * Globals::layout->field_0x30_rowHeight - Globals::Canvas->GetImage2DHeight(this->quickMenuTopImage);
        Globals::globals->setCoordsSteer(*(int *) &Globals::options[84], Globals::Canvas->GetImage2DWidth(this->image_0x31c),
                                         Globals::Canvas->GetImage2DWidth(this->image_0x310),
                                         Globals::Canvas->GetImage2DWidth(this->fuelGaugeBarImage), this->field_0x3f8, this->field_0x3fa, this->field_0x42c,
                                         this->field_0x42e, this->reticleX, this->reticleY, this->field_0x410, this->field_0x412, this->field_0x404, this->field_0x406);
        this->lockBracketX = this->reticleX;
        this->lockBracketY = this->reticleY;
        Globals::globals->setCoordsFire(*(int *) &Globals::options[88], Globals::Canvas->GetImage2DWidth(this->image_0x4),
                                        this->image_0x4, this->image_0x8, this->image_0x348, this->field_0xc, this->field_0xe, this->field_0x3e4, this->field_0x3e6, this->field_0x416,
                                        this->field_0x418, this->field_0x3f2, this->field_0x3f4, this->field_0x3ec, this->field_0x3ee, this->field_0x3fe, this->field_0x400);
        this->steerOption = *(int *) &Globals::options[84];
        this->fireOption = *(int *) &Globals::options[88];
    } else {
        this->field_0x3c4 = Globals::w / 2 - Globals::Canvas->GetImage2DWidth(this->quickMenuTopImage) / 2;
        this->menuOriginY = Globals::layout->field_0x1b4;
    }
    this->menuRowHeight = Globals::Canvas->GetImage2DHeight(this->quickMenuTopImage);
    this->field_0x3d0 = Globals::Canvas->GetImage2DHeight(this->quickMenuMiddleImage);
    this->field_0x3d4 = this->field_0x3c4 + Globals::layout->field_0x1b8;
    this->menuBaseY = this->menuOriginY + this->menuRowHeight - Globals::layout->field_0x30_rowHeight / 2 + Globals::layout->field_0x1bc;
    this->field_0x3dc = Globals::Canvas->GetImage2DWidth(this->quickMenuMiddleImage) - Globals::layout->field_0x1c0;
    this->field_0x43c = Globals::layout->field_0x1c4;
    this->field_0x43e = Globals::layout->field_0x1c4 + Globals::Canvas->GetImage2DWidth(this->shieldFrameImage);
    this->field_0x444 = Globals::layout->field_0x1c8;
    this->field_0x448 = Globals::layout->field_0x1cc;
    this->field_0x442 = Globals::layout->field_0x1d0;
    this->field_0x44a = Globals::layout->field_0x1d4;
    this->field_0x446 = Globals::Canvas->GetImage2DWidth(this->shieldBarFillImage);
    this->field_0x440 = this->field_0x43e;
    this->field_0x44c = Globals::Canvas->GetImage2DHeight(this->shieldBarFillImage);
    this->field_0x48c = 0;
    this->autofireEnabled = 0;
    this->field_0x488 = 0;
    this->field_0x490 = 0;
    this->field_0x484 = 0;
    this->field_0x476 = 1;
    this->field_0x474 = 1;
    this->field_0x498 = 0;
    this->field_0x49c = 0;
    this->timeExtenderTimer = 0;
    this->field_0x4c4 = 0;
    this->fireForTutorial = 0;
    this->quickMenuOpen = 0;
    this->field_0x15c = -1;
    this->eventQueue = new Array<ListItem *>();
    ArraySetLength(20, *this->eventQueue);
    this->field_0x160 = Globals::w >> 1;
    this->field_0x164 = Globals::h - this->field_0x4e4;
    Hud::RADAR_WIDTH = Globals::w - 28;
    Hud::RADAR_HEIGHT = Globals::h - 33 - this->field_0x4e4 - this->field_0x4ec - Globals::layout->field_0x1d8;
    this->field_0x1dc[2] = 0;
    this->field_0x1d0 = 10000;
    this->field_0x218 = Globals::w - 5;
    this->hasBoostButton = Globals::status->getShip()->getBoostDelay() > 0;
    this->hasShieldBar = Globals::status->getShip()->getMaxShieldHP() > 0;
    this->hasArmorRegen = Globals::status->getShip()->getMaxArmorHP() > 0;
    this->hasAutofireUI = Globals::status->getShip()->getFirePower() > 0.0f;
    this->hasCloak = Globals::status->getShip()->hasCloak();
    this->field_0x234 = 0;
    this->eventQueueDirty = 0;
    this->cargoFullFlag = 0;
    this->field_0x236 = 0;
    this->visible = 1;
    this->jumpMapSelectedFlag = 0;
    this->eventQueueTimer = 0;
    this->eventQueuePaused = 0;
    this->shieldHitFlash = 0;
    this->field_0x276 = 0;
    this->field_0x46c = 0;
    this->field_0x4cc = 0;
    this->currentSecondaryWeapon = 0;
    this->menuButtons = 0;
    this->field_0x4d0 = 0;
    this->field_0x277 = 0;
    this->weaponSelectState = 0;
    this->factionLogoImage = -1;
    this->fuelGaugeValue = 0;
    this->timeExtenderTimer = -1;
    this->field_0x280 = 1;
    this->field_0x0 = 0;
    this->field_0x281 = 0;
    this->timeExtenderDuration = -1;
    this->cargoAggregateCount = 0;
    this->field_0x4c8 = 0;
    this->hackingGameActive = 0;
    this->keyArray = new Array<void *>(); // lint: void_ptr (opaque touch-handle element type, matches exported void* touchId)
    ArraySetLength(25, *this->keyArray);
    this->elementBits = new int[25];
    for (int i = 0; i < 25; i++) {
        (*this->keyArray)[i] = 0;
        this->elementBits[i] = 0;
    }
    this->touchFlags = 0;
    if (Globals::status->inAlienOrbit() == 0)
        Globals::Canvas->Image2DCreate(g_Hud_raceBadge[((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()],
                                       this->factionLogoImage);
    this->secondaryLabelTimerSeed = -1;
    this->secondaryLabelTimer = 0;
    this->field_0x51c = String("");
    this->closeHudMenu();
    this->checkIfQuickMenuIsEmpty();
    this->releaseAllKeys();
    this->uintArray = 0;
    int pauseWidth = Globals::Canvas->GetImage2DWidth(this->pauseButtonPressedImage);
    Globals::pause_x = Globals::w - pauseWidth - Globals::layout->field_0x194;
    Globals::pause_y = Globals::layout->field_0x198;
}

void Hud::drawPauseButton() {
    Globals::Canvas->SetColor(0xffffffff);
    if ((this->touchFlags & 1) != 0)
        Globals::Canvas->DrawImage2D(this->pauseButtonPressedImage, this->field_0x40a, this->field_0x40c);
    else
        Globals::Canvas->DrawImage2D(this->pauseButtonImage, this->field_0x40a, this->field_0x40c);
}

void Hud::checkIfQuickMenuIsEmpty() {
    Array<Item *> *equip = Globals::status->getShip()->getEquipment(1);
    this->equipmentArray = equip;

    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip->size(); i++) {
            if ((*equip)[i] != 0) {
                hasSecondary = true;
                break;
            }
        }
    }
    if (!hasSecondary && Globals::status->getShip()->hasJumpDrive() == 0 && Globals::status->getWingmen() == 0)
        this->quickMenuEmpty = !Globals::status->getShip()->hasCloak();
    else
        this->quickMenuEmpty = 0;
    updateSecondaryWeaponString();
}


void Hud::drawMenu(int unused) {
    (void) unused;
    Globals::layout->drawMask();
    Globals::Canvas->DrawImage2D(this->quickMenuTopImage, this->field_0x3c4 + this->field_0x4cc,
                                 this->field_0x4d0 + this->menuOriginY);
    Globals::Canvas->DrawImage2D(this->image_0x35c, this->field_0x4cc + this->field_0x3d4 + this->field_0x3dc / 2,
                                 this->menuOriginY + this->field_0x4d0 + this->menuRowHeight / 2 -
                                     Globals::layout->quickMenuHeaderLift,
                                 0x11, 0x44);
    int y = this->menuOriginY + this->field_0x4d0 + this->menuRowHeight;
    if (this->menuButtons != 0 && this->menuButtons->size() != 0) {
        for (unsigned int i = 0; i < this->menuButtons->size() - 1; i++) {
            Globals::Canvas->DrawImage2D(this->quickMenuMiddleImage, this->field_0x3c4 + this->field_0x4cc, y);
            y += this->field_0x3d0;
        }
    }
    Globals::Canvas->DrawImage2D(this->quickMenuBottomImage, this->field_0x3c4 + this->field_0x4cc, y);
    if (this->menuButtons != 0 && this->menuButtons->size() != 0) {
        for (unsigned int i = 0; i < this->menuButtons->size(); i++)
            (*this->menuButtons)[i]->draw();
    }
    if (this->menuType != 0)
        return;
    if (Globals::status->getShip()->hasCloak() || Globals::status->getShip()->hasJumpDrive()) {
        unsigned int rowH = Globals::layout->field_0x30_rowHeight;
        String label = String("X ") + String(this->fuelGaugeValue);
        int gx = this->field_0x4cc + this->field_0x3d4 + this->field_0x3dc / 2;
        int gy = Globals::layout->field_0x288 + (y + rowH / 2);
        Globals::Canvas->DrawImage2D(this->image_0x374, gx, gy, 0x11, 0x14);
        Globals::Canvas->DrawImage2D(this->image_0x370, gx - Globals::layout->quickMenuIconGap,
                                     Globals::layout->field_0x30_rowHeight + gy + Globals::layout->field_0x28c, 0x11,
                                     0x12);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, label, Globals::layout->quickMenuIconGap + gx,
                                    gy + Globals::Canvas->GetImage2DHeight(this->image_0x374) / 2 -
                                        Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 +
                                        Globals::layout->field_0x234,
                                    false);
    }
}

void Hud::clearQueue() {
    unsigned int i = 1;
    while (i < this->eventQueue->size()) {
        delete (*this->eventQueue)[i];
        (*this->eventQueue)[i] = 0;
        i = i + 1;
    }
    this->eventQueuePaused = 0;
}

void Hud::hudEvent(int eventId, PlayerEgo *ego, int arg) {
    switch (eventId) {
        case 36:
        case 38:
        case 40:
        case 42:
            this->field_0x1e0 = *Globals::gameText->getText(3200);
            this->weaponSelectState = 0;
            break;
        case 5:
            this->field_0x1e0 = *Globals::gameText->getText(571) + String(" ") + *Globals::gameText->getText(38);
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 10:
            this->field_0x1e0 = *Globals::gameText->getText(546) + String(": ") + String(Globals::status->getStation()->getName(), false) +
                                (Globals::status->getStation()->getIndex() == 101 ? String("") : String(" ") + *Globals::gameText->getText(136));
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 12:
            this->field_0x1e0 = *Globals::gameText->getText(546) + String(": ") + *Globals::gameText->getText(547);
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 13:
            this->field_0x1e0 = *Globals::gameText->getText(546) + String(": ") + *Globals::gameText->getText(548);
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 15:
            this->field_0x1e0 = *Globals::gameText->getText(546) + String(": ") + *Globals::gameText->getText(545);
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 17:
            this->field_0x1e0 = *Globals::gameText->getText(308);
            break;
        case 28:
            this->field_0x276 = 1;
            this->field_0x464 = 0;
            return;
        case 34:
            this->field_0x1e0 = *Globals::gameText->getText(3199);
            break;
        case 35:
            this->field_0x27a = 1;
            this->field_0x468 = 0;
            this->weaponSelectState = 1;
            this->field_0x279 = 0;
            return;
        case 39:
            this->field_0x27a = 0;
            this->field_0x468 = 0;
            this->weaponSelectState = 1;
            this->field_0x279 = 0;
            return;
        case 45:
            this->field_0x1e0 = *Globals::gameText->getText(3202);
            break;
        case 1:
            if (this->hasAutofireUI == 0)
                return;
            this->field_0x1e0 = *Globals::gameText->getText(37) + String(" ") + *Globals::gameText->getText(38);
            break;
        case 2:
            if (this->hasAutofireUI == 0)
                return;
            this->field_0x1e0 = *Globals::gameText->getText(37) + String(" ") + *Globals::gameText->getText(39);
            break;
        case 3:
            if (this->hasBoostButton == 0 || !ego->readyToBoost())
                return;
            this->field_0x1e0 = *Globals::gameText->getText(314);
            break;
        case 4:
            if (this->hasBoostButton == 0)
                return;
            this->field_0x1e0 = *Globals::gameText->getText(315);
            break;
        case 6:
            this->field_0x1e0 = *Globals::gameText->getText(571) + String(" ") + *Globals::gameText->getText(39);
            Globals::sound->play(29, 0, 0, 0.0f);
            break;
        case 7:
            this->field_0x1e0 = *Globals::gameText->getText(553);
            break;
        case 8:
            this->field_0x1e0 = *Globals::gameText->getText(539);
            break;
        case 9:
            this->field_0x1e0 = *Globals::gameText->getText(540);
            break;
        case 11:
            this->field_0x1e0 = *Globals::gameText->getText(546) + String(": ") + *Globals::gameText->getText(550);
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 14:
            this->field_0x1e0 = *Globals::gameText->getText(546) + String(": ") + *Globals::gameText->getText(549);
            Globals::sound->play(28, 0, 0, 0.0f);
            break;
        case 16:
            this->field_0x1e0 = *Globals::gameText->getText(307);
            break;
        case 18:
            this->field_0x1e0 = *Globals::gameText->getText(309);
            break;
        case 19:
            this->field_0x1e0 = this->eventLines[19];
            break;
        case 20:
            this->field_0x1e0 = *Globals::gameText->getText(541);
            break;
        case 21:
            this->field_0x1e0 = *Globals::gameText->getText(525);
            break;
        case 22:
            this->field_0x1e0 = *Globals::gameText->getText(542);
            break;
        case 23:
            this->field_0x1e0 = *Globals::gameText->getText(543);
            break;
        case 24:
            this->field_0x1e0 = *Globals::gameText->getText(544);
            break;
        case 25:
            this->field_0x277 = 1;
            this->field_0x464 = 0;
            return;
        case 26:
            this->field_0x277 = 0;
            return;
        case 27:
            this->field_0x1e0 = *Globals::gameText->getText(322);
            break;
        case 29:
            this->field_0x276 = 0;
            return;
        case 31:
            this->field_0x1e0 = *Globals::gameText->getText(324);
            break;
        case 32:
            this->field_0x1e0 = *Globals::gameText->getText(218) + String(" ") + *Globals::gameText->getText(38);
            break;
        case 33:
            this->field_0x1e0 = *Globals::gameText->getText(218) + String(" ") + *Globals::gameText->getText(39);
            break;
        case 37:
            this->field_0x27a = 1;
            this->field_0x468 = 0;
            this->weaponSelectState = 1;
            this->field_0x279 = 1;
            return;
        case 41:
            this->field_0x27a = 0;
            this->field_0x468 = 0;
            this->weaponSelectState = 1;
            this->field_0x279 = 1;
            break;
        case 43:
            this->field_0x1e0 = *Globals::gameText->getText(3203);
            break;
        case 44:
            this->field_0x1e0 = *Globals::gameText->getText(3201);
            break;
        case 46:
            this->field_0x1e0 = *Globals::gameText->getText(316);
            break;
        case 47:
            this->field_0x1e0 = String(String("-") + String(arg) + String("t "), false) + *Globals::gameText->getText(1476);
            this->clearQueue();
            break;
        case 30:
            this->field_0x1e0 = String(String("-") + String(arg) + String("t "), false) + *Globals::gameText->getText(1396);
            this->clearQueue();
            break;
    }
    if (this->sameHudEventAsBefore(this->field_0x1e0))
        return;
    ListItem *item;
    if (eventId == 27 || eventId == 30 || eventId == 31 || eventId == 47)
        item = new ListItem(new String(this->field_0x1e0), 1);
    else
        item = new ListItem(new String(this->field_0x1e0));
    this->addToEventQueue(item);
    bool wide = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->field_0x1e0) >
                Globals::w / 2 - this->field_0x4e8 - 2 * this->field_0x4f0;
    this->eventScrolls = 0;
    this->field_0x1dc[2] = 1;
    this->letterbox = wide;
}



void Hud::drawChallengeModeScore(int unused) {
    (void) unused;
    int digitW, overlap, y, digitH, screenW;
    int step;
    int half, halfStep, left;
    Globals::Canvas->SetColor(0xffffffff);
    digitW = this->digitSprite->getFrameWidth();
    overlap = Globals::layout->field_0x2c_rowHeight;
    digitH = this->digitSprite->getFrameHeight();
    int scoreValue = Globals::status->field_0x184;
    y = Globals::layout->field_0x2c_rowHeight;
    screenW = Globals::w;
    String score(scoreValue);
    if (score.size() < 7) {
        int pad = 7 - score.size();
        for (int i = pad; i != 0; i--)
            score = String(ndkEntrance) + score;
    }
    Globals::Canvas->SetColor(0x77ccffff);
    step = digitW - overlap;
    half = screenW / 2;
    halfStep = step * 7 / 2;
    left = half - halfStep;
    int x = left;
    for (unsigned int i = 0; i < score.size(); i++) {
        int frame = score.SubString(i, i + 1).ValueOf();
        this->digitSprite->setFrame(frame);
        this->digitSprite->setPosition(x, y);
        this->digitSprite->draw(1.0f, 1.0f);
        x += step;
    }
    if (Globals::status->field_0x180 > 0 && Globals::status->field_0x18c >= 2) {
        Globals::Canvas->SetColor(0xffffffff);
        int rowGap = Globals::layout->field_0x2c_rowHeight;
        y = y + digitH + rowGap;
        if (Globals::status->field_0x180 <= 3000) {
            if (Globals::status->field_0x180 % 100 < 50)
                return;
            int multiplier = Globals::status->field_0x18c;
            String bonus((int) ((1.0f + (float) multiplier * 0.05f) * (float) (multiplier * 1000)));
            int bonusY = digitH + y;
            int offset = 0;
            for (unsigned int i = 0; i < bonus.size(); i++) {
                int frame = bonus.SubString(i, i + 1).ValueOf();
                this->digitSprite->setFrame(frame);
                this->digitSprite->setPosition(Globals::w / 2 - bonus.size() * step / 2 + offset,
                                               bonusY + Globals::layout->field_0x2c_rowHeight);
                offset += step;
                this->digitSprite->draw(1.0f, 1.0f);
            }
        }
        Globals::Canvas->DrawImage2D(this->multiplierIconImage, rowGap + left, y);
        float pulse = (float) (Globals::status->field_0x180 - 7000) * 0.01f;
        float scale = pulse < 0.0f ? 1.0f : pulse + 1.0f;
        String multiplier(Globals::status->field_0x18c);
        int mx = half + rowGap - halfStep;
        for (unsigned int i = 1; i - 1 < multiplier.size(); i++) {
            int frame = multiplier.SubString(i - 1, i).ValueOf();
            this->digitSprite->setFrame(frame);
            this->digitSprite->setPosition(mx + Globals::Canvas->GetImage2DWidth(this->multiplierIconImage), y);
            mx += step;
            this->digitSprite->draw(scale, scale);
        }
    }
    Globals::Canvas->SetColor(0xffffffff);
}



void Hud::hudEventMedal(int medalId, int percent) {
    if (percent >= 100)
        percent = 100;
    this->field_0x1e0 = *Globals::gameText->getText(medalId + 1507) + String(":") + String(percent) + String("%");
    if (this->sameHudEventAsBefore(String(this->field_0x1e0, false)))
        return;
    this->addToEventQueue(new ListItem(new String(this->field_0x1e0, false), 3));
    bool wide = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->field_0x1e0) >
                Globals::w / 2 - this->field_0x4e8 - 2 * this->field_0x4f0;
    this->eventScrolls = 0;
    this->field_0x1dc[2] = 1;
    this->letterbox = wide;
}



static CargoBay **g_Hud_imCargoA = nullptr;

static CargoBay **g_Hud_imCargoB = nullptr;



void Hud::initHudMenu(int menuType, Level *level) {
    if (this->menuButtons != 0) {
        ArrayReleaseClasses(*this->menuButtons);
        delete this->menuButtons;
        this->menuButtons = 0;
    }
    this->menuButtons = new Array<TouchButton *>();
    this->menuType = menuType;
    if (this->equipmentArray != 0)
        delete this->equipmentArray;
    this->equipmentArray = 0;
    this->equipmentArray = Globals::status->getShip()->getEquipment(1);
    this->updateSecondaryWeaponString();
    this->field_0x4cc = 0;
    int rowGap = Globals::layout->field_0x1dc;
    int y = this->menuBaseY;
    if (Globals::iPad != 0) {
        float top = (this->menuType == 3 ? (float) *(int *) &Globals::options[84]
                                         : (float) *(int *) &Globals::options[88] -
                                           (Globals::iPadHD != 0 ? 112.5f : Globals::iPadLarge != 0 ? 160.0f : 80.0f)) < 0.0f
                        ? 0.0f
                        : (this->menuType == 3 ? (float) *(int *) &Globals::options[84]
                                               : (float) *(int *) &Globals::options[88] -
                                                 (Globals::iPadHD != 0 ? 112.5f : Globals::iPadLarge != 0 ? 160.0f : 80.0f));
        this->menuOriginY = (int) top;
        y = (int) top + this->menuRowHeight - Globals::layout->field_0x30_rowHeight / 2 + 1;
        this->menuBaseY = y;
    }

    switch (menuType) {
        case 0: {
            bool hasSecondary = false;
            if (this->equipmentArray != 0) {
                for (unsigned int i = 0; i < this->equipmentArray->size(); i++) {
                    if ((*this->equipmentArray)[i] != 0) {
                        hasSecondary = true;
                        break;
                    }
                }
            }
            if (hasSecondary) {
                TouchButton *button = new TouchButton(*Globals::gameText->getText(266), 0, this->field_0x3d4, y,
                                                      this->field_0x3dc, 0x11, 4);
                button->id = 0x200;
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            if (Globals::status->getWingmen() != 0 && Globals::status->inSupernovaSystem() == 0 &&
                Globals::status->getCurrentCampaignMission() != 158) {
                TouchButton *button = new TouchButton(*Globals::gameText->getText(306), 0, this->field_0x3d4, y,
                                                      this->field_0x3dc, 0x11, 4);
                button->id = 0x400;
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            if (Globals::status->getShip()->hasCloak()) {
                Item *cloak = Globals::status->getShip()->getFirstEquipmentOfSort(21);
                TouchButton *button = new TouchButton(*Globals::gameText->getText(cloak->getIndex() + 1274), 0,
                                                      this->field_0x3d4, y, this->field_0x3dc, 0x11, 4);
                button->setPressProgressHighlight(false);
                if (level->getPlayer()->isCloaked() || level->getPlayer()->isChargingCloak() ||
                    level->getPlayer()->isRechargingCloak()) {
                    if (level->getPlayer()->isRechargingCloak())
                        button->setPressProgress(level->getPlayer()->getCloakRechargeRate());
                    button->setHalfTransparent(true);
                }
                button->id = 0x800;
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            if (Globals::status->getShip()->hasJumpDrive()) {
                TouchButton *button = new TouchButton(*Globals::gameText->getText(1359), 0, this->field_0x3d4, y,
                                                      this->field_0x3dc, 0x11, 4);
                if (level->getPlayer()->isChargingDrive() || level->getPlayer()->emergencySystemActive())
                    button->setHalfTransparent(true);
                button->id = 0x1000;
                ArrayAdd(button, *this->menuButtons);
            }
            Item *fuel = Globals::status->getShip()->getCargo(122);
            this->fuelGaugeValue = fuel != 0 ? fuel->getAmount() : 0;
            Globals::Canvas->Image2DCreate(1269, this->image_0x35c);
            break;
        }
        case 1:
            for (int i = 0; (unsigned int) i < this->equipmentArray->size(); i++) {
                if ((*this->equipmentArray)[i] == 0)
                    continue;
                TouchButton *button = new TouchButton(
                        *Globals::gameText->getText((*this->equipmentArray)[i]->getIndex() + 1274) +
                        String(String(" (") + String((*this->equipmentArray)[i]->getAmount()) + String(")"), false),
                        0, this->field_0x3d4, y, this->field_0x3dc, 0x11, 4);
                button->id = i == 0 ? 0x2000 : i == 1 ? 0x4000 : i == 2 ? 0x8000 : 0x10000;
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            Globals::Canvas->Image2DCreate(1270, this->image_0x35c);
            break;
        case 2: {
            int textIds[4] = {307, 308, 309, (unsigned char) Globals::status->field_f8 != 0 ? 311 : 310};
            int keys[4];
            keys[0] = 0x20000;
            keys[1] = 0x40000;
            keys[2] = 0x80000;
            keys[3] = 0x100000;
            for (unsigned int i = 0; i < 4; i++) {
                TouchButton *button = new TouchButton(*Globals::gameText->getText(textIds[i]), 0, this->field_0x3d4, y,
                                                      this->field_0x3dc, 0x11, 4);
                button->id = keys[i];
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            Globals::Canvas->Image2DCreate(1267, this->image_0x35c);
            break;
        }
        case 3: {
            if (Globals::iPad != 0)
                this->field_0x4cc = Globals::layout->buttonInsetX - this->field_0x3c4;
            if (Globals::status->inAlienOrbit() == 0) {
                TouchButton *button = new TouchButton(*Globals::gameText->getText(549), 0, this->field_0x3d4, y,
                                                      this->field_0x3dc, 0x11, 4);
                button->id = 0x1000000;
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
                if (!Globals::status->inEmptyOrbit()) {
                    TouchButton *dock = new TouchButton(
                            String(Globals::status->getStation()->getName(), false) +
                            (Globals::status->getStation()->getIndex() == 101
                                     ? String("")
                                     : String(" ") + *Globals::gameText->getText(136)),
                            0, this->field_0x3d4, y, this->field_0x3dc, 0x11, 4);
                    dock->id = 0x800000;
                    ArrayAdd(dock, *this->menuButtons);
                    y += rowGap + Globals::layout->field_0x30_rowHeight;
                }
                if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->currentOrbitHasWarpGate()) {
                    TouchButton *gate = new TouchButton(*Globals::gameText->getText(547), 0, this->field_0x3d4, y,
                                                        this->field_0x3dc, 0x11, 4);
                    gate->id = 0x400000;
                    ArrayAdd(gate, *this->menuButtons);
                    y += rowGap + Globals::layout->field_0x30_rowHeight;
                }
                if (level->getPlayer()->getRoute() != 0 &&
                    (unsigned char) ((Route *) (intptr_t) level->getPlayer()->getRoute())->getLastWaypoint()->state == 0) {
                    TouchButton *route = new TouchButton(*Globals::gameText->getText(573), 0, this->field_0x3d4, y,
                                                         this->field_0x3dc, 0x11, 4);
                    route->id = 0x2000000;
                    ArrayAdd(route, *this->menuButtons);
                    y += rowGap + Globals::layout->field_0x30_rowHeight;
                }
                if (Level::programmedStation != 0) {
                    TouchButton *autopilot = new TouchButton(
                            *Globals::gameText->getText(546) + String(": ") +
                            String(Level::programmedStation->getName(), false),
                            0, this->field_0x3d4, y, this->field_0x3dc, 0x11, 4);
                    autopilot->id = 0x200000;
                    ArrayAdd(autopilot, *this->menuButtons);
                    y += rowGap + Globals::layout->field_0x30_rowHeight;
                }
            }
            int targets = level->getNumDockingTargets();
            for (int i = 0; i < targets; i++) {
                if (((PlayerFixedObject *) (intptr_t) level->getDockingTarget(i))->getName().length == 0)
                    continue;
                TouchButton *button = new TouchButton(
                        ((PlayerFixedObject *) (intptr_t) level->getDockingTarget(i))->getName(), 0, this->field_0x3d4,
                        y, this->field_0x3dc, 0x11, 4);
                button->id = 0x4000000 << i;
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            if (Globals::iPad == 0 && this->menuButtons->size() >= 5) {
                for (unsigned int i = 0; i < this->menuButtons->size(); i++) {
                    TouchButton *button = (*this->menuButtons)[i];
                    button->setPosition((int) button->getPosition().x,
                                        (int) ((*this->menuButtons)[i]->getPosition().y -
                                               (float) Globals::layout->field_0x30_rowHeight));
                }
            }
            Globals::Canvas->Image2DCreate(1268, this->image_0x35c);
            break;
        }
    }

    unsigned int count = this->menuButtons->size();
    if (Globals::iPad != 0) {
        if (count != 0) {
            int shift = (4 - count) * (rowGap + Globals::layout->field_0x30_rowHeight);
            if (menuType == 3)
                shift -= Globals::layout->field_0x30_rowHeight;
            this->field_0x4d0 = shift;
            for (unsigned int i = 0; i < this->menuButtons->size(); i++) {
                (*this->menuButtons)[i]->translate(this->field_0x4cc, this->field_0x4d0);
                if (i < 10) {
                    Globals::sub_menu_buttons_x[i] = (int) (*this->menuButtons)[i]->getPosition().x;
                    Globals::sub_menu_buttons_y[i] = (int) (*this->menuButtons)[i]->getPosition().y;
                }
            }
        }
    } else {
        this->field_0x4d0 = count >= 5 ? -Globals::layout->field_0x30_rowHeight : 0;
        for (unsigned int i = 0; i < this->menuButtons->size(); i++) {
            if (i < 10) {
                Globals::sub_menu_buttons_x[i] = (int) (*this->menuButtons)[i]->getPosition().x;
                Globals::sub_menu_buttons_y[i] = (int) (*this->menuButtons)[i]->getPosition().y;
            }
        }
    }
    this->quickMenuOpen = 1;
}

Hud::~Hud() {
    if (this->equipmentArray != 0)
        delete this->equipmentArray;
    this->equipmentArray = 0;
    if (this->eventQueue != 0)
        delete this->eventQueue;
    this->eventQueue = 0;
    if (this->menuButtons != 0) {
        ArrayReleaseClasses(*this->menuButtons);
        delete this->menuButtons;
    }
    this->menuButtons = 0;
    if (this->uintArray != 0)
        delete this->uintArray;
    this->uintArray = 0;
}

void Hud::drawTitleImage(bool visible) {}

int Hud::RADAR_WIDTH;
int Hud::RADAR_HEIGHT;
int Hud::wingmanCommand;
