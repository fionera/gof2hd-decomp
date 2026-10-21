#include "game/ui/TouchButton.h"
#include "game/core/Globals.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Achievements.h"
#include "game/ui/Layout.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/GameText.h"

void TouchButton::setVisible(bool value) {
    this->visible = value;
}

void TouchButton::setPosition(int x, int y, unsigned char flags) {
    unsigned int f = (unsigned int) flags;
    this->x = x;
    this->y = y;
    this->flags0 = flags;
    if ((flags & 0x20) != 0) {
        y = y - this->layoutHeight;
        this->y = y;
    }
    if ((int) (f << 0x1e) < 0) {
        x = x - this->width;
        this->x = x;
    }
    if ((int) (f << 0x19) < 0) {
        this->y = y - this->layoutHeight / 2;
    }
    if ((int) (f << 0x1d) < 0) {
        this->x = x - this->width / 2;
    }
}

void TouchButton::translate(int dx, int dy) {
    this->x = dx + this->x;
    this->y = dy + this->y;
}

void TouchButton::setNumberText(String const &value) {
    this->numberText = value;
}

TouchButton::~TouchButton() {
}

bool TouchButton::OnTouchMove(int px, int py) {
    if (this->visible != 0 && this->halfTransparent == 0) {
        bool r;
        if (this->touched == 0)
            r = false;
        else
            r = touchedInside(px, py);
        this->touched = r;
        return r;
    }
    return false;
}

uint8_t TouchButton::isVisible() {
    return this->visible;
}

void TouchButton::setPressProgress(float value) {
    this->pressProgress = value;
}

bool TouchButton::isTouched() {
    return this->touched;
}

String TouchButton::getText() {
    return String(this->text, false);
}

void TouchButton::replaceTextKeepSize(String const &text) {
    this->text = text;
    if (this->kind == 10) {
        int w = this->width;
        int tw = Globals::Canvas->GetTextWidth(this->fontId, text);
        this->textOffsetX = w / 2 - tw / 2;
    }
}

void TouchButton::setSplitText(String const &value) {
    this->splitText = value;
}

Vector TouchButton::getPosition() {
    float fx = static_cast<float>(this->x);
    float fy = static_cast<float>(this->y);
    Vector pos;
    pos.x = fx;
    pos.y = fy;
    pos.z = 0.0f;
    return pos;
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


bool TouchButton::OnTouchBegin(int px, int py) {
    if (this->visible == 0 || this->halfTransparent != 0)
        return 0;
    bool r = touchedInside(px, py);
    this->touched = r;
    if (r == 0)
        return 0;
    Globals::sound->play(0x7c, 0, 0, 0.0f);
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
        Globals::sound->play(0x7b, 0, 0, 0.0f);
        res = 1;
    }
    return res;
}

int TouchButton::getWidth() {
    return this->width;
}

void TouchButton::setPosition(int x, int y) {
    setPosition(x, y, this->flags0);
}

