#include "class.h"

struct Array {
    uint32_t size;
    void **data;
    uint32_t capacity;
};

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;

extern "C" void String_ctor(void *self);
extern "C" void String_cstr_ctor(void *self, const char *text, bool copy);
extern "C" void String_copy_ctor(void *self, void *other, bool copy);
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
extern "C" int GameText_getLanguage();
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *font, String *text);

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
