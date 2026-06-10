#include "gof2/ScrollTouchBox.h"


extern "C" void ArrayReleaseClasses_StringPtr(void *self);
extern "C" void *Array_StringPtr_dtor(void *self);
extern "C" void String_ctor_string(void *self, String *src, bool copy);
extern "C" void String_dtor(void *self);
extern "C" void ScrollTouchBox_setText2(ScrollTouchBox *self, String *text, int font);
extern "C" int GameText_getLanguage();
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" int PaintCanvas_GetTextHeight(void *canvas, String *font);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, String *font, String *text);
extern "C" void PaintCanvas_DrawString(void *canvas, String *font, String *text, int x, int y, int unknown);
extern "C" void Array_StringPtr_ctor(void *self);
extern "C" void Globals_getLineArray(void *self, int font, String *text, int lineWidth, void *array);
extern "C" void String_ctor_cstr(void *self, char const *text, bool copy);
extern "C" void ArrayAdd_StringPtr(String *value, void *array);

// font-line-height accessor: the engine's font object stores its line height at +0x4.
// Modeled as a small opaque struct accessed via a typed helper.
struct FontMetrics {
    int field_0x0;
    int field_0x4;   // line height
};

// ---- setTextCentered_135910.cpp ----
void ScrollTouchBox::setTextCentered(bool centered)
{
    this->field_0x38 = centered;
}

// ---- getRelativeScrollStartPos_135918.cpp ----
float ScrollTouchBox::getRelativeScrollStartPos()
{
    int pos = this->field_0x34;
    if (pos > 0)
        return 0.0f;
    return -(float)pos / (float)this->field_0x18;
}

// ---- _ScrollTouchBox_1355d8.cpp ----
void operator delete(void *ptr) noexcept;

ScrollTouchBox::~ScrollTouchBox()
{
    void *lines = this->field_0x0;
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = this->field_0x0;
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        this->field_0x0 = 0;
    }
}

// ---- setPosition_13576c.cpp ----
void ScrollTouchBox::setPosition(int x, int y)
{
    this->field_0x4 = x;
    this->field_0x8 = y;
}

// ---- OnTouchEnd_135a98.cpp ----
void ScrollTouchBox::OnTouchEnd(int x, int y)
{
    if (this->field_0x30 != 0) {
        int delta = this->field_0x1c;
        float speed = 0.0f;
        if ((delta < 0 ? -delta : delta) > 3)
            speed = (float)delta;
        this->field_0x20 = 0.95f;
        this->field_0x30 = 0;
        this->field_0x34 = delta + this->field_0x34;
        this->field_0x24 = speed;
    }
}

// ---- OnTouchMove_135a72.cpp ----
void ScrollTouchBox::OnTouchMove(int x, int y)
{
    if (this->field_0x30 != 0 && this->field_0x18 > this->field_0x10) {
        int delta = y - this->field_0x2c;
        this->field_0x2c = y;
        this->field_0x1c = delta;
        this->field_0x20 = 1.0f;
        this->field_0x34 = delta + this->field_0x34;
    }
}

// ---- OnTouchBegin_135a54.cpp ----
void ScrollTouchBox::OnTouchBegin(int x, int y)
{
    if (touchIsInside(x, y)) {
        this->field_0x28 = y;
        this->field_0x2c = y;
        this->field_0x1c = 0;
        this->field_0x30 = 1;
    }
}

// ---- ScrollTouchBox_135598.cpp ----
__attribute__((visibility("hidden"))) extern String **g_ScrollTouchBox_defaultFont_135598;

ScrollTouchBox::ScrollTouchBox(int x, int y, int width, int height)
{
    this->field_0x28 = 0;
    this->field_0x0 = 0;
    this->field_0x4 = x;
    this->field_0x8 = y;
    this->field_0xc = width;
    this->field_0x10 = height;
    this->field_0x14 = width;
    this->field_0x30 = 0;
    this->field_0x34 = 0;
    this->field_0x38 = 0;
    // NEON vector store of zeros across the scroll-state block (0x18..0x24)
    this->field_0x18 = 0;
    this->field_0x1c = 0;
    this->field_0x20 = 0.0f;
    this->field_0x24 = 0.0f;
    this->field_0x3c = *g_ScrollTouchBox_defaultFont_135598;
}

