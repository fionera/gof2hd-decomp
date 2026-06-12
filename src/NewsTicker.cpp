#include "gof2/NewsTicker.h"
#include "gof2/SolarSystem.h"
#include "gof2/NewsItem.h"
#include "gof2/Status.h"
#include "gof2/GameText.h"
#include "gof2/String.h"
#include "gof2/PaintCanvas.h"

// Minimal view of NewsItem fields read here (NewsItem.h is opaque; not in this batch).
// Layout confirmed from Ghidra NewsTicker::NewsTicker @ 0x16c468.
namespace AbyssEngine {
struct NewsItemView {
    int      field_0x0;   // +0x0   type id
    uint8_t  field_0x4;   // +0x4   recurring flag
    uint8_t  pad5[3];
    int     *field_0x8;   // +0x8   -> faction byte table
    uint8_t  padc[4];
    int      field_0x10;  // +0x10  min level
    int      field_0x14;  // +0x14  max level
    uint8_t  field_0x18;  // +0x18  used flag
};
}
using AbyssEngine::NewsItemView;

extern void DisableClip();  // AbyssEngine::PaintCanvas::DisableClip (free fn, not methodized)
int GameText_getLanguage();
extern "C" void String_cstr_ctor(void *self, const char *text, bool copy);
extern "C" void String_plus(void *out, void *left, void *right);
extern "C" void String_plusAssign(void *self, void *other);
typedef Array<NewsItemView *> NewsItemArray;
// Minimal view of FileRead so loadTicker() resolves to the real FileRead::loadTicker.
class FileRead {
public:
    Array<NewsItem *> *loadTicker();
};
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_dtor(void *self);
extern "C" void Array_NewsItem_ctor(NewsItemArray *self);
extern "C" void *Array_NewsItem_dtor(NewsItemArray *self);
extern "C" void ArrayReleaseClasses_NewsItem(NewsItemArray *self);
extern "C" void ArrayAdd_NewsItem(void *item, NewsItemArray *array);
namespace AbyssEngine { namespace AERandom { int nextInt(void *random, int bound); } }

// ---- draw_15e174.cpp ----
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_draw_canvas;
__attribute__((visibility("hidden"))) extern int **g_NewsTicker_draw_fontHeight;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_draw_screenHeight;
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_draw_font;

void NewsTicker::draw()
{
    void **canvasHolder = g_NewsTicker_draw_canvas;
    ((PaintCanvas *)*canvasHolder)->SetColor(0x6f);

    int *fontHeightTable = *g_NewsTicker_draw_fontHeight;
    int *screenHeight = g_NewsTicker_draw_screenHeight;
    {
        int x0 = this->x;
        int y0 = this->y;
        int clipBottom = *screenHeight;
        int fontHeight = fontHeightTable[4];
        int fillHeight = (2 - y0) + clipBottom - fontHeight;
        ((PaintCanvas *)*canvasHolder)->FillRectangle(x0, y0 - 2,
                                  this->width, fillHeight);
    }

    ((PaintCanvas *)*canvasHolder)->EnableClip(this->x, this->y,
                           this->width, *screenHeight);
    ((PaintCanvas *)*canvasHolder)->SetColor(0x777777ff);

    void **fontHolder = g_NewsTicker_draw_font;
    String *text = &this->tickerText;
    float drawX = this->scrollOffset + (float)this->x;
    ((PaintCanvas *)*canvasHolder)->DrawString((unsigned int)(unsigned long)*fontHolder, (void *)text, (int)drawX,
                           this->y, false);

    int language = GameText_getLanguage();
    int textWidth = this->textWidth;
    float x = this->scrollOffset;
    if (language == 9) {
        if (x > (float)textWidth) {
            x += (float)this->x;
            int drawY = this->y;
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)text, (int)x, drawY, false);
        }
    } else {
        if (x < (float)(textWidth - this->width)) {
            x += (float)this->x;
            x += (float)textWidth;
            int drawY = this->y;
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)text, (int)x, drawY, false);
        }
    }

    return DisableClip();
}

// ---- OnTouchEnd_15e374.cpp ----
uint32_t NewsTicker::OnTouchEnd(int, int)
{
    if (this->touched == 0) {
        return 0;
    }
    this->touched = 0;
    return 1;
}

