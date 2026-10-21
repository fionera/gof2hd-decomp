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

namespace {
    struct ItemDatabase {
        unsigned int count;
        Item **itemTable;
    };
}

void liw_set_buildShipPreview(void *self, void *item, void *layout); // lint: void_ptr (imported symbol; param mangling must match lib)

// lint: void_ptr (imported symbol; param mangling must match lib)

void liw_set_fillRows(void *self, void *item, void *layout, int isShip, bool param6); // lint: void_ptr (imported symbol; param mangling must match lib)

// lint: void_ptr (imported symbol; param mangling must match lib)

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







static const float g_liw_s_wFull = 562.5f;
static const float g_liw_s_wAlt = 800.0f;
static const float g_liw_s_wMode = 400.0f;
static const unsigned int g_liw_s_baseAngle = 0u;

void ListItemWindow::set(ListItem *item, unsigned p2, unsigned p3,
                         unsigned p4, unsigned p5, bool p6) {
    this->item = item;
    this->param2 = p2;
    this->param3 = p3;
    this->param4 = p4;
    this->param5 = p5;

    Layout *layout = Globals::layout;

    int w, h, x, y;
    if (Globals::iPad == 0) {
        this->x = 0;
        this->y = 0;
        w = Globals::w;
        h = Globals::h;
        this->width = w;
        this->height = h;
        x = 0;
        y = 0;
    } else {
        float wf;
        if (Globals::iPadHD != 0) {
            h = 0x392;
            wf = g_liw_s_wMode;
        } else {
            bool alt = Globals::iPadLarge == 0;
            wf = alt ? g_liw_s_wAlt : g_liw_s_wFull;
            h = alt ? 0x28a : 0x514;
        }
        w = (int) wf;
        this->height = w;
        this->width = h;
        x = (Globals::w >> 1) - (h >> 1);
        y = (Globals::h >> 1) - (w >> 1);
        this->x = x;
        this->y = y;
    }
    *(uint32_t *) &this->baseAngle = g_liw_s_baseAngle;
    layout->setWindowDimensions(x, y, h, w);

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

    int isShip = this->item->isShip();
    if (isShip == 0) {
        this->previewHeight = 0;
        this->shows3DShipFlag = 0;
    } else {
        this->previewHeight =
                ((((this->height - layout->field_0xc_leftMargin) - layout->field_0x10_rightMargin) - layout->field_0x20_top) - layout->field_0x24)
                / 2
                - layout->field_0x2c_rowHeight;
        *(uint32_t *) &this->previewAngle = g_liw_s_baseAngle;
        this->shows3DShipFlag = 1;
        liw_set_buildShipPreview(this, item, layout);

        this->statsCur = new Array<int>();
        this->statsPrev = new Array<int>();
    }

    {
        int progH = this->previewHeight;

        int sel = (progH > 0) ? layout->field_0x1c : layout->field_0x5c;
        int rowH = layout->field_0x2c_rowHeight;
        int sx = this->x + rowH + (this->width >> 1);
        int sy = layout->field_0x20_top + this->y + rowH + layout->field_0xc_leftMargin + progH + sel;
        int sw = (this->width >> 1) - layout->buttonInsetX;
        int sh = ((this->height -
                   (layout->field_0xc_leftMargin + rowH * 2 + layout->field_0x20_top + progH + sel))
                  - layout->field_0x10_rightMargin) - layout->field_0x24;
        this->scrollWindow = new ScrollTouchWindow(sx, sy, sw, sh, false);
    }

    liw_set_fillRows(this, item, layout, isShip, p6);

    this->dragging = 0;
    this->previewAngle = 0.0f;
    this->dragAccum = 0x104;
    this->dragLastX = 0;
    this->dragSettled = 0;
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
    Layout **a = &Globals::layout;
    PaintCanvas **b = &Globals::Canvas;
    this->labels = 0;
    this->values = 0;
    this->statsCur = 0;
    this->statsPrev = 0;
    Layout *av = *a;
    PaintCanvas *canvas = *b;
    this->scrollWindow = 0;
    (void) av;
    int h = canvas->GetTextHeight((unsigned int) (long) canvas);
    this->textHalfHeight = h / 2 - 1;
    this->previewHeight = 0;
}
