#include "class.h"

extern "C" void _liw_String_ctor(void *s);
extern "C" void _liw_Matrix_ctor(void *m);
extern "C" int  _liw_GetTextHeight(void *canvas);

// Two PC-relative singletons (each holds the address of a pointer).
__attribute__((visibility("hidden"))) extern void ***g_liw_a;
__attribute__((visibility("hidden"))) extern void ***g_liw_b;

struct Vec4 { int a, b, c, d; };

// ListItemWindow::ListItemWindow()
struct ListItemWindow { ListItemWindow(); };
ListItemWindow::ListItemWindow()
{
    _liw_String_ctor((char *)this + 0x74);
    _liw_String_ctor((char *)this + 0x80);
    _liw_Matrix_ctor((char *)this + 0x98);
    _liw_Matrix_ctor((char *)this + 0xd4);

    void **a = *g_liw_a;
    void **b = *g_liw_b;
    *(Vec4 *)this = (Vec4){0, 0, 0, 0};
    void *av = *a;
    void *canvas = *b;
    pp(this, 0x18) = 0;
    (void)av;
    int h = _liw_GetTextHeight(canvas);
    i32(this, 0x1c) = h / 2 - 1;
    i32(this, 0x20) = 0;
}
