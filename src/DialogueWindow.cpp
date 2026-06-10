#include "gof2/DialogueWindow.h"
#include "gof2/Agent.h"   // defines the canonical (identical-layout) `struct RetStr`
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
// Mission.h (-> Station.h) and TouchButton.h each re-declare an identical, unguarded
// `struct RetStr`, which collides with Agent.h's. None of the RetStr-returning accessors
// are used in this TU, so the duplicate definitions are renamed away here to avoid the
// redefinition error without touching those (other-batch) headers.
#define RetStr RetStr
#include "gof2/Mission.h"
#undef RetStr
#include "gof2/Standing.h"
#include "gof2/String.h"
#define RetStr RetStr
#include "gof2/TouchButton.h"
#undef RetStr
// Layout's drawMask/drawBox are used via the local extern "C" prototypes below;
// the full Layout.h is not required here.


extern "C" int ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchBegin(void *self, int x, int y);
extern "C" int Mission_getTargetStation(Mission *self);
extern "C" void ArrayReleaseClasses_ImagePartPtr(void *self);
extern "C" void *Array_ImagePartPtr_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" void operator_delete_arr(void *self);
extern "C" int ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchMove(void *self, int x, int y);
extern "C" Agent *Mission_getAgent(Mission *self);
extern "C" void Mission_setWon(Mission *self, bool won);
extern "C" void Mission_setFailed(Mission *self, bool failed);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" int FModSound_stop(void *self, int sound);
extern "C" void FModSound_play(void *self, int sound, void *pos, int volume, int extra);
extern "C" int FModSound_getEventPauseLength(void *self, int sound);
extern "C" int Mission_getType(Mission *self);
extern "C" void *Mission_getClientImage(Mission *self);
extern "C" int Mission_getClientRace(Mission *self);
extern "C" int Mission_getStatusValue(Mission *self);
extern "C" void *Status_getStanding(void *status);
extern "C" int AERandom_nextInt(void *self, int max);
extern "C" int GameText_getLanguage(void);
extern "C" void String_ctor_literal(StringSlot *self, const char *text, bool copy);
extern "C" void String_assign_slot(String *self, StringSlot *other);
extern "C" void ScrollTouchWindow_setText4(void *self, StringSlot *style, StringSlot *text, int color);
extern "C" int Globals_getDialogueSoundId(void *self, int textId, Agent *agent);
extern "C" void ScrollTouchWindow_update(void *self, int dt);
extern "C" void ChoiceWindow_update(void *self, int dt);
extern "C" void FModSound_getPlayingProgress(void *self, int sound);
extern "C" int FModSound_isPlaying(void *self, int sound);
extern "C" int ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchEnd(void *self, int x, int y);
extern "C" void ChoiceWindow_set(void *self, String *text, bool flag);
extern "C" void *operator_new(unsigned size);
extern "C" void *operator_new_arr(unsigned size);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void ChoiceWindow_ctor(void *self);
extern "C" void TouchButton_ctor(void *self, String *text, int type, int x, int y, int width, int icon, int style);
extern "C" void ScrollTouchWindow_setText(void *self, StringSlot *style, StringSlot *text);
extern "C" int Agent_getRace(Agent *self);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void Layout_drawMask(void *layout);
extern "C" void ScrollTouchWindow_draw(void *self);
extern "C" void ChoiceWindow_draw(void *self);
struct Vec2;  // defined below (float x, y) -- only the pointer type is needed here
extern "C" void TouchButton_getPosition(Vec2 *out, void *self);

// Byte-offset accessor for EXTERNAL opaque objects (Status, sound config, the
// engine's button position records) whose layouts are not part of this class.
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }

// ---- OnTouchBegin_168318.cpp ----
typedef int (*TouchHandler)(void *, int, int);

__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchBegin;

