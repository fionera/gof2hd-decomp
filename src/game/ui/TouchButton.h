#ifndef GOF2_TOUCHBUTTON_H
#define GOF2_TOUCHBUTTON_H
#include "game/core/String.h"
#include "mathtypes.h"

// TouchButton — top-level on-screen button (global namespace).
// A label string plus a skin built from 2D images (normal / pressed / disabled,
// optionally a 9-patch frame), with touch hit-testing and an optional press-progress
// fill. The button kind selects which images and layout rules apply.
class TouchButton {
public:
    int           field_0x0;          // cleared in init()
    int           field_0x4;          // cleared in init()
    uint32_t      fontId;             // active-font handle / id
    String        text;               // primary label
    String        splitText;          // secondary / value label
    int           subId;              // sub-image / achievement-stage id (-1 == none)
    uint32_t      image;              // pre-supplied image handle (init case 0x13)
    String        numberText;         // shortcut / corner label
    int           adornImage;         // small adornment image id (-1 == none)
    int           imgFrameTL;         // 9-patch frame / pressed image
    int           imgFrameT;          // 9-patch frame (top / pressed-mid)
    int           imgFrameTR;         // 9-patch frame (top-right / pressed-right)
    int           imgFrameL;          // normal base image / left frame
    int           imgFrameM;          // normal mid frame
    int           imgFrameR;          // normal right frame
    int           imgFrameBL;         // disabled base image / disabled-left frame
    int           imgFrameB;          // disabled mid frame
    int           imgFrameBR;         // disabled right frame
    uint32_t      iconImage;          // medal/icon image (kind 4)
    int           iconOverlay;        // elite/overlay image (-1 == none)
    int           iconSmall;          // small medal overlay image (-1 == none)
    int           requestedWidth;     // caller-requested width (<1 == auto)
    int           kind;               // button kind / style selector
    unsigned char flags0;             // placement anchor flags
    unsigned char flags1;             // text-alignment flags
    int           x;                  // current x
    int           y;                  // current y
    int           initX;              // base x passed to init/setPosition
    int           initY;              // base y passed to init/setPosition
    int           height;             // image height
    int           layoutHeight;       // layout/hit height
    int           width;              // total width
    int           leftWidth;          // left frame width
    int           midWidth;           // mid frame width
    int           rightWidth;         // right frame width
    int           midStretch;         // middle stretch width
    int           textOffsetX;        // label x offset
    int           textOffsetY;        // label y offset
    int           textColor;          // label colour (ARGB, -1 == default)
    unsigned char touched;            // currently pressed
    unsigned char alwaysPressed;      // forced-pressed flag
    unsigned char visible;            // visible flag
    unsigned char halfTransparent;    // disabled/half-transparent flag
    uint32_t      iconTexId;          // icon texture id (kind 4)
    unsigned char progressHighlight;  // press-progress highlight flag
    float         pressProgress;      // press-progress fill (0..1)
    int           touchMargin;        // hit-test margin (from layout metrics)
    int           fontSpacing;        // cached font kerning/spacing

    TouchButton(String *text, int type, int x, int y, int width, int icon, int style);
    TouchButton(String const &text, int x, int y, int p4, unsigned char p5);
    TouchButton(int x, int y, String const &text, int p4, int p5, unsigned char p6);
    TouchButton(String const &text, int type, int x, int y, int p5, unsigned char p6, unsigned char p7);
    TouchButton(String const &text, int a, int b, int c, int d, unsigned char flags0, unsigned char flags1, unsigned int font, int kerning);
    TouchButton(unsigned int kind, int a, int b, int c, unsigned char flag);
    TouchButton(unsigned int kind, int a, int b, int c, int d, unsigned char flags0, unsigned char flags1);
    TouchButton(unsigned int kind, unsigned int image, int a, int b, int c, unsigned char flag);
    ~TouchButton();

    bool         OnTouchBegin(int px, int py);
    unsigned int OnTouchEnd(int px, int py);
    unsigned int OnTouchMove(int px, int py);
    void         draw();
    int          getHeight();
    Vector       getPosition();
    String       getText();
    int          getWidth();
    int          init(String const &text, unsigned int kind, int achId, int achStage, int width, int d_unused, int x, int y, unsigned char flags0, unsigned char flags1);
    uint8_t      isTouched();
    uint8_t      isVisible();
    void         replaceTextKeepSize(String const &text);
    void         resetTouch();
    void         setAlwaysPressed(bool value);
    void         setHalfTransparent(bool value);
    void         setNumberText(String const &value);
    void         setGamePadButtonImage(unsigned int image);
    void         setPosition(int x, int y, unsigned char flags);
    void         setPosition(int x, int y);
    void         setPressProgress(float value);
    void         setPressProgressHighlight(bool value);
    void         setSplitText(String const &value);
    void         setText(String const &text);
    void         setTextColor(int color);
    void         setVisible(bool value);
    void         setYPosition(int y);
    bool         touchedInside(int px, int py);
    void         translate(int dx, int dy);
};
#endif
