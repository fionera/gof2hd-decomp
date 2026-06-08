#include "class.h"

void *operator new(unsigned int size);
void operator delete(void *ptr) noexcept;

extern "C" void ArrayReleaseClasses_StringPtr(void *self);
extern "C" void *Array_StringPtr_dtor(void *self);
extern "C" void Array_StringPtr_ctor(void *self);
extern "C" void Globals_getLineArray(void *self, int font, String *text, int lineWidth, void *array);
extern "C" void String_ctor_cstr(void *self, char const *text, bool copy);
extern "C" void ArrayAdd_StringPtr(String *value, void *array);

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
