#include "game/ui/ListItemWindow.h"
#include "game/core/Globals.h"
#include "game/ship/Ship.h"
#include "game/mission/BluePrint.h"
#include "game/mission/PendingProduct.h"
#include "engine/render/AEGeometry.h"
#include "game/mission/Item.h"
#include "game/ui/ScrollTouchWindow.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
#include "game/ui/ListItem.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Engine.h"
#include "engine/core/ApplicationManager.h"
#include "game/mission/Status.h"
#include "game/world/Galaxy.h"
#include "game/world/SolarSystem.h"

namespace {
    struct ItemDatabase {
        unsigned int count;
        Item **itemTable;
    };
}

char *LISTITEMWINDOW_UNITS[62] = {
    (char *) "", (char *) "", (char *) "", (char *) "", (char *) "",
    (char *) "", (char *) "", (char *) "", (char *) "", (char *) "",
    (char *) "", (char *) "ms", (char *) "m", (char *) "km/h", (char *) "m",
    (char *) "", (char *) "", (char *) "", (char *) "", (char *) "ms",
    (char *) "", (char *) "%", (char *) "%", (char *) "", (char *) "ms",
    (char *) "%", (char *) "ms", (char *) "ms", (char *) "%", (char *) "ms",
    (char *) "", (char *) "", (char *) "%", (char *) "%", (char *) "",
    (char *) "ms", (char *) "ms", (char *) "ms", (char *) "", (char *) "%",
    (char *) "%", (char *) "ms", (char *) "ms", (char *) "ms", (char *) "m",
    (char *) "", (char *) "%", (char *) "m", (char *) "t", (char *) "km/h",
    (char *) "%", (char *) "m", (char *) "%", (char *) "m", (char *) "%",
    (char *) "", (char *) "%", (char *) "", (char *) "", (char *) "t",
    (char *) "", (char *) "",
};


void ListItemWindow::OnTouchBegin(int x, int y) {
    this->scrollWindow->OnTouchBegin(x, y);
    if (this->shows3DShipFlag &&
        this->x + (this->width >> 1) < x) {
        Layout *obj = (Layout *) Globals::layout;
        if (y < this->y + obj->field_0xc_leftMargin + obj->field_0x20_top + this->previewHeight) {
            this->dragLastX = x;
            this->dragStartX = x;
            this->dragDelta = 0;
            this->dragging = 1;
        }
    }
}

uint8_t ListItemWindow::shows3DShip() {
    return this->shows3DShipFlag;
}

void ListItemWindow::OnTouchMove(int x, int y) {
    this->scrollWindow->OnTouchMove(x, y);
    if (this->shows3DShipFlag && this->dragging) {
        int d = x - this->dragLastX;
        this->dragDelta = d;
        this->spinDamping = 1.0f;
        this->dragAccum = this->dragAccum + d;
        this->dragLastX = x;
    }
}

void ListItemWindow::OnTouchEnd(int x, int y) {
    this->scrollWindow->OnTouchEnd(x, y);
    if (this->shows3DShipFlag && this->dragging) {
        int dv = this->dragDelta;
        int sum = this->dragAccum + dv;
        float vel = (float) dv;
        int a = dv < 0 ? -dv : dv;
        float v = 0.0f;
        if (a > 3) v = vel;
        this->spinDamping = 0.9f;
        this->dragging = 0;
        this->dragAccum = sum;
        this->dragSettled = sum;
        this->spinVelocity = v;
    }
}



void ListItemWindow::render() {
    if (!this->shows3DShipFlag)
        return;

    PaintCanvas **canvas = &Globals::Canvas;
    (*canvas)->Begin3d();

    Layout *obj = Globals::layout;
    int s = obj->field_0x128;
    int h = this->previewHeight - s * 2;
    (*canvas)->EnableClip(
        this->x + s + (this->width >> 1) + obj->field_0x2c_rowHeight,
        this->y + s + obj->field_0xc_leftMargin + obj->field_0x20_top,
        ((this->width >> 1) - (obj->field_0x2c_rowHeight + s * 2)) - obj->buttonInsetX,
        h);
    (*canvas)->SetColor((unsigned int) -1);
    Matrix *m = (Matrix *) (*canvas)->CameraGetLocal(this->camera);
    this->previewTransform = *m;
    this->previewGeometry->render();
    (*canvas)->End3d();
    (*canvas)->DisableClip();
}







