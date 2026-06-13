#include "gof2/ScrollTouchBox.h"
#include "gof2/String.h"
#include "gof2/PaintCanvas.h"


extern "C" void ArrayReleaseClasses_StringPtr(void *self);
extern "C" void *Array_StringPtr_dtor(void *self);
extern "C" void String_ctor_string(void *self, String *src, bool copy);
extern "C" void ScrollTouchBox_setText2(ScrollTouchBox *self, String *text, int font);
int GameText_getLanguage();
extern "C" void Array_StringPtr_ctor(void *self);
void Globals_getLineArray(void *self, int font, String *text, int lineWidth, void *array);
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
    this->centered = centered;
}

// ---- getRelativeScrollStartPos_135918.cpp ----
float ScrollTouchBox::getRelativeScrollStartPos()
{
    int pos = this->scrollOffset;
    if (pos > 0)
        return 0.0f;
    return -(float)pos / (float)this->contentHeight;
}

// ---- _ScrollTouchBox_1355d8.cpp ----

ScrollTouchBox::~ScrollTouchBox()
{
    void *lines = this->lines;
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = this->lines;
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        this->lines = 0;
    }
}

// ---- setPosition_13576c.cpp ----
void ScrollTouchBox::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

// ---- OnTouchEnd_135a98.cpp ----
void ScrollTouchBox::OnTouchEnd(int x, int y)
{
    if (this->dragging != 0) {
        int delta = this->lastDelta;
        float speed = 0.0f;
        if ((delta < 0 ? -delta : delta) > 3)
            speed = (float)delta;
        this->damping = 0.95f;
        this->dragging = 0;
        this->scrollOffset = delta + this->scrollOffset;
        this->velocity = speed;
    }
}

// ---- OnTouchMove_135a72.cpp ----
void ScrollTouchBox::OnTouchMove(int x, int y)
{
    if (this->dragging != 0 && this->contentHeight > this->height) {
        int delta = y - this->lastTouchY;
        this->lastTouchY = y;
        this->lastDelta = delta;
        this->damping = 1.0f;
        this->scrollOffset = delta + this->scrollOffset;
    }
}

// ---- OnTouchBegin_135a54.cpp ----
void ScrollTouchBox::OnTouchBegin(int x, int y)
{
    if (touchIsInside(x, y)) {
        this->touchStartY = y;
        this->lastTouchY = y;
        this->lastDelta = 0;
        this->dragging = 1;
    }
}

// ---- ScrollTouchBox_135598.cpp ----
__attribute__((visibility("hidden"))) extern String **g_ScrollTouchBox_defaultFont_135598;

ScrollTouchBox::ScrollTouchBox(int x, int y, int width, int height)
{
    this->touchStartY = 0;
    this->lines = 0;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->textWidth = width;
    this->dragging = 0;
    this->scrollOffset = 0;
    this->centered = 0;
    // NEON vector store of zeros across the scroll-state block (0x18..0x24)
    this->contentHeight = 0;
    this->lastDelta = 0;
    this->damping = 0.0f;
    this->velocity = 0.0f;
    this->font = *g_ScrollTouchBox_defaultFont_135598;
}

// ---- setText_13570c.cpp ----
__attribute__((visibility("hidden"))) extern int **g_ScrollTouchBox_defaultWidth_13570c;

void ScrollTouchBox::setText(AbyssEngine::String text)
{
    String tmp;

    String_ctor_string(&tmp, &text, false);
    ScrollTouchBox_setText2(this, &tmp, **g_ScrollTouchBox_defaultWidth_13570c);
    ((String *)(&tmp))->dtor();
}

// ---- update_135998.cpp ----
void ScrollTouchBox::update(int dt)
{
    int height = this->height;
    int contentHeight = this->contentHeight;
    if (contentHeight < height)
        return;

    if (this->dragging == 0) {
        float velocity = this->damping * this->velocity;
        float absVelocity = -velocity;
        if (velocity > 0.0f)
            absVelocity = velocity;
        this->velocity = velocity;
        if (absVelocity > 1.5f)
            this->scrollOffset = (int)(velocity + (float)this->scrollOffset);
    }

    int pos = this->scrollOffset;
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
    this->damping = 1.0f;
    this->velocity = (float)pull * 0.5f;
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
    ((PaintCanvas*)canvas)->SetColor((unsigned int)-1);

    Array<String*> *firstLineArray = this->lines;
    if (firstLineArray != 0) {
        unsigned notSpecial = special ^ (firstLineArray != 0);
        char *flag = g_ScrollTouchBox_flag_135778;
        FontMetrics **fontHolder = g_ScrollTouchBox_font_135778;
        uint8_t *rtl = g_ScrollTouchBox_rtl_135778;

        for (unsigned i = 0; i < this->lines->size(); ++i) {
            Array<String*> *lineArray = this->lines;
            unsigned count = (unsigned)lineArray->size();
            int lastOffset;
            if (i != count - 1 || notSpecial != 0) {
                lastOffset = 0;
            } else {
                lastOffset = -8;
                if (*flag == 0)
                    lastOffset = -4;
            }

            int yBase = this->y;
            int lineY = (*fontHolder)->field_0x4 * (int)i + yBase + this->scrollOffset;
            if (count == 1 ||
                (yBase <= lineY &&
                 lineY + lastOffset <= (this->height + yBase) - ((PaintCanvas*)canvas)->GetTextHeight((unsigned int)(unsigned long)this->font))) {
                int x;
                String *font = this->font;
                String *line = lineArray->data()[i];
                canvas = *canvasHolder;
                if (GameText_getLanguage() == 9) {
                    *rtl = 0;
                    int left = this->x;
                    int width = this->width;
                    if (this->centered == 0) {
                        x = (left + width) - ((PaintCanvas*)canvas)->GetTextWidth((unsigned int)(unsigned long)font, line);
                    } else {
                        x = (left + (width >> 1)) - (((PaintCanvas*)canvas)->GetTextWidth((unsigned int)(unsigned long)font, line) >> 1);
                    }
                } else {
                    x = this->x;
                    if (this->centered != 0) {
                        int width = this->width;
                        x = (x + (width >> 1)) - (((PaintCanvas*)canvas)->GetTextWidth((unsigned int)(unsigned long)font, line) >> 1);
                    }
                }
                ((PaintCanvas*)canvas)->DrawString((unsigned int)(unsigned long)font, line, x, lineY, false);
            }
        }
    }

    *g_ScrollTouchBox_dirty_135778 = 1;
}

