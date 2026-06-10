#include "NewsTicker.h"


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
extern "C" void FileRead_ctor(void *self);
extern "C" Array *FileRead_loadTicker(void *self);
extern "C" void *FileRead_dtor(void *self);
extern "C" void Array_NewsItem_ctor(void *self);
extern "C" void *Array_NewsItem_dtor(void *self);
extern "C" void ArrayReleaseClasses_NewsItem(void *self);
extern "C" void ArrayAdd_NewsItem(void *item, void *array);
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
        int x0 = F<int>(this, 0x4);
        int y0 = F<int>(this, 0x8);
        int clipBottom = *screenHeight;
        int fontHeight = fontHeightTable[4];
        int fillHeight = (2 - y0) + clipBottom - fontHeight;
        PaintCanvas_FillRectangle(*canvasHolder, x0, y0 - 2,
                                  F<int>(this, 0x0c), fillHeight);
    }

    PaintCanvas_EnableClip(*canvasHolder, F<int>(this, 0x4), F<int>(this, 0x8),
                           F<int>(this, 0x0c), *screenHeight);
    PaintCanvas_SetColor(*canvasHolder, 0x777777ff);

    void **fontHolder = g_NewsTicker_draw_font;
    String *text = (String *)((char *)this + 0x14);
    float drawX = F<float>(this, 0x0) + (float)F<int>(this, 0x4);
    PaintCanvas_DrawString(*canvasHolder, *fontHolder, text, (int)drawX,
                           F<int>(this, 0x8), false);

    int language = GameText_getLanguage();
    int textWidth = F<int>(this, 0x10);
    float x = F<float>(this, 0x0);
    if (language == 9) {
        if (x > (float)textWidth) {
            x += (float)F<int>(this, 0x4);
            int drawY = *(volatile int *)((char *)this + 0x8);
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            PaintCanvas_DrawString(canvas, font, text, (int)x, drawY, false);
        }
    } else {
        if (x < (float)(textWidth - F<int>(this, 0x0c))) {
            x += (float)F<int>(this, 0x4);
            x += (float)textWidth;
            int drawY = *(volatile int *)((char *)this + 0x8);
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
    if (F<uint8_t>(this, 0x28) == 0) {
        return 0;
    }
    F<uint8_t>(this, 0x28) = 0;
    return 1;
}

// ---- _NewsTicker_15e0d2.cpp ----
NewsTicker::~NewsTicker()
{
    ((String *)((char *)this + 0x14))->~String();
}

// ---- update_15e0e4.cpp ----
void NewsTicker::update(int dt)
{
    if (F<uint8_t>(this, 0x28) != 0) {
        return;
    }

    float step = ((float)dt / 1000.0f) * 50.0f;
    if (GameText_getLanguage() == 9) {
        float x = F<float>(this, 0x0) + step;
        float width = (float)F<int>(this, 0x0c);
        F<float>(this, 0x0) = x;
        if (x > width) {
            F<float>(this, 0x0) = (float)-F<int>(this, 0x10);
        }
    } else {
        float x = F<float>(this, 0x0) - step;
        float minX = (float)-F<int>(this, 0x10);
        F<float>(this, 0x0) = x;
        if (x < minX) {
            F<int>(this, 0x0) = 0;
        }
    }
}

// ---- OnTouchMove_15e328.cpp ----
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchMove_screenWidth;

bool NewsTicker::OnTouchMove(int x, int)
{
    uint8_t touched = F<uint8_t>(this, 0x28);
    if (touched != 0) {
        float delta = (float)(F<int>(this, 0x2c) - x);
        float current = F<float>(this, 0x0);
        int *limit = g_NewsTicker_touchMove_screenWidth;
        float next = current - delta;
        F<float>(this, 0x0) = next;
        float maxX = (float)*limit;
        if (maxX < next) {
            F<float>(this, 0x0) = maxX;
        }
        F<int>(this, 0x2c) = x;
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
    return bottom - F<int>(this, 0x8);
}

// ---- replaceTokens_15c858.cpp ----
String NewsTicker::replaceTokens(String text)
{
    String out;
    String_copy_ctor(&out, &text, false);
    return out;
}

// ---- NewsTicker_15c468.cpp ----
struct Array {
    uint32_t size;
    void **data;
    uint32_t capacity;
};

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;





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
    String *tickerText = (String *)((char *)this + 0x14);
    String_ctor(tickerText);
    F<int>(this, 0x4) = x;
    F<int>(this, 0x8) = y;
    F<int>(this, 0x0c) = width;

    String empty;
    String_cstr_ctor(&empty, g_NewsTicker_ctor_empty, false);
    String_assign(tickerText, &empty);
    String_dtor(&empty);
    F<int>(this, 0x10) = 0;

    void *reader = operator new(1);
    FileRead_ctor(reader);
    Array *allItems = FileRead_loadTicker(reader);
    operator delete(FileRead_dtor(reader));

    Array *items = (Array *)operator new(0x0c);
    Array_NewsItem_ctor(items);

    void *random = *g_NewsTicker_ctor_random;
    int wanted = AERandom_nextInt(random, 1) + 2;

    for (uint32_t i = 0; i < allItems->size; ++i) {
        void *item = allItems->data[i];
        int minLevel = F<int>(item, 0x10);
        if (minLevel > 0 && minLevel <= level && level <= F<int>(item, 0x14)) {
            uint8_t *factions = F<uint8_t *>(item, 0x8);
            if (factions[faction] != 0) {
                ArrayAdd_NewsItem(NewsItem_clone(item), items);
            }
        }
    }

    int added = 0;
    int attempts = 0;
    while (added < wanted && attempts < 100) {
        ++attempts;
        if (allItems->size == 0) {
            break;
        }
        void *item = allItems->data[AERandom_nextInt(random, allItems->size)];
        void *system = Status_getSystem(*g_NewsTicker_ctor_status);
        if (SolarSystem_getIndex(system) > 0x15 && F<int>(item, 0x0) == 0x0d) {
            continue;
        }
        if (F<int>(item, 0x14) < 0xa1 || F<int>(item, 0x10) > level) {
            continue;
        }
        if (AERandom_nextInt(random, 100) > 0x31) {
            continue;
        }
        if (F<uint8_t *>(item, 0x8)[faction] == 0 || F<uint8_t>(item, 0x18) != 0) {
            continue;
        }
        if (F<uint8_t>(item, 0x4) != 0) {
            int64_t now = Status_getPlayingTime(*g_NewsTicker_ctor_status);
            int64_t last = F<int64_t>(*g_NewsTicker_ctor_status, 0x160);
            if ((uint32_t)(now - last) <= 0x493e0) {
                continue;
            }
            F<int64_t>(*g_NewsTicker_ctor_status, 0x160) = now;
            F<int>(*g_NewsTicker_ctor_status, 0x174) = F<int>(item, 0x0);
        }
        ArrayAdd_NewsItem(NewsItem_clone(item), items);
        F<uint8_t>(item, 0x18) = 1;
        ++added;
    }

    String separator;
    String_cstr_ctor(&separator, g_NewsTicker_ctor_separator, false);
    for (uint32_t i = 0; i < items->size; ++i) {
        void *item = items->data[i];
        String line;
        String_copy_ctor(&line, GameText_getText(*g_NewsTicker_ctor_text,
                                                 F<int>(item, 0x0) + 0x0cbe),
                         false);
        String replaced = replaceTokens(line);
        String_dtor(&line);

        if (F<uint8_t>(item, 0x4) != 0) {
            String *statusText = (String *)((char *)*g_NewsTicker_ctor_status + 0x168);
            if (F<uint8_t>(item, 0x18) == 0) {
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

    F<int>(this, 0x10) = PaintCanvas_GetTextWidth(*g_NewsTicker_ctor_canvas,
                                                  *g_NewsTicker_ctor_font, tickerText);
    if (F<int>(this, 0x10) < width) {
        String copy;
        String_copy_ctor(&copy, tickerText, false);
        String_plusAssign(tickerText, &copy);
        String_dtor(&copy);
        F<int>(this, 0x10) = PaintCanvas_GetTextWidth(*g_NewsTicker_ctor_canvas,
                                                      *g_NewsTicker_ctor_font, tickerText);
    }

    if (GameText_getLanguage() == 9) {
        width = -F<int>(this, 0x10);
    }
    F<int>(this, 0x2c) = 0;
    F<uint8_t>(this, 0x28) = 0;
    F<float>(this, 0x0) = (float)width;

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
    if (F<int>(this, 0x4) <= x &&
        x <= F<int>(this, 0x4) + F<int>(this, 0x0c) &&
        F<int>(this, 0x8) <= y) {
        int fontHeight = (*g_NewsTicker_touchBegin_font)[4];
        int bottom = *g_NewsTicker_touchBegin_screen;
        bottom += 2;
        __builtin_sub_overflow(bottom, fontHeight, &bottom);
        if (y <= bottom) {
            *(volatile int *)((char *)this + 0x2c) = x;
            *(volatile uint8_t *)((char *)this + 0x28) = 1;
        }
    }
    return F<uint8_t>(this, 0x28);
}
