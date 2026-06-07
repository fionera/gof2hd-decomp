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
