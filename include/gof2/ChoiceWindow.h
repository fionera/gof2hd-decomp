#ifndef GOF2_CHOICEWINDOW_H
#define GOF2_CHOICEWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// ChoiceWindow -- top-level modal choice/medal window (uses TouchButton + ScrollTouchWindow).
// Layout recovered from Ghidra (ctor @0x1567ec etc.).

struct TouchButton;
struct ScrollTouchWindow;

// Byte-offset accessor template, used for reads into objects whose full layout is not
// modelled in this batch (config blobs, medal image tables, foreign globals).
template <class T> static inline T &F(void *p, uint32_t off) { return *(T *)((char *)p + off); }

struct ChoiceWindow {
    int field_0x0;                      // +0x0   x
    int field_0x4;                      // +0x4   y
    int field_0x8;                      // +0x8   width
    int field_0xc;                      // +0xc   height
    void* field_0x10;                   // +0x10  TouchButton* (left)
    void* field_0x14;                   // +0x14  TouchButton* (right)
    void* field_0x18;                   // +0x18  TouchButton* (misc)
    void* field_0x1c;                   // +0x1c  ScrollTouchWindow*
    String field_0x20;                  // +0x20  title String (12 bytes -> 0x2c)
    int field_0x2c;                     // +0x2c  count
    int field_0x30;                     // +0x30  medal
    int field_0x34;                     // +0x34  medal image handle (-1 = none)
    int field_0x38;                     // +0x38  medal-bg image handle (-1 = none)
    String field_0x3c;                  // +0x3c  secondary text String (12 bytes -> 0x48)
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    int field_0x50;                     // +0x50
    int field_0x54;                     // +0x54
    uint8_t field_0x58;                 // +0x58  hasButtons flag
    uint8_t field_0x59;                 // +0x59  buttons-visible flag

    ChoiceWindow();
    ~ChoiceWindow();

    void set(String const &text);
    void set(String const &text, bool flag);
    void set(String const &a, String const &b);
    void set(String const &title, String const &message, bool flag);
    void set(String const &title, String const &message, bool hasButtons,
             String const &left, String const &right, String const &single,
             int y, int width);

    void setWidth(int width);
    void setHeight(int height);
    void setMedal(int medal, int count);
    void setMiscButton(String const &text);
    void removeButtons();
    uint8_t hasChoice();

    void update(int dt);
    void draw();

    int OnTouchBegin(int x, int y);
    int OnTouchMove(int x, int y);
    int OnTouchEnd(int x, int y);
};
#endif