// ---- setText_135600.cpp ----


__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_globals_135600;
__attribute__((visibility("hidden"))) extern FontMetrics **g_ScrollTouchBox_font_135600;
__attribute__((visibility("hidden"))) extern char g_ScrollTouchBox_empty_135600[];

void ScrollTouchBox::setText(AbyssEngine::String text, int font)
{
    void *lines = this->lines;
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = this->lines;
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        this->lines = 0;
    }

    this->font = (String *)(__SIZE_TYPE__)font;
    Array<String*> *lineArray = new Array<String*>();

    void **globals = g_ScrollTouchBox_globals_135600;
    int lineWidth = this->textWidth;
    this->lines = lineArray;
    Globals_getLineArray(*globals, font, &text, lineWidth, lineArray);

    FontMetrics **fontHolder = g_ScrollTouchBox_font_135600;
    int boxHeight = this->height;
    Array<String*> *curLines = this->lines;
    FontMetrics *fontObj = *fontHolder;
    int contentHeight = fontObj->field_0x4 * (int)curLines->size();
    this->contentHeight = contentHeight;
    if (contentHeight > boxHeight) {
        this->width = this->textWidth - *(int *)((char *)fontObj + 0x48);
        if (curLines != 0) {
            ArrayReleaseClasses_StringPtr(curLines);
            lines = this->lines;
            if (lines != 0)
                operator delete(Array_StringPtr_dtor(lines));
            this->lines = 0;
        }

        lineArray = new Array<String*>();
        int fontArg = font;
        lineWidth = this->width;
        this->lines = lineArray;
        Globals_getLineArray(*globals, fontArg, &text, lineWidth, lineArray);

        String *empty = (String *)operator new(sizeof(String));
        char const *emptyText = g_ScrollTouchBox_empty_135600;
        bool copy = false;
        String_ctor_cstr(empty, emptyText, copy);
        ArrayAdd_StringPtr(empty, this->lines);
        Array<String*> *finalLines = this->lines;
        FontMetrics *finalFont = *fontHolder;
        int finalCount = (int)finalLines->size();
        int finalLineHeight = finalFont->field_0x4;
        this->contentHeight = finalLineHeight * finalCount;
    }

    // NEON vector store of zeros across the scroll-state block
    this->scrollOffset = 0;
    this->lastDelta = 0;
    this->damping = 0.0f;
    this->velocity = 0.0f;
    this->touchStartY = 0;
}

// ---- setText2_135600.cpp ----
// Lay out 'text' in the given font slot. This is the engine's disambiguated name
// for the two-argument setText overload (whose body lives at 0x145600).
void ScrollTouchBox::setText2(AbyssEngine::String *text, int font)
{
    setText(*text, font);
}

// ---- setTextColor_135600.cpp ----
// Lay out 'text' using the supplied colour/font slot. ScrollTouchWindow::setText
// routes its coloured-text request through here; it resolves to the same
// two-argument setText layout path (the colour rides in the font/style slot).
void ScrollTouchBox::setTextColor(AbyssEngine::String *text, int color)
{
    setText(*text, color);
}

// ---- getRelativeScrollHeight_135948.cpp ----
float ScrollTouchBox::getRelativeScrollHeight()
{
    int height = this->height;
    int contentHeight = this->contentHeight;
    if (height > contentHeight)
        return 1.0f;

    int pos = this->scrollOffset;
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
    int left = this->x;
    int top = 0;
    if (left <= x)
        top = this->y;
    if (left > x || top > y)
        return false;

    bool inside = false;
    if (x < left + this->width)
        inside = y < top + this->height;
    return inside;
}

// ---- setYPosition_145772.cpp ----
// Move the box vertically without re-laying out its text (the line array and
// content height are unaffected; only the top edge changes).
void ScrollTouchBox::setYPosition(int y) {
    this->y = y;
}
