#ifndef GOF2_SCROLLTOUCHWINDOW_H
#define GOF2_SCROLLTOUCHWINDOW_H
#include "gof2/common.h"
// ScrollTouchWindow — a framed/plain window that hosts a ScrollTouchBox and draws a
// title + scroll bar. Field layout recovered from byte-offset accesses in the merged
// source (offsets shown in comments). The underlying ScrollTouchBox is held opaque.

struct ScrollTouchWindow {
    void*  field_0x0;        // +0x00  ScrollTouchBox* (the scrollable content box)
    String field_0x4;        // +0x04  title text
    unsigned char field_0x10;// +0x10  touch-active flag (set by OnTouchMove/End)
    unsigned char field_0x11;// +0x11  hasFrame flag
    int    field_0x14;       // +0x14  x
    int    field_0x18;       // +0x18  y
    int    field_0x1c;       // +0x1c  width
    int    field_0x20;       // +0x20  height

    ScrollTouchWindow(int x, int y, int w, int h);
    ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame);
    ~ScrollTouchWindow();

    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    void setTextCentered(bool centered);
    void setYPosition(int y);
    void update(int dt);
    void draw();
    void drawTextBG();
    void setText(AbyssEngine::String title, AbyssEngine::String text);
    void setText(AbyssEngine::String title, AbyssEngine::String text, int color);
};
#endif
