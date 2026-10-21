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




static const char g_Hud_oiSep[1] = {0};
static const unsigned char g_Hud_secColors[4 * 0xc] = {0};

void Hud::drawOrbitInformation() {
    if (Globals::status->inAlienOrbit() != 0) return;

    int *layout = (int *) Globals::layout;
    Globals::Canvas->SetColor((unsigned) (-1));
    int x = Globals::Canvas->GetImage2DWidth((unsigned) (0)) + layout[0x87];

    if (((SolarSystem *) ((long) Globals::status->getSystem()))->hasNoOwner() == 0)
        Globals::Canvas->DrawImage2D((unsigned) this->factionLogoImage, 3, 0);

    String *font = (String *) Globals::font;

    {
        char name[12];
        ((Station *) (name))->getName();
        Globals::Canvas->DrawString((unsigned) (long) (font), *(String *) (name), (x), (char) layout[0x88], false);
        {
            String *_s = ((String *) (name));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
    }
    Globals::Canvas->SetColor((unsigned) (-1));

    if (Globals::status->getCurrentCampaignMission() <= 0xf) return;

    SolarSystem *sys = (SolarSystem *) ((long) Globals::status->getSystem());
    int sec = sys->getSecurityLevel();
    int idx = ((SolarSystem *) ((long) Globals::status->getSystem()))->getIndex();
    int *status = (int *) Globals::status;
    if (idx == 0x1a && status[0x45] > 1) sec = 3;

    {
        char sysName[12], copy[12], sep[12], acc[12], full[12];
        ((SolarSystem *) (sysName))->getName();
        ((String *) (copy))->Set(((String *) (sysName))->data);
        ((String *) (sep))->ctor_char(g_Hud_oiSep, false);
        *(String *) acc = *(String *) copy + *(String *) sep;
        String *txt = (Globals::gameText)->getText(0);
        *(String *) full = *(String *) acc + *(String *) txt;
        Globals::Canvas->DrawString((unsigned) (long) (font), *(String *) (full), (x), (char) layout[0x89], false);
        {
            String *_s = ((String *) (full));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (acc));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (sep));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (copy));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        {
            String *_s = ((String *) (sysName));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
    }

    const unsigned char *row = g_Hud_secColors + sec * 0xc;
    Globals::Canvas->SetColor((unsigned char) (row[0]), (unsigned char) (row[4]), (unsigned char) (row[8]),
                              (unsigned char) (0xff));
    String *secTxt = (Globals::gameText)->getText(sec);
    Globals::Canvas->DrawString((unsigned) (long) (font), *(String *) (secTxt), (x), (char) layout[0x8a], false);
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
    String *font = (String *) Globals::font;
    PaintCanvas *canvas = Globals::Canvas;
    int x;
    if (this->letterbox == 0) {
        int base = this->eventLineMargin;
        int yBase = this->eventLineX;
        if (rightAlign == 0) {
            int w = Globals::Canvas->GetTextWidth((unsigned) (long) (canvas), *(String *) (font));
            x = (base + 3) - w;
        } else {
            x = -3 - base;
        }
        x = x + yBase;
    } else {
        if (rightAlign == 0) {
            int margin = this->eventLineMarginAlt;
            int screenW = *(int *) &Globals::w;
            int w = Globals::Canvas->GetTextWidth((unsigned) (long) (canvas), *(String *) (font));
            x = ((screenW - 1) - margin) - w;
        } else {
            x = this->eventLineMarginAlt + 1;
        }
    }
    char y = (char) (this->eventLineY - 1);
    Globals::Canvas->DrawString((unsigned) (long) (font), text, (x), (y), false);
}

void Hud::setCurrentSecondaryWeapon(Item *item) {
    this->currentSecondaryWeapon = item;
    this->updateSecondaryWeaponString();
}