int DialogueWindow::OnTouchBegin(int x, int y) {
    DialogueWindow *self = this;
    if (self->field_0x54 != 0) {
        ChoiceWindow_OnTouchBegin(self->field_0x50, x, y);
    } else {
        ScrollTouchWindow_OnTouchBegin(self->field_0x40, x, y);
        void *button = self->field_0x0;
        TouchHandler fn = g_dw_touchButtonOnTouchBegin;
        fn(button, x, y);
        fn(self->field_0x4, x, y);
        fn(self->field_0x8, x, y);
    }
    return 0;
}

// ---- length_168044.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_briefingCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successCounts[];
__attribute__((visibility("hidden"))) extern void **g_dw_status;

int DialogueWindow::length() {
    DialogueWindow *self = this;
    Mission *mission = self->field_0x4c;
    if (mission != 0 && ((Mission *)(mission))->isCampaignMission() != 0) {
        int kind = self->field_0x44;
        int *counts;
        if (kind == 1) {
            counts = g_dw_successCounts;
        } else {
            if (kind != 0) return 1;
            counts = g_dw_briefingCounts;
        }
        return counts[self->field_0x10] / 2;
    }
    if (self->field_0x44 == 0 && self->field_0x4c != 0 &&
        Mission_getTargetStation(self->field_0x4c) == 0x6c) {
        int result = 6;
        if (F<int>(*g_dw_status, 0x114) == 2) result = 0x12;
        return result;
    }
    return 1;
}

// ---- nextPage_16801e.cpp ----
int DialogueWindow::nextPage() {
    DialogueWindow *self = this;
    int page = self->field_0x48;
    int len = ((DialogueWindow *)(self))->length();
    if (page < len - 1) {
        self->field_0x48 = self->field_0x48 + 1;
        ((DialogueWindow *)(self))->loadContent();
        return 1;
    }
    return 0;
}

// ---- hasLevel_1680b6.cpp ----
bool DialogueWindow::hasLevel() {
    DialogueWindow *self = this;
    return self->field_0x58 != 0;
}

// ---- _DialogueWindow_167118.cpp ----
extern "C" DialogueWindow *_ZN14DialogueWindowD2Ev(DialogueWindow *self)
{
    void *p = self->field_0xc;
    if (p != 0) {
        ArrayReleaseClasses_ImagePartPtr(p);
        p = self->field_0xc;
        if (p != 0) {
            operator_delete(Array_ImagePartPtr_dtor(p));
        }
    }
    self->field_0xc = 0;

    p = self->field_0x5c;
    if (p != 0) operator_delete_arr(p);
    self->field_0x5c = 0;

    p = self->field_0x60;
    if (p != 0) operator_delete_arr(p);
    self->field_0x60 = 0;

    p = self->field_0x40;
    if (p != 0) operator_delete(ScrollTouchWindow_dtor(p));
    self->field_0x40 = 0;

    p = self->field_0x0;
    if (p != 0) { ((TouchButton *)(p))->dtor(); operator_delete(p); }
    self->field_0x0 = 0;

    p = self->field_0x4;
    if (p != 0) { ((TouchButton *)(p))->dtor(); operator_delete(p); }
    self->field_0x4 = 0;

    p = self->field_0x8;
    if (p != 0) { ((TouchButton *)(p))->dtor(); operator_delete(p); }
    self->field_0x8 = 0;

    ((String *)((String *)((char *)self + 0x34)))->dtor();
    ((String *)((String *)((char *)self + 0x28)))->dtor();
    return self;
}

// ---- OnTouchMove_168370.cpp ----
typedef int (*TouchHandler)(void *, int, int);

__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchMove;

int DialogueWindow::OnTouchMove(int x, int y) {
    DialogueWindow *self = this;
    if (self->field_0x54 != 0) {
        ChoiceWindow_OnTouchMove(self->field_0x50, x, y);
    } else {
        ScrollTouchWindow_OnTouchMove(self->field_0x40, x, y);
        void *button = self->field_0x0;
        TouchHandler fn = g_dw_touchButtonOnTouchMove;
        fn(button, x, y);
        fn(self->field_0x4, x, y);
        fn(self->field_0x8, x, y);
    }
    return 0;
}

