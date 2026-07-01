#include "game/core/AutoPilotList.h"
#include "game/core/Globals.h"
#include "engine/core/GameText.h"
#include "engine/render/PaintCanvas.h"
#include "game/ui/Layout.h"
#include "game/world/Level.h"
#include "game/mission/Status.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/Route.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"









static const char kEmpty[] = "";
static const char kApLit1[] = "";
static const char kApLit2[] = "";

AutoPilotList::AutoPilotList(Level *level) {
    this->entries = new Array<String *>();
    ArraySetLength(5, *(this->entries));
    this->count = 0;

    if (Level::programmedStation != 0) {
        String b(kApLit1);
        String c = *(Globals::gameText)->getText(0x222) + b;
        String a = Level::programmedStation->getName();
        (*this->entries)[0] = new String(c + a);
        this->count++;
    }

    if (((SolarSystem *) Globals::status->getSystem())->currentOrbitHasWarpGate() != 0) {
        String *s = new String;
        s->Set(((Globals::gameText)->getText(0x223))->data);
        (*this->entries)[1] = s;
        this->count++;
    }

    if (Globals::status->inEmptyOrbit() == 0) {
        String c = ((Station *) (&c))->getName();
        String b(kApLit2);
        String a = b + c;
        (*this->entries)[2] = new String(a + *(Globals::gameText)->getText(0x88));
        this->count++;
    }

    String *cancel = new String;
    cancel->Set(((Globals::gameText)->getText(0x225))->data);
    (*this->entries)[3] = cancel;
    this->count++;

    if (((PlayerEgo *) ((Level *) level)->getPlayer())->getRoute() != 0) {
        Route *route = (Route *) (intptr_t)((PlayerEgo *) ((Level *) level)->getPlayer())->getRoute();
        if (*((uint8_t *) route->getLastWaypoint() + 0x130) == 0) {
            String *s = new String;
            s->Set(((Globals::gameText)->getText(0x23d))->data);
            (*this->entries)[4] = s;
            this->count++;
        }
    }

    this->width = 0;
    this->selected = 0;
    unsigned char *font = (unsigned char *) Globals::font;
    for (uint32_t i = 0; i < this->entries->size(); i++) {
        if ((*this->entries)[i] != nullptr) {
            int w = Globals::Canvas->GetTextWidth(
                        (unsigned int) (uintptr_t) font, *(*this->entries)[i]) + 0x13;
            if (this->width < w)
                this->width = w;
        }
    }

    this->x = (Globals::w - this->width) / 2;
    this->y = (Globals::h + this->count * -0xf - 0xc) / 2;
    while ((*this->entries)[this->selected] == nullptr)
        this->down();
}

AutoPilotList::~AutoPilotList() {
    if (this->entries != nullptr) {
        ArrayReleaseClasses(*this->entries);
    }
    delete this->entries;
    this->entries = nullptr;
}

void AutoPilotList::down() {
    String **data = this->entries->data();
    int i = this->selected;
    do {
        i = (i < 4) ? i + 1 : 0;
    } while (data[i] == nullptr);
    this->selected = i;
}

void AutoPilotList::up() {
    String **data = this->entries->data();
    int i = this->selected;
    do {
        i = (i > 0) ? i - 1 : 4;
    } while (data[i] == nullptr);
    this->selected = i;
}

int AutoPilotList::touch(int px, int py) {
    int row;
    if (px < this->x ||
        this->x + this->width <= px ||
        (row = (py - this->y - 0xe) / 0xf, py - this->y < -0xf) ||
        (uint32_t)(row + 1) >= this->entries->size()) {
        return -1;
    }
    this->selected = 0;
    for (int i = 0; i <= row; i++)
        this->down();
    return this->selected;
}

int AutoPilotList::fire() {
    return this->selected;
}

String AutoPilotList::getTargetString() {
    int idx = this->selected;
    if (idx >= 0 && (uint32_t) idx < this->entries->size())
        return *(*this->entries)[idx];
    return String(kEmpty);
}

void AutoPilotList::draw() {
    String title;
    title.Set(((Globals::gameText)->getText(0x23c))->data);
    (Globals::layout)->drawWindow(title, this->x, this->y, this->width,
                                     this->count * 0xf + 0x16);

    int drawn = 0;
    for (uint32_t i = 0; i < this->entries->size(); i++) {
        String *text = (*this->entries)[i];
        if (text != nullptr) {
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *text,
                this->x, drawn * 0xf + this->y + 0x12, false);
            drawn++;
        }
    }
}