int Hud::sameHudEventAsBeforeAggregate(String str) {
    Array<ListItem *> *q = this->eventQueue;
    int i = (int) q->size();
    ListItem *e;
    do {
        i = i + -1;
        if (i < 1)
            return -1;
        e = (*q)[i];
    } while (e == 0 || ((String *) e->name)->Compare(str) != 0);
    return i;
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
    int i = (int) this->eventQueue->size() - 1;
    while (i >= 1) {
        ListItem *e = (*this->eventQueue)[i];
        if (e != 0 && ((String *) e->name)->Compare(str) == 0)
            return 1;
        i--;
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

Hud *Hud::checkIfQuickMenuIsEmpty() {
    Ship *ship = Globals::status->getShip();
    Array<Item *> *equip = ship->getEquipment(1);
    this->equipmentArray = equip;

    unsigned char empty;
    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip->size(); i++) {
            if ((*equip)[i] != 0) {
                hasSecondary = true;
                break;
            }
        }
    }
    if (hasSecondary) {
        empty = 0;
    } else if (ship->hasJumpDrive() == 0 && Globals::status->getWingmen() == 0) {
        empty = (unsigned char) ship->hasCloak();
    } else {
        empty = 0;
    }
    this->quickMenuEmpty = empty;

    updateSecondaryWeaponString();
    {
        Ship *ship2 = Globals::status->getShip();
        Array<Item *> *equip2 = ship2->getEquipment(1);
        this->equipmentArray = equip2;

        bool hasSecondary2 = false;
        if (equip2 != 0) {
            for (unsigned int i = 0; i < equip2->size(); i++) {
                if ((*equip2)[i] != 0) {
                    hasSecondary2 = true;
                    break;
                }
            }
        }
        unsigned char empty2;
        if (hasSecondary2) {
            empty2 = 0;
        } else if (ship2->hasJumpDrive() == 0 && Globals::status->getWingmen() == 0) {
            empty2 = (unsigned char) (ship2->hasCloak() == 0);
        } else {
            empty2 = 0;
        }
        this->quickMenuEmpty = empty2;
    }
    return this;
}


static const char g_Hud_dmPrefix[1] = {0};

