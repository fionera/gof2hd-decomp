#ifndef GOF2_SCROLLTOUCHWINDOW_H
#define GOF2_SCROLLTOUCHWINDOW_H
#include "gof2/common.h"
// ScrollTouchWindow — a framed/plain window that hosts a ScrollTouchBox and draws a
// title + scroll bar. Field layout recovered from byte-offset accesses in the merged
// source (offsets shown in comments). The underlying ScrollTouchBox is held opaque.

struct ScrollTouchWindow {
    void*  scrollBox;        // +0x00  ScrollTouchBox* (the scrollable content box)
    String title;        // +0x04  title text
    unsigned char touchActive;// +0x10  touch-active flag (set by OnTouchMove/End)
    unsigned char hasFrame;// +0x11  hasFrame flag
    int    x;       // +0x14  x
    int    y;       // +0x18  y
    int    width;       // +0x1c  width
    int    height;       // +0x20  height

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