void TouchButton::setGamePadButtonImage(unsigned int image) {
    this->adornImage = static_cast<int>(image);
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


// Medal button images by medal stage (bronze .. gold, elite variants) and their tint colours.
static const int g_TB_eliteMedalTints[4] = {(int) 0xfa792160, (int) 0xfa7921ff, (int) 0xffffffff, (int) 0xce8258ff};
static const int g_TB_eliteMedalImages[4] = {8045, 8035, 2415, 2413};
static const int g_TB_medalImages[4] = {2416, 2414, 2415, 2413};
static const int g_TB_medalTints[4] = {0x2198ff2f, (int) 0xfad10eff, (int) 0xffffffff, (int) 0xce8258ff};
// Small medal icon per achievement.
static const int g_TB_medalIcons[45] = {
    2376, 2377, 2378, 2379, 2380, 2381, 2382, 2383, 2384, 2385, 2386, 2387, 2388, 2389, 2390, 2391, 2392, 2393, 2394, 2395, 2396, 2397, 2398, 2399, 2400, 2401, 2402, 2403, 2404, 2405, 2406, 2407, 2408, 2409, 2410, 2411, 8036, 8037, 8038, 8039, 8040, 8041, 8042, 8043, 8044
};
// Nine-patch frame images per button kind: L, M, R, pressed L/M/R, selected L/M/R.
static const int g_TB_frameImagesTablet[21][9] = {
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {1125, 1124, 1127, 1129, 1128, 1131, 1133, 1132, 1135},
    {1114, 1115, 1117, 1120, 1121, 1123, 1120, 1121, 1123},
    {1125, 1124, 1126, 1129, 1128, 1130, 1133, 1132, 1134},
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {1112, 1115, 1117, 1118, 1121, 1123, 1118, 1121, 1123},
    {1116, 1115, 1113, 1122, 1121, 1119, 1122, 1121, 1119},
    {1713, 1712, 1714, 1718, 1717, 1709, 1718, 1717, 1709},
    {1710, 1712, 1714, 1715, 1717, 1709, 1715, 1717, 1709},
    {1713, 1712, 1711, 1718, 1717, 1716, 1718, 1717, 1716},
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {9506, 9508, 9510, 9507, 9509, 9511, 9507, 9509, 9511},
};
static const int g_TB_frameImagesPhone[21][9] = {
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {1125, 1124, 1127, 1129, 1128, 1131, 1133, 1132, 1135},
    {1114, 1115, 1117, 1120, 1121, 1123, 1120, 1121, 1123},
    {1125, 1124, 1126, 1129, 1128, 1130, 1133, 1132, 1134},
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {1112, 1115, 1117, 1118, 1121, 1123, 1118, 1121, 1123},
    {1116, 1115, 1113, 1122, 1121, 1119, 1122, 1121, 1119},
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {1112, 1115, 1117, 1118, 1121, 1123, 1118, 1121, 1123},
    {1116, 1115, 1113, 1122, 1121, 1119, 1122, 1121, 1119},
    {1116, 1115, 1117, 1122, 1121, 1123, 1122, 1121, 1123},
    {9506, 9508, 9510, 9507, 9509, 9511, 9507, 9509, 9511},
};

void TouchButton::init(const String &text, unsigned int subId, int kind, int medalIndex, int medalStage, int x, int y,
                       int width, unsigned char flags0, unsigned char flags1) {
    this->kind = kind;
    this->visible = 1;
    this->halfTransparent = 0;
    this->text = text;
    this->subId = subId;
    this->textColor = -1;
    this->requestedWidth = width;
    this->id = 0;
    this->flags1 = flags1;
    this->flags0 = flags0;
    this->touched = 0;
    this->alwaysPressed = 0;
    this->textOffsetX = 0;
    this->textOffsetY = 0;
    this->leftWidth = 0;
    this->midWidth = 0;
    this->rightWidth = 0;
    this->progressHighlight = 1;
    this->pressProgress = 0.0f;
    this->touchMargin = 0;
    this->height = 0;
    this->initX = x;
    this->initY = y;
    this->numberText = String("");
    this->splitText = String("");
    this->adornImage = -1;

    switch (kind) {
    case 4: {
        if (Globals::achievements->isEliteMedal(medalIndex)) {
            this->iconTexId = g_TB_eliteMedalTints[medalStage];
            Globals::Canvas->Image2DCreate(g_TB_eliteMedalImages[medalStage], this->iconImage);
        } else {
            this->iconTexId = g_TB_medalTints[medalStage];
            Globals::Canvas->Image2DCreate(g_TB_medalImages[medalStage], this->iconImage);
        }
        this->height = Globals::Canvas->GetImage2DHeight(this->iconImage);
        int w = Globals::Canvas->GetImage2DWidth(this->iconImage);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetY = this->height + 5;
        int textW = Globals::Canvas->GetTextWidth(this->fontId, text);
        this->iconOverlay = -1;
        this->textOffsetX = w / 2 - textW / 2;
        this->iconSmall = -1;
        Globals::Canvas->Image2DCreate(g_TB_medalIcons[medalIndex], (unsigned int &) this->iconSmall);
        if (medalStage != 0 || Globals::achievements->isEliteMedal(medalIndex))
            Globals::Canvas->Image2DCreate(2412, (unsigned int &) this->iconOverlay);
        break;
    }
    case 10: {
        Globals::Canvas->Image2DCreate(9000, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(9001, (unsigned int &) this->frameImages[0][0]);
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetTextWidth(this->fontId, text) / 2;
        this->textOffsetY = Globals::Canvas->GetTextHeight(this->fontId);
        this->touchMargin = Globals::layout->field_0x80_touchMargin;
        break;
    }
    case 12: {
        Globals::Canvas->Image2DCreate(1138, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(1139, (unsigned int &) this->frameImages[0][0]);
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]) / 2;
        this->textOffsetY = this->height / 2 - Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]) / 2;
        break;
    }
    case 13: {
        Globals::Canvas->Image2DCreate(1303, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(1304, (unsigned int &) this->frameImages[0][0]);
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]) / 2;
        this->textOffsetY = this->height / 2 - Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]) / 2;
        break;
    }
    case 14: {
        Globals::Canvas->Image2DCreate(1340, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(1339, (unsigned int &) this->frameImages[0][0]);
        unsigned short language = GameText::getLanguage();
        float lineFactor;
        if (language < 16 && ((1 << language) & 0x8c00))
            lineFactor = 1.0f;
        else
            lineFactor = language == 14 ? 1.0f : 1.5f;
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetTextWidth(this->fontId, text) / 2;
        this->textOffsetY = (int) ((float) (this->height / 2) +
                                   lineFactor * (float) Globals::Canvas->GetTextHeight(this->fontId));
        this->touchMargin = Globals::layout->field_0x80_touchMargin;
        break;
    }
    case 15: {
        Globals::Canvas->Image2DCreate(1342, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(1341, (unsigned int &) this->frameImages[0][0]);
        unsigned short language = GameText::getLanguage();
        float lineFactor;
        if (language < 16 && ((1 << language) & 0x8c00))
            lineFactor = 1.0f;
        else
            lineFactor = language == 14 ? 1.0f : 1.5f;
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetTextWidth(this->fontId, text) / 2;
        this->textOffsetY = (int) ((float) (this->height / 2) +
                                   lineFactor * (float) Globals::Canvas->GetTextHeight(this->fontId));
        this->touchMargin = Globals::layout->field_0x80_touchMargin;
        break;
    }
    case 16: {
        this->frameImages[1][0] = subId;
        Globals::Canvas->Image2DCreate(3001, (unsigned int &) this->frameImages[0][0]);
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]) / 2;
        this->textOffsetY = this->height / 2 - Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]) / 2;
        break;
    }
    case 17: {
        Globals::Canvas->Image2DCreate(3008, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(3009, (unsigned int &) this->frameImages[0][0]);
        unsigned short language = GameText::getLanguage();
        float lineFactor;
        if (language < 16 && ((1 << language) & 0x8c00))
            lineFactor = 1.0f;
        else
            lineFactor = language == 14 ? 1.0f : 1.5f;
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetTextWidth(this->fontId, text) / 2;
        this->textOffsetY = (int) ((float) (this->height / 2) +
                                   lineFactor * (float) Globals::Canvas->GetTextHeight(this->fontId));
        this->touchMargin = Globals::layout->field_0x80_touchMargin;
        break;
    }
    case 18: {
        Globals::Canvas->Image2DCreate(3008, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(3009, (unsigned int &) this->frameImages[0][0]);
        unsigned short language = GameText::getLanguage();
        float lineFactor;
        if (language < 16 && ((1 << language) & 0x8c00))
            lineFactor = 1.0f;
        else
            lineFactor = language == 14 ? 1.0f : 1.5f;
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetTextWidth(this->fontId, text) / 2;
        this->textOffsetY = (int) ((float) (this->height / 2) +
                                   lineFactor * (float) Globals::Canvas->GetTextHeight(this->fontId));
        this->touchMargin = Globals::layout->field_0x80_touchMargin;
        break;
    }
    case 19: {
        this->frameImages[1][0] = subId;
        this->frameImages[0][0] = this->image;
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]) / 2;
        this->textOffsetY = this->height / 2 - Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]) / 2;
        break;
    }
    case 20: {
        Globals::Canvas->Image2DCreate(8046, (unsigned int &) this->frameImages[1][0]);
        Globals::Canvas->Image2DCreate(8047, (unsigned int &) this->frameImages[0][0]);
        unsigned short language = GameText::getLanguage();
        float lineFactor;
        if (language < 16 && ((1 << language) & 0x8c00))
            lineFactor = 1.0f;
        else
            lineFactor = language == 14 ? 1.0f : 1.5f;
        this->frameImages[2][0] = this->frameImages[0][0];
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[1][0]);
        int w = Globals::Canvas->GetImage2DWidth(this->frameImages[1][0]);
        this->layoutHeight = this->height;
        this->width = w;
        this->leftWidth = w;
        this->textOffsetX = w / 2 - Globals::Canvas->GetTextWidth(this->fontId, text) / 2;
        this->textOffsetY = (int) ((float) (this->height / 2) +
                                   lineFactor * (float) Globals::Canvas->GetTextHeight(this->fontId));
        this->touchMargin = Globals::layout->field_0x80_touchMargin;
        break;
    }
    default: {
        if (Globals::iPad) {
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][0], (unsigned int &) this->frameImages[1][0]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][1], (unsigned int &) this->frameImages[1][1]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][2], (unsigned int &) this->frameImages[1][2]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][3], (unsigned int &) this->frameImages[0][0]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][4], (unsigned int &) this->frameImages[0][1]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][5], (unsigned int &) this->frameImages[0][2]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][6], (unsigned int &) this->frameImages[2][0]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][7], (unsigned int &) this->frameImages[2][1]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesTablet[kind][8], (unsigned int &) this->frameImages[2][2]);
        } else {
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][0], (unsigned int &) this->frameImages[1][0]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][1], (unsigned int &) this->frameImages[1][1]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][2], (unsigned int &) this->frameImages[1][2]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][3], (unsigned int &) this->frameImages[0][0]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][4], (unsigned int &) this->frameImages[0][1]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][5], (unsigned int &) this->frameImages[0][2]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][6], (unsigned int &) this->frameImages[2][0]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][7], (unsigned int &) this->frameImages[2][1]);
            Globals::Canvas->Image2DCreate(g_TB_frameImagesPhone[kind][8], (unsigned int &) this->frameImages[2][2]);
        }
        this->height = Globals::Canvas->GetImage2DHeight(this->frameImages[0][0]);
        this->leftWidth = Globals::Canvas->GetImage2DWidth(this->frameImages[0][0]);
        this->midWidth = Globals::Canvas->GetImage2DWidth(this->frameImages[0][1]);
        this->rightWidth = Globals::Canvas->GetImage2DWidth(this->frameImages[0][2]);
        this->layoutHeight = kind != 11 ? Globals::layout->field_0x30_rowHeight : this->height;
        if (kind == 0 || kind == 5 || kind == 6) {
            this->rightWidth -= 2;
        } else if (kind >= 7 && kind <= 9 && Globals::iPad) {
            if (Globals::iPadHD)
                this->layoutHeight = 70;
            else
                this->layoutHeight = Globals::iPadLarge ? 100 : 50;
        }
        this->setText(text);
        break;
    }
    }
    this->setPosition(x, y, flags0);
}


