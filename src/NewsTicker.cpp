#include "gof2/NewsTicker.h"

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

extern "C" void PaintCanvas_SetColor(void *canvas, uint32_t color);
extern "C" void PaintCanvas_FillRectangle(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_EnableClip(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_DrawString(void *canvas, void *font, String *text, int x, int y, bool flag);
extern "C" int GameText_getLanguage();
extern "C" void PaintCanvas_DisableClip(void *canvas);
extern "C" void String_copy_ctor(void *self, void *other, bool copy);
extern "C" void String_ctor(void *self);
extern "C" void String_cstr_ctor(void *self, const char *text, bool copy);
extern "C" void String_assign(void *self, void *other);
extern "C" void String_plus(void *out, void *left, void *right);
extern "C" void String_plusAssign(void *self, void *other);
extern "C" void String_dtor(void *self);
typedef Array<NewsItemView *> NewsItemArray;
extern "C" void FileRead_ctor(void *self);
extern "C" NewsItemArray *FileRead_loadTicker(void *self);
extern "C" void *FileRead_dtor(void *self);
extern "C" void Array_NewsItem_ctor(NewsItemArray *self);
extern "C" void *Array_NewsItem_dtor(NewsItemArray *self);
extern "C" void ArrayReleaseClasses_NewsItem(NewsItemArray *self);
extern "C" void ArrayAdd_NewsItem(void *item, NewsItemArray *array);
extern "C" int AERandom_nextInt(void *random, int max);
extern "C" void *NewsItem_clone(void *item);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int64_t Status_getPlayingTime(void *status);
extern "C" String *GameText_getText(void *text, int id);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *font, String *text);

// ---- draw_15e174.cpp ----
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_draw_canvas;
__attribute__((visibility("hidden"))) extern int **g_NewsTicker_draw_fontHeight;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_draw_screenHeight;
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_draw_font;

void NewsTicker::draw()
{
    void **canvasHolder = g_NewsTicker_draw_canvas;
    PaintCanvas_SetColor(*canvasHolder, 0x6f);

    int *fontHeightTable = *g_NewsTicker_draw_fontHeight;
    int *screenHeight = g_NewsTicker_draw_screenHeight;
    {
        int x0 = this->field_0x4;
        int y0 = this->field_0x8;
        int clipBottom = *screenHeight;
        int fontHeight = fontHeightTable[4];
        int fillHeight = (2 - y0) + clipBottom - fontHeight;
        PaintCanvas_FillRectangle(*canvasHolder, x0, y0 - 2,
                                  this->field_0xc, fillHeight);
    }

    PaintCanvas_EnableClip(*canvasHolder, this->field_0x4, this->field_0x8,
                           this->field_0xc, *screenHeight);
    PaintCanvas_SetColor(*canvasHolder, 0x777777ff);

    void **fontHolder = g_NewsTicker_draw_font;
    String *text = &this->field_0x14;
    float drawX = this->field_0x0 + (float)this->field_0x4;
    PaintCanvas_DrawString(*canvasHolder, *fontHolder, text, (int)drawX,
                           this->field_0x8, false);

    int language = GameText_getLanguage();
    int textWidth = this->field_0x10;
    float x = this->field_0x0;
    if (language == 9) {
        if (x > (float)textWidth) {
            x += (float)this->field_0x4;
            int drawY = this->field_0x8;
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            PaintCanvas_DrawString(canvas, font, text, (int)x, drawY, false);
        }
    } else {
        if (x < (float)(textWidth - this->field_0xc)) {
            x += (float)this->field_0x4;
            x += (float)textWidth;
            int drawY = this->field_0x8;
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            PaintCanvas_DrawString(canvas, font, text, (int)x, drawY, false);
        }
    }

    return PaintCanvas_DisableClip(*canvasHolder);
}

// ---- OnTouchEnd_15e374.cpp ----
uint32_t NewsTicker::OnTouchEnd(int, int)
{
    if (this->field_0x28 == 0) {
        return 0;
    }
    this->field_0x28 = 0;
    return 1;
}

// ---- _NewsTicker_15e0d2.cpp ----
NewsTicker::~NewsTicker()
{
    this->field_0x14.~String();
}

// ---- update_15e0e4.cpp ----
void NewsTicker::update(int dt)
{
    if (this->field_0x28 != 0) {
        return;
    }

    float step = ((float)dt / 1000.0f) * 50.0f;
    if (GameText_getLanguage() == 9) {
        float x = this->field_0x0 + step;
        float width = (float)this->field_0xc;
        this->field_0x0 = x;
        if (x > width) {
            this->field_0x0 = (float)-this->field_0x10;
        }
    } else {
        float x = this->field_0x0 - step;
        float minX = (float)-this->field_0x10;
        this->field_0x0 = x;
        if (x < minX) {
            this->field_0x0 = 0;
        }
    }
}

// ---- OnTouchMove_15e328.cpp ----
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchMove_screenWidth;

bool NewsTicker::OnTouchMove(int x, int)
{
    uint8_t touched = this->field_0x28;
    if (touched != 0) {
        float delta = (float)(this->field_0x2c - x);
        float current = this->field_0x0;
        int *limit = g_NewsTicker_touchMove_screenWidth;
        float next = current - delta;
        this->field_0x0 = next;
        float maxX = (float)*limit;
        if (maxX < next) {
            this->field_0x0 = maxX;
        }
        this->field_0x2c = x;
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
    return bottom - this->field_0x8;
}

// ---- replaceTokens_15c858.cpp ----
String NewsTicker::replaceTokens(String text)
{
    String out;
    String_copy_ctor(&out, &text, false);
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
    String *tickerText = &this->field_0x14;
    String_ctor(tickerText);
    this->field_0x4 = x;
    this->field_0x8 = y;
    this->field_0xc = width;

    String empty;
    String_cstr_ctor(&empty, g_NewsTicker_ctor_empty, false);
    String_assign(tickerText, &empty);
    String_dtor(&empty);
    this->field_0x10 = 0;

    void *reader = operator new(1);
    FileRead_ctor(reader);
    NewsItemArray *allItems = FileRead_loadTicker(reader);
    operator delete(FileRead_dtor(reader));

    NewsItemArray *items = (NewsItemArray *)operator new(sizeof(NewsItemArray));
    Array_NewsItem_ctor(items);

    void *random = *g_NewsTicker_ctor_random;
    int wanted = AERandom_nextInt(random, 1) + 2;

    for (uint32_t i = 0; i < allItems->size(); ++i) {
        NewsItemView *item = (*allItems)[i];
        int minLevel = item->field_0x10;
        if (minLevel > 0 && minLevel <= level && level <= item->field_0x14) {
            int *factions = item->field_0x8;
            if (((uint8_t *)factions)[faction] != 0) {
                ArrayAdd_NewsItem(NewsItem_clone(item), items);
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
        NewsItemView *item = (*allItems)[AERandom_nextInt(random, allItems->size())];
        void *system = Status_getSystem(*g_NewsTicker_ctor_status);
        if (SolarSystem_getIndex(system) > 0x15 && item->field_0x0 == 0x0d) {
            continue;
        }
        if (item->field_0x14 < 0xa1 || item->field_0x10 > level) {
            continue;
        }
        if (AERandom_nextInt(random, 100) > 0x31) {
            continue;
        }
        if (((uint8_t *)item->field_0x8)[faction] == 0 || item->field_0x18 != 0) {
            continue;
        }
        if (item->field_0x4 != 0) {
            char *status = (char *)*g_NewsTicker_ctor_status;
            int64_t now = Status_getPlayingTime(*g_NewsTicker_ctor_status);
            int64_t last = *(int64_t *)(status + 0x160);
            if ((uint32_t)(now - last) <= 0x493e0) {
                continue;
            }
            *(int64_t *)(status + 0x160) = now;
            *(int *)(status + 0x174) = item->field_0x0;
        }
        ArrayAdd_NewsItem(NewsItem_clone(item), items);
        item->field_0x18 = 1;
        ++added;
    }

    String separator;
    String_cstr_ctor(&separator, g_NewsTicker_ctor_separator, false);
    for (uint32_t i = 0; i < items->size(); ++i) {
        NewsItemView *item = (*items)[i];
        String line;
        String_copy_ctor(&line, GameText_getText(*g_NewsTicker_ctor_text,
                                                 item->field_0x0 + 0x0cbe),
                         false);
        String replaced = replaceTokens(line);
        String_dtor(&line);

        if (item->field_0x4 != 0) {
            String *statusText = (String *)((char *)*g_NewsTicker_ctor_status + 0x168);
            if (item->field_0x18 == 0) {
                String_assign(&replaced, statusText);
            } else {
                String_assign(statusText, &replaced);
            }
        }

        String combined;
        String_plus(&combined, &replaced, &separator);
        String_plusAssign(tickerText, &combined);
        String_dtor(&combined);
        String_dtor(&replaced);
    }

    this->field_0x10 = PaintCanvas_GetTextWidth(*g_NewsTicker_ctor_canvas,
                                                  *g_NewsTicker_ctor_font, tickerText);
    if (this->field_0x10 < width) {
        String copy;
        String_copy_ctor(&copy, tickerText, false);
        String_plusAssign(tickerText, &copy);
        String_dtor(&copy);
        this->field_0x10 = PaintCanvas_GetTextWidth(*g_NewsTicker_ctor_canvas,
                                                      *g_NewsTicker_ctor_font, tickerText);
    }

    if (GameText_getLanguage() == 9) {
        width = -this->field_0x10;
    }
    this->field_0x2c = 0;
    this->field_0x28 = 0;
    this->field_0x0 = (float)width;

    ArrayReleaseClasses_NewsItem(items);
    operator delete(Array_NewsItem_dtor(items));
    ArrayReleaseClasses_NewsItem(allItems);
    operator delete(Array_NewsItem_dtor(allItems));
    String_dtor(&separator);
}

// ---- OnTouchBegin_15e2d8.cpp ----
__attribute__((visibility("hidden"))) extern int **g_NewsTicker_touchBegin_font;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchBegin_screen;

uint8_t NewsTicker::OnTouchBegin(int x, int y)
{
    if (this->field_0x4 <= x &&
        x <= this->field_0x4 + this->field_0xc &&
        this->field_0x8 <= y) {
        int fontHeight = (*g_NewsTicker_touchBegin_font)[4];
        int bottom = *g_NewsTicker_touchBegin_screen;
        bottom += 2;
        __builtin_sub_overflow(bottom, fontHeight, &bottom);
        if (y <= bottom) {
            this->field_0x2c = x;
            this->field_0x28 = 1;
        }
    }
    return this->field_0x28;
}
