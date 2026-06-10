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

struct ScrollTouchBox {
    Array<String*>* field_0x0;          // +0x0  line array
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    int field_0x1c;                     // +0x1c
    float field_0x20;                   // +0x20
    float field_0x24;                   // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    uint8_t field_0x30;                 // +0x30
    int field_0x34;                     // +0x34
    uint8_t field_0x38;                 // +0x38
    String* field_0x3c;                 // +0x3c

    ScrollTouchBox(int x, int y, int width, int height);
    ~ScrollTouchBox();
    void setText(AbyssEngine::String text);
    void setText(AbyssEngine::String text, int font);
    void setTextCentered(bool centered);
    void setPosition(int x, int y);
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
