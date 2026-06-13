#include "gof2/DialogueWindow.h"
#include "gof2/ChoiceWindow.h"
#include "gof2/FModSound.h"
#include "gof2/ScrollTouchWindow.h"
#include "gof2/Status.h"
#include "gof2/Agent.h"   // defines the canonical (identical-layout) `struct RetStr`
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/PaintCanvas.h"
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


extern "C" void ArrayReleaseClasses_ImagePartPtr(void *self);
extern "C" void *Array_ImagePartPtr_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
namespace AbyssEngine { namespace AERandom { int nextInt(void *self, int max); } }
int GameText_getLanguage(void);
extern "C" void String_ctor_literal(StringSlot *self, const char *text, bool copy);
extern "C" void String_assign_slot(String *self, StringSlot *other);
int Globals_getDialogueSoundId(void *self, int textId, Agent *agent);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void ChoiceWindow_ctor(void *self);
using AbyssEngine::PaintCanvas;
extern "C" void Layout_drawMask(void *layout);
struct Vec2;  // defined below (float x, y) -- only the pointer type is needed here
void TouchButton_getPosition(Vec2 *out, void *self);

// Byte-offset accessor for EXTERNAL opaque objects (Status, sound config, the
// engine's button position records) whose layouts are not part of this class.
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }

// ---- OnTouchBegin_168318.cpp ----
typedef int (*TouchHandler)(void *, int, int);

__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchBegin;

int DialogueWindow::OnTouchBegin(int x, int y) {
    DialogueWindow *self = this;
    if (self->choiceActive != 0) {
        ((ChoiceWindow *)(self->choiceWindow))->OnTouchBegin(x, y);
    } else {
        ((ScrollTouchWindow *)(self->scrollWindow))->OnTouchBegin(x, y);
        void *button = self->prevButton;
        TouchHandler fn = g_dw_touchButtonOnTouchBegin;
        fn(button, x, y);
        fn(self->nextButton, x, y);
        fn(self->moreButton, x, y);
    }
    return 0;
}

// ---- length_168044.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_briefingCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successCounts[];
__attribute__((visibility("hidden"))) extern void **g_dw_status;

int DialogueWindow::length() {
    DialogueWindow *self = this;
    Mission *mission = self->mission;
    if (mission != 0 && ((Mission *)(mission))->isCampaignMission() != 0) {
        int kind = self->kind;
        int *counts;
        if (kind == 1) {
            counts = g_dw_successCounts;
        } else {
            if (kind != 0) return 1;
            counts = g_dw_briefingCounts;
        }
        return counts[self->campaignMission] / 2;
    }
    if (self->kind == 0 && self->mission != 0 &&
        ((Mission *)(self->mission))->getTargetStation() == 0x6c) {
        int result = 6;
        if (F<int>(*g_dw_status, 0x114) == 2) result = 0x12;
        return result;
    }
    return 1;
}

// ---- nextPage_16801e.cpp ----
int DialogueWindow::nextPage() {
    DialogueWindow *self = this;
    int page = self->page;
    int len = ((DialogueWindow *)(self))->length();
    if (page < len - 1) {
        self->page = self->page + 1;
        ((DialogueWindow *)(self))->loadContent();
        return 1;
    }
    return 0;
}

// ---- hasLevel_1680b6.cpp ----
bool DialogueWindow::hasLevel() {
    DialogueWindow *self = this;
    return self->level != 0;
}

// ---- _DialogueWindow_167118.cpp ----
DialogueWindow *_ZN14DialogueWindowD2Ev(DialogueWindow *self)
{
    void *p = self->faceParts;
    if (p != 0) {
        ArrayReleaseClasses_ImagePartPtr(p);
        p = self->faceParts;
        if (p != 0) {
            ::operator delete(Array_ImagePartPtr_dtor(p));
        }
    }
    self->faceParts = 0;

    p = self->briefingOffsets;
    if (p != 0) ::operator delete[](p);
    self->briefingOffsets = 0;

    p = self->successOffsets;
    if (p != 0) ::operator delete[](p);
    self->successOffsets = 0;

    p = self->scrollWindow;
    if (p != 0) ::operator delete(ScrollTouchWindow_dtor(p));
    self->scrollWindow = 0;

    p = self->prevButton;
    if (p != 0) { ((TouchButton *)(p))->dtor(); ::operator delete(p); }
    self->prevButton = 0;

    p = self->nextButton;
    if (p != 0) { ((TouchButton *)(p))->dtor(); ::operator delete(p); }
    self->nextButton = 0;

    p = self->moreButton;
    if (p != 0) { ((TouchButton *)(p))->dtor(); ::operator delete(p); }
    self->moreButton = 0;

    ((String *)((String *)((char *)self + 0x34)))->dtor();
    ((String *)((String *)((char *)self + 0x28)))->dtor();
    return self;
}

