#include "ScrollTouchBox.h"


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

// ---- setTextCentered_135910.cpp ----
void ScrollTouchBox::setTextCentered(bool centered)
{
    F<uint8_t>(this, 0x38) = centered;
}

// ---- getRelativeScrollStartPos_135918.cpp ----
float ScrollTouchBox::getRelativeScrollStartPos()
{
    int pos = F<int>(this, 0x34);
    if (pos > 0)
        return 0.0f;
    return -(float)pos / (float)F<int>(this, 0x18);
}

// ---- _ScrollTouchBox_1355d8.cpp ----
void operator delete(void *ptr) noexcept;

ScrollTouchBox::~ScrollTouchBox()
{
    void *lines = F<void *>(this, 0x0);
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = F<void *>(this, 0x0);
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        F<void *>(this, 0x0) = 0;
    }
}

// ---- setPosition_13576c.cpp ----
void ScrollTouchBox::setPosition(int x, int y)
{
    F<int>(this, 0x4) = x;
    F<int>(this, 0x8) = y;
}

// ---- OnTouchEnd_135a98.cpp ----
void ScrollTouchBox::OnTouchEnd(int x, int y)
{
    if (F<uint8_t>(this, 0x30) != 0) {
        int delta = F<int>(this, 0x1c);
        float speed = 0.0f;
        if ((delta < 0 ? -delta : delta) > 3)
            speed = (float)delta;
        F<float>(this, 0x20) = 0.95f;
        F<uint8_t>(this, 0x30) = 0;
        F<int>(this, 0x34) = delta + F<int>(this, 0x34);
        F<float>(this, 0x24) = speed;
    }
}

// ---- OnTouchMove_135a72.cpp ----
void ScrollTouchBox::OnTouchMove(int x, int y)
{
    if (F<uint8_t>(this, 0x30) != 0 && F<int>(this, 0x18) > F<int>(this, 0x10)) {
        int delta = y - F<int>(this, 0x2c);
        F<int>(this, 0x2c) = y;
        F<int>(this, 0x1c) = delta;
        F<float>(this, 0x20) = 1.0f;
        F<int>(this, 0x34) = delta + F<int>(this, 0x34);
    }
}

// ---- OnTouchBegin_135a54.cpp ----
void ScrollTouchBox::OnTouchBegin(int x, int y)
{
    if (touchIsInside(x, y)) {
        F<int>(this, 0x28) = y;
        F<int>(this, 0x2c) = y;
        F<int>(this, 0x1c) = 0;
        F<uint8_t>(this, 0x30) = 1;
    }
}

// ---- ScrollTouchBox_135598.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_defaultFont_135598;

struct Int4 {
    int a;
    int b;
    int c;
    int d;
};

ScrollTouchBox::ScrollTouchBox(int x, int y, int width, int height)
{
    F<int>(this, 0x28) = 0;
    F<void *>(this, 0x0) = 0;
    F<int>(this, 0x4) = x;
    F<int>(this, 0x8) = y;
    F<int>(this, 0xc) = width;
    F<int>(this, 0x10) = height;
    F<int>(this, 0x14) = width;
    F<uint8_t>(this, 0x30) = 0;
    F<int>(this, 0x34) = 0;
    F<uint8_t>(this, 0x38) = 0;
    F<Int4>(this, 0x18) = Int4();
    F<void *>(this, 0x3c) = *g_ScrollTouchBox_defaultFont_135598;
}

// ---- setText_13570c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_defaultWidth_13570c;

void ScrollTouchBox::setText(AbyssEngine::String text)
{
    char tmp[12];

    String_ctor_string(tmp, &text, false);
    ScrollTouchBox_setText2(this, (String *)tmp, **(int **)g_ScrollTouchBox_defaultWidth_13570c);
    String_dtor(&tmp);
}

// ---- update_135998.cpp ----
void ScrollTouchBox::update(int dt)
{
    int height = F<int>(this, 0x10);
    int contentHeight = F<int>(this, 0x18);
    if (contentHeight < height)
        return;

    if (F<uint8_t>(this, 0x30) == 0) {
        float velocity = F<float>(this, 0x20) * F<float>(this, 0x24);
        float absVelocity = -velocity;
        if (velocity > 0.0f)
            absVelocity = velocity;
        F<float>(this, 0x24) = velocity;
        if (absVelocity > 1.5f)
            F<int>(this, 0x34) = (int)(velocity + (float)F<int>(this, 0x34));
    }

    int pos = F<int>(this, 0x34);
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
    F<float>(this, 0x20) = 1.0f;
    F<float>(this, 0x24) = (float)pull * 0.5f;
}