// Mesh resource of each ship hull, and of its optional add-on part (0xffff = none).
static const unsigned short LISTITEMWINDOW_SHIP_MESHES[64] = {
    17000, 17001, 17002, 17003, 17004, 17005, 17006, 17007, 17008, 17009, 17010, 17011, 17012,
    17013, 17014, 17015, 17016, 17017, 17018, 17019, 17020, 17021, 17022, 17023, 17024, 17025,
    17026, 17027, 17028, 17029, 17030, 17031, 17032, 17033, 17034, 17035, 17036, 17211, 17216,
    17221, 17227, 17233, 17239, 17245, 17251, 17257, 17263, 17269, 17275, 17281, 17287, 17293,
    17299, 17305, 17311, 17317, 17323, 17329, 17335, 17341, 17347, 17353, 17359, 17365,
};
static const unsigned short LISTITEMWINDOW_SHIP_ADDON_MESHES[64] = {
    0xffff, 0xffff, 0xffff, 0xffff, 18704, 0xffff, 0xffff, 0xffff, 18708, 18709, 0xffff, 0xffff,
    18712, 0xffff, 0xffff, 0xffff, 18716, 0xffff, 18718, 0xffff, 0xffff, 18721, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 18731, 0xffff, 0xffff, 0xffff, 18735,
    0xffff, 0xffff, 0xffff, 18739, 0xffff, 18741, 18742, 18743, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 17286, 17292, 0xffff, 0xffff, 17310, 17316, 18818, 18819, 18820, 18821, 18822,
    18823, 0xffff, 0xffff, 17370,
};
// Item attributes that are never listed.
static const int LISTITEMWINDOW_HIDDEN_ATTRIBUTES[11] = {0, 1, 4, 5, 6, 7, 8, 60, 61};
// Label text id of each item attribute.
static const int LISTITEMWINDOW_KEY_TEXT_IDS[62] = {
    320, 320, 320, 133, 132, 132, 132, 132, 132, 146, 138, 149, 151, 156, 139, 158,
    153, 164, 148, 149, 165, 140, 157, 153, 141, 157, 149, 152, 157, 141, 142, 143,
    164, 144, 145, 157, 149, 149, 150, 147, 146, 157, 157, 149, 151, 159, 157, 151,
    145, 156, 139, 151, 160, 151, 157, 161, 157, 162, 159, 163, 174, 174,
};
static const int LISTITEMWINDOW_NULL_ATTRIBUTE = -979797979;

