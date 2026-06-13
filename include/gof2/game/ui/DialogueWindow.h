#ifndef GOF2_DIALOGUEWINDOW_H
#define GOF2_DIALOGUEWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// String and Vector come from common.h / math.h (do not re-declare here).
// Mission/Level/Agent are forward-declared in fwd.h (included via common.h).

struct StringSlot {
    char bytes[12];
};

class DialogueWindow {
public:
    void* prevButton;                    // +0x0
    void* nextButton;                    // +0x4
    void* moreButton;                    // +0x8
    void* faceParts;                    // +0xc
    int campaignMission;                     // +0x10
    int frameX;                     // +0x14
    int frameY;                     // +0x18
    int frameWidth;                     // +0x1c
    int frameHeight;                     // +0x20
    void* clientImage;                   // +0x24
    void* scrollWindow;                   // +0x40
    int kind;                     // +0x44
    int page;                     // +0x48
    Mission* mission;                // +0x4c
    void* choiceWindow;                   // +0x50
    uint8_t choiceActive;                 // +0x54
    void* level;                   // +0x58
    int*  briefingOffsets;                   // +0x5c  campaign briefing offset table
    int*  successOffsets;                   // +0x60  campaign success offset table
    int voiceSound;                     // +0x64
    int autoAdvanceTimer;                     // +0x68
    int pauseLength;                     // +0x6c
    uint8_t field_0x70;                 // +0x70

    // ---- methods (converted from free functions) ----
    int OnTouchBegin(int x, int y);
    int OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    ~DialogueWindow();
    DialogueWindow * ctor_default();
    DialogueWindow * ctor_mission(Mission *mission, Level *level, int kind);
    DialogueWindow * ctor_text(String *text, String *agentName, int *parts);
    void draw();
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