TouchButton::TouchButton(unsigned int kind, int a, int b, int c, int d,
                         unsigned char flags0, unsigned char flags1) {
    PaintCanvas **canvas = &Globals::Canvas;
    this->fontId = (uint32_t) (uintptr_t) Globals::font;
    this->fontSpacing = (*canvas)->FontGetSpacing(this->fontId);

    String tmp("", false);
    this->init(tmp, kind, a, -1, -1, b, c, d, flags0, flags1);
}


void TouchButton::draw() {
    unsigned int savedColor = Globals::Canvas->GetColor();
    if (!this->visible)
        return;
    if (this->halfTransparent) {
        Globals::Canvas->SetColor(0xffffff2f);
        Globals::layout->setDrawColor(0xffffff2f);
    } else {
        Globals::Canvas->SetColor(0xffffffff);
    }
    short savedSpacing = Globals::Canvas->FontGetSpacing(this->fontId);
    Globals::Canvas->FontSetSpacing(this->fontId, (short) this->fontSpacing);

    switch (this->kind) {
    case 4:
        Globals::Canvas->DrawImage2D(this->iconImage, this->x, this->y);
        if (this->iconSmall != -1) {
            Globals::Canvas->SetColor(this->iconTexId);
            Globals::Canvas->DrawImage2D(this->iconSmall, this->x + (this->width >> 1),
                                         this->y + (this->height >> 1) - 1, 0x11, 0x44);
            Globals::Canvas->SetColor(0xffffffff);
            if (this->touched || this->alwaysPressed)
                Globals::Canvas->DrawImage2D(this->iconOverlay, this->x, this->y);
        }
        Globals::Canvas->SetColor(this->iconSmall == -1 ? 0x777777ff : 0xffffffff);
        Globals::Canvas->DrawString(this->fontId, this->text, this->x + this->textOffsetX, this->y + this->textOffsetY,
                                    false);
        break;
    case 16:
        Globals::Canvas->DrawImage2D(this->frameImages[1][0], this->x, this->y);
        if (this->touched || this->alwaysPressed)
            Globals::Canvas->DrawImage2D(this->frameImages[0][0], this->x, this->y);
        break;
    default: {
        int leftCap = this->touched ? this->frameImages[0][0] : ((this->alwaysPressed ? this->frameImages[2][0] : this->frameImages[1][0]));
        switch (this->kind) {
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
        case 18:
        case 19:
        case 20:
            break;
        default: {
            // Three-part stretchable frame.
            int middle;
            int rightCap;
            if (this->touched) {
                middle = this->frameImages[0][1];
                rightCap = this->frameImages[0][2];
            } else {
                middle = (this->alwaysPressed ? this->frameImages[2][1] : this->frameImages[1][1]);
                rightCap = (this->alwaysPressed ? this->frameImages[2][2] : this->frameImages[1][2]);
            }
            Globals::layout->drawBGPattern(middle, this->leftWidth + this->x, this->y, this->midStretch, this->height);
            Globals::Canvas->DrawImage2D(rightCap, this->x + this->leftWidth + this->midStretch, this->y);
            break;
        }
        }
        Globals::Canvas->DrawImage2D(leftCap, this->x, this->y);
        Globals::layout->setDrawColor(0xffffffff);
        if (this->pressProgress > 0.0f) {
            Globals::Canvas->SetColor(0xffffff80);
            Globals::layout->setDrawColor(0xffffff80);
            int filled =
                (int) (this->pressProgress * (float) (this->leftWidth + this->midStretch + this->rightWidth));
            Globals::Canvas->DrawRegion2D((this->progressHighlight ? this->frameImages[0][0] : this->frameImages[1][0]), 0, 0,
                                          this->leftWidth > filled ? filled : this->leftWidth, this->height, 0.0f, 0,
                                          0, this->x, this->y);
            if (this->leftWidth < filled) {
                int rest = filled - this->leftWidth;
                Globals::layout->drawBGPattern((this->progressHighlight ? this->frameImages[0][1] : this->frameImages[1][1]),
                                               this->leftWidth + this->x, this->y,
                                               this->midStretch > rest ? rest : this->midStretch, this->height);
                if (this->midStretch + this->leftWidth < filled) {
                    int right = filled - this->leftWidth - this->midStretch;
                    Globals::Canvas->DrawRegion2D(this->frameImages[this->progressHighlight ? 0 : 1][2], 0, 0,
                                                  this->rightWidth > right ? right : this->rightWidth, this->height,
                                                  0.0f, 0, 0, this->midStretch + this->leftWidth + this->x, this->y);
                }
            }
            Globals::layout->setDrawColor(0xffffffff);
        }
        if (this->halfTransparent)
            Globals::Canvas->SetColor((unsigned char) (this->textColor >> 24), (unsigned char) (this->textColor >> 16),
                                      (unsigned char) (this->textColor >> 8), 47);
        else
            Globals::Canvas->SetColor(this->textColor);
        if (this->subId != -1) {
            if (this->kind != 19)
                Globals::Canvas->DrawImage2D(this->subId, this->x + this->textOffsetX, this->y + this->textOffsetY);
        } else {
            Globals::Canvas->DrawString(this->fontId, this->text, this->x + this->textOffsetX,
                                        this->y + this->textOffsetY, false);
            if (this->splitText.size() != 0) {
                if (this->kind == 10)
                    Globals::Canvas->DrawString(
                        this->fontId, this->splitText,
                        this->x + this->width / 2 - Globals::Canvas->GetTextWidth(this->fontId, this->splitText) / 2,
                        this->height + this->y - Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) * 2, false);
                else
                    Globals::Canvas->DrawString(this->fontId, this->splitText,
                                                this->x + this->width - this->textOffsetX -
                                                    Globals::Canvas->GetTextWidth(this->fontId, this->splitText),
                                                this->y + this->textOffsetY, false);
            }
            if (this->numberText.size() != 0) {
                Globals::Canvas->SetColor(0x777777ff);
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, this->numberText,
                                            this->leftWidth + this->x -
                                                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->numberText),
                                            this->y + this->textOffsetY, false);
                Globals::Canvas->SetColor(this->textColor);
            }
            if (this->adornImage != -1) {
                Globals::Canvas->SetColor(0xffffffff);
                Globals::Canvas->DrawImage2D(this->adornImage, this->x + this->width + 6 - this->leftWidth, this->y + 1,
                                             0x11, 0x14);
                Globals::Canvas->SetColor(this->textColor);
            }
        }
        break;
    }
    }
    Globals::layout->setDrawColor(0xffffffff);
    Globals::Canvas->FontSetSpacing(this->fontId, savedSpacing);
    Globals::Canvas->SetColor(savedColor);
}