void ListItemWindow::set(ListItem *item, unsigned p2, unsigned p3,
                         unsigned p4, unsigned p5, bool showPrice) {
    this->item = item;
    this->param2 = p2;
    this->param3 = p3;
    this->param4 = p4;
    this->param5 = p5;

    if (Globals::iPad) {
        float windowHeight = Globals::iPadHD ? 562.5f : (Globals::iPadLarge ? 800.0f : 400.0f);
        this->width = Globals::iPadHD ? 914 : (Globals::iPadLarge ? 1300 : 650);
        this->height = (int) windowHeight;
        this->x = (Globals::w >> 1) - (this->width >> 1);
        this->y = (Globals::h >> 1) - (this->height >> 1);
        if (Globals::iPadHD)
            this->baseAngle = -0.3f;
        else
            this->baseAngle = Globals::retinaDisplay ? -0.4f : -0.2f;
    } else {
        this->x = 0;
        this->y = 0;
        this->width = Globals::w;
        this->height = Globals::h;
        this->baseAngle = 0.0f;
    }
    Globals::layout->setWindowDimensions(this->x, this->y, this->width, this->height);

    if (this->labels != 0) {
        ArrayReleaseClasses(*this->labels);
        delete this->labels;
    }
    this->labels = 0;

    if (this->values != 0) {
        ArrayReleaseClasses(*this->values);
        delete this->values;
    }
    this->values = 0;

    this->labels = new Array<String *>();
    this->values = new Array<String *>();

    if (item->isShip()) {
        this->previewHeight = (this->height - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x10_rightMargin -
                               Globals::layout->field_0x20_top - Globals::layout->field_0x24) / 2 -
                              Globals::layout->field_0x2c_rowHeight;
        this->previewAngle = 2.8f;
        this->shows3DShipFlag = 1;

        int race = item->ship->getRace();
        this->previewGeometry = Globals::globals->getShipGroup(item->ship->getIndex(), race, true);
        Globals::Canvas->TransformCreate(this->shipTransform);
        Globals::Canvas->TransformAddMesh(this->shipTransform, LISTITEMWINDOW_SHIP_MESHES[item->ship->getIndex()], false);
        this->addonTransform = -1;
        if (LISTITEMWINDOW_SHIP_ADDON_MESHES[item->ship->getIndex()] != 0xffff) {
            Globals::Canvas->TransformCreate(this->addonTransform);
            Globals::Canvas->TransformAddMesh(this->addonTransform,
                                              LISTITEMWINDOW_SHIP_ADDON_MESHES[item->ship->getIndex()], false);
        }

        unsigned short texture = race == 0 ? 10095 : race == 1 ? 10097 : race == 3 ? 10096 : race == 2 ? 10098 : 10099;
        Globals::Canvas->TextureCreate(texture, this->shipTexture, false);

        Globals::Canvas->CameraCreate(this->camera);
        Globals::Canvas->CameraSetPerspective(this->camera, 0.9203f, 200.0f, 30000.0f);
        Globals::Canvas->CameraSetCurrent(this->camera);
        Matrix camMatrix = *(Matrix *) Globals::Canvas->CameraGetLocal(this->camera);

        int shipIndex = item->getIndex();
        float offsetX = shipIndex == 51 ? 800.0f : 0.0f;
        float offsetY = shipIndex == 51 ? 700.0f : 0.0f;
        float offsetZ = shipIndex == 51 ? -2000.0f : 0.0f;
        if (Globals::iPadHD) {
            float s = 0.1f;
            AbyssEngine::AEMath::MatrixSetTranslation(camMatrix, 1400.0f - (float) (Globals::w - 1024) * s,
                                                      1736.0f - (float) (2048 - Globals::w) * s * s,
                                                      -5531.0f - (float) (Globals::h - 1536) * s);
            AbyssEngine::AEMath::MatrixSetRotation(camMatrix, 0.39269909f, 3.14159274f, -0.1f);
        } else if (Globals::iPadLarge) {
            float s = 0.6f;
            AbyssEngine::AEMath::MatrixSetTranslation(camMatrix, 1900.0f - (float) (Globals::w - 1024) * s,
                                                      1736.0f - (float) (2048 - Globals::w) * s * s,
                                                      -5531.0f - (float) (Globals::h - 1536) * s);
            AbyssEngine::AEMath::MatrixSetRotation(camMatrix, 0.39269909f, 3.14159274f, -0.1f);
        } else if (Globals::iPad) {
            float s = 0.6f;
            AbyssEngine::AEMath::MatrixSetTranslation(camMatrix, offsetX + (1274.0f - (float) (Globals::w - 1024) * s),
                                                      offsetY + (1736.0f - (float) (1024 - Globals::w) * s * s),
                                                      offsetZ + (-5531.0f - (float) (Globals::h - 768) * s));
            AbyssEngine::AEMath::MatrixSetRotation(camMatrix, 0.39269909f, 3.14159274f, -0.1f);
        } else {
            if (Globals::n9) {
                AbyssEngine::AEMath::MatrixSetTranslation(camMatrix, offsetX + 1750.0f, offsetY + 1500.0f, offsetZ + -3500.0f);
                AbyssEngine::AEMath::MatrixSetRotation(camMatrix, 0.5735988f, 3.14159274f, -0.1f);
            } else {
                AbyssEngine::AEMath::MatrixSetTranslation(camMatrix, offsetX + 1380.0f, offsetY + 1500.0f, offsetZ + -3500.0f);
                AbyssEngine::AEMath::MatrixSetRotation(camMatrix, 0.5735988f, 3.14159274f, -0.1f);
            }
        }
        Globals::Canvas->CameraSetLocal(this->camera, camMatrix);

        ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(-5.0f, 1.0f, -5.0f, 0x4000);
        ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(1.0f, 1.0f, 1.0f, 0x4000);
        ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorAmbient(0.25f, 0.25f, 0.25f, 0x4000);
        ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(0.5f, 0.5f, 0.5f, 0x4000);

        Globals::Canvas->Image2DCreate(1291, (unsigned int &) this->scrollThumbImage);
        this->scrollBarOffsetX = Globals::Canvas->GetImage2DWidth(this->scrollThumbImage);
        this->scrollBarTrackLength = Globals::Canvas->GetImage2DHeight(this->scrollThumbImage);
        this->scrollBarX = this->x + (this->width >> 1) + Globals::layout->field_0x2c_rowHeight +
                           ((this->width >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX) / 2;
        this->scrollBarY = this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                           this->previewHeight / 2;
        Globals::Canvas->Image2DCreate(1298, (unsigned int &) this->arrowDownImage);
        Globals::Canvas->Image2DCreate(1299, (unsigned int &) this->arrowUpImage);
        Globals::Canvas->Image2DCreate(1300, (unsigned int &) this->arrowEqualImage);
        this->field_0x60 = Globals::Canvas->GetImage2DWidth(this->arrowDownImage);

        this->statsCur = new Array<int>();
        this->statsPrev = new Array<int>();
    } else {
        this->previewHeight = 0;
        this->shows3DShipFlag = 0;
    }

    this->scrollWindow = new ScrollTouchWindow(
        this->x + (this->width >> 1) + Globals::layout->field_0x2c_rowHeight,
        this->y + Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
            this->previewHeight + (this->previewHeight > 0 ? Globals::layout->field_0x1c : Globals::layout->field_0x5c),
        (this->width >> 1) - Globals::layout->buttonInsetX,
        this->height - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x2c_rowHeight * 2 - Globals::layout->field_0x20_top - this->previewHeight - (this->previewHeight > 0 ? Globals::layout->field_0x1c : Globals::layout->field_0x5c) - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
        false);

    if (item->isItem() || item->isBluePrint() || item->isPendingProduct()) {
        Item *itm = item->isItem() ? item->item
                    : item->isBluePrint() ? (*Globals::items)[item->bluePrint->getIndex()]
                                          : (*Globals::items)[item->pendingProduct->blueprintIndex];
        (*Globals::status->field_54)[itm->getIndex()] = true;

        for (int i = 0; i < 62; i++) {
            bool special = (itm->getIndex() >= 9 && itm->getIndex() < 12) || itm->getIndex() == 228;
            bool hidden = false;
            for (int j = 0; j < 11; j++) {
                if (LISTITEMWINDOW_HIDDEN_ATTRIBUTES[j] == i || (special && i == 13)) {
                    hidden = true;
                    break;
                }
            }
            if (hidden)
                continue;

            int attr = itm->getAttribute(i);
            if (attr == LISTITEMWINDOW_NULL_ATTRIBUTE)
                continue;

            String *label = new String(*Globals::gameText->getText(LISTITEMWINDOW_KEY_TEXT_IDS[i]), false);
            String *value = 0;
            switch (i) {
                case 15:
                case 16:
                case 23:
                case 30:
                case 31:
                case 57:
                case 58:
                    if (i == 23)
                        value = new String(*Globals::gameText->getText(attr == 0 ? 135 : (attr == 1 ? 154 : 155)), false);
                    else
                        value = new String(*Globals::gameText->getText(attr == 0 ? 135 : 134), false);
                    break;
                case 2:
                    value = new String(*Globals::gameText->getText(attr + 221), false);
                    break;
                default:
                    if (i == 39 || i == 40) {
                        value = new String(String(attr > 0 ? "+" : "") + attr + String(LISTITEMWINDOW_UNITS[i]));
                        break;
                    }
                    switch (i) {
                        case 38:
                            if (item->getIndex() == 85 && Globals::status->hardCoreMode())
                                value = new String(attr * 2);
                            break;
                        case 51:
                        case 53:
                            value = new String(String((int) ((float) attr * 0.1f)) + String(LISTITEMWINDOW_UNITS[i]));
                            break;
                        case 12:
                            if (itm->getSort() == 11) {
                                attr = itm->getAttribute(i) / 1000;
                                value = new String(String(attr) + String("s"));
                                delete label;
                                label = new String(*Globals::gameText->getText(157), false);
                            } else {
                                float hours = (float) attr / 3600.0f;
                                int range = (int) (hours * (float) (itm->getAttribute(13) * 250));
                                int rest = range % 100;
                                attr = (range + rest) % 100 == 0 ? range + rest : range - rest;
                            }
                            break;
                        case 13:
                            attr *= 250;
                            break;
                        case 14:
                            value = new String(String((int) ((float) attr * 0.5f)) + String(LISTITEMWINDOW_UNITS[i]));
                            break;
                        case 49:
                            attr = (int) ((float) attr * 50.0f);
                            break;
                    }
                    if (value == 0) {
                        if (String(LISTITEMWINDOW_UNITS[i]).size() == 0)
                            value = new String(attr);
                        else
                            value = new String(String(attr) + String(LISTITEMWINDOW_UNITS[i]));
                    }
                    break;
            }
            ArrayAdd(label, *this->labels);
            ArrayAdd(value, *this->values);

            if (i == 11 && (itm->getType() == 0 || itm->getType() == 2 || itm->getSort() == 39)) {
                ArrayAdd(new String(*Globals::gameText->getText(188), false), *this->labels);
                int damage = itm->getAttribute(9);
                if (damage == 0)
                    damage = itm->getAttribute(10);
                float dps = (float) damage / (float) itm->getAttribute(11) * 1000.0f;
                String fraction = String((int) ((dps - (float) (int) dps) * 100.0f)).SubString(0, 2);
                ArrayAdd(new String((int) dps + String(".") + fraction), *this->values);
            }
        }

        if (!item->isBluePrint() && !item->isPendingProduct() && showPrice) {
            ArrayAdd(new String(String("\n") + *Globals::gameText->getText(132)), *this->labels);
            ArrayAdd(new String(String("\n") + Globals::layout->formatCredits(itm->getSinglePrice())), *this->values);
        }

        String description;
        bool hasPriceRows = false;
        if (item->isItem()) {
            Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
            if ((*Globals::status->field_0x3c)[item->getIndex()] > 0) {
                String system("");
                if ((*Globals::status->field_0x44)[item->getIndex()] ==
                    ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex())
                    system = *Globals::gameText->getText(215);
                else
                    system = (*systems)[(*Globals::status->field_0x44)[item->getIndex()]]->getName();
                this->str74 = (GameText::getLanguage() == 9 ? String("") : String("-> ")) +
                              Globals::layout->formatCredits((*Globals::status->field_0x3c)[item->getIndex()]) +
                              String(" (") + system + String(")");
                hasPriceRows = true;
            } else {
                this->str74 = String("");
            }
            if ((*Globals::status->field_0x40)[item->getIndex()] > 0) {
                String system("");
                if ((*Globals::status->field_0x48)[item->getIndex()] ==
                    ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex())
                    system = *Globals::gameText->getText(215);
                else
                    system = (*systems)[(*Globals::status->field_0x48)[item->getIndex()]]->getName();
                this->str80 = (GameText::getLanguage() == 9 ? String("") : String("-> ")) +
                              Globals::layout->formatCredits((*Globals::status->field_0x40)[item->getIndex()]) +
                              String(" (") + system + String(")");
                hasPriceRows = true;
            } else {
                this->str80 = String("");
            }
        }
        description = hasPriceRows ? String("\n\n") + *Globals::gameText->getText(214) + String("\n") +
                                         String(this->str74) + String("\n") + String(this->str80)
                                   : String("");
        this->scrollWindow->setText(String(""),
                                    *Globals::gameText->getText(item->getIndex() + 1041) + description);
    } else if (item->isShip()) {
        ArrayAdd(new String(*Globals::gameText->getText(165), false), *this->labels);
        if (item->ship->getBaseHP() < item->ship->getMaxHP()) {
            ArrayAdd(new String(String(item->ship->getMaxHP()) + String(" (+)")), *this->values);
            ArrayAdd(item->ship->getMaxHP(), *this->statsCur);
        } else {
            ArrayAdd(new String(item->ship->getBaseHP()), *this->values);
            ArrayAdd(item->ship->getBaseHP(), *this->statsCur);
        }
        ArrayAdd(Globals::status->getShip()->getBaseHP(), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(166), false), *this->labels);
        if (item->ship->getBaseLoad() < item->ship->getModdedLoad()) {
            ArrayAdd(new String(String(item->ship->getModdedLoad()) + String(" (+)")), *this->values);
            ArrayAdd(item->ship->getModdedLoad(), *this->statsCur);
        } else {
            ArrayAdd(new String(item->ship->getBaseLoad()), *this->values);
            ArrayAdd(item->ship->getBaseLoad(), *this->statsCur);
        }
        ArrayAdd(Globals::status->getShip()->getBaseLoad(), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(265), false), *this->labels);
        ArrayAdd(new String(item->ship->getSlots(0)), *this->values);
        ArrayAdd(item->ship->getSlots(0), *this->statsCur);
        ArrayAdd(Globals::status->getShip()->getSlots(0), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(266), false), *this->labels);
        ArrayAdd(new String(item->ship->getSlots(1)), *this->values);
        ArrayAdd(item->ship->getSlots(1), *this->statsCur);
        ArrayAdd(Globals::status->getShip()->getSlots(1), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(267), false), *this->labels);
        ArrayAdd(new String(item->ship->getSlots(2)), *this->values);
        ArrayAdd(item->ship->getSlots(2), *this->statsCur);
        ArrayAdd(Globals::status->getShip()->getSlots(2), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(269), false), *this->labels);
        if (item->ship->getNumAddedDeviceSlots() > 0) {
            ArrayAdd(new String(String(item->ship->getSlots(3)) + String(" (+)")), *this->values);
            ArrayAdd(item->ship->getSlots(3) + item->ship->getNumAddedDeviceSlots(), *this->statsCur);
        } else {
            ArrayAdd(new String(item->ship->getSlots(3)), *this->values);
            ArrayAdd(item->ship->getSlots(3), *this->statsCur);
        }
        ArrayAdd(Globals::status->getShip()->getSlots(3), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(164), false), *this->labels);
        if (item->ship->getHandlingForShop() > item->ship->getUnmoddedHandling())
            ArrayAdd(new String(String((int) (item->ship->getHandling() * 100.0f)) + String(" (+)")), *this->values);
        else
            ArrayAdd(new String((int) (item->ship->getHandlingForShop() * 100.0f)), *this->values);
        ArrayAdd((int) (item->ship->getHandlingForShop() * 100.0f), *this->statsCur);
        ArrayAdd((int) (Globals::status->getShip()->getUnmoddedHandling() * 100.0f), *this->statsPrev);

        ArrayAdd(new String(*Globals::gameText->getText(132), false), *this->labels);
        ArrayAdd(new String(Globals::layout->formatCredits(item->getPrice())), *this->values);
        ArrayAdd(item->getPrice(), *this->statsCur);
        ArrayAdd(Globals::status->getShip()->getPrice(), *this->statsPrev);

        this->scrollWindow->setText(String(""), *Globals::gameText->getText(item->ship->getIndex() + 977));
    }

    this->dragging = 0;
    this->previewAngle = 0.0f;
    this->dragLastX = 0;
    this->dragSettled = 0;
    this->dragAccum = 260;
    this->dragDelta = 0;
    this->spinDamping = 0.0f;
    this->spinVelocity = 0.0f;
    this->dragStartX = 0;
}