void Hud::drawMenu(int unused) {
    (void) unused;
    int *layout = (int *) Globals::layout;
    ((Layout *) (layout))->drawMask();

    Globals::Canvas->DrawImage2D((unsigned) this->quickMenuTopImage, this->field_0x3c4 + this->menuOriginX, 0);

    int hx = this->menuOriginX + this->field_0x3d4 + this->field_0x3dc / 2;
    char hy = (char) ((char) this->menuOriginYBase + (char) this->menuOriginY + (char) (this->menuRowHeight / 2)
                      - (char) layout[0x8b]);
    Globals::Canvas->DrawImage2D((unsigned) this->pauseButtonImage, hx, hy, (unsigned char) 0x11);

    int y = this->menuOriginY + this->menuOriginYBase + this->menuRowHeight;

    if (this->menuButtons != 0 && this->menuButtons->size() != 0) {
        unsigned int count = (unsigned int) this->menuButtons->size();
        for (unsigned int i = 0; i < count - 1; i++) {
            Globals::Canvas->DrawImage2D((unsigned) this->quickMenuMiddleImage, this->field_0x3c4 + this->menuOriginX,
                                         0);
            y += this->field_0x3d0;
            count = (unsigned int) this->menuButtons->size();
        }
    }

    Globals::Canvas->DrawImage2D((unsigned) this->quickMenuBottomImage, this->field_0x3c4 + this->menuOriginX, 0);

    if (this->menuButtons != 0 && this->menuButtons->size() != 0) {
        unsigned int n = (unsigned int) this->menuButtons->size();
        for (unsigned int i = 0; i < n; i++) {
            (*this->menuButtons)[i]->draw();
            n = (unsigned int) this->menuButtons->size();
        }
    }

    if (this->menuType != 0) return;

    Ship *ship = Globals::status->getShip();
    if (!ship->hasCloak() && ship->hasJumpDrive() == 0) return;

    char prefix[12], num[12], label[12];
    ((String *) (prefix))->ctor_char(g_Hud_dmPrefix, false);
    ((String *) (num))->Set((long long) (this->fuelGaugeValue));
    *(String *) label = *(String *) prefix + *(String *) num;
    {
        String *_s = ((String *) (num));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (prefix));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }

    int gx = this->menuOriginX + this->field_0x3d4 + this->field_0x3dc / 2;
    unsigned char gy = (unsigned char) ((char) y + (char) (layout[0xc] / 2)
                                        + (char) layout[0xa2]);
    Globals::Canvas->DrawImage2D((unsigned) this->fuelGaugeBarImage, gx, gy, (unsigned char) 0x11);
    Globals::Canvas->DrawImage2D((unsigned) this->fuelGaugeIconImage, gx - layout[0x8c],
                                 (char) layout[0xc] + (char) gy + (char) layout[0xa3], (unsigned char) 0x11);

    int barW = layout[0x8c];
    String *font = (String *) Globals::font;
    int ih = Globals::Canvas->GetImage2DHeight((unsigned) (0));
    int th = Globals::Canvas->GetTextHeight(0);
    char ty = (char) (((gy + (char) (ih / 2)) - (char) (th / 2)) + (char) layout[0x8d]);
    Globals::Canvas->DrawString((unsigned) (long) (font), *(String *) (label), (barW + gx), (ty), false);
    {
        String *_s = ((String *) (label));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
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
    (void) ego;

    switch (eventId) {
        case 1:
        case 2:
            if (this->hasAutofireUI == 0) return;
            break;
        case 3:
            if (this->hasBoostButton == 0 || ((PlayerEgo *) ((long) arg))->readyToBoost() == 0) return;
            break;
        case 4:
            if (this->hasBoostButton == 0) return;
            break;

        case 0x23:
            this->field_0x468 = 0;
            this->field_0x27a = 1;
            this->weaponSelectState = 1;
            return;
        case 0x25:
            this->field_0x468 = 0;
            this->field_0x27a = 1;
            this->weaponSelectState = 0x101;
            return;
        case 0x27:
            this->field_0x468 = 0;
            this->field_0x27a = 0;
            this->weaponSelectState = 1;
            return;
        case 0x29:
            this->field_0x468 = 0;
            this->field_0x27a = 0;
            this->weaponSelectState = 0x101;
            return;
        case 0x24:
        case 0x26:
        case 0x28:
        case 0x2a:
            *(unsigned char *) &this->weaponSelectState = 0;
            break;

        default:
            break;
    }

    String *line = (String *) &this->field_0x1e0;
    char probe[12];
    ((String *) (probe))->Set((line)->data);
    unsigned int dup = sameHudEventAsBefore(*(String *) probe);
    {
        String *_s = ((String *) (probe));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    if (dup != 0)
        return;

    String *str = new String(*line);

    unsigned int idBit = (unsigned int) (eventId - 1);
    ListItem *item;
    if (idBit < 0x15 && ((1u << (idBit & 0x1f)) & g_Hud_heImportantMask) != 0)
        item = new ListItem(str, 1);
    else
        item = new ListItem(str, 0);
    addToEventQueue(item);

    String *font = (String *) Globals::font;
    int w = Globals::Canvas->GetTextWidth((unsigned) (long) (Globals::Canvas), *(String *) (font));
    int screenW = *(int *) &Globals::w;
    this->eventScrollTick = 0;
    this->eventScrolls = 1;
    this->letterbox =
            (unsigned char) ((screenW / 2 - this->eventLineMargin) + this->eventLineMarginAlt * -2 < w);
}



static const char g_Hud_csZero[1] = {0};

void Hud::drawChallengeModeScore(int unused) {
    (void) unused;
    int *layout = (int *) Globals::layout;
    int *status = (int *) Globals::status;
    int screenW = *(int *) &Globals::w;
    Sprite *sprite = this->digitSprite;

    Globals::Canvas->SetColor((unsigned) (-1));
    int fw = ((Sprite *) (sprite))->getFrameWidth();
    int pad = layout[0xb];
    int fh = ((Sprite *) (sprite))->getFrameHeight();
    int y = layout[0xb];

    char score[12];
    ((String *) (score))->Set((long long) (status[0x61]));
    int slen = (int) ((String *) score)->size();
    if (slen < 7) {
        for (int k = 0; k < 7 - slen; k++) {
            char z[12], acc[12];
            ((String *) (z))->ctor_char(g_Hud_csZero, false);
            *(String *) acc = *(String *) z + *(String *) score;
            *((String *) (score)) = *((String *) (acc));
            {
                String *_s = ((String *) (acc));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            {
                String *_s = ((String *) (z));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
        }
    }

    Globals::Canvas->SetColor((unsigned) (-1));
    int dw = fw - pad;
    int half = screenW / 2;
    int span = (dw * 7) / 2;
    int startX = half - span;
    {
        int len = (int) ((String *) score)->size();
        int x = startX;
        for (int i = 1; (unsigned int) (i - 1) < (unsigned int) len; i++) {
            char ch[12];
            *((String *) (ch)) = ((String *) score)->SubString(i - 1, i);
            int frame = ((String *) (ch))->ValueOf();
            {
                String *_s = ((String *) (ch));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            ((Sprite *) (sprite))->setFrame(frame);
            ((Sprite *) (sprite))->setPosition(x, y);
            ((Sprite *) (sprite))->draw(1.0f, 1.0f);
            x += dw;
        }
    }

    if (status[0x60] > 0 && status[0x63] > 1) {
        Globals::Canvas->SetColor((unsigned) (-1));
        int yRow = y + fh + pad;
        int scoreVal = status[0x60];
        if (scoreVal < 0xbb9) {
            if (scoreVal % 100 >= 0x32) {
                int mult = status[0x63];
                float bonus = (float) mult;
                float base = (float) (mult * 1000);
                char bonusStr[12];
                ((String *) (bonusStr))->Set((long long) (int) ((bonus * 0.0f + 1.0f) * base));
                int bl = (int) ((String *) bonusStr)->size();
                int bx = (screenW / 2 - ((bl * dw) >> 1));
                int bonusY = fh + yRow + pad;
                int len = (int) ((String *) bonusStr)->size();
                int x = bx;
                for (int i = 1; (unsigned int) (i - 1) < (unsigned int) len; i++) {
                    char ch[12];
                    *((String *) (ch)) = ((String *) bonusStr)->SubString(i - 1, i);
                    int frame = ((String *) (ch))->ValueOf();
                    {
                        String *_s = ((String *) (ch));
                        if (_s->data) delete[] _s->data;
                        _s->data = nullptr;
                        _s->length = 0;
                    }
                    ((Sprite *) (sprite))->setFrame(frame);
                    ((Sprite *) (sprite))->setPosition(x, bonusY);
                    ((Sprite *) (sprite))->draw(1.0f, 1.0f);
                    x += dw;
                }
                {
                    String *_s = ((String *) (bonusStr));
                    if (_s->data) delete[] _s->data;
                    _s->data = nullptr;
                    _s->length = 0;
                }
            }
        }
        Globals::Canvas->DrawImage2D((unsigned) this->multiplierIconImage, pad + startX, 0);

        char timeStr[12];
        ((String *) (timeStr))->Set((long long) (status[0x63]));
        int tx = (half + pad) - span + Globals::Canvas->GetImage2DWidth((unsigned) (0));
        int len = (int) ((String *) timeStr)->size();
        int x = tx;
        for (int i = 1; (unsigned int) (i - 1) < (unsigned int) len; i++) {
            char ch[12];
            *((String *) (ch)) = ((String *) timeStr)->SubString(i - 1, i);
            int frame = ((String *) (ch))->ValueOf();
            {
                String *_s = ((String *) (ch));
                if (_s->data) delete[] _s->data;
                _s->data = nullptr;
                _s->length = 0;
            }
            ((Sprite *) (sprite))->setFrame(frame);
            ((Sprite *) (sprite))->setPosition(x, yRow);
            ((Sprite *) (sprite))->draw(1.0f, 1.0f);
            x += dw;
        }
        {
            String *_s = ((String *) (timeStr));
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
    }
    Globals::Canvas->SetColor((unsigned) (-1));
    {
        String *_s = ((String *) (score));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
}



static const char g_Hud_meSep[1] = {0};
static const char g_Hud_meEnd[1] = {0};

void Hud::hudEventMedal(int medalId, int percent) {
    GameText *gt = Globals::gameText;
    String *name = gt->getText(medalId + 0x5e3);

    char sep[12], acc1[12], num[12], acc2[12], end[12], acc3[12];
    ((String *) (sep))->ctor_char(g_Hud_meSep, false);
    *(String *) acc1 = *(String *) name + *(String *) sep;
    if (percent >= 100) percent = 100;
    ((String *) (num))->Set((long long) (percent));
    *(String *) acc2 = *(String *) acc1 + *(String *) num;
    ((String *) (end))->ctor_char(g_Hud_meEnd, false);
    *(String *) acc3 = *(String *) acc2 + *(String *) end;

    String *dst = &this->field_0x1e0;
    *((String *) (dst)) = *((String *) (acc3));
    {
        String *_s = ((String *) (acc3));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (end));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (acc2));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (num));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (acc1));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    {
        String *_s = ((String *) (sep));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }

    char probe[12];
    ((String *) (probe))->Set(((String *) (dst))->data);
    int same = sameHudEventAsBefore(*(String *) probe);
    {
        String *_s = ((String *) (probe));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    if (same != 0) return;

    String *str = new String(*(String *) dst);
    ListItem *item = new ListItem(str, 3);
    addToEventQueue(item);

    int w = Globals::Canvas->GetTextWidth((unsigned) (long) (Globals::Canvas), *(String *) ((String *) Globals::font));
    int screenW = *(int *) &Globals::w;
    this->eventScrollTick = 0;
    this->eventScrolls = 1;
    this->letterbox = ((screenW / 2 - this->eventLineMargin) + this->eventLineMarginAlt * -2 < w) ? 1 : 0;
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
                        String(String(" (") + String((*this->equipmentArray)[i]->getAmount()) + String(")")),
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
                            String(Globals::status->getStation()->getName()) +
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
                            String(Level::programmedStation->getName()),
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