// ---- _NewsTicker_15e0d2.cpp ----
NewsTicker::~NewsTicker()
{
    this->tickerText.~String();
}

// ---- update_15e0e4.cpp ----
void NewsTicker::update(int dt)
{
    if (this->touched != 0) {
        return;
    }

    float step = ((float)dt / 1000.0f) * 50.0f;
    if (GameText_getLanguage() == 9) {
        float x = this->scrollOffset + step;
        float width = (float)this->width;
        this->scrollOffset = x;
        if (x > width) {
            this->scrollOffset = (float)-this->textWidth;
        }
    } else {
        float x = this->scrollOffset - step;
        float minX = (float)-this->textWidth;
        this->scrollOffset = x;
        if (x < minX) {
            this->scrollOffset = 0;
        }
    }
}

// ---- OnTouchMove_15e328.cpp ----
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchMove_screenWidth;

bool NewsTicker::OnTouchMove(int x, int)
{
    uint8_t touched = this->touched;
    if (touched != 0) {
        float delta = (float)(this->lastTouchX - x);
        float current = this->scrollOffset;
        int *limit = g_NewsTicker_touchMove_screenWidth;
        float next = current - delta;
        this->scrollOffset = next;
        float maxX = (float)*limit;
        if (maxX < next) {
            this->scrollOffset = maxX;
        }
        this->lastTouchX = x;
    }
    return touched != 0;
}

// ---- getHeight_15e2b4.cpp ----
__attribute__((visibility("hidden"))) extern int **g_NewsTicker_getHeight_font;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_getHeight_screen;

int NewsTicker::getHeight()
{
    int fontHeight = (*g_NewsTicker_getHeight_font)[4];
    int top = *g_NewsTicker_getHeight_screen;
    top += 2;
    int bottom;
    __builtin_sub_overflow(top, fontHeight, &bottom);
    return bottom - this->y;
}

// ---- replaceTokens_15c858.cpp ----
String NewsTicker::replaceTokens(String text)
{
    String out;
    ((String *)(&out))->ctor_copy((String *)(&text), false);
    return out;
}

// ---- NewsTicker_15c468.cpp ----
#include <new>


__attribute__((visibility("hidden"))) extern int *g_NewsTicker_ctor_canary;
__attribute__((visibility("hidden"))) extern const char g_NewsTicker_ctor_empty[];
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_ctor_random;
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_ctor_status;
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_ctor_text;
__attribute__((visibility("hidden"))) extern const char g_NewsTicker_ctor_separator[];
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_ctor_font;
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_ctor_canvas;