// ---- setText_13570c.cpp ----
__attribute__((visibility("hidden"))) extern int **g_ScrollTouchBox_defaultWidth_13570c;

void ScrollTouchBox::setText(AbyssEngine::String text)
{
    String tmp;

    String_ctor_string(&tmp, &text, false);
    ScrollTouchBox_setText2(this, &tmp, **g_ScrollTouchBox_defaultWidth_13570c);
    String_dtor(&tmp);
}

// ---- update_135998.cpp ----
void ScrollTouchBox::update(int dt)
{
    int height = this->field_0x10;
    int contentHeight = this->field_0x18;
    if (contentHeight < height)
        return;

    if (this->field_0x30 == 0) {
        float velocity = this->field_0x20 * this->field_0x24;
        float absVelocity = -velocity;
        if (velocity > 0.0f)
            absVelocity = velocity;
        this->field_0x24 = velocity;
        if (absVelocity > 1.5f)
            this->field_0x34 = (int)(velocity + (float)this->field_0x34);
    }

    int pos = this->field_0x34;
    int pull;
    if (pos < 1)
        goto negative;

    pull = -pos;
    goto apply;

negative:
    {
        int over = contentHeight - height;
        int min = -over;
        if (min <= pos)
            return;
        over += pos;
        pull = -over;
    }

apply:
    this->field_0x20 = 1.0f;
    this->field_0x24 = (float)pull * 0.5f;
}

// ---- draw_135778.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_canvas_135778;
__attribute__((visibility("hidden"))) extern char *g_ScrollTouchBox_flag_135778;
__attribute__((visibility("hidden"))) extern FontMetrics **g_ScrollTouchBox_font_135778;
__attribute__((visibility("hidden"))) extern uint8_t *g_ScrollTouchBox_rtl_135778;
__attribute__((visibility("hidden"))) extern uint8_t *g_ScrollTouchBox_dirty_135778;