// ---- isFirstPage_1680ac.cpp ----
bool DialogueWindow::isFirstPage() {
    DialogueWindow *self = this;
    return self->field_0x48 == 0;
}

// ---- previousPage_1680c4.cpp ----
int DialogueWindow::previousPage() {
    DialogueWindow *self = this;
    int page = self->field_0x48;
    if (page <= 0) return 0;
    self->field_0x48 = page - 1;
    ((DialogueWindow *)(self))->loadContent();
    return 1;
}

// ---- isLastPage_168008.cpp ----
bool DialogueWindow::isLastPage() {
    DialogueWindow *self = this;
    int page = self->field_0x48;
    int len = ((DialogueWindow *)(self))->length();
    return page == len - 1;
}

// ---- hasBriefingDialogue_1671b4.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_briefingDialogueIds[];

extern "C" bool DialogueWindow_hasBriefingDialogue(int id)
{
    if (id > 0xa1) return false;
    return g_dw_briefingDialogueIds[id] > 0;
}

// ---- hasSuccessDialogue_1671d4.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_successDialogueIds[];

extern "C" bool DialogueWindow_hasSuccessDialogue(int id)
{
    if (id > 0xa1) return false;
    return g_dw_successDialogueIds[id] > 0;
}

// ---- set_166ee4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_dw_statusForSet;

void DialogueWindow::set(Mission *mission, int kind, int campaign) {
    DialogueWindow *self = this;
    if (kind == 1) {
        self->field_0x4c = mission;
        self->field_0x44 = kind;
        goto won;
    }
    self->field_0x4c = mission;
    self->field_0x44 = kind;
    if (kind == 2) {
        Agent *agent = Mission_getAgent(mission);
        if (agent != 0 && ((Agent *)(agent))->isGenericAgent() == 0) {
            ((Agent *)(agent))->setOfferAccepted(false);
        }
        Mission_setFailed(mission, true);
        goto finish;
    }
    goto finish;

won:
    Mission_getAgent(mission);
    Mission_setWon(mission, true);

finish:
    self->field_0x48 = 0;
    if (campaign == -1) {
        campaign = Status_getCurrentCampaignMission(*g_dw_statusForSet);
    }
    self->field_0x10 = campaign;
    ((DialogueWindow *)(self))->loadContent();
}

// ---- loadContent_1671f4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextLoad;
__attribute__((visibility("hidden"))) extern void **g_dw_soundLoad;
__attribute__((visibility("hidden"))) extern void **g_dw_randomLoad;
__attribute__((visibility("hidden"))) extern void **g_dw_statusLoad;
__attribute__((visibility("hidden"))) extern void **g_dw_imageFactoryLoad;
__attribute__((visibility("hidden"))) extern void **g_dw_globalsLoad;
__attribute__((visibility("hidden"))) extern const char g_dw_emptyLoad[];
__attribute__((visibility("hidden"))) extern int g_dw_campaignBriefingTextIds[];
__attribute__((visibility("hidden"))) extern int g_dw_campaignSuccessTextIds[];
__attribute__((visibility("hidden"))) extern void *g_dw_defaultClientImage;

