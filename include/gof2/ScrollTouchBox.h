#ifndef GOF2_SCROLLTOUCHBOX_H
#define GOF2_SCROLLTOUCHBOX_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
//
// ScrollTouchBox — a scrollable, touch-draggable text box.
//   +0x00  Array<String*>*  line array (std::vector<String*>*)
//   +0x04  int  x            +0x08 int  y
//   +0x0c  int  width        +0x10 int  height
//   +0x14  int  textWidth    +0x18 int  contentHeight
//   +0x1c  int  lastDelta    +0x20 float damping  +0x24 float velocity
//   +0x28  int  touchStartY  +0x2c int  lastTouchY
//   +0x30  uint8 dragging    +0x34 int  scrollOffset
//   +0x38  uint8 centered    +0x3c String* font

class ScrollTouchBox {
public:
    Array<String*>* lines;          // +0x0  line array
    int x;                      // +0x4
    int y;                      // +0x8
    int width;                      // +0xc
    int height;                     // +0x10
    int textWidth;                     // +0x14
    int contentHeight;                     // +0x18
    int lastDelta;                     // +0x1c
    float damping;                   // +0x20
    float velocity;                   // +0x24
    int touchStartY;                     // +0x28
    int lastTouchY;                     // +0x2c
    uint8_t dragging;                 // +0x30
    int scrollOffset;                     // +0x34
    uint8_t centered;                 // +0x38
    String* font;                 // +0x3c

    ScrollTouchBox(int x, int y, int width, int height);
    ~ScrollTouchBox();
    void setText(AbyssEngine::String text);
    void setText(AbyssEngine::String text, int font);
    void setTextCentered(bool centered);
    void setPosition(int x, int y);
    void setYPosition(int y);
    void update(int dt);
    void draw();
    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    bool touchIsInside(int x, int y);
    float getRelativeScrollStartPos();
    float getRelativeScrollHeight();
};
#endif