TouchButton::TouchButton(String const &text, int type, int x, int y, int p5, unsigned char p6, unsigned char p7) {
    this->fontId = (uint32_t) (uintptr_t) Globals::font;
    this->fontSpacing = Globals::Canvas->FontGetSpacing(this->fontId);
    init(text, 0xffffffff, type, -1, -1, x, y, p5, p6, p7);
}

void TouchButton::setText(String const &text) {
    this->text = text;
    int textW = Globals::Canvas->GetTextWidth(this->fontId, text);
    if (this->subId != -1)
        textW = Globals::Canvas->GetImage2DWidth(this->subId);
    if (this->requestedWidth > 0)
        this->midStretch = this->requestedWidth - this->leftWidth - this->rightWidth;
    else
        this->midStretch = textW;
    this->width = this->leftWidth + this->midStretch + this->rightWidth;
    if ((this->flags1 & 2) != 0) {
        this->textOffsetX = this->width - (textW + this->leftWidth);
    } else if ((this->flags1 & 1) != 0) {
        this->textOffsetX = this->leftWidth;
    } else {
        this->textOffsetX = (this->width - textW) / 2;
        if (this->kind == 5)
            this->textOffsetX = (this->width - textW) / 2 + 5;
        else if (this->kind == 6)
            this->textOffsetX = (this->width - textW) / 2 - 5;
    }
    this->textOffsetY = (this->height - Globals::Canvas->GetTextHeight(this->fontId)) / 2;
    if (this->kind == 3)
        this->textOffsetY = this->textOffsetY + 2;
    if (this->subId != -1) {
        this->textOffsetY = (this->height - Globals::Canvas->GetImage2DHeight(this->subId)) / 2;
        if (this->kind == 1)
            this->textOffsetX = this->textOffsetX + 3;
    }
    this->setPosition(this->initX, this->initY, this->flags0);
}