void DialogueWindow::loadContent() {
    DialogueWindow *self = this;
    StringSlot tmp;
    StringSlot style;

    void **gameText = g_dw_gameTextLoad;
    void **sound = g_dw_soundLoad;

    ((TouchButton *)(self->field_0x4))->replaceTextKeepSize((String *)((GameText *)(*gameText))->getText(0xb4));
    self->field_0x70 = 0;
    self->field_0x68 = 0;
    self->field_0x6c = 0;
    FModSound_stop(*sound, self->field_0x64);
    self->field_0x64 = -1;
    ((TouchButton *)(self->field_0x4))->setPressProgress(0);

    void *parts = self->field_0xc;
    if (parts != 0) {
        ArrayReleaseClasses_ImagePartPtr(parts);
        parts = self->field_0xc;
        if (parts != 0) operator_delete(Array_ImagePartPtr_dtor(parts));
    }
    self->field_0xc = 0;

    Mission *mission = self->field_0x4c;
    int kind = self->field_0x44;
    int page = self->field_0x48;
    int textId = -1;

    if (mission != 0 && ((Mission *)(mission))->isCampaignMission() != 0) {
        int base;
        if (kind == 1) {
            base = self->field_0x60[self->field_0x10];
            textId = g_dw_campaignSuccessTextIds[base + page * 2 + 1];
        } else if (kind == 0) {
            base = self->field_0x5c[self->field_0x10];
            textId = g_dw_campaignBriefingTextIds[base + page * 2 + 1];
        } else {
            textId = 0x10 + 0x63d;
        }
        self->field_0x24 = g_dw_defaultClientImage;
        ((String *)((String *)((char *)self + 0x34)))->assign((String *)((GameText *)(*gameText))->getText(0x63d + (textId & 0xff)));
        ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
    } else if (mission != 0) {
        if ((page & 1) != 0) {
            self->field_0x24 = g_dw_defaultClientImage;
            ((String *)((String *)((char *)self + 0x34)))->assign((String *)((GameText *)(*gameText))->getText(0x63d));
            self->field_0x70 = 1;
        } else {
            self->field_0x24 = Mission_getClientImage(mission);
            ((Mission *)(&tmp))->getClientName();
            String_assign_slot((String *)((char *)self + 0x34), &tmp);
            ((String *)(&tmp))->dtor();
            self->field_0x70 = 0;
        }

        if (kind == 1 || kind == 0 || kind == 2) {
            Agent *agent = Mission_getAgent(mission);
            if (GameText_getLanguage() == 1 && agent != 0) {
                textId = ((DialogueWindow *)(self))->pickGermanGenericTextBecauseWeSaved100EurosWithThat(kind, agent);
            } else {
                textId = 0x188 + AERandom_nextInt(*g_dw_randomLoad, 5);
            }
        } else {
            textId = 0x20f;
        }
        ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));

        if (kind == 1) {
            void *standing = Status_getStanding(*g_dw_statusLoad);
            ((Standing *)(standing))->applyMissionCompleted(Mission_getClientRace(mission));
        }
        if (Mission_getTargetStation(mission) == 0x6c && kind == 0) {
            textId = 0x1ca;
            ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
        }
        if (Mission_getType(mission) == 0x0c && kind == 0) {
            textId = 0x174;
            ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
        }
    } else {
        self->field_0x24 = g_dw_defaultClientImage;
        textId = 0x10;
        ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
        ((String *)((String *)((char *)self + 0x34)))->assign((String *)((GameText *)(*gameText))->getText(0x63d));
    }

    String_ctor_literal(&style, g_dw_emptyLoad, false);
    ((String *)(&tmp))->ctor_copy((String *)((char *)self + 0x28), false);
    ScrollTouchWindow_setText4(self->field_0x40, &style, &tmp, 0);
    ((String *)(&tmp))->dtor();
    ((String *)(&style))->dtor();

    ((TouchButton *)(self->field_0x0))->setVisible(self->field_0x48 != 0);
    ((TouchButton *)(self->field_0x8))->setVisible(((DialogueWindow *)(self))->length() > 1);
    self->field_0xc = ((ImageFactory *)(*g_dw_imageFactoryLoad))->loadChar((int *)self->field_0x24);

    if (((DialogueWindow *)(self))->isLastPage() != 0) {
        ((TouchButton *)(self->field_0x4))->replaceTextKeepSize((String *)((GameText *)(*gameText))->getText(0xb5));
    }

    Agent *agent = mission == 0 ? (Agent *)0 : Mission_getAgent(mission);
    int soundId = Globals_getDialogueSoundId(*g_dw_globalsLoad, textId, agent);
    self->field_0x64 = soundId;
    if (soundId >= 0) {
        FModSound_play(*sound, soundId, 0, 0, 0);
        self->field_0x6c = FModSound_getEventPauseLength(*sound, soundId);
    }
}

