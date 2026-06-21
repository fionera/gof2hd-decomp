#ifndef GOF2_CHOICEWINDOW_H
#define GOF2_CHOICEWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// ChoiceWindow -- top-level modal choice/medal dialog. Hosts a scrollable message
// (ScrollTouchWindow) plus up to three TouchButtons and an optional medal graphic.

class TouchButton;
class ScrollTouchWindow;

class ChoiceWindow {
public:
    int x;
    int y;
    int width;
    int height;
    TouchButton* leftButton;
    TouchButton* rightButton;
    TouchButton* miscButton;
    ScrollTouchWindow* scrollWindow;
    String title;
    int count;
    int medal;
    int medalImage;          // -1 == none
    int medalBgImage;        // -1 == none
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

    // Retitles the two choice buttons in place. The shipped code combines this
    // with the set() overloads when it opens an agent-chat dialog; it is kept as
    // a standalone helper because it touches the private TouchButton labels.
    void setButtonText(String const &left, String const &right);
};
#endif
