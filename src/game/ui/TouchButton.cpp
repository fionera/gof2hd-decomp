#include "game/ui/TouchButton.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Achievements.h"
#include "game/ui/Layout.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"

unsigned int GameText_getLanguage();

void TouchButton::setVisible(bool value) {
    this->visible = value;
}

void TouchButton::setPosition(int x, int y, unsigned char flags) {
    unsigned int f = (unsigned int)flags;
    this->x = x;
    this->y = y;
    this->flags0 = flags;
    if ((flags & 0x20) != 0) {
        y = y - this->layoutHeight;
        this->y = y;
    }
    if ((int)(f << 0x1e) < 0) {
        x = x - this->width;
        this->x = x;
    }
    if ((int)(f << 0x19) < 0) {
        this->y = y - this->layoutHeight / 2;
    }
    if ((int)(f << 0x1d) < 0) {
        this->x = x - this->width / 2;
    }
}

void TouchButton::translate(int dx, int dy) {
    this->x = dx + this->x;
    this->y = dy + this->y;
}

void TouchButton::setNumberText(String *value) {
    this->numberText.assign(value);
}

TouchButton::~TouchButton() {
}

unsigned int TouchButton::OnTouchMove(int px, int py) {
    if (this->visible != 0 && this->halfTransparent == 0) {
        unsigned int r;
        if (this->touched == 0)
            r = 0;
        else
            r = touchedInside(px, py);
        this->touched = (unsigned char)r;
        return r;
    }
    return 0;
}

uint8_t TouchButton::isVisible() {
    return this->visible;
}

void TouchButton::setPressProgress(float value) {
    this->pressProgress = value;
}

uint8_t TouchButton::isTouched() {
    return this->touched;
}

String TouchButton::getText() {
    return this->text;
}

void TouchButton::replaceTextKeepSize(String *text) {
    this->text.assign(text);
    if (this->kind == 10) {
        int w = this->width;
        int tw = gCanvas->GetTextWidth(this->fontId, text);
        this->textOffsetX = w / 2 - tw / 2;
    }
}

void TouchButton::setSplitText(String *value) {
    this->splitText.assign(value);
}

struct Vec3 { float x, y, z; };

void TouchButton_getPosition(Vec3 *out, TouchButton *self)
{
    out->z = 0;
    out->y = (float)self->y;
    out->x = (float)self->x;
}

void TouchButton::setTextColor(int color) {
    this->textColor = color;
}

void TouchButton::setAlwaysPressed(bool value) {
    this->alwaysPressed = value;
}

void TouchButton::resetTouch() {
    this->touched = 0;
}

// FModSound singleton (double-deref).
extern FModSound **g_TB_sound;

bool TouchButton::OnTouchBegin(int px, int py) {
    if (this->visible == 0 || this->halfTransparent != 0)
        return false;
    int r = touchedInside(px, py);
    this->touched = (unsigned char)r;
    if (r == 0)
        return false;
    (*g_TB_sound)->play(0x7c, 0, 0, 0.0f);
    return this->touched != 0;
}

unsigned int TouchButton::OnTouchEnd(int px, int py) {
    unsigned int res;
    if (this->visible == 0 || this->halfTransparent != 0) {
        res = 0;
    } else if (this->touched == 0 || touchedInside(px, py) == 0) {
        res = 0;
        this->touched = 0;
    } else {
        this->touched = 0;
        (*g_TB_sound)->play(0x7b, 0, 0, 0.0f);
        res = 1;
    }
    return res;
}

int TouchButton::getWidth() {
    return this->width;
}

void TouchButton::setPosition2(int x, int y) {
    setPosition(x, y, this->flags0);
}

void TouchButton::setHalfTransparent(bool value) {
    this->halfTransparent = value;
}

void TouchButton::setPressProgressHighlight(bool value) {
    this->progressHighlight = value;
}

int TouchButton::getHeight() {
    return this->height;
}

