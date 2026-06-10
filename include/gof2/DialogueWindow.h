#ifndef GOF2_DIALOGUEWINDOW_H
#define GOF2_DIALOGUEWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// String and Vector come from common.h / math.h (do not re-declare here).
// Mission/Level/Agent are forward-declared in fwd.h (included via common.h).

struct StringSlot {
    char bytes[12];
};

struct DialogueWindow {
    void* field_0x0;                    // +0x0
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    int field_0x1c;                     // +0x1c
    int field_0x20;                     // +0x20
    void* field_0x24;                   // +0x24
    void* field_0x40;                   // +0x40
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    Mission* field_0x4c;                // +0x4c
    void* field_0x50;                   // +0x50
    uint8_t field_0x54;                 // +0x54
    void* field_0x58;                   // +0x58
    int*  field_0x5c;                   // +0x5c  campaign briefing offset table
    int*  field_0x60;                   // +0x60  campaign success offset table
    int field_0x64;                     // +0x64
    int field_0x68;                     // +0x68
    int field_0x6c;                     // +0x6c
    uint8_t field_0x70;                 // +0x70

    // ---- methods (converted from free functions) ----
    int OnTouchBegin(int x, int y);
    int OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
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
    void update(int dt);
};
#endif
