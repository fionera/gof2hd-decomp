#ifndef GOF2_DIALOGUEWINDOW_H
#define GOF2_DIALOGUEWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Mission;
class Level;
class Agent;
class ImagePart;
class TouchButton;
class ChoiceWindow;
class ScrollTouchWindow;

// File-static German fallback-text tables (g_dw_germanSuccessTexts /
// g_dw_germanOtherTexts / g_dw_germanBriefingTexts). A flat int[] table read two
// ways: male agents index a per-race row block starting at offset 0
// (base + race*8 + variant*4, two int variants per race), female agents read a
// shared two-int variant block at offset 0x48 (base + 0x48 + variant*4). Both
// fields are the int element at the start of their respective region; callers
// add the race*8/variant*4 index past it.
struct DialogueWindowGermanTextTable {
    int maleRaceRow;       // +0x00 start of the male per-race row block (race*8 + variant*4 indexed)
    int field_0x4[17];     // remainder of the male per-race rows
    int femaleVariantBase; // +0x48 start of the shared female two-int variant block (variant*4 indexed)
    int field_0x4c[1];     // second female variant slot
};

class DialogueWindow {
public:
    TouchButton *prevButton;
    TouchButton *nextButton;
    TouchButton *moreButton;
    Array<ImagePart *> *faceParts; // face image-part list (loadChar result)
    int campaignMission;
    int frameX;
    int frameY;
    int frameWidth;
    int frameHeight;
    void *clientImage;
    String bodyText; // dialogue body text
    String agentName; // agent / client name (title text)
    ScrollTouchWindow *scrollWindow;
    int kind;
    int page;
    Mission *mission;
    ChoiceWindow *choiceWindow;
    uint8_t choiceActive;
    Level *level;
    int *briefingOffsets; // campaign briefing offset table
    int *successOffsets; // campaign success offset table
    int voiceSound;
    int autoAdvanceTimer;
    int pauseLength;
    uint8_t mirrorFace; // draw the portrait mirrored

    DialogueWindow();

    DialogueWindow(Mission *mission, Level *level, int kind);

    DialogueWindow(String *text, String *agentName, int *parts);

    ~DialogueWindow();

    int OnTouchBegin(int x, int y);

    int OnTouchEnd(int x, int y);

    int OnTouchMove(int x, int y);

    void draw();

    int getMode();

    bool hasLevel();

    int init();

    bool isFirstPage();

    bool isLastPage();

    int length();

    void loadContent();

    int nextPage();

    int pickGermanGenericTextBecauseWeSaved100EurosWithThat(int kind, Agent *agent);

    int previousPage();

    void set(Mission *mission, int kind, int campaign);

    void setLevel(Level *level);

    void update(int dt);

    // Static campaign-dialogue lookups: do the offset tables list a briefing /
    // success dialogue for this campaign-mission id?  (id capped at 0xa1.)
    static bool hasBriefingDialogue(int id);

    static bool hasSuccessDialogue(int id);
};
#endif
