#include "engine/audio/FModSound.h"
#include "game/ui/Hud.h"
#include "game/weapons/Radar.h"
#include "game/mission/Mission.h"
#include "game/mission/Item.h"
#include "engine/render/Sprite.h"
#include "engine/core/GameText.h"
#include "game/core/Globals.h"
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

void Status_replaceHash(void *out, void *tmpl, void *a, void *b, void *c); // lint: void_ptr (external symbol; param types are mangling-load-bearing)

// lint: void_ptr (external symbol; param types are mangling-load-bearing)

void Image2DCreate(void *canvas, unsigned short id, void *outField); // lint: void_ptr (external symbol; param types are mangling-load-bearing)

// lint: void_ptr (external symbol; param types are mangling-load-bearing)

void Hud_loadImages(Hud * self);

void Hud_buildQuickMenu(Hud *self, int menuType);

static unsigned int g_Hud_heImportantMask = 0;

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
    this->hitFlashTimer = 0;
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

uint8_t Hud::cargoFull() {
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

uint8_t Hud::isHackingGameActive() {
    return this->hackingGameActive;
}

void Hud::setHackingGameActive(bool value) {
    this->hackingGameActive = value;
}

void Hud::setJumpMapSelected(bool value) {
    this->jumpMapSelectedFlag = value;
}

uint8_t Hud::jumpMapSelected() {
    return this->jumpMapSelectedFlag;
}

static Level **g_Hud_level = nullptr;

void Hud::draw(long long t0, long long t1, PlayerEgo *ego, bool letterbox, unsigned int x, unsigned int y) {
    (void) t0;
    (void) t1;
    (void) x;
    (void) y;

    this->letterbox = (unsigned char) letterbox;

    PaintCanvas *canvas = Globals::Canvas;

    canvas->DrawImage2D((unsigned) this->reticleImage, this->field_0x42c, 0);
    {
        unsigned char flags = this->touchFlags;
        unsigned short bx, by;
        int img;
        if ((flags & 0x40) != 0) {
            bx = this->reticleX;
            by = this->reticleY;
            img = this->lockBracketLockedImage;
            this->lockBracketX = bx;
            this->lockBracketY = by;
        } else {
            bx = this->lockBracketX;
            by = this->lockBracketY;
            img = this->lockBracketImage;
        }
        canvas->DrawImage2D((unsigned) img, bx, by, '\x11');
    }

    {
        Status *st = Globals::status;
        bool show = false;
        if (st->inAlienOrbit() == 0) {
            show = true;
        } else if (st->getCurrentCampaignMission() == 0x9a) {
            Level *lvl = *g_Hud_level;
            if (lvl != 0 && lvl->getNumDockingTargets() > 0) show = true;
        }
        if (show && st->getCurrentCampaignMission() > 1) {
            int img = ((this->touchFlags & 0x80) != 0)
                          ? this->orbitMarkerActiveImage
                          : this->orbitMarkerIdleImage;
            canvas->DrawImage2D((unsigned) img, this->field_0x3f8, 0);
        }
    }

    {
        PlayerEgo *e = ego;
        Player *player = *(Player **) ego;
        float scale = (float) this->field_0x446;
        canvas->SetColor((unsigned) 0xffffffffu);

        unsigned short barX = this->field_0x442;
        unsigned short barY = this->field_0x44a;
        if (this->hasShieldBar != 0) {
            int shp = player->getShieldHP();
            int frame = (shp < 2 || this->shieldHitFlash == 0) ? this->shieldFrameImage : this->shieldFrameHitImage;
            canvas->DrawImage2D((unsigned) frame, this->field_0x43c, this->field_0x442);
            canvas->DrawImage2D((unsigned) this->barDividerImage, this->field_0x43e, this->field_0x442);
            canvas->DrawImage2D((unsigned) this->shieldBarBgImage, this->field_0x440, this->field_0x44a);
            int rate = player->getShieldDamageRate();
            int w = (int) ((float) rate * scale);
            canvas->DrawRegion2D((unsigned) this->shieldBarFillImage, 0, 0, w, this->field_0x44c,
                                 (float) w, 0, 0, 0, this->field_0x440);
            barX = this->field_0x444;
            barY = this->field_0x448;
        }

        int ahp = player->getArmorHP();
        int aframe = (ahp < 1) ? this->armorFrameLowImage : this->armorFrameImage;
        canvas->DrawImage2D((unsigned) aframe, this->field_0x43c, barX);
        canvas->DrawImage2D((unsigned) this->barDividerImage, this->field_0x43e, barX);
        canvas->DrawImage2D((unsigned) this->armorBarBgImage, this->field_0x440, barY);
        int hrate = e->getHullDamageRate();
        int hw = (int) ((float) hrate * scale);
        canvas->DrawRegion2D((unsigned) this->armorBarFillImage, 0, 0, hw, this->field_0x44c,
                             (float) hw, 0, 0, 0, this->field_0x440);

        if (this->hasArmorRegen != 0) {
            int arate = player->getArmorDamageRate();
            int aw = (int) ((float) arate * scale);
            canvas->DrawRegion2D((unsigned) this->armorRegenFillImage, 0, 0, aw, this->field_0x44c,
                                 (float) aw, 0, 0, 0, this->field_0x440);
        }
    }

    {
        Level *lvl = *g_Hud_level;
        PlayerEgo *player = lvl ? lvl->getPlayer() : nullptr;

        if (player != 0 && player->hasAutoTurret() != 0) {
            bool on = player->autoTurretIsEnabled() != 0 || ((this->autoTurretFlags & 0x20) != 0);
            int img = on ? this->autoTurretOnImage : this->autoTurretOffImage;
            canvas->DrawImage2D((unsigned) img, this->field_0x3fe, 0);
        } else {
            if (this->secondaryLabelTimer > 0) {
                String *font = (String *) Globals::font;
                int screenW = *(int *) &Globals::w;
                unsigned short iconW = this->field_0x3ec;
                canvas->SetColor((unsigned char) 0xff, 0xff, 0xff, 0xff);
                canvas->DrawImage2D((unsigned) this->pauseButtonPressedImage, this->field_0x3ec, 0);
                int textW = Globals::Canvas->GetTextWidth(
                    (unsigned) (long) canvas, *(String *) (font));
                int tx = this->field_0x3ec + ((screenW - iconW) - textW) / 2;
                Globals::Canvas->DrawString((unsigned) (long) canvas,
                                            this->field_0x51c, tx, 0, false);
                canvas->SetColor((unsigned) 0xffffffffu);
                int t = this->secondaryLabelTimer;
                if (t > 4000) t = 0;
                this->secondaryLabelTimer = t;
            }
        }
    }

    drawOrbitInformation();

    canvas->SetColor((unsigned) 0xffffffffu);
    canvas->DrawImage2D((unsigned) this->missionBannerImage, this->field_0x438, 0);

    drawEventQueue();

    if (this->quickMenuOpen != 0)
        drawMenu(0);

    if (Globals::status != nullptr && Globals::status->inSupernovaSystem() != 0)
        drawChallengeModeScore(0);

    drawPauseButton();

    if (this->messageActive != 0) {
        canvas->SetColor((unsigned char) 0xff, 0xff, 0xff, 0xff);
        String *font = (String *) Globals::font;
        int screenW = *(int *) &Globals::w;
        int w = Globals::Canvas->GetTextWidth((unsigned) (long) canvas, *(String *) (font));
        Globals::Canvas->DrawString((unsigned) (long) canvas,
                                    this->field_0x51c, screenW / 2 - w / 2, this->field_0x3e2, false);
        canvas->SetColor((unsigned) 0xffffffffu);
    }
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
        return;
    }
    if (this->eventQueueTimer > 2000 && this->eventQueuePaused == 0)
        this->eventQueuePaused = 1;
}




