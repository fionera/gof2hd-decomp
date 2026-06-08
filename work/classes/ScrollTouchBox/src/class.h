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
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    int f_10; // 0x10
    int f_14; // 0x14
    int f_18; // 0x18
    int f_1c; // 0x1c
    int f_20; // 0x20
    int f_24; // 0x24
    int f_28; // 0x28
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    int f_38; // 0x38
    void* f_3c; // 0x3c
    unsigned char _pad_40[8];
    int f_48; // 0x48

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