// ---- draw_135778.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_canvas_135778;
__attribute__((visibility("hidden"))) extern char *g_ScrollTouchBox_flag_135778;
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_font_135778;
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

    void *firstLineArray = *(void * volatile *)((char *)this + 0x0);
    if (firstLineArray != 0) {
        unsigned notSpecial = special ^ (firstLineArray != 0);
        char *flag = g_ScrollTouchBox_flag_135778;
        void **fontHolder = g_ScrollTouchBox_font_135778;
        uint8_t *rtl = g_ScrollTouchBox_rtl_135778;

        for (unsigned i = 0; i < F<unsigned>(*(void * volatile *)((char *)this + 0x0), 0x0); ++i) {
            void *lineArray = *(void * volatile *)((char *)this + 0x0);
            unsigned count = F<unsigned>(lineArray, 0x0);
            int lastOffset;
            if (i != count - 1 || notSpecial != 0) {
                lastOffset = 0;
            } else {
                lastOffset = -8;
                if (*flag == 0)
                    lastOffset = -4;
            }

            int yBase = F<int>(this, 0x8);
            int lineY = F<int>(*fontHolder, 0x4) * (int)i + yBase + F<int>(this, 0x34);
            if (count == 1 ||
                (yBase <= lineY &&
                 lineY + lastOffset <= (F<int>(this, 0x10) + yBase) - PaintCanvas_GetTextHeight(canvas, F<String *>(this, 0x3c)))) {
                int x;
                String *font = F<String *>(this, 0x3c);
                String *line = F<String *>(F<void *>(lineArray, 0x4), i * 4);
                canvas = *canvasHolder;
                if (GameText_getLanguage() == 9) {
                    *rtl = 0;
                    int left = F<int>(this, 0x4);
                    int width = F<int>(this, 0xc);
                    if (F<uint8_t>(this, 0x38) == 0) {
                        x = (left + width) - PaintCanvas_GetTextWidth(canvas, font, line);
                    } else {
                        x = (left + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                } else {
                    x = F<int>(this, 0x4);
                    if (F<uint8_t>(this, 0x38) != 0) {
                        int width = F<int>(this, 0xc);
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
void operator delete(void *ptr) noexcept;


__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_globals_135600;
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_font_135600;
__attribute__((visibility("hidden"))) extern char g_ScrollTouchBox_empty_135600[];

struct Int4 {
    int a;
    int b;
    int c;
    int d;
};

void ScrollTouchBox::setText(AbyssEngine::String text, int font)
{
    void *lines = F<void *>(this, 0x0);
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = F<void *>(this, 0x0);
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        F<void *>(this, 0x0) = 0;
    }

    F<int>(this, 0x3c) = font;
    lines = operator new(0xc);
    Array_StringPtr_ctor(lines);

    void **globals = g_ScrollTouchBox_globals_135600;
    int lineWidth = F<int>(this, 0x14);
    *(void * volatile *)((char *)this + 0x0) = lines;
    Globals_getLineArray(*globals, font, &text, lineWidth, lines);

    void **fontHolder = g_ScrollTouchBox_font_135600;
    int boxHeight = F<int>(this, 0x10);
    void *lineArray = F<void *>(this, 0x0);
    void *fontObj = *fontHolder;
    int contentHeight = F<int>(fontObj, 0x4) * F<int>(lineArray, 0x0);
    F<int>(this, 0x18) = contentHeight;
    if (contentHeight > boxHeight) {
        F<int>(this, 0xc) = F<int>(this, 0x14) - F<int>(fontObj, 0x48);
        if (lineArray != 0) {
            ArrayReleaseClasses_StringPtr(lineArray);
            lines = F<void *>(this, 0x0);
            if (lines != 0)
                operator delete(Array_StringPtr_dtor(lines));
            F<void *>(this, 0x0) = 0;
        }

        lines = operator new(0xc);
        Array_StringPtr_ctor(lines);
        int fontArg = font;
        lineWidth = *(volatile int *)((char *)this + 0xc);
        *(void * volatile *)((char *)this + 0x0) = lines;
        Globals_getLineArray(*globals, fontArg, &text, lineWidth, lines);

        String *empty = (String *)operator new(0xc);
        char const *emptyText = g_ScrollTouchBox_empty_135600;
        bool copy = false;
        String_ctor_cstr(empty, emptyText, copy);
        ArrayAdd_StringPtr(empty, F<void *>(this, 0x0));
        void *finalLines = F<void *>(this, 0x0);
        void *finalFont = *fontHolder;
        int finalCount = F<int>(finalLines, 0x0);
        int finalLineHeight = F<int>(finalFont, 0x4);
        F<int>(this, 0x18) = finalLineHeight * finalCount;
    }

    *(volatile int *)((char *)this + 0x34) = 0;
    *(volatile int *)((char *)this + 0x2d) = 0;
    *(volatile int *)((char *)this + 0x29) = 0;
    F<Int4>(this, 0x1c) = Int4();
}

// ---- getRelativeScrollHeight_135948.cpp ----
float ScrollTouchBox::getRelativeScrollHeight()
{
    int height = F<int>(this, 0x10);
    int contentHeight = F<int>(this, 0x18);
    if (height > contentHeight)
        return 1.0f;

    int pos = F<int>(this, 0x34);
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
    char *base = (char *)this;
    int left = *(int *)(base + 0x4);
    int top;
    if (left <= x)
        top = *(int *)(base + 0x8);
    if (left > x || top > y)
        return false;

    bool inside = false;
    if (x < left + *(int *)(base + 0xc))
        inside = y < top + *(int *)(base + 0x10);
    return inside;
}