// Security-level text colours (r, g, b) indexed by SolarSystem::getSecurityLevel().
static const int g_Hud_securityColors[4][3] = {
    {255, 42, 0},
    {255, 108, 0},
    {237, 237, 0},
    {0, 237, 0},
};

void Hud::drawOrbitInformation() {
    if (Globals::status->inAlienOrbit() != 0)
        return;
    Globals::Canvas->SetColor(0xffffffff);
    int x = Globals::Canvas->GetImage2DWidth(this->factionLogoImage);
    x += Globals::layout->orbitInfoTextX;
    if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->hasNoOwner() == 0)
        Globals::Canvas->DrawImage2D(this->factionLogoImage, 3, 3);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, Globals::status->getStation()->getName(), x,
                                Globals::layout->orbitInfoStationY, false);
    Globals::Canvas->SetColor(0x777777ff);
    if (Globals::status->getCurrentCampaignMission() < 16)
        return;
    int security = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getSecurityLevel();
    if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 26 && Globals::status->field_114 > 1)
        security = 3;
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                String(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getName(), false) + String(" ") +
                                    *Globals::gameText->getText(137),
                                x, Globals::layout->orbitInfoSystemY, false);
    Globals::Canvas->SetColor(g_Hud_securityColors[security][0], g_Hud_securityColors[security][1],
                              g_Hud_securityColors[security][2], 255);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(security + 402), x,
                                Globals::layout->orbitInfoSecurityY, false);
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