// Shared initialiser for every TouchButton flavour. It records geometry and
// flags, assigns the label text, then -- driven by `kind` -- creates the 2D
// images that make up the button skin (normal / pressed / disabled, plus the
// 9-patch background frame for menu buttons) and computes text/icon offsets.
// Finally it lays the button out via setPosition().
//
// The image-id lookup tables are fixed read-only data in the binary; they are
// reached through helpers that take the row index.
extern "C" unsigned int   TB_iconTexId(int eliteVariant, int stage);              // -> icon texture id
extern "C" unsigned short TB_iconImgId(int eliteVariant, int stage);              // image for icon create
extern "C" unsigned short TB_medalSmallId(int achId);                             // small medal overlay
extern "C" unsigned short TB_frameId(int useAltSkin, unsigned int kind, int slot); // 9-patch frame ids

// Skin/language toggle globals.
extern char  *g_TB_useAltSkin;     // bool: alternate skin table
extern char  *g_TB_langWide;       // bool: wide-language toggle
extern char  *g_TB_langWide2;      // bool: second toggle
extern void **g_TB_layoutMetrics;  // Layout providing the touch-margin / default-height metrics
extern const char g_TB_emptyStr[];

// Helper: GetImage2DWidth/Height of the just-created image, then derive the
// centred label X offset. Used by several of the simple image-only kinds.
static void tb_basicImageGeometry(TouchButton *self, void *canvas)
{
    self->height = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
    int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
    self->layoutHeight = self->height;
    self->width = w;
    self->leftWidth = w;
    int tw = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
    int h = self->height;
    self->textOffsetX = w / 2 - tw / 2;
    int th = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
    self->textOffsetY = h / 2 - th / 2;
}

