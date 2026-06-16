#include "game/ui/ListItemWindow.h"
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
#include "game/core/PaintCanvasClass.h"

// Heavy data-driven preview / row-fill regions kept in the engine: the ship
// preview builder (transform/camera/light setup) and the attribute row filler
// (the large per-attribute stat switch). They take the window, the item and the
// already-resolved layout pointer.
extern "C" void liw_set_buildShipPreview(void *self, void *item, void *layout);
extern "C" void liw_set_fillRows(void *self, void *item, void *layout, int isShip, bool param6);

// Tail of render() (0x1ac428): final canvas cleanup after the preview geometry.
extern "C" void _liw_render_tail(void *c, int a, int h, void *sp);

// PC-relative game singletons (pointers to the live screen/layout/canvas state).
__attribute__((visibility("hidden"))) extern int **g_liw_screen;

void ListItemWindow::OnTouchBegin(int x, int y)
{
    this->scrollWindow->OnTouchBegin(x, y);
    if (this->shows3DShipFlag &&
        this->x + (this->width >> 1) < x) {
        int *obj = *g_liw_screen;  // RAWREAD: opaque screen-state int array, no modeled class
        if (y < this->y + obj[0xc / 4] + obj[0x20 / 4] + this->previewHeight) {
            this->dragLastX = x;
            this->dragStartX = x;
            this->dragDelta = 0;
            this->dragging = 1;
        }
    }
}

uint8_t ListItemWindow::shows3DShip()
{
    return this->shows3DShipFlag;
}

void ListItemWindow::OnTouchMove(int x, int y)
{
    this->scrollWindow->OnTouchMove(x, y);
    if (this->shows3DShipFlag && this->dragging) {
        int d = x - this->dragLastX;
        this->dragDelta = d;
        this->spinDamping = 1.0f;
        this->dragAccum = this->dragAccum + d;
        this->dragLastX = x;
    }
}