NewsTicker::NewsTicker(int x, int y, int width, int faction, int level)
{
    String *tickerText = &this->tickerText;
    ((String *)(tickerText))->ctor();
    this->x = x;
    this->y = y;
    this->width = width;

    String empty;
    String_cstr_ctor(&empty, g_NewsTicker_ctor_empty, false);
    ((String *)(tickerText))->assign(&empty);
    ((String *)(&empty))->dtor();
    this->textWidth = 0;

    void *reader = operator new(1);
    FileRead_ctor(reader);
    NewsItemArray *allItems = (NewsItemArray *)((FileRead *)reader)->loadTicker();
    operator delete(FileRead_dtor(reader));

    NewsItemArray *items = (NewsItemArray *)operator new(sizeof(NewsItemArray));
    Array_NewsItem_ctor(items);

    void *random = *g_NewsTicker_ctor_random;
    int wanted = AbyssEngine::AERandom::nextInt(random, 1) + 2;

    for (uint32_t i = 0; i < allItems->size(); ++i) {
        NewsItemView *item = (*allItems)[i];
        int minLevel = item->field_0x10;
        if (minLevel > 0 && minLevel <= level && level <= item->field_0x14) {
            int *factions = item->field_0x8;
            if (((uint8_t *)factions)[faction] != 0) {
                ArrayAdd_NewsItem(((NewsItem *)(item))->clone(), items);
            }
        }
    }

    int added = 0;
    int attempts = 0;
    while (added < wanted && attempts < 100) {
        ++attempts;
        if (allItems->size() == 0) {
            break;
        }
        NewsItemView *item = (*allItems)[AbyssEngine::AERandom::nextInt(random, allItems->size())];
        void *system = (void *)((Status *)(*g_NewsTicker_ctor_status))->getSystem();
        if (((SolarSystem *)(system))->getIndex() > 0x15 && item->field_0x0 == 0x0d) {
            continue;
        }
        if (item->field_0x14 < 0xa1 || item->field_0x10 > level) {
            continue;
        }
        if (AbyssEngine::AERandom::nextInt(random, 100) > 0x31) {
            continue;
        }
        if (((uint8_t *)item->field_0x8)[faction] == 0 || item->field_0x18 != 0) {
            continue;
        }
        if (item->field_0x4 != 0) {
            char *status = (char *)*g_NewsTicker_ctor_status;
            int64_t now = ((Status *)(*g_NewsTicker_ctor_status))->getPlayingTime();
            int64_t last = *(int64_t *)(status + 0x160);
            if ((uint32_t)(now - last) <= 0x493e0) {
                continue;
            }
            *(int64_t *)(status + 0x160) = now;
            *(int *)(status + 0x174) = item->field_0x0;
        }
        ArrayAdd_NewsItem(((NewsItem *)(item))->clone(), items);
        item->field_0x18 = 1;
        ++added;
    }

    String separator;
    String_cstr_ctor(&separator, g_NewsTicker_ctor_separator, false);
    for (uint32_t i = 0; i < items->size(); ++i) {
        NewsItemView *item = (*items)[i];
        String line;
        ((String *)(&line))->ctor_copy((String *)(((GameText *)(*g_NewsTicker_ctor_text))->getText(item->field_0x0 + 0x0cbe)), false);
        String replaced = replaceTokens(line);
        ((String *)(&line))->dtor();

        if (item->field_0x4 != 0) {
            String *statusText = (String *)((char *)*g_NewsTicker_ctor_status + 0x168);
            if (item->field_0x18 == 0) {
                ((String *)(&replaced))->assign(statusText);
            } else {
                ((String *)(statusText))->assign(&replaced);
            }
        }

        String combined;
        String_plus(&combined, &replaced, &separator);
        String_plusAssign(tickerText, &combined);
        ((String *)(&combined))->dtor();
        ((String *)(&replaced))->dtor();
    }

    this->textWidth = ((PaintCanvas *)*g_NewsTicker_ctor_canvas)->GetTextWidth(
                          (unsigned int)(unsigned long)*g_NewsTicker_ctor_font, (void *)tickerText);
    if (this->textWidth < width) {
        String copy;
        ((String *)(&copy))->ctor_copy((String *)(tickerText), false);
        String_plusAssign(tickerText, &copy);
        ((String *)(&copy))->dtor();
        this->textWidth = ((PaintCanvas *)*g_NewsTicker_ctor_canvas)->GetTextWidth(
                              (unsigned int)(unsigned long)*g_NewsTicker_ctor_font, (void *)tickerText);
    }

    if (GameText_getLanguage() == 9) {
        width = -this->textWidth;
    }
    this->lastTouchX = 0;
    this->touched = 0;
    this->scrollOffset = (float)width;

    ArrayReleaseClasses_NewsItem(items);
    operator delete(Array_NewsItem_dtor(items));
    ArrayReleaseClasses_NewsItem(allItems);
    operator delete(Array_NewsItem_dtor(allItems));
    ((String *)(&separator))->dtor();
}

// ---- OnTouchBegin_15e2d8.cpp ----
__attribute__((visibility("hidden"))) extern int **g_NewsTicker_touchBegin_font;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchBegin_screen;

uint8_t NewsTicker::OnTouchBegin(int x, int y)
{
    if (this->x <= x &&
        x <= this->x + this->width &&
        this->y <= y) {
        int fontHeight = (*g_NewsTicker_touchBegin_font)[4];
        int bottom = *g_NewsTicker_touchBegin_screen;
        bottom += 2;
        __builtin_sub_overflow(bottom, fontHeight, &bottom);
        if (y <= bottom) {
            this->lastTouchX = x;
            this->touched = 1;
        }
    }
    return this->touched;
}

// ---- C-ABI dtor wrapper (recovered shim) ----

// NewsTicker_dtor — C-ABI destructor. Runs ~NewsTicker() (which releases the
// ticker String) and returns the storage for the caller's operator delete.
extern "C" void *NewsTicker_dtor(void *p)
{
    if (p) ((NewsTicker *)p)->~NewsTicker();
    return p;
}
