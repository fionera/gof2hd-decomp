#ifndef GOF2_TOUCHBUTTON_H
#define GOF2_TOUCHBUTTON_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// TouchButton — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.

struct TouchButton;    // opaque; we only ever take a TouchButton* and offset-cast.

// 12-byte return-by-value String aggregate (matches other gof2 getters).

// ---- tiny offset-cast helpers -------------------------------------------------
#ifndef GOF2_BIP_HELPERS
#define GOF2_BIP_HELPERS
static inline char*           B (void* p, int off) { return (char*)p + off; }
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }
#endif

class TouchButton {
public: void* _opaque; 
    // ---- methods (converted from free functions) ----
    bool OnTouchBegin(int px, int py);
    unsigned int OnTouchEnd(int px, int py);
    unsigned int OnTouchMove(int px, int py);
    TouchButton * ctor(String *text, int type, int x, int y, int width, int icon, int style);
    TouchButton * ctor8(String *text, int type, int x, int y, int width, int icon, int mode);
    TouchButton * ctor5(String *text, int x, int y, int p4, unsigned char p5);
    TouchButton * ctor6(int x, int y, String *text, int p4, int p5, unsigned char p6);
    TouchButton * ctor7(String *text, int type, int x, int y, int p5, unsigned char p6, unsigned char p7);
    void setPosition3(int x, int y, int align);
    unsigned int touch_end(int x, int y);
    void draw();
    void dtor();
    int getHeight();
    RetStr getText();
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
};  // no offset accesses observed
#endif