static inline bool span(unsigned short o, int w, unsigned int v) {
    return o <= v && v <= (unsigned int) o + w;
}

static inline bool cspan(unsigned short o, int w, unsigned int v) {
    return (unsigned int) o - w <= v && v <= (unsigned int) o + w;
}

unsigned int Hud::touchedElement(unsigned int x, unsigned int y) {
    Array<TouchButton *> *menu = this->menuButtons;
    if (this->quickMenuOpen != 0 && menu != 0) {
        for (unsigned int i = 0; i < menu->size(); i++) {
            if ((*menu)[i]->OnTouchBegin((int) x, (int) y) != 0)
                return *(unsigned int *) (*this->menuButtons)[i];
            menu = this->menuButtons;
        }
        return 0;
    }

    int w = this->touchHalfExtent;
    int w2 = this->touchHalfExtentSmall;

    bool cinematic = *(char *) &Globals::iPad != 0;

    if (cinematic) {
        if (span(this->field_0x40a, w, x) && span(this->field_0x40c, w, y)) return 1;
        if (this->hasBoostButton != 0 && span(this->field_0x410, w, x) && span(this->field_0x412, w, y)) return 2;
        if (span(this->field_0x3f8, w, x) && span(this->field_0x3fa, w, y)) return 0x40;
        if (span(this->field_0x404, w, x) && span(this->field_0x406, w, y)) return 0x100;
        if (cspan(this->reticleX, w, x) && cspan(this->reticleY, w2, y)) return 0x20;
        if (span(this->field_0x3f2, w, x) && span(this->field_0x3f4, w, y)) {
            this->field_0x470 = 1000;
            return 0x80;
        }
        if (cspan(this->field_0x3ec, w, x) && span(this->field_0x3ee, w, y)) return 8;
        if (cspan(this->field_0x3e4, w2 >> 1, x) && cspan(this->field_0x3e6, w2 >> 1, y)) return 0x10;
        if (this->quickMenuEmpty == 0 && span(this->field_0x416, this->field_0x41a, x) &&
            span(this->field_0x418, this->field_0x41c, y))
            return 4;
        if (span(this->field_0x3fe, w, x) && span(this->field_0x400, w, y)) return 0x20000000;
        if (this->hackingGameActive != 0) {
            if (span(this->field_0x454, w, x) && span(this->field_0x456, w, y)) return 0x200;
            if (span(this->field_0x458, w, x) && span(this->field_0x45a, w, y)) return 0x400;
            if (span(this->field_0x45e, w, x) && span(this->field_0x460, w, y)) return 0x800;
        }
        return 0;
    }

    if (this->hackingGameActive != 0) {
        if (span(this->field_0x454, w, x) && span(this->field_0x456, w, y)) return 0x200;
        if (span(this->field_0x458, w, x) && span(this->field_0x45a, w, y)) return 0x400;
        if (span(this->field_0x45e, w, x) && span(this->field_0x460, w, y)) return 0x800;
    }

    int screenW = *(int *) &Globals::w;
    int screenH = *(int *) &Globals::h;

    if (y < (unsigned int) (screenH >> 2)) {
        if (span(this->field_0x40a, w, x) && span(this->field_0x40c, w, y)) return 1;
    } else if (x < (unsigned int) (screenW >> 1)) {
        if (this->hasBoostButton != 0 && cspan(this->field_0x410, w, x) && span(this->field_0x412, w, y)) return 2;
        if (span(this->field_0x3f8, w, x) && span(this->field_0x3fa, w, y)) return 0x40;
        if (span(this->field_0x404, w, x) && span(this->field_0x406, w, y)) return 0x100;
        if (cspan(this->reticleX, w, x) && cspan(this->reticleY, w2, y)) return 0x20;
    } else {
        if (span(this->field_0x3f2, w, x) && span(this->field_0x3f4, w, y)) {
            this->field_0x470 = 1000;
            return 0x80;
        }
        if (cspan(this->field_0x3ec, w, x) && span(this->field_0x3ee, w, y)) return 8;
        if (span(this->field_0x3e4, w2, x) && span(this->field_0x3e6, w2, y)) return 0x10;
        if (this->quickMenuEmpty == 0 && span(this->field_0x416, this->field_0x41a, x) &&
            span(this->field_0x418, this->field_0x41c, y))
            return 4;
        if (span(this->field_0x3fe, w, x) && span(this->field_0x400, w, y)) return 0x20000000;
    }
    return 0;
}

