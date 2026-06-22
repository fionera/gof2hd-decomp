#ifndef GOF2_SCROLLTOUCHBOX_H
#define GOF2_SCROLLTOUCHBOX_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class ScrollTouchBox {
public:
    Array<String *> *lines; // wrapped lines of laid-out text
    int x;
    int y;
    int width;
    int height;
    int textWidth;
    int contentHeight;
    int lastDelta;
    float damping;
    float velocity;
    int touchStartY;
    int lastTouchY;
    uint8_t dragging;
    int scrollOffset;
    uint8_t centered;
    String *font;

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