// DialogueWindow::~DialogueWindow() — real destructor. Releases the face-part
// image array, the campaign briefing/success offset tables, the scroll window
// and the three touch buttons, then destroys the agent-name / body-text strings.
DialogueWindow::~DialogueWindow()
{
    _ZN14DialogueWindowD2Ev(this);
}

// ---- OnTouchMove_168370.cpp ----
typedef int (*TouchHandler)(void *, int, int);

__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchMove;

int DialogueWindow::OnTouchMove(int x, int y) {
    DialogueWindow *self = this;
    if (self->choiceActive != 0) {
        ((ChoiceWindow *)(self->choiceWindow))->OnTouchMove(x, y);
    } else {
        ((ScrollTouchWindow *)(self->scrollWindow))->OnTouchMove(x, y);
        void *button = self->prevButton;
        TouchHandler fn = g_dw_touchButtonOnTouchMove;
        fn(button, x, y);
        fn(self->nextButton, x, y);
        fn(self->moreButton, x, y);
    }
    return 0;
}

// ---- isFirstPage_1680ac.cpp ----
bool DialogueWindow::isFirstPage() {
    DialogueWindow *self = this;
    return self->page == 0;
}

// ---- previousPage_1680c4.cpp ----
int DialogueWindow::previousPage() {
    DialogueWindow *self = this;
    int page = self->page;
    if (page <= 0) return 0;
    self->page = page - 1;
    ((DialogueWindow *)(self))->loadContent();
    return 1;
}

// ---- isLastPage_168008.cpp ----
bool DialogueWindow::isLastPage() {
    DialogueWindow *self = this;
    int page = self->page;
    int len = ((DialogueWindow *)(self))->length();
    return page == len - 1;
}

// ---- hasBriefingDialogue_1671b4.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_briefingDialogueIds[];

bool DialogueWindow_hasBriefingDialogue(int id)
{
    if (id > 0xa1) return false;
    return g_dw_briefingDialogueIds[id] > 0;
}

bool DialogueWindow::hasBriefingDialogue(int id) {
    if (id > 0xa1) return false;
    return g_dw_briefingDialogueIds[id] > 0;
}

// ---- hasSuccessDialogue_1671d4.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_successDialogueIds[];

bool DialogueWindow_hasSuccessDialogue(int id)
{
    if (id > 0xa1) return false;
    return g_dw_successDialogueIds[id] > 0;
}

bool DialogueWindow::hasSuccessDialogue(int id) {
    if (id > 0xa1) return false;
    return g_dw_successDialogueIds[id] > 0;
}

// ---- set_166ee4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_dw_statusForSet;