int TouchButton::init(String *text, unsigned int kind, int achId, int achStage, int width, int d_unused, int x, int y, unsigned char flags0, unsigned char flags1) {
    void *canvas = gCanvas;

    this->kind     = (int)kind;
    this->visible  = 1;
    this->text.assign(text);
    this->subId    = achStage;          // generic "second image / sub-id" slot
    this->textColor = -1;
    this->requestedWidth = width;
    this->field_0x0 = 0;
    this->field_0x4 = 0;
    this->flags1   = flags1;
    this->flags0   = flags0;
    this->touched  = 0;
    this->alwaysPressed = 0;
    this->textOffsetX = 0;
    this->textOffsetY = 0;
    this->leftWidth = 0;
    this->midWidth = 0;
    this->rightWidth = 0;
    this->progressHighlight = 1;
    this->pressProgress = 0;
    this->touchMargin = 0;
    this->height   = 0;
    this->initX    = x;
    this->initY    = y;

    this->numberText = g_TB_emptyStr;
    this->splitText  = g_TB_emptyStr;
    this->adornImage = -1;

    switch (kind) {
    case 4: {   // medal / achievement button
        void *ach = gAchievements;
        int elite = (((Achievements *)(ach))->isEliteMedal(achId) != 0) ? 1 : 0;
        this->iconTexId = TB_iconTexId(elite, achStage);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_iconImgId(elite, achStage), &this->iconImage);
        this->height = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetY = this->height + 5;
        int tw = ((PaintCanvas*)(canvas))->GetTextWidth(this->fontId, &this->fontId);
        this->iconOverlay = -1;
        this->textOffsetX = w / 2 - tw / 2;
        this->iconSmall = -1;
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_medalSmallId(achId), (unsigned int*)&this->iconSmall);
        if (achStage != 0 || ((Achievements *)(ach))->isEliteMedal(achId) != 0)
            ((PaintCanvas*)(canvas))->Image2DCreate(0x96c, (unsigned int*)&this->iconOverlay);
        break;
    }
    case 10: {  // toggle-style button with a 0x2329 background
        ((PaintCanvas*)(canvas))->Image2DCreate(9000, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0x2329, (unsigned int*)&this->imgFrameTL);
        this->imgFrameBL = this->imgFrameTL;
        this->height = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        int tw = ((PaintCanvas*)(canvas))->GetTextWidth(this->fontId, &this->fontId);
        this->textOffsetX = w / 2 - tw / 2;
        this->textOffsetY = ((PaintCanvas*)(canvas))->GetTextHeight(this->fontId);
        this->touchMargin = *(int *)((char *)*g_TB_layoutMetrics + 0x80);
        break;
    }
    case 0xc:   // simple two-image button (0x472 / 0x473)
        ((PaintCanvas*)(canvas))->Image2DCreate(0x472, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0x473, (unsigned int*)&this->imgFrameTL);
        tb_basicImageGeometry(this, canvas);
        break;
    case 0xd:
        ((PaintCanvas*)(canvas))->Image2DCreate(0x517, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0x518, (unsigned int*)&this->imgFrameTL);
        this->imgFrameBL = this->imgFrameTL;
        tb_basicImageGeometry(this, canvas);
        break;
    case 0xe:
        ((PaintCanvas*)(canvas))->Image2DCreate(0x53c, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0x53b, (unsigned int*)&this->imgFrameTL);
        goto wide_text_layout;
    case 0xf:
        ((PaintCanvas*)(canvas))->Image2DCreate(0x53e, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0x53d, (unsigned int*)&this->imgFrameTL);
        goto wide_text_layout;
    case 0x10:  // background supplied by caller
        this->imgFrameL = achStage;
        ((PaintCanvas*)(canvas))->Image2DCreate(0xbb9, (unsigned int*)&this->imgFrameTL);
        tb_basicImageGeometry(this, canvas);
        break;
    case 0x11:
        ((PaintCanvas*)(canvas))->Image2DCreate(0xbc0, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0xbc1, (unsigned int*)&this->imgFrameTL);
        goto wide_text_layout;
    case 0x12:
        ((PaintCanvas*)(canvas))->Image2DCreate(0xbc0, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0xbc1, (unsigned int*)&this->imgFrameTL);
        goto wide_text_layout;
    case 0x14:
        ((PaintCanvas*)(canvas))->Image2DCreate(0x1f6e, (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(0x1f6f, (unsigned int*)&this->imgFrameTL);
        goto wide_text_layout;
    case 0x13:  // pre-supplied images (caller + ctor variant)
        this->imgFrameL = achStage;
        this->imgFrameTL = (int)this->image;
        tb_basicImageGeometry(this, canvas);
        break;
    default: {  // generic menu button: 9-patch frame from the skin table
        int alt = (*g_TB_useAltSkin != 0) ? 1 : 0;
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 0), (unsigned int*)&this->imgFrameL);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 1), (unsigned int*)&this->imgFrameM);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 2), (unsigned int*)&this->imgFrameR);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 3), (unsigned int*)&this->imgFrameTL);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 4), (unsigned int*)&this->imgFrameT);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 5), (unsigned int*)&this->imgFrameTR);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 6), (unsigned int*)&this->imgFrameBL);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 7), (unsigned int*)&this->imgFrameB);
        ((PaintCanvas*)(canvas))->Image2DCreate(TB_frameId(alt, kind, 8), (unsigned int*)&this->imgFrameBR);

        this->height = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        this->leftWidth = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);   // left frame width
        this->midWidth = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);    // mid frame width
        int rightW = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        this->rightWidth = rightW;
        // height: kind 0xb uses the raw image height; otherwise a layout default
        if (kind != 0xb)
            this->layoutHeight = *(int *)((char *)*g_TB_layoutMetrics + 0x30);
        else
            this->layoutHeight = this->height;

        if (kind < 7 && ((1 << (kind & 0xff)) & 0x61) != 0) {
            this->rightWidth = rightW - 2;
        } else if ((kind - 7) < 3 && *g_TB_useAltSkin != 0) {
            int hh;
            if (*g_TB_langWide != 0)
                hh = 0x46;
            else
                hh = (*g_TB_langWide2 != 0) ? 0x64 : 0x32;
            this->layoutHeight = hh;
        }
        setText((String *)&this->fontId);
        break;
    }
    }

    goto done;