void TouchButton::setYPosition(int y) {
    setPosition(this->x, y, this->flags0);
}

bool TouchButton::touchedInside(int px, int py) {
    int x = this->x;
    int left = x - 1;
    if (this->kind == 3) {
        int inset = Globals::layout->field_0x38;
        if (left + inset > px)
            return false;
        if (this->width + (x + 1 - inset) <= px)
            return false;
        if (this->y - 1 > py)
            return false;
        return this->layoutHeight + this->y + 1 >= py;
    }
    int m = this->touchMargin;
    if (left - m > px)
        return false;
    if (this->width + (x + 1 + m) <= px)
        return false;
    if (this->y + ~m > py)
        return false;
    return this->layoutHeight + (this->y + m) + 1 >= py;
}

TouchButton::TouchButton(unsigned int kind, unsigned int image,
                         int a, int b, int c, unsigned char flag) {
    PaintCanvas **canvas = &Globals::Canvas;
    this->fontId = (uint32_t) (uintptr_t) Globals::font;
    this->image = image;
    this->fontSpacing = (*canvas)->FontGetSpacing(this->fontId);

    String tmp("", false);
    this->init(tmp, kind, a, -1, -1, b, c, 0, flag, 0x44);
}

TouchButton::TouchButton(String const &text, int x, int y, int p4, unsigned char p5) {
    this->fontId = (uint32_t) (uintptr_t) Globals::font;
    this->fontSpacing = Globals::Canvas->FontGetSpacing(this->fontId);
    init(text, 0xffffffff, x, -1, -1, y, p4, 0, p5, 0x44);
}