void ScrollTouchBox::draw()
{
    int language = GameText_getLanguage();
    unsigned special = 0;
    unsigned shifted = (unsigned)(language - 10);
    if ((unsigned short)shifted < 6)
        special = (0x33U >> (shifted & 0x3f)) & 1;

    void **canvasHolder = g_ScrollTouchBox_canvas_135778;
    void *canvas = *canvasHolder;
    PaintCanvas_SetColor(canvas, -1);

    Array<String*> *firstLineArray = this->field_0x0;
    if (firstLineArray != 0) {
        unsigned notSpecial = special ^ (firstLineArray != 0);
        char *flag = g_ScrollTouchBox_flag_135778;
        FontMetrics **fontHolder = g_ScrollTouchBox_font_135778;
        uint8_t *rtl = g_ScrollTouchBox_rtl_135778;

        for (unsigned i = 0; i < this->field_0x0->size(); ++i) {
            Array<String*> *lineArray = this->field_0x0;
            unsigned count = (unsigned)lineArray->size();
            int lastOffset;
            if (i != count - 1 || notSpecial != 0) {
                lastOffset = 0;
            } else {
                lastOffset = -8;
                if (*flag == 0)
                    lastOffset = -4;
            }

            int yBase = this->field_0x8;
            int lineY = (*fontHolder)->field_0x4 * (int)i + yBase + this->field_0x34;
            if (count == 1 ||
                (yBase <= lineY &&
                 lineY + lastOffset <= (this->field_0x10 + yBase) - PaintCanvas_GetTextHeight(canvas, this->field_0x3c))) {
                int x;
                String *font = this->field_0x3c;
                String *line = lineArray->data()[i];
                canvas = *canvasHolder;
                if (GameText_getLanguage() == 9) {
                    *rtl = 0;
                    int left = this->field_0x4;
                    int width = this->field_0xc;
                    if (this->field_0x38 == 0) {
                        x = (left + width) - PaintCanvas_GetTextWidth(canvas, font, line);
                    } else {
                        x = (left + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                } else {
                    x = this->field_0x4;
                    if (this->field_0x38 != 0) {
                        int width = this->field_0xc;
                        x = (x + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                }
                PaintCanvas_DrawString(canvas, font, line, x, lineY, 0);
            }
        }
    }

    *g_ScrollTouchBox_dirty_135778 = 1;
}

// ---- setText_135600.cpp ----
void *operator new(__SIZE_TYPE__ size);


__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_globals_135600;
__attribute__((visibility("hidden"))) extern FontMetrics **g_ScrollTouchBox_font_135600;
__attribute__((visibility("hidden"))) extern char g_ScrollTouchBox_empty_135600[];

void ScrollTouchBox::setText(AbyssEngine::String text, int font)
{
    void *lines = this->field_0x0;
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = this->field_0x0;
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        this->field_0x0 = 0;
    }

    this->field_0x3c = (String *)(__SIZE_TYPE__)font;
    Array<String*> *lineArray = new Array<String*>();

    void **globals = g_ScrollTouchBox_globals_135600;
    int lineWidth = this->field_0x14;
    this->field_0x0 = lineArray;
    Globals_getLineArray(*globals, font, &text, lineWidth, lineArray);

    FontMetrics **fontHolder = g_ScrollTouchBox_font_135600;
    int boxHeight = this->field_0x10;
    Array<String*> *curLines = this->field_0x0;
    FontMetrics *fontObj = *fontHolder;
    int contentHeight = fontObj->field_0x4 * (int)curLines->size();
    this->field_0x18 = contentHeight;
    if (contentHeight > boxHeight) {
        this->field_0xc = this->field_0x14 - *(int *)((char *)fontObj + 0x48);
        if (curLines != 0) {
            ArrayReleaseClasses_StringPtr(curLines);
            lines = this->field_0x0;
            if (lines != 0)
                operator delete(Array_StringPtr_dtor(lines));
            this->field_0x0 = 0;
        }

        lineArray = new Array<String*>();
        int fontArg = font;
        lineWidth = this->field_0xc;
        this->field_0x0 = lineArray;
        Globals_getLineArray(*globals, fontArg, &text, lineWidth, lineArray);

        String *empty = (String *)operator new(sizeof(String));
        char const *emptyText = g_ScrollTouchBox_empty_135600;
        bool copy = false;
        String_ctor_cstr(empty, emptyText, copy);
        ArrayAdd_StringPtr(empty, this->field_0x0);
        Array<String*> *finalLines = this->field_0x0;
        FontMetrics *finalFont = *fontHolder;
        int finalCount = (int)finalLines->size();
        int finalLineHeight = finalFont->field_0x4;
        this->field_0x18 = finalLineHeight * finalCount;
    }

    // NEON vector store of zeros across the scroll-state block
    this->field_0x34 = 0;
    this->field_0x1c = 0;
    this->field_0x20 = 0.0f;
    this->field_0x24 = 0.0f;
    this->field_0x28 = 0;
}

// ---- getRelativeScrollHeight_135948.cpp ----
float ScrollTouchBox::getRelativeScrollHeight()
{
    int height = this->field_0x10;
    int contentHeight = this->field_0x18;
    if (height > contentHeight)
        return 1.0f;

    int pos = this->field_0x34;
    if (pos >= 1) {
        pos = height - pos;
    } else if (pos < height - contentHeight) {
        pos = pos + contentHeight;
    } else {
        pos = height;
    }

    return (float)pos / (float)contentHeight;
}

// ---- touchIsInside_135a22.cpp ----
bool ScrollTouchBox::touchIsInside(int x, int y)
{
    int left = this->field_0x4;
    int top = 0;
    if (left <= x)
        top = this->field_0x8;
    if (left > x || top > y)
        return false;

    bool inside = false;
    if (x < left + this->field_0xc)
        inside = y < top + this->field_0x10;
    return inside;
}