// ---- DialogueWindow_166f5c.cpp ----
DialogueWindow * DialogueWindow::ctor_default() {
    DialogueWindow *self = this;
    ((String *)((String *)((char *)self + 0x28)))->ctor();
    ((String *)((String *)((char *)self + 0x34)))->ctor();
    ((DialogueWindow *)(self))->init();
    return self;
}

// ---- update_167f94.cpp ----
__attribute__((visibility("hidden"))) extern void *g_dw_soundConfig;
__attribute__((visibility("hidden"))) extern void **g_dw_fmodSound;

void DialogueWindow::update(int dt) {
    DialogueWindow *self = this;
    void *scroll = self->field_0x40;
    if (scroll != 0) {
        ScrollTouchWindow_update(scroll, dt);
    }
    if (self->field_0x54 != 0) {
        ChoiceWindow_update(self->field_0x50, dt);
    }
    if (F<uint8_t>(g_dw_soundConfig, 0xe) != 0 && self->field_0x64 != -1) {
        void **sound = g_dw_fmodSound;
        FModSound_getPlayingProgress(*sound, self->field_0x64);
        void *playingSound = *sound;
        int playingId = self->field_0x64;
        if (FModSound_isPlaying(playingSound, playingId) == 0 &&
            ((DialogueWindow *)(self))->isLastPage() == 0) {
            int elapsed = self->field_0x68;
            if (elapsed >= self->field_0x6c) {
                ((DialogueWindow *)(self))->nextPage();
                elapsed = self->field_0x68;
            }
            self->field_0x68 = elapsed + dt;
        }
    }
}

// ---- OnTouchEnd_1683c8.cpp ----
__attribute__((visibility("hidden"))) extern void **g_dw_statusTouchEnd;
__attribute__((visibility("hidden"))) extern void **g_dw_soundChoice;
__attribute__((visibility("hidden"))) extern void **g_dw_soundVoice;
__attribute__((visibility("hidden"))) extern void **g_dw_soundPrev;
__attribute__((visibility("hidden"))) extern void **g_dw_soundNext;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextTouchEnd;

int DialogueWindow::OnTouchEnd(int x, int y) {
    DialogueWindow *self = this;
    if (self->field_0x54 != 0) {
        int r = ChoiceWindow_OnTouchEnd(self->field_0x50, x, y);
        if (r == 1) {
            goto choice_close;
        }
        if (r != 0) goto choice_return_zero;
        self->field_0x54 = 0;
        if (Status_getCurrentCampaignMission(*g_dw_statusTouchEnd) == 0x0f) {
            void **sound = g_dw_soundChoice;
            FModSound_play(*sound, 0xa2, 0, 0, 0);
            FModSound_stop(*sound, F<int>(*sound, 0));
            FModSound_play(*sound, 0x88, 0, 0, 0);
        }
        if (self->field_0x64 != -1) {
            FModSound_stop(*g_dw_soundVoice, self->field_0x64);
        }
        return 1;

choice_close:
        self->field_0x54 = 0;
choice_return_zero:
        return 0;
    }

    ScrollTouchWindow_OnTouchEnd(self->field_0x40, x, y);
    if (((TouchButton *)(self->field_0x0))->OnTouchEnd(x, y) != 0) {
        FModSound_stop(*g_dw_soundPrev, self->field_0x64);
        ((DialogueWindow *)(self))->previousPage();
    }
    if (((TouchButton *)(self->field_0x4))->OnTouchEnd(x, y) != 0) {
        FModSound_stop(*g_dw_soundNext, self->field_0x64);
        if (((DialogueWindow *)(self))->nextPage() == 0) {
            return 1;
        }
    }
    if (((TouchButton *)(self->field_0x8))->OnTouchEnd(x, y) != 0) {
        void *choice = self->field_0x50;
        String *text = (String *)((GameText *)(*g_dw_gameTextTouchEnd))->getText(0x18c);
        ChoiceWindow_set(choice, text, true);
        self->field_0x54 = 1;
    }
    return 0;
}