ListItemWindow::~ListItemWindow() {
    if (this->labels) {
        ArrayReleaseClasses(*this->labels);
        delete this->labels;
        this->labels = 0;
    }
    if (this->values) {
        ArrayReleaseClasses(*this->values);
        delete this->values;
        this->values = 0;
    }
    if (this->statsCur) {
        ArrayRelease(*(this->statsCur));
        delete this->statsCur;
        this->statsCur = 0;
    }
    if (this->statsPrev) {
        ArrayRelease(*(this->statsPrev));
        delete this->statsPrev;
        this->statsPrev = 0;
    }
    delete this->scrollWindow;
    this->scrollWindow = 0;
}








static String **g_liw_d_arrowL = nullptr;

static String **g_liw_d_arrowR = nullptr;


void ListItemWindow::draw() {
    if (Globals::iPad)
        Globals::layout->drawMask();
    Globals::Canvas->SetColor(0xff);
    Globals::Canvas->FillRectangle(this->x, this->y, this->width, this->height);
    Globals::layout->drawBox(2, this->x, this->y, this->width, this->height, String(""));
    if (Globals::iPad)
        Globals::layout->drawBox(7, this->x, this->y, this->width, this->height, String(""));
    Globals::layout->drawHeader(String(*Globals::gameText->getText(390), false));
    bool isShip = this->item->isShip();
    Globals::Canvas->SetColor(0xffffffff);

    // Title box with the item / ship image.
    if (this->item->isItem() || this->item->isBluePrint() || this->item->isPendingProduct()) {
        Globals::layout->drawBox(1, this->x + Globals::layout->buttonInsetX, this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                     (this->width >> 1) - (Globals::layout->field_0x2c_rowHeight + Globals::layout->buttonInsetX), Globals::layout->field_0x5c,
                     String(*Globals::gameText->getText(this->item->getIndex() + 1274), false), 2);
        Item *shown;
        if (this->item->isItem())
            shown = this->item->item;
        else if (this->item->isBluePrint())
            shown = (*Globals::items)[this->item->bluePrint->getIndex()];
        else
            shown = (*Globals::items)[this->item->pendingProduct->blueprintIndex];
        Globals::imageFactory->drawItem(shown->getIndex(), shown->getType(),
                                        this->x + Globals::layout->buttonInsetX + Globals::layout->field_0x2c_rowHeight,
                                        this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                            Globals::layout->field_0x5c / 2 - Globals::layout->field_0x2c8 / 2 + Globals::layout->field_0x124);
    } else if (isShip) {
        Globals::layout->drawBox(1, this->x + Globals::layout->buttonInsetX, this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                     (this->width >> 1) - (Globals::layout->field_0x2c_rowHeight + Globals::layout->buttonInsetX), Globals::layout->field_0x5c,
                     String(*Globals::gameText->getText(this->item->ship->getIndex() + 913), false), 2);
        Globals::imageFactory->drawShip(this->item->ship->getIndex(),
                                        this->x + Globals::layout->buttonInsetX + Globals::layout->field_0x2c_rowHeight,
                                        this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                            Globals::layout->field_0x5c / 2 - Globals::layout->field_0x2c8 / 2 + Globals::layout->field_0x124);
    }

    // Attribute rows.
    if (this->labels != 0) {
        int rowY = this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + Globals::layout->field_0x5c +
                   Globals::layout->field_0x2c_rowHeight;
        int rowGap = Globals::layout->field_0x2c_rowHeight;
        if ((unsigned int) ((Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight) * this->labels->size() + rowY) >
            (unsigned int) (Globals::h - Globals::layout->field_0x10_rightMargin))
            rowGap = 2;
        for (unsigned int i = 0; i < this->labels->size(); i++) {
            Globals::Canvas->SetColor(0x255d8dff);
            Globals::layout->drawBox(6, this->x + Globals::layout->buttonInsetX, rowY,
                         (this->width >> 1) - (Globals::layout->field_0x2c_rowHeight + Globals::layout->buttonInsetX), Globals::layout->field_0x1c,
                         String(*(*this->labels)[i], false));
            Globals::Canvas->SetColor(0xffffffff);
            if (isShip && i < this->statsCur->size()) {
                if (i < this->statsCur->size() - 1) {
                    int cur = (*this->statsCur)[i];
                    int prev = (*this->statsPrev)[i];
                    Globals::Canvas->DrawImage2D(cur > prev ? this->arrowUpImage
                                                            : (cur < prev ? this->arrowDownImage : this->arrowEqualImage),
                                                 this->x + (this->width >> 1) - Globals::layout->field_0x2c_rowHeight -
                                                     this->field_0x60,
                                                 rowY);
                }
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *(*this->values)[i],
                    this->x + (this->width >> 1) + 10 - Globals::layout->field_0x2c_rowHeight - this->field_0x60 -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, *(*this->values)[i]),
                    rowY + Globals::layout->field_0x1c / 2 - this->textHalfHeight, false);
            } else {
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*this->values)[i],
                                            this->x + (this->width >> 1) - Globals::layout->field_0x2c_rowHeight * 2 -
                                                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, *(*this->values)[i]),
                                            rowY + Globals::layout->field_0x1c / 2 - this->textHalfHeight, false);
            }
            rowY = rowY + rowGap + Globals::layout->field_0x1c;
        }
    }

    // 3D preview / description area.
    if (this->previewHeight >= 1) {
        Globals::Canvas->SetColor(0x255d8dff);
        Globals::layout->drawBox(8, this->x + (this->width >> 1) + Globals::layout->field_0x2c_rowHeight,
                     this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                     (this->width >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX, this->previewHeight, String(""));
        Globals::layout->drawBox(0, this->x + (this->width >> 1) + Globals::layout->field_0x2c_rowHeight,
                     this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                         (this->previewHeight > 0 ? this->previewHeight + Globals::layout->field_0x2c_rowHeight : 0),
                     (this->width >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX, Globals::layout->field_0x1c,
                     String(*Globals::gameText->getText(280), false));
        Globals::Canvas->SetColor(0xffffffff);
        Globals::Canvas->DrawImage2D(this->scrollThumbImage, this->scrollBarX - this->scrollBarOffsetX,
                                     this->scrollBarY - this->scrollBarTrackLength / 3);
        Globals::Canvas->DrawImage2D(this->scrollThumbImage, this->scrollBarX,
                                     this->scrollBarY - this->scrollBarTrackLength / 3, 1);
    } else {
        Globals::layout->drawBox(1, this->x + (this->width >> 1) + Globals::layout->field_0x2c_rowHeight,
                     this->y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                     (this->width >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX, Globals::layout->field_0x5c,
                     String(*Globals::gameText->getText(280), false));
    }
    this->scrollWindow->drawTextBG();
    this->scrollWindow->draw();
}

static const float g_liw_u_angleTable[64] = {
    1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f,
    1.4f, 1.3f, 1.3f, 1.3f, 1.4f, 1.3f, 1.5f, 1.3f, 1.4f, 1.5f,
    1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f
};
static const float g_liw_u_angleScale = 120.0f;

void ListItemWindow::update(int frameTime) {
    this->scrollWindow->update(frameTime);

    if (this->shows3DShipFlag == 0)
        return;

    if (this->dragging == 0) {
        float spin = this->spinDamping * this->spinVelocity;
        float mag = spin > 0.0f ? spin : -spin;
        this->spinVelocity = spin;
        if (mag > 1.0f) {
            float angle = (float) this->dragAccum;
            this->dragAccum = (int) (spin + angle);
        }
    }

    int idx = this->item->ship->getIndex();

    float tableAngle = g_liw_u_angleTable[idx] + this->baseAngle;
    this->previewAngle = (float) this->dragAccum / g_liw_u_angleScale;

    PaintCanvas **canvas = &Globals::Canvas;
    Matrix *loc = (Matrix *) (*canvas)->TransformGetLocal(this->shipTransform);
    AbyssEngine::AEMath::MatrixSetRotation(*loc, 0.0f, this->previewAngle, 0.0f);
    loc = (Matrix *) (*canvas)->TransformGetLocal(this->shipTransform);
    AbyssEngine::AEMath::MatrixSetScaling(*loc, tableAngle, tableAngle, tableAngle);

    if (this->addonTransform != -1) {
        loc = (Matrix *) (*canvas)->TransformGetLocal(this->addonTransform);
        AbyssEngine::AEMath::MatrixSetRotation(*loc, 0.0f, this->previewAngle, 0.0f);
        loc = (Matrix *) (*canvas)->TransformGetLocal(this->addonTransform);
        AbyssEngine::AEMath::MatrixSetScaling(*loc, tableAngle, tableAngle, tableAngle);
    }

    this->previewGeometry->setRotation(0.0f, this->previewAngle, 0.0f);
}



ListItemWindow::ListItemWindow() {
    this->labels = 0;
    this->values = 0;
    this->statsCur = 0;
    this->statsPrev = 0;
    this->scrollWindow = 0;
    this->textHalfHeight = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 - 1;
    this->previewHeight = 0;
}