void DialogueWindow::set(Mission *mission, int kind, int campaign) {
    DialogueWindow *self = this;
    if (kind == 1) {
        self->mission = mission;
        self->kind = kind;
        goto won;
    }
    self->mission = mission;
    self->kind = kind;
    if (kind == 2) {
        Agent *agent = ((Mission *)(mission))->getAgent();
        if (agent != 0 && ((Agent *)(agent))->isGenericAgent() == 0) {
            ((Agent *)(agent))->setOfferAccepted(false);
        }
        ((Mission *)(mission))->setFailed(true);
        goto finish;
    }
    goto finish;

won:
    ((Mission *)(mission))->getAgent();
    ((Mission *)(mission))->setWon(true);

finish:
    self->page = 0;
    if (campaign == -1) {
        campaign = ((Status *)(*g_dw_statusForSet))->getCurrentCampaignMission();
    }
    self->campaignMission = campaign;
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

    ((TouchButton *)(self->nextButton))->replaceTextKeepSize((String *)((GameText *)(*gameText))->getText(0xb4));
    self->field_0x70 = 0;
    self->autoAdvanceTimer = 0;
    self->pauseLength = 0;
    ((FModSound *)(*sound))->stop(self->voiceSound);
    self->voiceSound = -1;
    ((TouchButton *)(self->nextButton))->setPressProgress(0);

    void *parts = self->faceParts;
    if (parts != 0) {
        ArrayReleaseClasses_ImagePartPtr(parts);
        parts = self->faceParts;
        if (parts != 0) ::operator delete(Array_ImagePartPtr_dtor(parts));
    }
    self->faceParts = 0;

    Mission *mission = self->mission;
    int kind = self->kind;
    int page = self->page;
    int textId = -1;

    if (mission != 0 && ((Mission *)(mission))->isCampaignMission() != 0) {
        int base;
        if (kind == 1) {
            base = self->successOffsets[self->campaignMission];
            textId = g_dw_campaignSuccessTextIds[base + page * 2 + 1];
        } else if (kind == 0) {
            base = self->briefingOffsets[self->campaignMission];
            textId = g_dw_campaignBriefingTextIds[base + page * 2 + 1];
        } else {
            textId = 0x10 + 0x63d;
        }
        self->clientImage = g_dw_defaultClientImage;
        ((String *)((String *)((char *)self + 0x34)))->assign((String *)((GameText *)(*gameText))->getText(0x63d + (textId & 0xff)));
        ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
    } else if (mission != 0) {
        if ((page & 1) != 0) {
            self->clientImage = g_dw_defaultClientImage;
            ((String *)((String *)((char *)self + 0x34)))->assign((String *)((GameText *)(*gameText))->getText(0x63d));
            self->field_0x70 = 1;
        } else {
            self->clientImage = (void *)(intptr_t)((Mission *)(mission))->getClientImage();
            ((Mission *)(&tmp))->getClientName();
            String_assign_slot((String *)((char *)self + 0x34), &tmp);
            ((String *)(&tmp))->dtor();
            self->field_0x70 = 0;
        }

        if (kind == 1 || kind == 0 || kind == 2) {
            Agent *agent = ((Mission *)(mission))->getAgent();
            if (GameText_getLanguage() == 1 && agent != 0) {
                textId = ((DialogueWindow *)(self))->pickGermanGenericTextBecauseWeSaved100EurosWithThat(kind, agent);
            } else {
                textId = 0x188 + AbyssEngine::AERandom::nextInt(*g_dw_randomLoad, 5);
            }
        } else {
            textId = 0x20f;
        }
        ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));

        if (kind == 1) {
            void *standing = (void *)(intptr_t)((Status *)(*g_dw_statusLoad))->getStanding();
            ((Standing *)(standing))->applyMissionCompleted(((Mission *)(mission))->getClientRace());
        }
        if (((Mission *)(mission))->getTargetStation() == 0x6c && kind == 0) {
            textId = 0x1ca;
            ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
        }
        if (((Mission *)(mission))->getType() == 0x0c && kind == 0) {
            textId = 0x174;
            ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
        }
    } else {
        self->clientImage = g_dw_defaultClientImage;
        textId = 0x10;
        ((String *)((String *)((char *)self + 0x28)))->assign((String *)((GameText *)(*gameText))->getText(textId));
        ((String *)((String *)((char *)self + 0x34)))->assign((String *)((GameText *)(*gameText))->getText(0x63d));
    }

    String_ctor_literal(&style, g_dw_emptyLoad, false);
    ((String *)(&tmp))->ctor_copy((String *)((char *)self + 0x28), false);
    ((ScrollTouchWindow *)(self->scrollWindow))->setText4(*(String *)(&style), *(String *)(&tmp), 0);
    ((String *)(&tmp))->dtor();
    ((String *)(&style))->dtor();

    ((TouchButton *)(self->prevButton))->setVisible(self->page != 0);
    ((TouchButton *)(self->moreButton))->setVisible(((DialogueWindow *)(self))->length() > 1);
    self->faceParts = ((ImageFactory *)(*g_dw_imageFactoryLoad))->loadChar((int *)self->clientImage);

    if (((DialogueWindow *)(self))->isLastPage() != 0) {
        ((TouchButton *)(self->nextButton))->replaceTextKeepSize((String *)((GameText *)(*gameText))->getText(0xb5));
    }

    Agent *agent = mission == 0 ? (Agent *)0 : ((Mission *)(mission))->getAgent();
    int soundId = Globals_getDialogueSoundId(*g_dw_globalsLoad, textId, agent);
    self->voiceSound = soundId;
    if (soundId >= 0) {
        ((FModSound *)(*sound))->play(soundId, 0, 0, 0);
        self->pauseLength = ((FModSound *)(*sound))->getEventPauseLength(soundId);
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
    void *scroll = self->scrollWindow;
    if (scroll != 0) {
        ((ScrollTouchWindow *)(scroll))->update(dt);
    }
    if (self->choiceActive != 0) {
        ((ChoiceWindow *)(self->choiceWindow))->update(dt);
    }
    if (F<uint8_t>(g_dw_soundConfig, 0xe) != 0 && self->voiceSound != -1) {
        void **sound = g_dw_fmodSound;
        ((FModSound *)(*sound))->getPlayingProgress(self->voiceSound);
        void *playingSound = *sound;
        int playingId = self->voiceSound;
        if (((FModSound *)(playingSound))->isPlaying(playingId) == 0 &&
            ((DialogueWindow *)(self))->isLastPage() == 0) {
            int elapsed = self->autoAdvanceTimer;
            if (elapsed >= self->pauseLength) {
                ((DialogueWindow *)(self))->nextPage();
                elapsed = self->autoAdvanceTimer;
            }
            self->autoAdvanceTimer = elapsed + dt;
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
    if (self->choiceActive != 0) {
        int r = ((ChoiceWindow *)(self->choiceWindow))->OnTouchEnd(x, y);
        if (r == 1) {
            goto choice_close;
        }
        if (r != 0) goto choice_return_zero;
        self->choiceActive = 0;
        if (((Status *)(*g_dw_statusTouchEnd))->getCurrentCampaignMission() == 0x0f) {
            void **sound = g_dw_soundChoice;
            ((FModSound *)(*sound))->play(0xa2, 0, 0, 0);
            ((FModSound *)(*sound))->stop(F<int>(*sound, 0));
            ((FModSound *)(*sound))->play(0x88, 0, 0, 0);
        }
        if (self->voiceSound != -1) {
            ((FModSound *)(*g_dw_soundVoice))->stop(self->voiceSound);
        }
        return 1;

choice_close:
        self->choiceActive = 0;
choice_return_zero:
        return 0;
    }

    ((ScrollTouchWindow *)(self->scrollWindow))->OnTouchEnd(x, y);
    if (((TouchButton *)(self->prevButton))->OnTouchEnd(x, y) != 0) {
        ((FModSound *)(*g_dw_soundPrev))->stop(self->voiceSound);
        ((DialogueWindow *)(self))->previousPage();
    }
    if (((TouchButton *)(self->nextButton))->OnTouchEnd(x, y) != 0) {
        ((FModSound *)(*g_dw_soundNext))->stop(self->voiceSound);
        if (((DialogueWindow *)(self))->nextPage() == 0) {
            return 1;
        }
    }
    if (((TouchButton *)(self->moreButton))->OnTouchEnd(x, y) != 0) {
        void *choice = self->choiceWindow;
        String *text = (String *)((GameText *)(*g_dw_gameTextTouchEnd))->getText(0x18c);
        ((ChoiceWindow *)(choice))->set(*text, true);
        self->choiceActive = 1;
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

    int *briefingOffsets = (int *)::operator new[](0x288);
    self->briefingOffsets = briefingOffsets;
    int *successOffsets = (int *)::operator new[](0x288);
    self->successOffsets = successOffsets;

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

    self->mission = 0;
    self->choiceWindow = 0;
    self->level = 0;
    self->clientImage = 0;
    self->faceParts = 0;
    self->voiceSound = -1;
    self->autoAdvanceTimer = 0;
    self->field_0x70 = 0;

    void *layout = *g_dw_layoutInit;
    int frameW = F<int>(layout, 0x54);
    int frameH = F<int>(layout, 0x58);
    self->frameWidth = frameW;
    self->frameHeight = frameH;
    int frameX = half_round_to_zero(*g_dw_screenWidth) - half_round_to_zero(frameW);
    int frameY = half_round_to_zero(*g_dw_screenHeight) - half_round_to_zero(frameH);
    self->frameX = frameX;
    self->frameY = frameY;

    void *scroll = ::operator new(0x24);
    int margin = F<int>(layout, 0x4c);
    ScrollTouchWindow_ctor(scroll,
                           frameX + margin * 2 + F<int>(layout, 0x2d4),
                           F<int>(layout, 0x8) + frameY,
                           frameW - margin * 2 - F<int>(layout, 0x2d4),
                           frameH - margin * 2 - F<int>(layout, 0x8) - F<int>(layout, 0x30),
                           false);
    self->scrollWindow = scroll;

    void *choice = ::operator new(0x5c);
    ChoiceWindow_ctor(choice);
    self->choiceWindow = choice;

    void **gameText = g_dw_gameTextInit;
    void *button = ::operator new(0xc8);
    String *label = (String *)((GameText *)(*gameText))->getText(0xb3);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    ((TouchButton *)(button))->ctor(label, 5, self->frameX + margin, self->frameY - margin + self->frameHeight, F<int>(layout, 0x50), 0x21, 4);
    self->prevButton = button;

    button = ::operator new(0xc8);
    label = (String *)((GameText *)(*gameText))->getText(0xb4);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    ((TouchButton *)(button))->ctor(label, 6, self->frameX + self->frameWidth - margin, self->frameY - margin + self->frameHeight, F<int>(layout, 0x50), 0x22, 4);
    self->nextButton = button;

    button = ::operator new(0xc8);
    label = (String *)((GameText *)(*gameText))->getText(0x18b);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    ((TouchButton *)(button))->ctor(label, 0, self->frameX + half_round_to_zero(self->frameWidth), self->frameY + self->frameHeight - margin, self->frameWidth - margin * 4 - F<int>(layout, 0x50) * 2, 0x24, 4);
    self->choiceActive = 0;
    self->moreButton = button;
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

    void *scroll = self->scrollWindow;
    String_ctor_literal(&blank, g_dw_emptyString, false);
    ((String *)(&copy))->ctor_copy(text, false);
    ((ScrollTouchWindow *)(scroll))->setText(*(String *)(&blank), *(String *)(&copy));
    ((String *)(&copy))->dtor();
    ((String *)(&blank))->dtor();

    ((TouchButton *)(self->moreButton))->setVisible(false);
    ((TouchButton *)(self->prevButton))->setVisible(false);

    self->faceParts = ((ImageFactory *)(*g_dw_imageFactoryCtor))->loadChar(parts);
    void *old = self->nextButton;
    if (old != 0) {
        ((TouchButton *)(old))->dtor();
        ::operator delete(old);
    }
    self->nextButton = 0;

    void *button = ::operator new(0xc8);
    String *buttonText = (String *)((GameText *)(*g_dw_gameTextCtor))->getText(0x20c);
    void *layout = *g_dw_layoutCtor;
    int margin = F<int>(layout, 0x4c);
    int x = self->frameX + self->frameWidth / 2;
    int y = self->frameY + self->frameHeight - margin;
    int width = self->frameWidth - margin * 4 - F<int>(layout, 0x50) * 2;
    ((TouchButton *)(button))->ctor(buttonText, 0, x, y, width, 0x24, 4);
    self->nextButton = button;

    ((String *)((String *)((char *)self + 0x34)))->assign(agentName);
    self->voiceSound = -1;
    self->page = 0;
    self->pauseLength = 0;
    return self;
}

// ---- DialogueWindow_166bfc.cpp ----
DialogueWindow * DialogueWindow::ctor_mission(Mission *mission, Level *level, int kind) {
    DialogueWindow *self = this;
    ((String *)((String *)((char *)self + 0x28)))->ctor();
    ((String *)((String *)((char *)self + 0x34)))->ctor();
    ((DialogueWindow *)(self))->init();
    self->level = level;
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
    int race = ((Agent *)(agent))->getRace();
    int male;
    if (race < 10) {
        race = ((Agent *)(agent))->getRace();
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
        index = AbyssEngine::AERandom::nextInt(*random, 2);
        texts = g_dw_germanSuccessTexts;
    } else {
        if (kind != 0) {
            index = AbyssEngine::AERandom::nextInt(*random, 2);
            texts = g_dw_germanOtherTexts;
        } else {
            index = AbyssEngine::AERandom::nextInt(*random, 2);
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

    ((PaintCanvas*)*g_dw_paintCanvas)->SetColor((unsigned int)-1);
    void *layout = *g_dw_layoutDraw;
    Layout_drawMask(layout);
    ((String *)(&title))->ctor_copy((String *)((char *)self + 0x34), false);
    ((Layout *)(layout))->drawBox(7, self->frameX, self->frameY, self->frameWidth, self->frameHeight, &title, 1);
    ((String *)(&title))->dtor();

    ((ScrollTouchWindow *)(self->scrollWindow))->draw();

    layout = *g_dw_layoutDraw;
    int margin = F<int>(layout, 0x4c);
    ((ImageFactory *)(*g_dw_imageFactoryDraw))->drawChar((Arr *)self->faceParts, self->frameX + margin, self->frameY + margin + F<int>(layout, 0x8), self->field_0x70);

    ButtonDraw drawButton = g_dw_touchButtonDraw;
    drawButton(self->prevButton);
    drawButton(self->nextButton);
    drawButton(self->moreButton);

    if (self->choiceActive != 0) {
        ((ChoiceWindow *)(self->choiceWindow))->draw();
    }

    if (*g_dw_drawPositionsReady == 0) {
        if (self->moreButton != 0) {
            TouchButton_getPosition(&pos, self->moreButton);
            F<int>(g_dw_moreButtonX, 0x08) = (int)pos.x;
            TouchButton_getPosition(&pos, self->moreButton);
            F<int>(g_dw_moreButtonY, 0x08) = (int)pos.y;
        }
        if (self->nextButton != 0) {
            TouchButton_getPosition(&pos, self->nextButton);
            F<int>(g_dw_nextButtonX, 0x0c) = (int)pos.x;
            TouchButton_getPosition(&pos, self->nextButton);
            F<int>(g_dw_nextButtonY, 0x0c) = (int)pos.y;
        }
        *g_dw_drawReadyFlag = 1;
    }
}

// ---- setLevel_1780c0.cpp ----
// Attach the campaign Level whose briefing/success text this window is showing.
// hasLevel() reports whether a level was bound this way.
void DialogueWindow::setLevel(Level *level) {
    this->level = level;
}

// ---- ABI shims -------------------------------------------------------------
// ModStation builds and drives its embedded DialogueWindow through a handful of
// other-unit-inlined entry points ("_ou" / "_ote" / "_r2d").  Each is the exact
// same work as the corresponding real member, just reached from the inlined
// station-update / station-render / station-touch paths, so they forward to the
// recovered members.

// Default + mission + message constructors (in-place; the raw object pointer is
// allocated by ModStation via operator new(0x74)).
extern "C" void DialogueWindow_ctor_ou(DialogueWindow *dw) {
    dw->ctor_default();
}

extern "C" void DialogueWindow_ctor_mission_ou(DialogueWindow *dw, void *mission,
                                               int level, int kind) {
    dw->ctor_mission((Mission *)mission, (Level *)level, kind);
}

extern "C" void DialogueWindow_ctor_msg_ou(DialogueWindow *dw, int titleStr,
                                           int bodyStr, int *parts) {
    dw->ctor_text((String *)(intptr_t)titleStr, (String *)(intptr_t)bodyStr, parts);
}

// set()/update() reached from the inlined station logic.  setMission passes the
// "won" kind (1) and lets the window resolve the current campaign mission (-1).
extern "C" void DialogueWindow_setMission_ou(void *dw, void *mission, int kind) {
    ((DialogueWindow *)dw)->set((Mission *)mission, kind, -1);
}

extern "C" void DialogueWindow_update_ou(int dw) {
    ((DialogueWindow *)(intptr_t)dw)->update(0);
}

// OnTouchEnd / destructor reached from the inlined station touch handler.
extern "C" int DialogueWindow_OnTouchEnd_ote(int dw, int xy) {
    return ((DialogueWindow *)(intptr_t)dw)->OnTouchEnd(xy, xy);
}

extern "C" void DialogueWindow_dtor_ote(DialogueWindow *dw) {
    dw->~DialogueWindow();
}

// Render: ModStation passes itself; the live DialogueWindow pointer lives at
// ModStation + 0x84 (m_pDialogueWindow).  Draw it through the real member.
extern "C" void DialogueWindow_draw_r2d(void *modStation) {
    DialogueWindow *dw = *(DialogueWindow **)((char *)modStation + 0x84);
    if (dw != 0) dw->draw();
}