wide_text_layout: {
        // Shared epilogue for the 'arrow' style kinds (0xe/0xf/0x11/0x12/0x14):
        // a language-dependent vertical text factor is applied.
        unsigned int lang = GameText_getLanguage();
        float factor;
        if ((lang & 0xffff) < 0x10 && ((1 << (lang & 0xff)) & 0x8c00) != 0)
            factor = 1.0f;
        else
            factor = (lang == 0xe) ? 1.0f : 1.5f;

        this->imgFrameBL = this->imgFrameTL;
        this->height = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        int tw = ((PaintCanvas*)(canvas))->GetTextWidth(this->fontId, &this->fontId);
        int h = this->height;
        this->textOffsetX = w / 2 - tw / 2;
        int th = ((PaintCanvas*)(canvas))->GetTextHeight(this->fontId);
        this->textOffsetY = (int)((float)(h / 2) + factor * (float)th);
        this->touchMargin = *(int *)((char *)*g_TB_layoutMetrics + 0x80);
    }

done:
    setPosition(x, y, flags0);
    return 0;
}

// Empty-label constructor body: kind + position + a single flag byte. Builds an
// empty label, samples the default spacing/kerning, then runs init().
extern unsigned int *g_TB_defSpacing; // default glyph spacing

void TouchButton_168ffc(TouchButton *self, unsigned int kind,
                        int a, int b, int c, int d,
                        unsigned char flags0, unsigned char flags1)
{
    void *canvas = gCanvas;
    self->fontId = *g_TB_defSpacing;
    self->fontSpacing = ((PaintCanvas*)(canvas))->FontGetSpacing(self->fontId);

    String tmp(g_TB_emptyStr, false);
    self->init(&tmp, kind, a, b, c, d, -1, -1, flags0, flags1);
}

// TouchButton::draw()
//   Renders the button for its current state. Saves the canvas colour and font
//   spacing, draws the appropriate skin (single icon, 9-patch frame + label, a
//   progress fill, or a plain image) depending on `kind` and the pressed /
//   highlight flags, then restores spacing and colour.
extern Layout **g_TB_d_layoutA;   // pressed/disabled tint layout
extern Layout **g_TB_d_layoutBG;  // background-pattern layout
extern Layout **g_TB_d_layoutC;   // progress-fill layout
extern Layout **g_TB_d_layoutEnd; // final restore layout
extern String **g_TB_d_unitStr;   // "%"-style unit String
extern unsigned int g_TB_d_frameMask; // kinds that get a frame

