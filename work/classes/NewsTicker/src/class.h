#ifndef WORK_CLASSES_NEWSTICKER_SRC_CLASS_H
#define WORK_CLASSES_NEWSTICKER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef long long int64_t;

namespace AbyssEngine {
struct String;
}
typedef AbyssEngine::String String;

struct NewsTicker {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    void* f_8; // 0x8
    int f_c; // 0xc
    int f_10; // 0x10
    int f_14; // 0x14
    int f_18; // 0x18
    unsigned char _pad_1c[12];
    int f_28; // 0x28
    int f_2c; // 0x2c

    NewsTicker(int x, int y, int width, int faction, int level);
    ~NewsTicker();

    int getHeight();
    void update(int dt);
    void draw();
    uint8_t OnTouchBegin(int x, int y);
    bool OnTouchMove(int x, int y);
    uint32_t OnTouchEnd(int x, int y);
    String replaceTokens(String text);
};

namespace AbyssEngine {
struct String {
    uint32_t a;
    uint32_t b;
    uint32_t c;

    String();
    String(const char *text, bool copy);
    String(const String &other, bool copy);
    String(int value);
    ~String();
    String &operator=(const String &other);
    String &operator+=(const String &other);
    int IndexOf(const String &needle);
    void SubString(String *out, int start, int end);
};

String operator+(const String &left, const String &right);
}

template <class T>
static inline T &F(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

#endif
