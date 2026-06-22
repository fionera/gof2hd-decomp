#ifndef GOF2_CHOICEWINDOW_H
#define GOF2_CHOICEWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class TouchButton;
class ScrollTouchWindow;

// Draw-time button-position cache record. ChoiceWindow::draw() stashes the
// on-screen pixel coordinates of its choice buttons into a small int-record
// pointed to by the g_ChoiceWindow_posTarget{A,B,C,D} globals so other code can
// read where the buttons ended up. The record stores two int32 coordinate
// slots: one at +0x8 and one at +0xc. The same physical slot carries different
// meaning depending on which posTarget global points at the record, so the
// per-call-site names are exposed as a union over the same memory.
struct ChoiceWindowButtonPosCache {
    void *field_0x0;        // +0x0 opaque
    int field_0x4;          // +0x4 opaque
    union {                 // +0x8 int32 coordinate slot
        int field_0x8;
        int rightButtonPosX;    // posTargetA->[+0x8]
        int rightButtonPosY;    // posTargetB->[+0x8]
        int singleButtonPosX;   // posTargetC->[+0x8] (single-button layout)
        int singleButtonPosY;   // posTargetD->[+0x8] (single-button layout)
    };
    union {                 // +0xc int32 coordinate slot
        int field_0xc;
        int leftButtonPosX;     // posTargetC->[+0xc]
        int leftButtonPosY;     // posTargetD->[+0xc]
    };
};

class ChoiceWindow {
public:
    int x;
    int y;
    int width;
    int height;
    TouchButton *leftButton;
    TouchButton *rightButton;
    TouchButton *miscButton;
    ScrollTouchWindow *scrollWindow;
    String title;
    int count;
    int medal;
    int medalImage; // -1 == none
    int medalBgImage; // -1 == none
    String medalText;
    int padding;
    int padding2;
    int padding3;
    int padding4;
    uint8_t hasButtons;
    uint8_t buttonsVisible;

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

    void left();

    void right();

    void update(int dt);

    void draw();

    int OnTouchBegin(int x, int y);

    int OnTouchMove(int x, int y);

    int OnTouchEnd(int x, int y);

    // Fire callback for the left choice button; the shipped build records the
    // press through hasChoice()/the button state, so this does no work itself.
    int fire();
};
#endif
