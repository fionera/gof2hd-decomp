#include "class.h"

extern "C" void  _liw_ArrayReleaseClasses(void *a);   // 0x6facc
extern "C" void  _liw_ArrayRelease(void *a);          // 0x7369c
extern "C" void *_liw_StringArray_dtor(void *a);      // 0x6f64c
extern "C" void *_liw_IntArray_dtor(void *a);         // 0x70204
extern "C" void *_liw_STW_dtor(void *w);              // 0x75afc
extern "C" void  _liw_String_dtor(void *s);           // 0x6ee30

// ListItemWindow::~ListItemWindow()
struct ListItemWindow { ~ListItemWindow(); };
ListItemWindow::~ListItemWindow()
{
    if (pp(this, 0x0)) {
        _liw_ArrayReleaseClasses(pp(this, 0x0));
        if (pp(this, 0x0)) operator delete(_liw_StringArray_dtor(pp(this, 0x0)));
        pp(this, 0x0) = 0;
    }
    if (pp(this, 0x4)) {
        _liw_ArrayReleaseClasses(pp(this, 0x4));
        if (pp(this, 0x4)) operator delete(_liw_StringArray_dtor(pp(this, 0x4)));
        pp(this, 0x4) = 0;
    }
    if (pp(this, 0x8)) {
        _liw_ArrayRelease(pp(this, 0x8));
        if (pp(this, 0x8)) operator delete(_liw_IntArray_dtor(pp(this, 0x8)));
        pp(this, 0x8) = 0;
    }
    if (pp(this, 0xc)) {
        _liw_ArrayRelease(pp(this, 0xc));
        if (pp(this, 0xc)) operator delete(_liw_IntArray_dtor(pp(this, 0xc)));
        pp(this, 0xc) = 0;
    }
    if (pp(this, 0x18)) operator delete(_liw_STW_dtor(pp(this, 0x18)));
    pp(this, 0x18) = 0;
    _liw_String_dtor((char *)this + 0x80);
    _liw_String_dtor((char *)this + 0x74);
}
