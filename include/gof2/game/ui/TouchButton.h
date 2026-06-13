#ifndef GOF2_TOUCHBUTTON_H
#define GOF2_TOUCHBUTTON_H
#include "gof2/common.h"
// TouchButton — top-level on-screen button (NO namespace).
// Members are named/typed from accessor bodies, ctor init, draw() usage and the
// reference binary (android_2.0.16_libgof2hdaa.so). Declared in offset order.

class TouchButton {
public:
    int          field_0x0;        // +0x00  cleared in init()
    int          field_0x4;        // +0x04  cleared in init()
    uint32_t     fontId;           // +0x08  active-font handle / id
    String       text;             // +0x0c  primary label String
    String       splitText;        // +0x18  secondary / value label String (size() peeked at +0x20)
    int          subId;            // +0x24  sub-image / "achStage" id (-1 == none)
    uint32_t     image;            // +0x28  pre-supplied image handle (init case 0x13)
    String       numberText;       // +0x2c  shortcut / corner label String (size() peeked at +0x34)
    int          adornImage;       // +0x38  small adornment image id (-1 == none)
    int          imgFrameTL;       // +0x3c  9-patch frame / pressed image
    int          imgFrameT;        // +0x40  9-patch frame (top / pressed-mid)
    int          imgFrameTR;       // +0x44  9-patch frame (top-right / pressed-right)
    int          imgFrameL;        // +0x48  normal base image / left frame
    int          imgFrameM;        // +0x4c  normal mid frame
    int          imgFrameR;        // +0x50  normal right frame
    int          imgFrameBL;       // +0x54  disabled base image / disabled-left frame
    int          imgFrameB;        // +0x58  disabled mid frame
    int          imgFrameBR;       // +0x5c  disabled right frame
    uint32_t     iconImage;        // +0x60  medal/icon image (kind 4)
    int          iconOverlay;      // +0x64  elite/overlay image (-1 == none)
    int          iconSmall;        // +0x68  small medal overlay image (-1 == none)
    int          requestedWidth;   // +0x6c  caller-requested width (<1 == auto)
    int          kind;             // +0x70  button kind / style selector
    unsigned char flags0;          // +0x74  placement anchor flags
    unsigned char flags1;          // +0x75  text-alignment flags
    int          x;                // +0x78  current x
    int          y;                // +0x7c  current y
    int          initX;            // +0x80  base x passed to init/setPosition
    int          initY;            // +0x84  base y passed to init/setPosition
    int          height;           // +0x88  image height
    int          layoutHeight;     // +0x8c  layout/hit height
    int          width;            // +0x90  total width
    int          leftWidth;        // +0x94  left frame width
    int          midWidth;         // +0x98  mid frame width
    int          rightWidth;       // +0x9c  right frame width
    int          midStretch;       // +0xa0  middle stretch width
    int          textOffsetX;      // +0xa4  label x offset
    int          textOffsetY;      // +0xa8  label y offset
    int          textColor;        // +0xac  label colour (ARGB, -1 == default)
    unsigned char touched;         // +0xb0  currently pressed
    unsigned char alwaysPressed;   // +0xb1  forced-pressed flag
    unsigned char visible;         // +0xb2  visible flag
    unsigned char halfTransparent; // +0xb3  disabled/half-transparent flag
    uint32_t     iconTexId;        // +0xb4  icon texture id (kind 4)
    unsigned char progressHighlight; // +0xb8  press-progress highlight flag
    float        pressProgress;    // +0xbc  press-progress fill (0..1)
    int          touchMargin;      // +0xc0  hit-test margin (from layout metrics)
    int          fontSpacing;      // +0xc4  cached font kerning/spacing

    // ---- methods (converted from free functions) ----
    bool OnTouchBegin(int px, int py);
    unsigned int OnTouchEnd(int px, int py);
    unsigned int OnTouchMove(int px, int py);
    // ---- constructors / destructor (demangle to TouchButton::TouchButton / ~TouchButton) ----
    // The three embedded label Strings (text/splitText/numberText) are real members:
    // they auto-construct/destruct, so no placement ctor/dtor is needed.
    TouchButton(String *text, int type, int x, int y, int width, int icon, int style);
    TouchButton(String *text, int x, int y, int p4, unsigned char p5);
    TouchButton(int x, int y, String *text, int p4, int p5, unsigned char p6);
    TouchButton(String *text, int type, int x, int y, int p5, unsigned char p6, unsigned char p7);
    ~TouchButton();
    void setPosition3(int x, int y, int align);
    unsigned int touch_end(int x, int y);
    void draw();
    int getHeight();
    String getText();
    int getWidth();
    int init(String *text, unsigned int kind, int achId, int achStage, int width, int d_unused, int x, int y, unsigned char flags0, unsigned char flags1);
    uint8_t isTouched();
    uint8_t isVisible();
    void replaceTextKeepSize(String *text);
    void resetTouch();
    void setAlwaysPressed(bool value);
    void setHalfTransparent(bool value);
    void setNumberText(String *value);
    void setPosition(int x, int y, unsigned char flags);
    void setPosition2(int x, int y);
    void setPressProgress(float value);
    void setPressProgressHighlight(bool value);
    void setSplitText(String *value);
    void setText(String *text);
    void setTextColor(int color);
    void setVisible(bool value);
    void setYPosition(int y);
    bool touchedInside(int px, int py);
    void translate(int dx, int dy);
};
#endif