void ListItemWindow::OnTouchEnd(int x, int y)
{
    this->scrollWindow->OnTouchEnd(x, y);
    if (this->shows3DShipFlag && this->dragging) {
        int dv = this->dragDelta;
        int sum = this->dragAccum + dv;
        float vel = (float)dv;
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

__attribute__((visibility("hidden"))) extern void **g_liw_r_canvas;
__attribute__((visibility("hidden"))) extern void **g_liw_r_obj;

void ListItemWindow::render()
{
    if (!this->shows3DShipFlag)
        return;

    PaintCanvas *canvas = (PaintCanvas *)*g_liw_r_canvas;
    canvas->Begin3d();

    int *obj = (int *)*g_liw_r_obj;  // RAWREAD: opaque layout/screen-state int array, no modeled class
    int s = obj[0x128 / 4];
    int h = this->previewHeight - s * 2;
    canvas->EnableClip(
        this->x + s + (this->width >> 1) + obj[0x2c / 4],
        this->y + s + obj[0xc / 4] + obj[0x20 / 4],
        ((this->width >> 1) - (obj[0x2c / 4] + s * 2)) - obj[0x28 / 4],
        h);
    canvas->SetColor((unsigned int)(long)canvas);
    void *m = canvas->CameraGetLocal((unsigned int)(long)canvas);
    this->previewTransform = *(Matrix *)m;
    this->previewGeometry->render();
    canvas->End3d();
    int dummy;
    _liw_render_tail((void *)canvas, 0, h, &dummy);
}

// ---- ListItemWindow::set ------------------------------------------------------
// Re-targets the window onto a new ListItem: stores the item and its four
// mode/colour parameters, picks the window geometry from the current language /
// fullscreen state, rebuilds the label/value (and, for ships, stat) arrays,
// builds the rotating 3D preview for ships, constructs the inner scroll window,
// fills the rows and resets the preview spin state.

// PC-relative game state roots (pointers to live flags / screen dimensions).
__attribute__((visibility("hidden"))) extern char **g_liw_s_fullscreen;
__attribute__((visibility("hidden"))) extern char **g_liw_s_modeFlag;
__attribute__((visibility("hidden"))) extern char **g_liw_s_altFlag;
__attribute__((visibility("hidden"))) extern int   *g_liw_s_screenW;
__attribute__((visibility("hidden"))) extern int   *g_liw_s_screenH;
__attribute__((visibility("hidden"))) extern void **g_liw_s_layoutHolder;

// Width/height presets (read-only constants in the binary).
extern const float g_liw_s_wFull;
extern const float g_liw_s_wAlt;
extern const float g_liw_s_wMode;
extern const unsigned int g_liw_s_baseAngle;   // default preview angle bits

void ListItemWindow::set(ListItem *item, unsigned p2, unsigned p3,
                         unsigned p4, unsigned p5, bool p6)
{
    this->item = item;
    this->param2 = p2;
    this->param3 = p3;
    this->param4 = p4;
    this->param5 = p5;

    Layout *layout = (Layout *)*g_liw_s_layoutHolder;

    int w, h, x, y;
    if (*g_liw_s_fullscreen == 0) {
        // borderless: window spans the whole screen origin.
        this->x = 0;
        this->y = 0;
        w = *g_liw_s_screenW;
        h = *g_liw_s_screenH;
        this->width = w;
        this->height = h;
        x = 0;
        y = 0;
    } else {
        // pick width/height from the HUD-mode + language flags.
        float wf;
        if (**g_liw_s_modeFlag != 0) {
            h = 0x392;
            wf = g_liw_s_wMode;
        } else {
            bool alt = **g_liw_s_altFlag == 0;
            wf = alt ? g_liw_s_wAlt : g_liw_s_wFull;
            h = alt ? 0x28a : 0x514;
        }
        w = (int)wf;
        this->height = w;
        this->width = h;
        x = (*g_liw_s_screenW >> 1) - (h >> 1);
        y = (*g_liw_s_screenH >> 1) - (w >> 1);
        this->x = x;
        this->y = y;
    }
    *(uint32_t *)&this->baseAngle = g_liw_s_baseAngle;   // default preview angle bits
    layout->setWindowDimensions(x, y, h, w);

    // Tear down the label array.
    if (this->labels != 0) {
        for (String *str : *this->labels) delete str;
        this->labels->clear();
        delete this->labels;
    }
    this->labels = 0;
    // Tear down the value array.
    if (this->values != 0) {
        for (String *str : *this->values) delete str;
        this->values->clear();
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
        // progress bar height + 3D ship preview (transform/camera/light setup).
        this->previewHeight =
            ((((this->height - layout->field_0xc) - layout->field_0x10) - layout->field_0x20) - layout->field_0x24) / 2
            - layout->field_0x2c;
        *(uint32_t *)&this->previewAngle = g_liw_s_baseAngle;   // default preview angle bits
        this->shows3DShipFlag = 1;
        liw_set_buildShipPreview(this, item, layout);

        // ship stat rows need the two int arrays too.
        this->statsCur  = new Array<int>();
        this->statsPrev = new Array<int>();
    }

    // Inner scroll window, positioned below the preview / header.
    {
        int progH = this->previewHeight;
        // selector: field_0x1c (row spacing) when a preview is shown, else field_0x5c.
        int sel = (progH > 0) ? layout->field_0x1c : layout->field_0x5c;
        int rowH = layout->field_0x2c;
        int sx = this->x + rowH + (this->width >> 1);
        int sy = layout->field_0x20 + this->y + rowH + layout->field_0xc + progH + sel;
        int sw = (this->width >> 1) - layout->buttonInsetX;
        int sh = ((this->height -
                  (layout->field_0xc + rowH * 2 + layout->field_0x20 + progH + sel))
                  - layout->field_0x10) - layout->field_0x24;
        this->scrollWindow = new ScrollTouchWindow(sx, sy, sw, sh, false);
    }

    // Populate the rows (ship stats / item attribute table / blueprint price).
    liw_set_fillRows(this, item, layout, isShip, p6);

    // Reset the rotating-preview spin state.
    this->dragging       = 0;
    this->previewAngle   = 0.0f;
    this->dragAccum      = 0x104;
    this->dragLastX      = 0;
    this->dragSettled    = 0;
    this->dragDelta      = 0;
    this->spinDamping    = 0.0f;
    this->spinVelocity   = 0.0f;
    this->dragStartX     = 0;
}

ListItemWindow::~ListItemWindow()
{
    if (this->labels) {
        for (String *str : *this->labels) delete str;
        this->labels->clear();
        delete this->labels;
        this->labels = 0;
    }
    if (this->values) {
        for (String *str : *this->values) delete str;
        this->values->clear();
        delete this->values;
        this->values = 0;
    }
    if (this->statsCur) {
        this->statsCur->clear();
        delete this->statsCur;
        this->statsCur = 0;
    }
    if (this->statsPrev) {
        this->statsPrev->clear();
        delete this->statsPrev;
        this->statsPrev = 0;
    }
    delete this->scrollWindow;
    this->scrollWindow = 0;
}

// ---- ListItemWindow::draw -----------------------------------------------------
// Renders the list window chrome: background box, header, the ship/item preview
// (3D ship render or 2D item icon), each visible list row with its value text
// and up/down trend arrows, and the footer / scrollbar.

// PC-relative game state roots.
__attribute__((visibility("hidden"))) extern char  *g_liw_d_maskFlag;
__attribute__((visibility("hidden"))) extern void **g_liw_d_canvas;
__attribute__((visibility("hidden"))) extern void **g_liw_d_layout;
__attribute__((visibility("hidden"))) extern int   *g_liw_d_headerId;
__attribute__((visibility("hidden"))) extern void **g_liw_d_gameText;
__attribute__((visibility("hidden"))) extern void **g_liw_d_imageFactory;
__attribute__((visibility("hidden"))) extern void **g_liw_d_itemDB;
__attribute__((visibility("hidden"))) extern void **g_liw_d_arrowL;
__attribute__((visibility("hidden"))) extern void **g_liw_d_arrowR;
__attribute__((visibility("hidden"))) extern int   *g_liw_d_scrollLimit;

void ListItemWindow::draw()
{
    Layout *layout = (Layout *)*g_liw_d_layout;
    uint32_t canvasHandle = (uint32_t)(unsigned long)*g_liw_d_canvas;
    PaintCanvas *canvas = (PaintCanvas *)(long)canvasHandle;
    bool masked = *g_liw_d_maskFlag != 0;

    if (masked)
        layout->drawMask();

    canvas->SetColor(canvasHandle);
    canvas->FillRectangle((int)canvasHandle, this->x, this->y, this->width);

    {
        String s("", false);
        layout->drawBox(2, this->x, this->y, this->width, this->height, &s, 0);
    }
    if (masked) {
        String s("", false);
        layout->drawBox(7, this->x, this->y, this->width, this->height, &s, 0);
    }

    {
        String s;
        s.ctor_copy(((GameText *)(*g_liw_d_gameText))->getText(*g_liw_d_headerId), false);
        layout->drawHeader1(&s);
    }

    ListItem *li = this->item;
    int isShip = li->isShip();
    canvas->SetColor(canvasHandle);

    int isItemish = li->isItem();
    if (isItemish == 0 && li->isBluePrint() == 0 && li->isPendingProduct() == 0) {
        if (isShip != 0) {
            int boxX = this->x, boxY = this->y, w = this->width;
            int c0c = layout->field_0xc, c20 = layout->field_0x20, c28 = layout->buttonInsetX, c2c = layout->field_0x2c;
            int color = layout->field_0x5c;
            int textId = *g_liw_d_headerId;
            li->ship->getIndex();
            String s;
            s.ctor_copy(((GameText *)(*g_liw_d_gameText))->getText(textId), false);
            layout->drawBox8(1, c28 + boxX, boxY + c0c + c20, (w >> 1) - (c2c + c28), color, &s, 2);

            ImageFactory *fac = (ImageFactory *)*g_liw_d_imageFactory;
            int shipIdx = li->ship->getIndex();
            char *L = (char *)layout;  // RAWREAD: 0x124/0x2c8 have no named Layout member (sparse layout)
            fac->drawShip(shipIdx, this->x + layout->buttonInsetX + layout->field_0x2c,
                ((this->y + layout->field_0xc + layout->field_0x20 + layout->field_0x5c / 2) - i32(L, 0x2c8) / 2) + i32(L, 0x124));
        }
    } else {
        char *L = (char *)layout;  // RAWREAD: 0x124/0x2c8 have no named Layout member (sparse layout)
        int boxX = this->x, boxY = this->y, w = this->width;
        int c0c = layout->field_0xc, c20 = layout->field_0x20, c28 = layout->buttonInsetX, c2c = layout->field_0x2c;
        int color = layout->field_0x5c;
        int textId = *g_liw_d_headerId;
        li->getIndex();
        String s;
        s.ctor_copy(((GameText *)(*g_liw_d_gameText))->getText(textId), false);
        layout->drawBox8(1, c28 + boxX, boxY + c0c + c20, (w >> 1) - (c2c + c28), color, &s, 2);

        Item *itemPtr;
        if (li->isItem() == 0) {
            int idx;
            void *db = *g_liw_d_itemDB;  // RAWREAD: itemDB is an opaque/unmodeled engine struct
            if (li->isBluePrint() == 0)
                idx = li->pendingProduct->blueprintIndex;
            else
                idx = li->bluePrint->getIndex();
            itemPtr = *(Item **)(i32(db, 0x4) + idx * 4);
        } else {
            itemPtr = li->item;
        }

        ImageFactory *fac = (ImageFactory *)*g_liw_d_imageFactory;
        int idx = itemPtr->getIndex();
        int type = itemPtr->getType();
        fac->drawItem4(idx, type,
            layout->buttonInsetX + this->x + layout->field_0x2c,
            i32(L, 0x124) + ((this->y + layout->field_0xc + layout->field_0x20 + layout->field_0x5c / 2) - i32(L, 0x2c8) / 2));
    }

    // Visible value rows.
    Array<String *> *rows = this->labels;
    if (rows != 0) {
        uint32_t n = rows->size();
        int rowH = layout->field_0x2c;
        int yTop = layout->field_0x5c + layout->field_0xc + this->y + layout->field_0x20 + rowH;
        if ((uint32_t)(*g_liw_d_scrollLimit - layout->field_0x10) < (uint32_t)((layout->field_0x1c + rowH) * n + yTop))
            rowH = 2;
        int ycur = yTop;
        for (uint32_t i = 0; i < n; i++) {
            canvas->SetColor(canvasHandle);
            int color = layout->field_0x1c;
            String s;
            s.ctor_copy((*rows)[i], false);
            layout->drawBox(6, layout->buttonInsetX + this->x, ycur, (this->width >> 1) - (layout->field_0x2c + layout->buttonInsetX), color, &s, 0);
            canvas->SetColor(canvasHandle);

            int textX, textY;
            void *valStr;
            bool centered;
            bool drewArrow = false;
            if (isShip != 0) {
                uint32_t cur = this->statsCur->size();
                if (i < cur) {
                    if (i < cur - 1) {
                        int a = (*this->statsCur)[i];
                        int b = (*this->statsPrev)[i];
                        int trendImage = this->arrowEqualImage;        // equal
                        if (a < b) trendImage = this->arrowDownImage;  // down
                        if (b < a) trendImage = this->arrowUpImage;    // up
                        canvas->DrawImage2D(trendImage,
                            ((this->x + (this->width >> 1)) - layout->field_0x2c) - this->arrowSeparator, 0);
                    }
                    int sep = this->arrowSeparator;
                    valStr = (*this->values)[i];
                    void *arrowStr = *g_liw_d_arrowR;
                    int tw = canvas->GetTextWidth(canvasHandle, arrowStr);
                    centered = (char)((char)ycur + (char)(layout->field_0x1c / 2) - (char)this->textHalfHeight);
                    textX = (((this->x + (this->width >> 1) + 10) - layout->field_0x2c) - sep) - tw;
                    textY = layout->field_0x1c;
                    drewArrow = true;
                }
            }
            if (!drewArrow) {
                valStr = (*this->values)[i];
                void *arrowStr = *g_liw_d_arrowL;
                int tw = canvas->GetTextWidth(canvasHandle, arrowStr);
                centered = (char)((char)ycur + (char)(layout->field_0x1c / 2) - (char)this->textHalfHeight);
                textX = (this->x + (this->width >> 1) + layout->field_0x2c * -2) - tw;
                textY = layout->field_0x1c;
            }
            canvas->DrawString(canvasHandle, valStr, textX, textY, centered);
            ycur = ycur + rowH + layout->field_0x1c;
        }
    }

    // Footer / progress.
    if (this->previewHeight < 1) {
        String s;
        s.ctor_copy(((GameText *)(*g_liw_d_gameText))->getText(*g_liw_d_headerId), false);
        layout->drawBox(1, this->x + (this->width >> 1) + layout->field_0x2c, this->y + layout->field_0xc + layout->field_0x20, ((this->width >> 1) - layout->field_0x2c) - layout->buttonInsetX, layout->field_0x5c, &s, 0);
    } else {
        canvas->SetColor(canvasHandle);
        {
            String s("", false);
            layout->drawBox(8, this->x + (this->width >> 1) + layout->field_0x2c, this->y + layout->field_0xc + layout->field_0x20, ((this->width >> 1) - layout->field_0x2c) - layout->buttonInsetX, this->previewHeight, &s, 0);
        }
        int prog = this->previewHeight;
        int yBox = this->y + layout->field_0xc + layout->field_0x20;
        if (prog > 0) yBox = yBox + prog + layout->field_0x2c;
        {
            String s;
            s.ctor_copy(((GameText *)(*g_liw_d_gameText))->getText(*g_liw_d_headerId), false);
            layout->drawBox(0, this->x + (this->width >> 1) + layout->field_0x2c, yBox, ((this->width >> 1) - layout->field_0x2c) - layout->buttonInsetX, layout->field_0x1c, &s, 0);
        }
        canvas->SetColor(canvasHandle);
        canvas->DrawImage2D(this->scrollThumbImage, this->scrollBarX - this->scrollBarOffsetX, 0);
        int half = this->scrollBarTrackLength / 3;
        canvas->DrawImage2D(this->scrollThumbImage, this->scrollBarX, this->scrollBarY - half, (unsigned char)1);
    }

    this->scrollWindow->drawTextBG();
    this->scrollWindow->draw();
}

// ---- ListItemWindow::update ---------------------------------------------------
// Advances the scroll window, applies inertial spin damping to the rotating 3D
// ship preview, and pushes the resulting rotation onto its geometry.

// Engine matrix-rotation/scaling helpers and PC-relative preview roots.
void MatrixSetRotation(void *m, float x, float y, float z);
extern "C" void MatrixSetScaling(void *m, float x, float y, float z);

__attribute__((visibility("hidden"))) extern uint32_t    *g_liw_u_tf;          // preview transform handle
__attribute__((visibility("hidden"))) extern const float *g_liw_u_angleTable;  // per-ship base angle
extern const float g_liw_u_angleScale;

void ListItemWindow::update(int frameTime)
{
    this->scrollWindow->update(frameTime);

    if (this->shows3DShipFlag == 0)
        return;

    if (this->dragging == 0) {
        float spin = this->spinDamping * this->spinVelocity;
        float mag = spin > 0.0f ? spin : -spin;
        this->spinVelocity = spin;
        if (mag > 1.0f) {
            float angle = (float)this->dragAccum;
            this->dragAccum = (int)(spin + angle);
        }
    }

    int idx = this->item->ship->getIndex();

    float baseAngle = this->baseAngle;
    float angle = (float)this->dragAccum / g_liw_u_angleScale;
    this->previewAngle = angle;

    uint32_t tf = *g_liw_u_tf;
    PaintCanvas *canvas = (PaintCanvas *)(long)tf;
    void *loc = canvas->TransformGetLocal(tf);
    MatrixSetRotation(loc, angle, 0.0f, 0.0f);
    loc = canvas->TransformGetLocal(tf);
    float tableAngle = g_liw_u_angleTable[idx] + baseAngle;
    MatrixSetScaling(loc, tableAngle, tableAngle, tableAngle);

    if (this->previewSentinel != -1) {
        loc = canvas->TransformGetLocal(tf);
        MatrixSetRotation(loc, angle, 0.0f, 0.0f);
        loc = canvas->TransformGetLocal(tf);
        MatrixSetScaling(loc, tableAngle, tableAngle, tableAngle);
    }

    this->previewGeometry->setRotation(tableAngle, tableAngle, tableAngle);
}

// dtor(): C-ABI complete-object destructor. Runs ~ListItemWindow and returns the
// object pointer so the caller can hand it to operator delete.
void *ListItemWindow::dtor()
{
    this->~ListItemWindow();
    return this;
}

// touch_end(): touch-release entry point invoked by the owning screen. Releases
// the drag-scroll of the 3D ship preview; returns a status int the caller drops.
int ListItemWindow::touch_end(int x, int y)
{
    this->OnTouchEnd(x, y);
    return 0;
}

// PC-relative game singletons consulted by the constructor (the live screen and
// the global PaintCanvas, each held as a pointer-to-pointer).
__attribute__((visibility("hidden"))) extern void ***g_liw_a;
__attribute__((visibility("hidden"))) extern void ***g_liw_b;

ListItemWindow::ListItemWindow()
{
    void **a = *g_liw_a;
    void **b = *g_liw_b;
    this->labels = 0;
    this->values = 0;
    this->statsCur = 0;
    this->statsPrev = 0;
    void *av = *a;
    PaintCanvas *canvas = (PaintCanvas *)*b;
    this->scrollWindow = 0;
    (void)av;
    int h = canvas->GetTextHeight((unsigned int)(long)canvas);
    this->textHalfHeight = h / 2 - 1;
    this->previewHeight = 0;
}