void TouchButton::draw() {
    void *canvas = gCanvas;
    unsigned int savedColor = ((PaintCanvas*)(canvas))->GetColor();

    if (this->visible == 0)
        return;

    if (this->halfTransparent != 0) {
        gCanvas->SetColor(0xffffff2f);
        (*g_TB_d_layoutA)->setDrawColor(-0xd1);
    } else {
        gCanvas->SetColor(0xffffffff);
    }

    short savedSpacing = (short)((PaintCanvas*)(canvas))->FontGetSpacing(this->fontId);
    ((PaintCanvas*)(canvas))->FontSetSpacing(this->fontId, (short)this->fontSpacing);

    unsigned int kind = this->kind;
    int icon = -1;       // image to draw at the tail
    int iconY = 0;
    bool tailIcon = false;

    if (kind == 0x10) {
        ((PaintCanvas*)(canvas))->DrawImage2D(this->imgFrameL, this->x, this->y);
        if (this->touched != 0 || this->alwaysPressed != 0) {
            icon = this->imgFrameTL;
            iconY = this->x;
            tailIcon = true;
        }
    } else if (kind == 4) {
        ((PaintCanvas*)(canvas))->DrawImage2D(this->iconImage, this->x, this->y);
        if (this->iconSmall != -1) {
            gCanvas->SetColor(0xffffffff);
            ((PaintCanvas*)(canvas))->DrawImage2D(this->iconSmall, this->x + (this->width >> 1), (this->y + (this->height >> 1)) - 1, (unsigned char)0x11, (unsigned char)0x44);
            gCanvas->SetColor(0xffffffff);
            if (this->touched != 0 || this->alwaysPressed != 0)
                ((PaintCanvas*)(canvas))->DrawImage2D(this->iconOverlay, this->x, this->y);
        }
        gCanvas->SetColor(0xffffffff);
        ((PaintCanvas*)(canvas))->DrawString(this->fontId, &this->text, this->x + this->textOffsetX, this->y + this->textOffsetY, false);
    } else {
        // generic frame / label kinds.
        int base;
        if (this->touched != 0)
            base = this->imgFrameTL;
        else if (this->alwaysPressed != 0)
            base = this->imgFrameBL;
        else
            base = this->imgFrameL;

        if (kind <= 0x14 && ((1u << (kind & 0xff)) & g_TB_d_frameMask) != 0) {
            // draw the 9-patch background frame + middle stretch.
            unsigned int frameLeft;
            int frameMid;
            if (this->touched != 0) {
                frameLeft = (unsigned int)this->imgFrameT;
                frameMid  = this->imgFrameTR;
            } else if (this->alwaysPressed != 0) {
                frameLeft = (unsigned int)this->imgFrameB;
                frameMid  = this->imgFrameBR;
            } else {
                frameLeft = (unsigned int)this->imgFrameM;
                frameMid  = this->imgFrameR;
            }
            (*g_TB_d_layoutBG)->drawBGPattern(frameLeft, this->leftWidth + this->x, this->y, this->midStretch, this->height);
            ((PaintCanvas*)(canvas))->DrawImage2D(frameMid, this->x + this->leftWidth + this->midStretch, this->y);
        }
        ((PaintCanvas*)(canvas))->DrawImage2D(base, this->x, this->y);

        Layout *layoutC = *g_TB_d_layoutC;
        layoutC->setDrawColor(-1);

        // optional progress fill.
        float prog = this->pressProgress;
        if (prog > 0.0f) {
            gCanvas->SetColor(0xffffffff);
            layoutC->setDrawColor(-0x80);
            int span = this->leftWidth;
            int total = this->rightWidth + this->midStretch + span;
            int filled = (int)(prog * (float)total);
            int leftImg = (this->progressHighlight == 0) ? this->imgFrameL : this->imgFrameTL;
            int drawW = (filled < span) ? filled : span;
            ((PaintCanvas*)(canvas))->DrawRegion2D((unsigned int)leftImg, 0, 0, drawW, this->height, (float)filled, 0, 0, 0, this->x);

            int mid = this->leftWidth;
            if (mid < filled) {
                int midImg = (this->progressHighlight == 0) ? this->imgFrameM : this->imgFrameT;
                int patW = this->midStretch;
                if (filled - mid < patW) patW = filled - mid;
                layoutC->drawBGPattern((unsigned int)midImg, mid + this->x, this->y, patW, this->height);
                mid = this->leftWidth;
            }
            int rstart = this->midStretch + mid;
            if (rstart < filled) {
                int rImg = (this->progressHighlight == 0) ? this->imgFrameR : this->imgFrameTR;
                int rW = (filled - mid) - this->midStretch;
                if (this->rightWidth < rW) rW = this->rightWidth;
                ((PaintCanvas*)(canvas))->DrawRegion2D((unsigned int)rImg, 0, 0, rW, this->height, (float)filled, 0, 0, 0, rstart + this->x);
            }
            layoutC->setDrawColor(-1);
        }

        // label colour: tinted when disabled.
        unsigned int lblColor = (unsigned int)this->textColor;
        if (this->halfTransparent != 0)
            gCanvas->SetColor((unsigned char)(lblColor >> 16), (unsigned char)(lblColor >> 8), (unsigned char)lblColor, (unsigned char)(lblColor >> 24));
        else
            gCanvas->SetColor(0xffffffff);

        if (this->subId == -1) {
            // primary label
            ((PaintCanvas*)(canvas))->DrawString(this->fontId, &this->text, this->x + this->textOffsetX, this->y + this->textOffsetY, false);

            // secondary / value label when its length is set.
            if (this->splitText.size() != 0) {
                String *t = &this->text;
                int px = this->x;
                int w  = this->width;
                int tx, ty;
                if (this->kind == 10) {
                    int tw = ((PaintCanvas*)(canvas))->GetTextWidth(this->fontId, t);
                    int th = ((PaintCanvas*)(canvas))->GetTextHeight(this->fontId);
                    ty = this->y + this->height + th * -2;
                    tx = (px + w / 2) - tw / 2;
                } else {
                    int off = this->textOffsetX;
                    int tw  = ((PaintCanvas*)(canvas))->GetTextWidth(this->fontId, t);
                    ty = this->y + this->textOffsetY;
                    tx = ((w + px) - off) - tw;
                }
                ((PaintCanvas*)(canvas))->DrawString(this->fontId, &this->splitText, tx, ty, false);
            }

            // shortcut / corner label when its length is set.
            if (this->numberText.size() != 0) {
                gCanvas->SetColor(0xffffffff);
                String *u = *g_TB_d_unitStr;
                int tw = ((PaintCanvas*)(canvas))->GetTextWidth(this->fontId, u);
                ((PaintCanvas*)(canvas))->DrawString(this->fontId, &this->numberText, (this->leftWidth + this->x) - tw, this->y + this->textOffsetY, false);
                gCanvas->SetColor(0xffffffff);
            }

            // small adornment image when set.
            if (this->adornImage != -1) {
                gCanvas->SetColor(0xffffffff);
                ((PaintCanvas*)(canvas))->DrawImage2D(this->adornImage, (this->x + this->width + 6) - this->leftWidth, this->y + 1, (unsigned char)0x11, (unsigned char)0x14);
                gCanvas->SetColor(0xffffffff);
            }
        } else if (this->kind != 0x13) {
            icon = base;
            iconY = this->textOffsetX + this->x;
            tailIcon = true;
        }
    }

    if (tailIcon)
        ((PaintCanvas*)(canvas))->DrawImage2D(icon, this->x + this->textOffsetX + 0, iconY);

    (*g_TB_d_layoutEnd)->setDrawColor(-1);
    ((PaintCanvas*)(canvas))->FontSetSpacing(this->fontId, savedSpacing);
    gCanvas->SetColor(savedColor);
}

