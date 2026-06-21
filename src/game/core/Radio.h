#ifndef GOF2_RADIO_H
#define GOF2_RADIO_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class RadioMessage;
class ImagePart;
class PlayerEgo;
class LevelScript;

// In-game radio overlay: owns the scripted RadioMessage list, evaluates their
// trigger conditions every frame and renders the currently active message
// (speaker portrait, wrapped dialogue text and an optional voice sample).
class Radio {
public:
    Array<RadioMessage*>* messages;       // scripted message list
    RadioMessage* currentMessage;         // message currently being shown
    Array<String*>* textLines;            // wrapped lines of the active message
    Array<ImagePart*>* imageParts;        // loaded portrait glyph tiles
    int* imagePartBuffer;                 // raw 5-int portrait descriptor
    int64_t startTime;                    // time the active message appeared
    int displayDuration;                  // how long the message stays on screen
    uint8_t lastMessageShownFlag;         // last scripted message has been shown
    uint8_t soundPending;                 // voice sample still needs to be played
    int soundId;                          // resolved dialogue sound id (-1 = none)
    String* font;                         // font used for the active message
    int boxWidth;                         // overlay box width
    int boxX;                             // overlay box left edge
    int boxY;                             // overlay box top edge

    Radio();
    ~Radio();

    bool isShowingMessage();
    uint8_t lastMessageShown();
    RadioMessage* getMessage(int index);
    void setCurrentMessage(RadioMessage* message);
    void setMessages(Array<RadioMessage*>* messages);
    void update(long time, PlayerEgo* ego, LevelScript* script);
    void draw(int64_t time, PlayerEgo* ego, LevelScript* script);
};
#endif
