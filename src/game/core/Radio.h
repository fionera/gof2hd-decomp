#ifndef GOF2_RADIO_H
#define GOF2_RADIO_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/render/ImagePart.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/LevelScript.h"

class ImagePart;
class LevelScript;
class PlayerEgo;
class RadioMessage;

class Radio {
public:
    Array<RadioMessage *> *messages;      // 0x00
    RadioMessage *currentMessage;         // 0x04
    Array<String *> *rows;                // 0x08
    Array<ImagePart *> *faceImages;       // 0x0c
    int *face;                            // 0x10
    int field_0x14;                       // 0x14
    int64_t msgStartTime;                 // 0x18
    int64_t msgCheckTick;                 // 0x20 (never touched by v2)
    int msgTime;                          // 0x28
    bool lastMessageWasShown;             // 0x2c
    bool playSound;                       // 0x2d
    int soundId;                          // 0x30
    unsigned int font;                    // 0x34
    int boxWidth;                         // 0x38
    int boxX;                             // 0x3c
    int boxY;                             // 0x40
    int field_0x44;                       // 0x44

    Radio();

    ~Radio();

    bool isShowingMessage();

    uint8_t lastMessageShown();

    RadioMessage *getMessage(int index);

    void setCurrentMessage(RadioMessage *message);

    void setMessages(Array<RadioMessage *> *messages);

    void update(long time, PlayerEgo *ego, LevelScript *script);

    void draw(int64_t time, PlayerEgo *ego, LevelScript *script);
};
#endif
