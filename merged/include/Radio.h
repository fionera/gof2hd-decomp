#ifndef WORK_CLASSES_RADIO_SRC_CLASS_H
#define WORK_CLASSES_RADIO_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

struct Agent;
struct FModSound;
struct GameText;
struct Globals;
struct ImageFactory;
struct ImagePart;
struct Layout;
struct LevelScript;
struct PaintCanvas;
struct PlayerEgo;
struct Radio;
struct RadioMessage;
struct Vector;
struct Wanted;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

namespace AbyssEngine {
struct String {
    char raw[12];

    String(const char *text, bool copy) noexcept;
    String(const String &other, bool copy) noexcept;
    ~String() noexcept;
};
}
using AbyssEngine::String;

template <class T>
static inline T &F(void *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }
extern "C" void *operator_new__(uint32_t size);
extern "C" void operator_delete(void *ptr);
extern "C" void operator_delete__(void *ptr);

struct Radio {
    Radio();
    ~Radio();

    RadioMessage *getMessage(int index);
    uint8_t lastMessageShown();
    bool isShowingMessage();
    void setMessages(Array<RadioMessage *> *messages);
    void update(long time, PlayerEgo *ego, LevelScript *script);
    void draw(int64_t time, PlayerEgo *ego, LevelScript *script);
};

struct RadioMessage {
    void setRadio(Radio *radio) noexcept;
    int triggered(int64_t time, PlayerEgo *ego, LevelScript *script) noexcept;
    int getImageID() noexcept;
    int getTextID() noexcept;
    void finish() noexcept;
};

struct Wanted {
    String getName() noexcept;
    int *getImageParts() noexcept;
};

struct GameText {
    String *getText(int textId) noexcept;
};

struct PaintCanvas {
    void SetColor(uint32_t color) noexcept;
};

struct Layout {
    void setDrawColor(int color) noexcept;
};

struct ImageFactory {
    Array<ImagePart *> *loadChar(int *parts) noexcept;
    int *createChar(int one, int index) noexcept;
    void drawChar(Array<ImagePart *> *parts, int x, int y, bool flag) noexcept;
};

struct Globals {
    void drawLines(String *font, Array<String *> *lines, int x, int y) noexcept;
    void getLineArray(String *font, String *text, int width, Array<String *> *lines) noexcept;
    int getDialogueSoundId(int textId, Agent *agent) noexcept;
};

struct FModSound {
    void play(int sound, Vector *pos, Vector *vel, float volume) noexcept;
};

struct Agent {
    Agent(int param_1, String *name, int param_3, int param_4, int param_5,
          bool param_6, int param_7, int param_8, int param_9, int param_10) noexcept;
    ~Agent() noexcept;
};

extern "C" void ArrayReleaseClasses_ImagePart(void *array);
extern "C" void *Array_ImagePart_dtor(void *array);
extern "C" void ArrayReleaseClasses_String(void *array);
extern "C" void Array_String_ctor(void *array);
extern "C" void *Array_String_dtor(void *array);
extern "C" void Layout_drawBox(Layout *layout, int style, int x, int y, int width,
                                int height, String *title);

#endif