// ---- init_166c58.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_briefingPartCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successPartCounts[];
__attribute__((visibility("hidden"))) extern const char g_dw_defaultAgentName[];
__attribute__((visibility("hidden"))) extern int *g_dw_screenWidth;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutInit;
__attribute__((visibility("hidden"))) extern int *g_dw_screenHeight;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextInit;

static inline int half_round_to_zero(int v)
{
    return (v + ((unsigned)v >> 31)) >> 1;
}

int DialogueWindow::init() {
    DialogueWindow *self = this;
    StringSlot name;

    int *briefingOffsets = (int *)operator_new_arr(0x288);
    self->field_0x5c = briefingOffsets;
    int *successOffsets = (int *)operator_new_arr(0x288);
    self->field_0x60 = successOffsets;

    int briefingSum = 0;
    int successSum = 0;
    for (int i = 0; i != 0xa2; ++i) {
        int briefingCount = g_dw_briefingPartCounts[i];
        int successCount = g_dw_successPartCounts[i];
        briefingOffsets[i] = briefingSum;
        successOffsets[i] = successSum;
        successSum += briefingCount;
        briefingSum += successCount;
    }

    String_ctor_literal(&name, g_dw_defaultAgentName, false);
    String_assign_slot((String *)((char *)self + 0x34), &name);
    ((String *)(&name))->dtor();

    self->field_0x4c = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->field_0x24 = 0;
    self->field_0xc = 0;
    self->field_0x64 = -1;
    self->field_0x68 = 0;
    self->field_0x70 = 0;

    void *layout = *g_dw_layoutInit;
    int frameW = F<int>(layout, 0x54);
    int frameH = F<int>(layout, 0x58);
    self->field_0x1c = frameW;
    self->field_0x20 = frameH;
    int frameX = half_round_to_zero(*g_dw_screenWidth) - half_round_to_zero(frameW);
    int frameY = half_round_to_zero(*g_dw_screenHeight) - half_round_to_zero(frameH);
    self->field_0x14 = frameX;
    self->field_0x18 = frameY;

    void *scroll = operator_new(0x24);
    int margin = F<int>(layout, 0x4c);
    ScrollTouchWindow_ctor(scroll,
                           frameX + margin * 2 + F<int>(layout, 0x2d4),
                           F<int>(layout, 0x8) + frameY,
                           frameW - margin * 2 - F<int>(layout, 0x2d4),
                           frameH - margin * 2 - F<int>(layout, 0x8) - F<int>(layout, 0x30),
                           false);
    self->field_0x40 = scroll;

    void *choice = operator_new(0x5c);
    ChoiceWindow_ctor(choice);
    self->field_0x50 = choice;

    void **gameText = g_dw_gameTextInit;
    void *button = operator_new(0xc8);
    String *label = (String *)((GameText *)(*gameText))->getText(0xb3);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 5,
                     self->field_0x14 + margin,
                     self->field_0x18 - margin + self->field_0x20,
                     F<int>(layout, 0x50), 0x21, 4);
    self->field_0x0 = button;

    button = operator_new(0xc8);
    label = (String *)((GameText *)(*gameText))->getText(0xb4);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 6,
                     self->field_0x14 + self->field_0x1c - margin,
                     self->field_0x18 - margin + self->field_0x20,
                     F<int>(layout, 0x50), 0x22, 4);
    self->field_0x4 = button;

    button = operator_new(0xc8);
    label = (String *)((GameText *)(*gameText))->getText(0x18b);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 0,
                     self->field_0x14 + half_round_to_zero(self->field_0x1c),
                     self->field_0x18 + self->field_0x20 - margin,
                     self->field_0x1c - margin * 4 - F<int>(layout, 0x50) * 2,
                     0x24, 4);
    self->field_0x54 = 0;
    self->field_0x8 = button;
    return 0;
}