Hud::Hud() {
    init();
}


static const char g_Hud_ccHashX[1] = {0};
static const char g_Hud_ccHashN[1] = {0};
static const char g_Hud_ccUnit[1] = {0};
static const char g_Hud_ccUnit2[1] = {0};

void Hud::catchCargo(int itemId, int count, bool single, bool missionDelivery, bool extender,
                     bool slotMode, bool aggregate) {
    this->field_0x1d0 = 0;
    this->cargoFullFlag = single ? 1 : 0;

    if (missionDelivery) {
        GameText *gt = Globals::gameText;
        String *base = gt->getText(0x219);
        String *dst = &this->field_0x1f4;
        *((String *) (dst)) = *((String *) (base));

        Status *tmpl = Globals::status;
        char a40[12];
        ((String *) (a40))->Set(((String *) (dst))->data);
        int type = Globals::status->getMission()->getType();
        String *typeTxt = gt->getText(type == 3 ? 0x56e : 0x56f);
        char a4c[12];
        ((String *) (a4c))->Set(((String *) (typeTxt))->data);
        char a58[12];
        ((String *) (a58))->ctor_char(g_Hud_ccHashX, false);
        char out1[12];
        Status_replaceHash(out1, tmpl, a40, a4c, a58);
        *((String *) (dst)) = *((String *) (out1));
        {
            String *_s = ((String *) (out1));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a58));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a4c));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a40));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }

        tmpl = Globals::status;
        char a64[12];
        ((String *) (a64))->Set(((String *) (dst))->data);
        char a70[12];
        ((String *) (a70))->Set((long long) (1));
        char a7c[12];
        ((String *) (a7c))->ctor_char(g_Hud_ccHashN, false);
        char out2[12];
        Status_replaceHash(out2, tmpl, a64, a70, a7c);
        *((String *) (dst)) = *((String *) (out2));
        {
            String *_s = ((String *) (out2));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a7c));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a70));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a64));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }

        String *str = new String(*(String *) dst);
        ListItem *item = new ListItem(str);
        item->itemId = itemId;
        addToEventQueue(item);
        return;
    }

    if (single) {
        GameText *gt = Globals::gameText;
        String *txt = gt->getText(0x142);
        *((String *) (&this->field_0x1f4)) = *((String *) (txt));
        String *str = new String(this->field_0x1f4);
        ListItem *item = new ListItem(str, 1);
        addToEventQueue(item);
        return;
    }

    if (count < 1) return;

    GameText *gt = Globals::gameText;

    if (aggregate && this->eventQueueDirty != 0) {
        char a0[12];
        ((String *) (a0))->Set((long long) (this->cargoAggregateCount));
        char ac[12];
        ((String *) (ac))->ctor_char(g_Hud_ccUnit, false);
        char a94[12];
        *(String *) a94 = *(String *) a0 + *(String *) ac;
        char a88[12];
        ((String *) (a88))->Set(((String *) (a94))->data);
        String *unit = gt->getText(itemId + 0x4fa);
        char k34[12];
        *(String *) k34 = *(String *) a88 + *(String *) unit;
        {
            String *_s = ((String *) (a88));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a94));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (ac));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (a0));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }

        char b8[12];
        ((String *) (b8))->Set(((String *) (k34))->data);
        int idx = sameHudEventAsBeforeAggregate(*(String *) b8);
        {
            String *_s = ((String *) (b8));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        if (idx >= 0) {
            this->eventQueueTimer = 2000;
            this->cargoAggregateCount += count;
            char nAc[12];
            ((String *) (nAc))->Set((long long) (this->cargoAggregateCount));
            char nC4[12];
            ((String *) (nC4))->ctor_char(g_Hud_ccUnit2, false);
            char nA0[12];
            *(String *) nA0 = *(String *) nAc + *(String *) nC4;
            char n94[12];
            ((String *) (n94))->Set(((String *) (nA0))->data);
            String *u2 = gt->getText(itemId + 0x4fa);
            char n88[12];
            *(String *) n88 = *(String *) n94 + *(String *) u2;
            *((String *) ((*this->eventQueue)[idx]->name)) = *((String *) (n88));
            {
                String *_s = ((String *) (n88));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            {
                String *_s = ((String *) (n94));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            {
                String *_s = ((String *) (nA0));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            {
                String *_s = ((String *) (nC4));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            {
                String *_s = ((String *) (nAc));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            {
                String *_s = ((String *) (k34));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            return;
        }
        {
            String *_s = ((String *) (k34));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
    }

    this->cargoAggregateCount = count;
    char a0[12];
    ((String *) (a0))->Set((long long) (this->cargoAggregateCount));
    char ac[12];
    ((String *) (ac))->ctor_char(g_Hud_ccUnit, false);
    char a94[12];
    *(String *) a94 = *(String *) a0 + *(String *) ac;
    char a88[12];
    ((String *) (a88))->Set(((String *) (a94))->data);
    String *unit = gt->getText(itemId + 0x4fa);
    char k34[12];
    *(String *) k34 = *(String *) a88 + *(String *) unit;
    *((String *) (&this->field_0x1f4)) = *((String *) (k34));
    {
        String *_s = ((String *) (k34));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (a88));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (a94));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (ac));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (a0));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }

    String *str = new String(this->field_0x1f4);
    ListItem *item = new ListItem(str);
    item->itemId = itemId;
    if (!slotMode || extender) item->buttonKind = 2;
    if (slotMode) item->selectable = 1;
    addToEventQueue(item);
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
    Globals::Canvas->SetColor(0xff, 0xff, 0xff, (unsigned char) alpha);
    int textOffset = (int) ((targetMode == 0 ? -2.0f : -1.0f) * bannerScale);
    Globals::Canvas->DrawImage2D(this->image_0x354, this->field_0x3e0,
                                 (Radar::drawTarget != 0 ? this->field_0x3e2 : 0) - Globals::layout->field_0x1e4);
    ListItem *item = (*this->eventQueue)[1];
    if (item != 0) {
        int y = bannerY + barY;
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
    if (e == 0) {
        for (int i = 0; i != 0x19; i = i + 1) {
            if ((*this->keyArray)[i] == key) {
                this->touchFlags = this->touchFlags & ~(unsigned int) this->elementBits[i];
                this->elementBits[i] = 0;
                (*this->keyArray)[i] = 0;
            }
        }
    } else {
        unsigned int j;
        for (j = 0; j < 0x19; j = j + 1) {
            if ((*this->keyArray)[j] == key) {
                unsigned int v = (unsigned int) this->elementBits[j];
                if (e == v)
                    v = this->touchFlags;
                else
                    v = this->touchFlags & ~v;
                this->touchFlags = v | e;
                this->elementBits[j] = e;
                goto done;
            }
        }
        for (j = 0; j < 0x19; j = j + 1) {
            if ((*this->keyArray)[j] == 0) {
                (*this->keyArray)[j] = key;
                this->elementBits[j] = e;
                this->touchFlags = e | this->touchFlags;
                break;
            }
        }
    }
done:
    return this->touchFlags;
}

unsigned int Hud::sameHudEventAsBefore(String str) {
    for (unsigned int i = this->eventQueue->size() - 1; (int) i > 0; i--) {
        ListItem *e = (*this->eventQueue)[i];
        if (e != 0 && ((String *) e->name)->Compare(str) == 0)
            return 1;
    }
    return 0;
}



static int **g_Hud_initOutX = nullptr;

static int **g_Hud_initOutY = nullptr;

static const int g_Hud_raceBadge[4] = {1190, 1187, 1189, 1188};
static const char g_Hud_initMsg[1] = {0};

int Hud::init() {
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
    Globals::Canvas->Image2DCreate(1320, this->image_0x4f4);
    Globals::Canvas->Image2DCreate(1319, this->image_0x504);
    Globals::Canvas->Image2DCreate(1257, this->image_0x4f8);
    Globals::Canvas->Image2DCreate(1258, this->image_0x508);
    Globals::Canvas->Image2DCreate(1214, this->image_0x4fc);
    Globals::Canvas->Image2DCreate(1215, this->image_0x50c);
    Globals::Canvas->Image2DCreate(1322, this->image_0x500);
    Globals::Canvas->Image2DCreate(1321, this->image_0x510);
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

    this->field_0x4ac = 0;
    this->field_0x4b0 = 0;
    this->field_0x4b4 = 0;
    this->field_0x4b8 = 0;
    this->field_0x4a1 = 0;
    this->field_0x4a2 = 0;
    this->field_0x4a3 = 0;
    this->field_0x4a4 = 0;
    this->field_0x4d4 = Globals::layout->field_0x12c;
    this->field_0x4d8 = Globals::layout->field_0x130;
    this->field_0x4dc = Globals::layout->field_0x134;
    this->analogStickRadius = Globals::layout->field_0x138;
    this->field_0x4e4 = Globals::layout->field_0x13c;
    this->field_0x4e8 = Globals::layout->field_0x140;
    this->field_0x4ec = Globals::layout->field_0x144;
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
    this->field_0x3f6 = Globals::Canvas->GetImage2DWidth(this->image_0x4f4);
    this->field_0x3f2 = Globals::w - this->field_0x3f6 - Globals::layout->field_0x16c;
    this->field_0x3f4 = Globals::h - Globals::layout->field_0x170 - Globals::Canvas->GetImage2DHeight(this->image_0x4f4);
    this->field_0x41a = Globals::Canvas->GetImage2DWidth(this->image_0x34c);
    this->field_0x41c = Globals::layout->field_0x174;
    this->field_0x416 = Globals::w - Globals::layout->field_0x178 - this->field_0x41a;
    this->field_0x418 = Globals::h - Globals::layout->field_0x17c - Globals::Canvas->GetImage2DHeight(this->image_0x34c);
    int imageW = Globals::Canvas->GetImage2DWidth(this->image_0x310);
    this->field_0x3fc = imageW;
    this->field_0x3f8 = Globals::layout->field_0x180;
    this->field_0x3fa = Globals::h - Globals::layout->field_0x184 - imageW;
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
    this->field_0x460 = this->field_0x3ee;
    this->field_0x45e = Globals::w / 2 - this->field_0x45c / 2;
    this->field_0x45a = Globals::h / 2 - Globals::layout->helpWindowY;
    this->field_0x456 = Globals::h / 2 - Globals::layout->helpWindowY;
    this->field_0x42c = Globals::layout->field_0x1a4;
    this->field_0x42e = Globals::h - Globals::layout->field_0x1a8 - Globals::Canvas->GetImage2DHeight(this->image_0x31c);
    this->field_0x422 = Globals::Canvas->GetImage2DWidth(this->image_0x304);
    this->reticleX = this->field_0x42c + this->field_0x430 / 2;
    this->lockBracketX = this->field_0x42c + this->field_0x430 / 2;
    this->reticleY = this->field_0x42e + this->field_0x430 / 2;
    this->lockBracketY = this->field_0x42e + this->field_0x430 / 2;
    imageW = Globals::Canvas->GetImage2DWidth(this->fuelGaugeIconImage);
    this->field_0x414 = imageW;
    this->field_0x410 = Globals::layout->field_0x1ac;
    this->field_0x412 = Globals::h - Globals::layout->field_0x1b0 - imageW;
    imageW = Globals::Canvas->GetImage2DWidth(this->image_0x394);
    this->field_0x408 = imageW;
    this->field_0x404 = Globals::layout->field_0x188;
    this->field_0x406 = Globals::h - Globals::layout->field_0x18c - imageW;
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
    this->field_0x43e = Globals::Canvas->GetImage2DWidth(this->shieldFrameImage) + Globals::layout->field_0x1c4;
    this->field_0x444 = Globals::layout->field_0x1c8;
    this->field_0x448 = Globals::layout->field_0x1cc;
    this->field_0x442 = Globals::layout->field_0x1d0;
    this->field_0x44a = Globals::layout->field_0x1d4;
    this->field_0x446 = Globals::Canvas->GetImage2DWidth(this->shieldBarFillImage);
    this->field_0x440 = this->field_0x43e;
    this->field_0x44c = Globals::Canvas->GetImage2DHeight(this->shieldBarFillImage);
    this->menuOriginX = 0;
    this->menuOriginYBase = 0;
    this->touchHalfExtent = 0;
    this->touchHalfExtentSmall = 0;
    this->autofireEnabled = 0;
    this->field_0x476 = 1;
    this->field_0x474 = 1;
    this->eventLineMargin = 0;
    this->eventLineMarginAlt = 0;
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
    this->field_0x1d0 = 10000;
    this->field_0x1dc[2] = 0;
    this->field_0x218 = Globals::w - 5;
    this->hasBoostButton = Globals::status->getShip()->getBoostDelay() > 0;
    this->hasShieldBar = Globals::status->getShip()->getMaxShieldHP() > 0;
    this->hasArmorRegen = Globals::status->getShip()->getMaxArmorHP() > 0;
    this->hasAutofireUI = Globals::status->getShip()->getFirePower() > 0.0f;
    this->hasCloak = Globals::status->getShip()->hasCloak();
    this->eventQueueDirty = 0;
    this->field_0x234 = 0;
    this->cargoFullFlag = 0;
    this->field_0x236 = 0;
    this->visible = 1;
    this->jumpMapSelectedFlag = 0;
    this->eventQueueTimer = 0;
    this->eventQueuePaused = 0;
    this->shieldHitFlash = 0;
    this->hitFlashTimer = 0;
    this->currentSecondaryWeapon = 0;
    this->menuButtons = 0;
    this->field_0x4cc = 0;
    this->field_0x4d0 = 0;
    this->field_0x276 = 0;
    this->field_0x277 = 0;
    this->weaponSelectState = 0;
    this->fuelGaugeValue = 0;
    this->factionLogoImage = -1;
    this->field_0x0 = 0;
    this->field_0x280 = 1;
    this->field_0x281 = 0;
    this->timeExtenderTimer = -1;
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
    Globals::pause_x = Globals::w - Globals::Canvas->GetImage2DWidth(this->pauseButtonPressedImage) - Globals::layout->field_0x194;
    Globals::pause_y = Globals::layout->field_0x198;
    return 0;
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
        String label = String("X ") + String(this->fuelGaugeValue);
        int gx = this->field_0x4cc + this->field_0x3d4 + this->field_0x3dc / 2;
        int gy = y + Globals::layout->field_0x30_rowHeight / 2 + Globals::layout->field_0x288;
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
        case 30:
            this->field_0x1e0 = String(String("-") + String(arg) + String("t "), false) + *Globals::gameText->getText(1396);
            this->clearQueue();
            break;
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
    }
    if (this->sameHudEventAsBefore(this->field_0x1e0) != 0)
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
    Globals::Canvas->SetColor(0xffffffff);
    int digitW = this->digitSprite->getFrameWidth();
    int overlap = Globals::layout->field_0x2c_rowHeight;
    int digitH = this->digitSprite->getFrameHeight();
    int y = Globals::layout->field_0x2c_rowHeight;
    int screenW = Globals::w;
    String score(Globals::status->field_0x184);
    if (score.size() < 7) {
        int pad = 7 - score.size();
        for (int i = 0; i < pad; i++)
            score = String("0") + score;
    }
    Globals::Canvas->SetColor(0x77ccffff);
    int step = digitW - overlap;
    int half = screenW / 2;
    int span = step * 7 / 2;
    int left = half - span;
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
        int mx = half + rowGap - span;
        for (unsigned int i = 0; i < multiplier.size(); i++) {
            int frame = multiplier.SubString(i, i + 1).ValueOf();
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
    if (this->sameHudEventAsBefore(String(this->field_0x1e0, false)) != 0)
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
    int y;
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
    } else {
        y = this->menuBaseY;
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
            for (unsigned int i = 0; i < this->equipmentArray->size(); i++) {
                if ((*this->equipmentArray)[i] == 0)
                    continue;
                TouchButton *button = new TouchButton(
                        *Globals::gameText->getText((*this->equipmentArray)[i]->getIndex() + 1274) +
                        String(String(" (") + String((*this->equipmentArray)[i]->getAmount()) + String(")"), false),
                        0, this->field_0x3d4, y, this->field_0x3dc, 0x11, 4);
                switch (i) {
                    case 0:
                        button->id = 0x2000;
                        break;
                    case 1:
                        button->id = 0x4000;
                        break;
                    case 2:
                        button->id = 0x8000;
                        break;
                    default:
                        button->id = 0x10000;
                        break;
                }
                ArrayAdd(button, *this->menuButtons);
                y += rowGap + Globals::layout->field_0x30_rowHeight;
            }
            Globals::Canvas->Image2DCreate(1270, this->image_0x35c);
            break;
        case 2: {
            int textIds[4] = {307, 308, 309, (unsigned char) Globals::status->field_f8 != 0 ? 311 : 310};
            int keys[4] = {0x20000, 0x40000, 0x80000, 0x100000};
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
                    float x = button->getPosition().x;
                    (*this->menuButtons)[i]->getPosition();
                    button->setPosition((int) x, (int) ((*this->menuButtons)[i]->getPosition().y -
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
