#ifndef WORK_CLASSES_SCROLLTOUCHBOX_SRC_CLASS_H
#define WORK_CLASSES_SCROLLTOUCHBOX_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {
struct String {
    String(String const &);
};
}

typedef AbyssEngine::String String;
struct Array;

struct ScrollTouchBox {
    ScrollTouchBox(int x, int y, int width, int height);
    ~ScrollTouchBox();

    void setText(AbyssEngine::String text, int font);
    void setText(AbyssEngine::String text);
    void setPosition(int x, int y);
    void draw();
    void setTextCentered(bool centered);
    float getRelativeScrollStartPos();
    float getRelativeScrollHeight();
    void update(int dt);
    bool touchIsInside(int x, int y);
    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
};

template <class T>
static inline T &F(void *p, unsigned off)
{
    return *(T *)((char *)p + off);
}

#endif