// ---- DialogueWindow_166fa0.cpp ----
__attribute__((visibility("hidden"))) extern const char g_dw_emptyString[];
__attribute__((visibility("hidden"))) extern void **g_dw_imageFactoryCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutCtor;

DialogueWindow * DialogueWindow::ctor_text(String *text, String *agentName, int *parts) {
    DialogueWindow *self = this;
    StringSlot blank;
    StringSlot copy;

    ((String *)((String *)((char *)self + 0x28)))->ctor();
    ((String *)((String *)((char *)self + 0x34)))->ctor();
    ((DialogueWindow *)(self))->init();

    void *scroll = self->field_0x40;
    String_ctor_literal(&blank, g_dw_emptyString, false);
    ((String *)(&copy))->ctor_copy(text, false);
    ScrollTouchWindow_setText(scroll, &blank, &copy);
    ((String *)(&copy))->dtor();
    ((String *)(&blank))->dtor();

    ((TouchButton *)(self->field_0x8))->setVisible(false);
    ((TouchButton *)(self->field_0x0))->setVisible(false);

    self->field_0xc = ((ImageFactory *)(*g_dw_imageFactoryCtor))->loadChar(parts);
    void *old = self->field_0x4;
    if (old != 0) {
        ((TouchButton *)(old))->dtor();
        operator_delete(old);
    }
    self->field_0x4 = 0;

    void *button = operator_new(0xc8);
    String *buttonText = (String *)((GameText *)(*g_dw_gameTextCtor))->getText(0x20c);
    void *layout = *g_dw_layoutCtor;
    int margin = F<int>(layout, 0x4c);
    int x = self->field_0x14 + self->field_0x1c / 2;
    int y = self->field_0x18 + self->field_0x20 - margin;
    int width = self->field_0x1c - margin * 4 - F<int>(layout, 0x50) * 2;
    TouchButton_ctor(button, buttonText, 0, x, y, width, 0x24, 4);
    self->field_0x4 = button;

    ((String *)((String *)((char *)self + 0x34)))->assign(agentName);
    self->field_0x64 = -1;
    self->field_0x48 = 0;
    self->field_0x6c = 0;
    return self;
}

// ---- DialogueWindow_166bfc.cpp ----
DialogueWindow * DialogueWindow::ctor_mission(Mission *mission, Level *level, int kind) {
    DialogueWindow *self = this;
    ((String *)((String *)((char *)self + 0x28)))->ctor();
    ((String *)((String *)((char *)self + 0x34)))->ctor();
    ((DialogueWindow *)(self))->init();
    self->field_0x58 = level;
    ((DialogueWindow *)(self))->set(mission, kind, -1);
    return self;
}

// ---- pickGermanGenericTextBecauseWeSaved100EurosWithThat_1680e0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_dw_random;
__attribute__((visibility("hidden"))) extern int g_dw_germanBriefingTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanSuccessTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanOtherTexts[];