// Full label/menu-button constructor (DialogueWindow / SpaceLounge / StarMap, 7-arg).
// Seeds the font id from the active-font global, caches the current font spacing,
// then delegates to the shared init().
extern int **g_TB_c1;   // active-font id holder (double-deref)

TouchButton::TouchButton(String *text, int type, int x, int y, int p5, unsigned char p6, unsigned char p7) {
    this->fontId = (uint32_t)**g_TB_c1;
    this->fontSpacing = gCanvas->FontGetSpacing(this->fontId);
    init(text, (unsigned int)type, x, y, p5, 0, 0, 0, p6, p7);
}

void TouchButton::setText(String *text) {
    this->text.assign(text);
    int w = gCanvas->GetTextWidth(this->fontId, text);
    if (this->subId != -1)
        w = gCanvas->GetImage2DWidth(0);
    int a94 = this->leftWidth;
    int x;
    if (this->requestedWidth < 1)
        x = w;
    else
        x = (this->requestedWidth - a94) - this->rightWidth;
    this->midStretch = x;
    x = this->rightWidth + x + a94;
    this->width = x;
    unsigned char fl = this->flags1;
    if ((fl & 2) == 0) {
        if ((fl & 1) != 0) {
            this->textOffsetX = a94;
            goto height;
        }
        x = (x - w) / 2;
        this->textOffsetX = x;
        if (this->kind == 6) {
            x = x + -5;
        } else {
            if (this->kind != 5)
                goto height;
            x = x + 5;
        }
    } else {
        x = x - (w + a94);
    }
    this->textOffsetX = x;
height:
    int h = this->height;
    int th = gCanvas->GetTextHeight(this->fontId);
    th = (h - th) / 2;
    this->textOffsetY = th;
    if (this->kind == 3)
        this->textOffsetY = th + 2;
    if (this->subId != -1) {
        h = this->height;
        int ih = gCanvas->GetImage2DHeight(0);
        this->textOffsetY = (h - ih) / 2;
        if (this->kind == 1)
            this->textOffsetX = this->textOffsetX + 3;
    }
    setPosition(this->initX, this->initY, this->flags0);
}

void TouchButton::setYPosition(int y) {
    setPosition(this->x, y, this->flags0);
}

