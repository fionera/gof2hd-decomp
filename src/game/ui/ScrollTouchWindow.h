#ifndef GOF2_SCROLLTOUCHWINDOW_H
#define GOF2_SCROLLTOUCHWINDOW_H
#include "common.h"   // AbyssEngine::String

class ScrollTouchBox;

// ScrollTouchWindow — a framed or plain window that hosts a ScrollTouchBox and draws a
// title plus a scroll bar.
class ScrollTouchWindow {
public:
    ScrollTouchBox* scrollBox;   // the scrollable content box
    String          title;       // title text
    uint8_t         touchActive; // touch-active flag (set by OnTouchMove/End)
    uint8_t         hasFrame;    // whether the window draws a frame
    int             x;
    int             y;
    int             width;
    int             height;

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
