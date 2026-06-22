#ifndef GOF2_SCROLLTOUCHWINDOW_H
#define GOF2_SCROLLTOUCHWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class ScrollTouchBox;

class ScrollTouchWindow {
public:
    ScrollTouchBox *scrollBox; // the scrollable content box
    String title; // title text
    uint8_t touchActive; // touch-active flag (set by OnTouchMove/End)
    uint8_t hasFrame; // whether the window draws a frame
    int x;
    int y;
    int width;
    int height;

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