bool TouchButton::touchedInside(int px, int py) {
    int x = this->x;
    int xm1 = x - 1;
    int h;
    int top;
    if (this->kind == 3) {
        int v = ((int *)gCanvas)[0x38 / 4];
        if (xm1 + v > px)
            return false;
        if (this->width + ((x + 1) - v) <= px)
            return false;
        top = this->y;
        if (top - 1 > py)
            return false;
        h = this->layoutHeight;
    } else {
        int m = this->touchMargin;
        if (xm1 - m > px)
            return false;
        if (this->width + (x + m) + 1 <= px)
            return false;
        top = this->y + ~m;
        if (top > py)
            return false;
        h = this->layoutHeight;
        top = this->y + m;
    }
    return h + top + 1 >= py;
}

// Image-handle constructor body: also stashes the second unsigned argument as a
// pre-supplied image handle (see init() case 0x13). Otherwise the usual empty-label
// construction + spacing save + init().
void TouchButton_168d9c(TouchButton *self, unsigned int kind, unsigned int image,
                        int a, int b, int c, unsigned char flag)
{
    void *canvas = gCanvas;
    self->image = image;
    self->fontId = *g_TB_defSpacing;
    self->fontSpacing = ((PaintCanvas*)(canvas))->FontGetSpacing(self->fontId);

    String tmp(g_TB_emptyStr, false);
    self->init(&tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
}

// 5-arg label constructor: same setup as the other label ctors, then init().
TouchButton::TouchButton(String *text, int x, int y, int p4, unsigned char p5) {
    this->fontId = (uint32_t)**g_TB_c1;
    this->fontSpacing = gCanvas->FontGetSpacing(this->fontId);
    init(text, 0xffffffff, 4, x, y, p4, 0, 0, p5, 0x44);
}

// Constructor body that temporarily overrides the shared font glyph spacing with
// this button's (spacing, kerning) pair, runs the common init(), then restores it.
TouchButton *TouchButton_168f30(TouchButton *self, String *text,
                                int a, int b, int c, int d,
                                unsigned char flags0, unsigned char flags1,
                                unsigned int spacing, int kerning)
{
    self->fontSpacing = kerning;
    self->fontId = spacing;

    void *canvas = gCanvas;
    short prev = ((PaintCanvas*)(canvas))->FontGetSpacing(self->fontId);
    ((PaintCanvas*)(canvas))->FontSetSpacing(spacing, (short)kerning);

    self->init(text, spacing, a, b, c, d, -1, -1, flags0, flags1);

    ((PaintCanvas*)(canvas))->FontSetSpacing(spacing, prev);
    return self;
}

TouchButton::TouchButton(int x, int y, String *text, int p4, int p5, unsigned char p6) {
    this->fontId = (uint32_t)**g_TB_c1;
    this->fontSpacing = gCanvas->FontGetSpacing(this->fontId);
    init(text, 0xffffffff, 4, x, y, p4, p5, 0, p6, 0x44);
}

// Empty-label convenience constructor body: kind + position + a single flag byte,
// fixed extra parameter (0x44).
void TouchButton_168cb0(TouchButton *self, unsigned int kind,
                        int a, int b, int c, unsigned char flag)
{
    void *canvas = gCanvas;
    self->fontId = *g_TB_defSpacing;
    self->fontSpacing = ((PaintCanvas*)(canvas))->FontGetSpacing(self->fontId);

    String tmp(g_TB_emptyStr, false);
    self->init(&tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
}

// ---- TouchButton(String const& text, int type, int x, int y, int width, int icon, int style) ----
// `icon` is the optional sub-image id (init's achStage slot); `style` becomes flags0.
// flags1 defaults to 0x44 like the other label ctors.
TouchButton::TouchButton(String *text, int type, int x, int y, int width, int icon, int style) {
    this->fontId = (uint32_t)**g_TB_c1;
    this->fontSpacing = gCanvas->FontGetSpacing(this->fontId);
    init(text, (unsigned int)type, 0, icon, width, 0, x, y, (unsigned char)style, 0x44);
}

// Three-argument placement helper: positions the button using `align` as the
// anchor-flags byte (the same flags consumed by setPosition()).
void TouchButton::setPosition3(int x, int y, int align) {
    setPosition(x, y, (unsigned char)align);
}

// Touch-release handler entry point: forwards to OnTouchEnd, which clears the
// pressed flag and reports whether the release counts as a click.
unsigned int TouchButton::touch_end(int x, int y) {
    return OnTouchEnd(x, y);
}
