#ifndef GOF2_SCROLLTOUCHWINDOW_H
#define GOF2_SCROLLTOUCHWINDOW_H
#include "gof2/common.h"
// ScrollTouchWindow — a framed/plain window that hosts a ScrollTouchBox and draws a
// title + scroll bar. Field layout recovered from byte-offset accesses in the merged
// source (offsets shown in comments). The underlying ScrollTouchBox is held opaque.

class ScrollTouchWindow {
public:
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
    // Touch-release handler exposed under the engine's "touch_end" name; forwards to OnTouchEnd.
    void touch_end(int x, int y);
    void setTextCentered(bool centered);
    void setYPosition(int y);
    // Window-level reposition (engine-name alias used by ChoiceWindow); forwards
    // the new top Y to the hosted ScrollTouchBox.
    void setPosition(int y);
    void update(int dt);
    // Keyboard / D-pad scroll: nudge the hosted ScrollTouchBox by `amount` steps
    // (positive scrolls the content down, negative up); the box settles via update().
    void scroll(int amount);
    void draw();
    void drawTextBG();
    void setText(AbyssEngine::String title, AbyssEngine::String text);
    void setText(AbyssEngine::String title, AbyssEngine::String text, int color);
    // Title+coloured-text setter exposed under the engine's "setText4" name; forwards to setText.
    void setText4(AbyssEngine::String title, AbyssEngine::String text, int color);
};
#endif