int DialogueWindow::pickGermanGenericTextBecauseWeSaved100EurosWithThat(int kind, Agent *agent) {
    DialogueWindow *self = this;
    (void)self;
    int race = Agent_getRace(agent);
    int male;
    if (race < 10) {
        race = Agent_getRace(agent);
        male = ((Agent *)(agent))->isMale();
        if (race == 3) {
            if (((Agent *)(agent))->getImageParts() == 0) {
                race = 3;
            } else {
                int *parts = ((Agent *)(agent))->getImageParts();
                race = 0;
                if (*parts == 2) race = 3;
            }
        }
    } else {
        male = ((Agent *)(agent))->isMale();
        race = 10;
    }

    int index;
    int *texts;
    void **random = g_dw_random;
    if (kind == 2) {
        index = AERandom_nextInt(*random, 2);
        texts = g_dw_germanSuccessTexts;
    } else {
        if (kind != 0) {
            index = AERandom_nextInt(*random, 2);
            texts = g_dw_germanOtherTexts;
        } else {
            index = AERandom_nextInt(*random, 2);
            texts = g_dw_germanBriefingTexts;
        }
    }

    int *picked = (int *)((char *)texts + index * 4 + 0x48);
    if (male != 0) {
        picked = (int *)((char *)texts + race * 8 + index * 4);
    }
    return *picked;
}

// ---- draw_168198.cpp ----
struct Vec2 {
    float x;
    float y;
};

typedef void (*ButtonDraw)(void *);


__attribute__((visibility("hidden"))) extern int *g_dw_drawGuard;
__attribute__((visibility("hidden"))) extern void **g_dw_paintCanvas;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutDraw;
__attribute__((visibility("hidden"))) extern void **g_dw_imageFactoryDraw;
__attribute__((visibility("hidden"))) extern ButtonDraw g_dw_touchButtonDraw;
__attribute__((visibility("hidden"))) extern int *g_dw_drawPositionsReady;
__attribute__((visibility("hidden"))) extern int *g_dw_moreButtonX;
__attribute__((visibility("hidden"))) extern int *g_dw_moreButtonY;
__attribute__((visibility("hidden"))) extern int *g_dw_nextButtonX;
__attribute__((visibility("hidden"))) extern int *g_dw_nextButtonY;
__attribute__((visibility("hidden"))) extern int *g_dw_drawReadyFlag;

void DialogueWindow::draw() {
    DialogueWindow *self = this;
    StringSlot title;
    Vec2 pos;

    PaintCanvas_SetColor(*g_dw_paintCanvas, -1);
    void *layout = *g_dw_layoutDraw;
    Layout_drawMask(layout);
    ((String *)(&title))->ctor_copy((String *)((char *)self + 0x34), false);
    ((Layout *)(layout))->drawBox(7, self->field_0x14, self->field_0x18, self->field_0x1c, self->field_0x20, &title, 1);
    ((String *)(&title))->dtor();

    ScrollTouchWindow_draw(self->field_0x40);

    layout = *g_dw_layoutDraw;
    int margin = F<int>(layout, 0x4c);
    ((ImageFactory *)(*g_dw_imageFactoryDraw))->drawChar((Arr *)self->field_0xc, self->field_0x14 + margin, self->field_0x18 + margin + F<int>(layout, 0x8), self->field_0x70);

    ButtonDraw drawButton = g_dw_touchButtonDraw;
    drawButton(self->field_0x0);
    drawButton(self->field_0x4);
    drawButton(self->field_0x8);

    if (self->field_0x54 != 0) {
        ChoiceWindow_draw(self->field_0x50);
    }

    if (*g_dw_drawPositionsReady == 0) {
        if (self->field_0x8 != 0) {
            TouchButton_getPosition(&pos, self->field_0x8);
            F<int>(g_dw_moreButtonX, 0x08) = (int)pos.x;
            TouchButton_getPosition(&pos, self->field_0x8);
            F<int>(g_dw_moreButtonY, 0x08) = (int)pos.y;
        }
        if (self->field_0x4 != 0) {
            TouchButton_getPosition(&pos, self->field_0x4);
            F<int>(g_dw_nextButtonX, 0x0c) = (int)pos.x;
            TouchButton_getPosition(&pos, self->field_0x4);
            F<int>(g_dw_nextButtonY, 0x0c) = (int)pos.y;
        }
        *g_dw_drawReadyFlag = 1;
    }
}