TouchButton::TouchButton(String const &text,
                         int a, int b, int c, int d,
                         unsigned char flags0, unsigned char flags1,
                         unsigned int spacing, int kerning) {
    this->fontId = spacing;
    this->fontSpacing = kerning;

    PaintCanvas **canvas = &Globals::Canvas;
    int prev = (*canvas)->FontGetSpacing(this->fontId);
    (*canvas)->FontSetSpacing(spacing, (short) kerning);

    this->init(text, 0xffffffffu, a, -1, -1, b, c, d, flags0, flags1);

    (*canvas)->FontSetSpacing(spacing, prev);
}

TouchButton::TouchButton(int x, int y, String const &text, int p4, int p5, unsigned char p6) {
    this->fontId = (uint32_t) (uintptr_t) Globals::font;
    this->fontSpacing = Globals::Canvas->FontGetSpacing(this->fontId);
    init(text, 0xffffffff, 4, x, y, p4, p5, 0, p6, 0x44);
}

TouchButton::TouchButton(unsigned int kind, int a, int b, int c, unsigned char flag) {
    this->fontId = (uint32_t) (uintptr_t) Globals::font;
    PaintCanvas *canvas = Globals::Canvas;
    this->fontSpacing = ((PaintCanvas *) (canvas))->FontGetSpacing(this->fontId);

    String tmp("", false);
    this->init(tmp, kind, a, -1, -1, b, c, 0, flag, 0x44);
}
