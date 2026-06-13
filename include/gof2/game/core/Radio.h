#ifndef GOF2_RADIO_H
#define GOF2_RADIO_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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
struct Wanted;

namespace AbyssEngine {

}
using AbyssEngine::String;

#include <new>
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
extern "C" void *operator_new__(uint32_t size);

// Generic byte-offset field accessor (read or write) used by the recovered code.

// String factory helpers (the shared common.h String exposes no engine ctors).
static inline String RadioStringCopy(const String &src) { String r; r.s = src.s; return r; }
static inline String RadioStringFromCstr(const char *c) {
    String r;
    for (const char *p = c; p && *p; ++p) r.s.push_back((char16_t)(unsigned char)*p);
    return r;
}
extern "C" void operator_delete(void *ptr);
extern "C" void operator_delete__(void *ptr);

// Minimal Layout view with the byte offsets Radio reads (full Layout lives in Layout.h,
// which is not part of this batch and is not layout-compatible with common.h's placement-new).
struct Layout {
    uint8_t field_0x0;                  // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    unsigned char _pad_c[0x98 - 0xc];
    int field_0x98;                     // +0x98
    int field_0x9c;                     // +0x9c
    unsigned char _pad_a0[0x2d4 - 0xa0];
    int field_0x2d4;                    // +0x2d4
    int field_0x2d8;                    // +0x2d8

    void setDrawColor(int color);
    void drawBox(int style, int x, int y, int width, int height, String *title);
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

extern "C" void Layout_drawBox(Layout *layout, int style, int x, int y, int width,
                                int height, String *title);

class Radio {
public:
    Array<RadioMessage *>* messages;         // +0x0  (typed message list)
    RadioMessage* currentMessage;            // +0x4
    Array<String *>* textLines;              // +0x8  (wrapped text lines)
    Array<ImagePart *>* imageParts;          // +0xc  (loaded glyph tiles)
    int* imagePartBuffer;                    // +0x10 (raw 5-int part buffer)
    int64_t startTime;                 // +0x18
    int displayDuration;                     // +0x28
    uint8_t lastMessageShownFlag;                 // +0x2c
    uint8_t soundPending;                 // +0x2d
    int soundId;                     // +0x30
    String* font;                 // +0x34
    int boxWidth;                     // +0x38
    int boxX;                     // +0x3c
    int boxY;                     // +0x40

    Radio();
    ~Radio();
    bool isShowingMessage();
    uint8_t lastMessageShown();
    RadioMessage *getMessage(int index);
    void addMessage(int textId);
    void setMessages(Array<RadioMessage *> *messages);
    void update(long time, PlayerEgo *ego, LevelScript *script);
    void draw(int64_t time, PlayerEgo *ego, LevelScript *script);
};
#endif
