#include "DialogueWindow.h"


extern "C" int ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchBegin(void *self, int x, int y);
extern "C" int Mission_isCampaignMission(Mission *self);
extern "C" int Mission_getTargetStation(Mission *self);
extern "C" int DialogueWindow_length(DialogueWindow *self);
extern "C" void DialogueWindow_loadContent(DialogueWindow *self);
extern "C" void ArrayReleaseClasses_ImagePartPtr(void *self);
extern "C" void *Array_ImagePartPtr_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void *TouchButton_dtor(void *self);
extern "C" void String_dtor(String *self);
extern "C" void operator_delete(void *self);
extern "C" void operator_delete_arr(void *self);
extern "C" int ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchMove(void *self, int x, int y);
extern "C" Agent *Mission_getAgent(Mission *self);
extern "C" void Mission_setWon(Mission *self, bool won);
extern "C" void Mission_setFailed(Mission *self, bool failed);
extern "C" int Agent_isGenericAgent(Agent *self);
extern "C" void Agent_setOfferAccepted(Agent *self, bool accepted);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" String *GameText_getText(void *self, int id);
extern "C" void TouchButton_replaceTextKeepSize(void *self, String *text);
extern "C" void TouchButton_setPressProgress(void *self, int progress);
extern "C" void TouchButton_setVisible(void *self, bool visible);
extern "C" int FModSound_stop(void *self, int sound);
extern "C" void FModSound_play(void *self, int sound, void *pos, int volume, int extra);
extern "C" int FModSound_getEventPauseLength(void *self, int sound);
extern "C" int Mission_getType(Mission *self);
extern "C" void *Mission_getClientImage(Mission *self);
extern "C" void Mission_getClientName(StringSlot *out, Mission *self);
extern "C" int Mission_getClientRace(Mission *self);
extern "C" int Mission_getStatusValue(Mission *self);
extern "C" void Mission_getTargetStationName(StringSlot *out, Mission *self);
extern "C" void Agent_getStationName(StringSlot *out, Agent *self);
extern "C" int Agent_isStoryAgent(Agent *self);
extern "C" void *Status_getStanding(void *status);
extern "C" void Standing_applyMissionCompleted(void *self, int race);
extern "C" int AERandom_nextInt(void *self, int max);
extern "C" int GameText_getLanguage(void);
extern "C" void String_ctor_literal(StringSlot *self, const char *text, bool copy);
extern "C" void String_ctor_copy(StringSlot *self, String *text, bool copy);
extern "C" void String_assign(String *self, String *other);
extern "C" void String_assign_slot(String *self, StringSlot *other);
extern "C" void String_dtor(StringSlot *self);
extern "C" void ScrollTouchWindow_setText(void *self, StringSlot *style, StringSlot *text, int color);
extern "C" void *ImageFactory_loadChar(void *self, void *parts);
extern "C" int DialogueWindow_isLastPage(DialogueWindow *self);
extern "C" int DialogueWindow_pickGermanGenericTextBecauseWeSaved100EurosWithThat(DialogueWindow *self, int kind, Agent *agent);
extern "C" int Globals_getDialogueSoundId(void *self, int textId, Agent *agent);
extern "C" void String_ctor(String *self);
extern "C" int DialogueWindow_init(DialogueWindow *self);
extern "C" void ScrollTouchWindow_update(void *self, int dt);
extern "C" void ChoiceWindow_update(void *self, int dt);
extern "C" void FModSound_getPlayingProgress(void *self, int sound);
extern "C" int FModSound_isPlaying(void *self, int sound);
extern "C" int DialogueWindow_nextPage(DialogueWindow *self);
extern "C" int ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchEnd(void *self, int x, int y);
extern "C" void ChoiceWindow_set(void *self, String *text, bool flag);
extern "C" int DialogueWindow_previousPage(DialogueWindow *self);
extern "C" void *operator_new(unsigned size);
extern "C" void *operator_new_arr(unsigned size);
extern "C" void String_assign(String *self, StringSlot *other);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void ChoiceWindow_ctor(void *self);
extern "C" void TouchButton_ctor(void *self, String *text, int type, int x, int y, int width, int icon, int style);
extern "C" void ScrollTouchWindow_setText(void *self, StringSlot *style, StringSlot *text);
extern "C" void *ImageFactory_loadChar(void *self, int *parts);
extern "C" void DialogueWindow_set(DialogueWindow *self, Mission *mission, int kind, int campaign);
extern "C" int Agent_getRace(Agent *self);
extern "C" int Agent_isMale(Agent *self);
extern "C" int *Agent_getImageParts(Agent *self);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void Layout_drawMask(void *layout);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, StringSlot *title);
extern "C" void ScrollTouchWindow_draw(void *self);
extern "C" void ImageFactory_drawChar(void *self, void *parts, int x, int y, bool flipped);
extern "C" void ChoiceWindow_draw(void *self);
extern "C" void TouchButton_getPosition(Vec2 *out, void *self);

// ---- OnTouchBegin_168318.cpp ----
typedef int (*TouchHandler)(void *, int, int);

__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchBegin;

extern "C" int DialogueWindow_OnTouchBegin(DialogueWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_OnTouchBegin(F<void *>(self, 0x50), x, y);
    } else {
        ScrollTouchWindow_OnTouchBegin(F<void *>(self, 0x40), x, y);
        void *button = F<void *>(self, 0x0);
        TouchHandler fn = g_dw_touchButtonOnTouchBegin;
        fn(button, x, y);
        fn(F<void *>(self, 0x4), x, y);
        fn(F<void *>(self, 0x8), x, y);
    }
    return 0;
}

// ---- length_168044.cpp ----
__attribute__((visibility("hidden"))) extern int g_dw_briefingCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successCounts[];
__attribute__((visibility("hidden"))) extern void **g_dw_status;

extern "C" int DialogueWindow_length(DialogueWindow *self)
{
    Mission *mission = F<Mission *>(self, 0x4c);
    if (mission != 0 && Mission_isCampaignMission(mission) != 0) {
        int kind = F<int>(self, 0x44);
        int *counts;
        if (kind == 1) {
            counts = g_dw_successCounts;
        } else {
            if (kind != 0) return 1;
            counts = g_dw_briefingCounts;
        }
        return counts[F<int>(self, 0x10)] / 2;
    }
    if (F<int>(self, 0x44) == 0 && F<void *>(self, 0x4c) != 0 &&
        Mission_getTargetStation(F<Mission *>(self, 0x4c)) == 0x6c) {
        int result = 6;
        if (F<int>(*g_dw_status, 0x114) == 2) result = 0x12;
        return result;
    }
    return 1;
}

// ---- nextPage_16801e.cpp ----
extern "C" int DialogueWindow_nextPage(DialogueWindow *self)
{
    int page = F<int>(self, 0x48);
    int len = DialogueWindow_length(self);
    if (page < len - 1) {
        F<int>(self, 0x48) = F<int>(self, 0x48) + 1;
        DialogueWindow_loadContent(self);
        return 1;
    }
    return 0;
}

// ---- hasLevel_1680b6.cpp ----
extern "C" bool DialogueWindow_hasLevel(DialogueWindow *self)
{
    return F<void *>(self, 0x58) != 0;
}

// ---- _DialogueWindow_167118.cpp ----
extern "C" DialogueWindow *_ZN14DialogueWindowD2Ev(DialogueWindow *self)
{
    void *p = F<void *>(self, 0x0c);
    if (p != 0) {
        ArrayReleaseClasses_ImagePartPtr(p);
        p = F<void *>(self, 0x0c);
        if (p != 0) {
            operator_delete(Array_ImagePartPtr_dtor(p));
        }
    }
    F<void *>(self, 0x0c) = 0;

    p = F<void *>(self, 0x5c);
    if (p != 0) operator_delete_arr(p);
    F<void *>(self, 0x5c) = 0;

    p = F<void *>(self, 0x60);
    if (p != 0) operator_delete_arr(p);
    F<void *>(self, 0x60) = 0;

    p = F<void *>(self, 0x40);
    if (p != 0) operator_delete(ScrollTouchWindow_dtor(p));
    F<void *>(self, 0x40) = 0;

    p = F<void *>(self, 0x00);
    if (p != 0) operator_delete(TouchButton_dtor(p));
    F<void *>(self, 0x00) = 0;

    p = F<void *>(self, 0x04);
    if (p != 0) operator_delete(TouchButton_dtor(p));
    F<void *>(self, 0x04) = 0;

    p = F<void *>(self, 0x08);
    if (p != 0) operator_delete(TouchButton_dtor(p));
    F<void *>(self, 0x08) = 0;

    String_dtor((String *)((char *)self + 0x34));
    String_dtor((String *)((char *)self + 0x28));
    return self;
}

// ---- OnTouchMove_168370.cpp ----
typedef int (*TouchHandler)(void *, int, int);

__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchMove;

extern "C" int DialogueWindow_OnTouchMove(DialogueWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_OnTouchMove(F<void *>(self, 0x50), x, y);
    } else {
        ScrollTouchWindow_OnTouchMove(F<void *>(self, 0x40), x, y);
        void *button = F<void *>(self, 0x0);
        TouchHandler fn = g_dw_touchButtonOnTouchMove;
        fn(button, x, y);
        fn(F<void *>(self, 0x4), x, y);
        fn(F<void *>(self, 0x8), x, y);
    }
    return 0;
}

// ---- isFirstPage_1680ac.cpp ----
extern "C" bool DialogueWindow_isFirstPage(DialogueWindow *self)
{
    return F<void *>(self, 0x48) == 0;
}

// ---- previousPage_1680c4.cpp ----
extern "C" int DialogueWindow_previousPage(DialogueWindow *self)
{
    int page = F<int>(self, 0x48);
    if (page <= 0) return 0;
    F<int>(self, 0x48) = page - 1;
    DialogueWindow_loadContent(self);
    return 1;
}

// ---- isLastPage_168008.cpp ----
extern "C" bool DialogueWindow_isLastPage(DialogueWindow *self)
{
    int page = F<int>(self, 0x48);
    int len = DialogueWindow_length(self);
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

extern "C" void DialogueWindow_set(DialogueWindow *self, Mission *mission, int kind, int campaign)
{
    if (kind == 1) {
        F<Mission *>(self, 0x4c) = mission;
        F<int>(self, 0x44) = kind;
        goto won;
    }
    F<Mission *>(self, 0x4c) = mission;
    F<int>(self, 0x44) = kind;
    if (kind == 2) {
        Agent *agent = Mission_getAgent(mission);
        if (agent != 0 && Agent_isGenericAgent(agent) == 0) {
            Agent_setOfferAccepted(agent, false);
        }
        Mission_setFailed(mission, true);
        goto finish;
    }
    goto finish;

won:
    Mission_getAgent(mission);
    Mission_setWon(mission, true);

finish:
    F<int>(self, 0x48) = 0;
    if (campaign == -1) {
        campaign = Status_getCurrentCampaignMission(*g_dw_statusForSet);
    }
    F<int>(self, 0x10) = campaign;
    DialogueWindow_loadContent(self);
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

extern "C" void DialogueWindow_loadContent(DialogueWindow *self)
{
    StringSlot tmp;
    StringSlot style;

    void **gameText = g_dw_gameTextLoad;
    void **sound = g_dw_soundLoad;

    TouchButton_replaceTextKeepSize(F<void *>(self, 0x04), GameText_getText(*gameText, 0xb4));
    F<uint8_t>(self, 0x70) = 0;
    F<int>(self, 0x68) = 0;
    F<int>(self, 0x6c) = 0;
    FModSound_stop(*sound, F<int>(self, 0x64));
    F<int>(self, 0x64) = -1;
    TouchButton_setPressProgress(F<void *>(self, 0x04), 0);

    void *parts = F<void *>(self, 0x0c);
    if (parts != 0) {
        ArrayReleaseClasses_ImagePartPtr(parts);
        parts = F<void *>(self, 0x0c);
        if (parts != 0) operator_delete(Array_ImagePartPtr_dtor(parts));
    }
    F<void *>(self, 0x0c) = 0;

    Mission *mission = F<Mission *>(self, 0x4c);
    int kind = F<int>(self, 0x44);
    int page = F<int>(self, 0x48);
    int textId = -1;

    if (mission != 0 && Mission_isCampaignMission(mission) != 0) {
        int base;
        if (kind == 1) {
            base = F<int *>(self, 0x60)[F<int>(self, 0x10)];
            textId = g_dw_campaignSuccessTextIds[base + page * 2 + 1];
        } else if (kind == 0) {
            base = F<int *>(self, 0x5c)[F<int>(self, 0x10)];
            textId = g_dw_campaignBriefingTextIds[base + page * 2 + 1];
        } else {
            textId = 0x10 + 0x63d;
        }
        F<void *>(self, 0x24) = g_dw_defaultClientImage;
        String_assign((String *)((char *)self + 0x34), GameText_getText(*gameText, 0x63d + (textId & 0xff)));
        String_assign((String *)((char *)self + 0x28), GameText_getText(*gameText, textId));
    } else if (mission != 0) {
        if ((page & 1) != 0) {
            F<void *>(self, 0x24) = g_dw_defaultClientImage;
            String_assign((String *)((char *)self + 0x34), GameText_getText(*gameText, 0x63d));
            F<uint8_t>(self, 0x70) = 1;
        } else {
            F<void *>(self, 0x24) = Mission_getClientImage(mission);
            Mission_getClientName(&tmp, mission);
            String_assign_slot((String *)((char *)self + 0x34), &tmp);
            String_dtor(&tmp);
            F<uint8_t>(self, 0x70) = 0;
        }

        if (kind == 1 || kind == 0 || kind == 2) {
            Agent *agent = Mission_getAgent(mission);
            if (GameText_getLanguage() == 1 && agent != 0) {
                textId = DialogueWindow_pickGermanGenericTextBecauseWeSaved100EurosWithThat(self, kind, agent);
            } else {
                textId = 0x188 + AERandom_nextInt(*g_dw_randomLoad, 5);
            }
        } else {
            textId = 0x20f;
        }
        String_assign((String *)((char *)self + 0x28), GameText_getText(*gameText, textId));

        if (kind == 1) {
            void *standing = Status_getStanding(*g_dw_statusLoad);
            Standing_applyMissionCompleted(standing, Mission_getClientRace(mission));
        }
        if (Mission_getTargetStation(mission) == 0x6c && kind == 0) {
            textId = 0x1ca;
            String_assign((String *)((char *)self + 0x28), GameText_getText(*gameText, textId));
        }
        if (Mission_getType(mission) == 0x0c && kind == 0) {
            textId = 0x174;
            String_assign((String *)((char *)self + 0x28), GameText_getText(*gameText, textId));
        }
    } else {
        F<void *>(self, 0x24) = g_dw_defaultClientImage;
        textId = 0x10;
        String_assign((String *)((char *)self + 0x28), GameText_getText(*gameText, textId));
        String_assign((String *)((char *)self + 0x34), GameText_getText(*gameText, 0x63d));
    }

    String_ctor_literal(&style, g_dw_emptyLoad, false);
    String_ctor_copy(&tmp, (String *)((char *)self + 0x28), false);
    ScrollTouchWindow_setText(F<void *>(self, 0x40), &style, &tmp, 0);
    String_dtor(&tmp);
    String_dtor(&style);

    TouchButton_setVisible(F<void *>(self, 0x00), F<int>(self, 0x48) != 0);
    TouchButton_setVisible(F<void *>(self, 0x08), DialogueWindow_length(self) > 1);
    F<void *>(self, 0x0c) = ImageFactory_loadChar(*g_dw_imageFactoryLoad, F<void *>(self, 0x24));

    if (DialogueWindow_isLastPage(self) != 0) {
        TouchButton_replaceTextKeepSize(F<void *>(self, 0x04), GameText_getText(*gameText, 0xb5));
    }

    Agent *agent = mission == 0 ? (Agent *)0 : Mission_getAgent(mission);
    int soundId = Globals_getDialogueSoundId(*g_dw_globalsLoad, textId, agent);
    F<int>(self, 0x64) = soundId;
    if (soundId >= 0) {
        FModSound_play(*sound, soundId, 0, 0, 0);
        F<int>(self, 0x6c) = FModSound_getEventPauseLength(*sound, soundId);
    }
}

// ---- DialogueWindow_166f5c.cpp ----
extern "C" DialogueWindow *DialogueWindow_ctor_default(DialogueWindow *self)
{
    String_ctor((String *)((char *)self + 0x28));
    String_ctor((String *)((char *)self + 0x34));
    DialogueWindow_init(self);
    return self;
}

// ---- update_167f94.cpp ----
__attribute__((visibility("hidden"))) extern void *g_dw_soundConfig;
__attribute__((visibility("hidden"))) extern void **g_dw_fmodSound;

extern "C" void DialogueWindow_update(DialogueWindow *self, int dt)
{
    void *scroll = F<void *>(self, 0x40);
    if (scroll != 0) {
        ScrollTouchWindow_update(scroll, dt);
    }
    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_update(F<void *>(self, 0x50), dt);
    }
    if (F<uint8_t>(g_dw_soundConfig, 0xe) != 0 && F<int>(self, 0x64) != -1) {
        void **sound = g_dw_fmodSound;
        FModSound_getPlayingProgress(*sound, F<int>(self, 0x64));
        void *playingSound = *sound;
        int playingId = F<int>(self, 0x64);
        if (FModSound_isPlaying(playingSound, playingId) == 0 &&
            DialogueWindow_isLastPage(self) == 0) {
            int elapsed = F<int>(self, 0x68);
            if (elapsed >= F<int>(self, 0x6c)) {
                DialogueWindow_nextPage(self);
                elapsed = F<int>(self, 0x68);
            }
            F<int>(self, 0x68) = elapsed + dt;
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

extern "C" int DialogueWindow_OnTouchEnd(DialogueWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x54) != 0) {
        int r = ChoiceWindow_OnTouchEnd(F<void *>(self, 0x50), x, y);
        if (r == 1) {
            goto choice_close;
        }
        if (r != 0) goto choice_return_zero;
        F<uint8_t>(self, 0x54) = 0;
        if (Status_getCurrentCampaignMission(*g_dw_statusTouchEnd) == 0x0f) {
            void **sound = g_dw_soundChoice;
            FModSound_play(*sound, 0xa2, 0, 0, 0);
            FModSound_stop(*sound, F<int>(*sound, 0));
            FModSound_play(*sound, 0x88, 0, 0, 0);
        }
        if (F<int>(self, 0x64) != -1) {
            FModSound_stop(*g_dw_soundVoice, F<int>(self, 0x64));
        }
        return 1;

choice_close:
        F<uint8_t>(self, 0x54) = 0;
choice_return_zero:
        return 0;
    }

    ScrollTouchWindow_OnTouchEnd(F<void *>(self, 0x40), x, y);
    if (TouchButton_OnTouchEnd(F<void *>(self, 0x0), x, y) != 0) {
        FModSound_stop(*g_dw_soundPrev, F<int>(self, 0x64));
        DialogueWindow_previousPage(self);
    }
    if (TouchButton_OnTouchEnd(F<void *>(self, 0x4), x, y) != 0) {
        FModSound_stop(*g_dw_soundNext, F<int>(self, 0x64));
        if (DialogueWindow_nextPage(self) == 0) {
            return 1;
        }
    }
    if (TouchButton_OnTouchEnd(F<void *>(self, 0x8), x, y) != 0) {
        void *choice = F<void *>(self, 0x50);
        String *text = GameText_getText(*g_dw_gameTextTouchEnd, 0x18c);
        ChoiceWindow_set(choice, text, true);
        F<uint8_t>(self, 0x54) = 1;
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

extern "C" int DialogueWindow_init(DialogueWindow *self)
{
    StringSlot name;

    int *briefingOffsets = (int *)operator_new_arr(0x288);
    F<int *>(self, 0x5c) = briefingOffsets;
    int *successOffsets = (int *)operator_new_arr(0x288);
    F<int *>(self, 0x60) = successOffsets;

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
    String_assign((String *)((char *)self + 0x34), &name);
    String_dtor(&name);

    F<void *>(self, 0x4c) = 0;
    F<void *>(self, 0x50) = 0;
    F<void *>(self, 0x58) = 0;
    F<void *>(self, 0x24) = 0;
    F<void *>(self, 0x0c) = 0;
    F<int>(self, 0x64) = -1;
    F<int>(self, 0x68) = 0;
    F<uint8_t>(self, 0x70) = 0;

    void *layout = *g_dw_layoutInit;
    int frameW = F<int>(layout, 0x54);
    int frameH = F<int>(layout, 0x58);
    F<int>(self, 0x1c) = frameW;
    F<int>(self, 0x20) = frameH;
    int frameX = half_round_to_zero(*g_dw_screenWidth) - half_round_to_zero(frameW);
    int frameY = half_round_to_zero(*g_dw_screenHeight) - half_round_to_zero(frameH);
    F<int>(self, 0x14) = frameX;
    F<int>(self, 0x18) = frameY;

    void *scroll = operator_new(0x24);
    int margin = F<int>(layout, 0x4c);
    ScrollTouchWindow_ctor(scroll,
                           frameX + margin * 2 + F<int>(layout, 0x2d4),
                           F<int>(layout, 0x08) + frameY,
                           frameW - margin * 2 - F<int>(layout, 0x2d4),
                           frameH - margin * 2 - F<int>(layout, 0x08) - F<int>(layout, 0x30),
                           false);
    F<void *>(self, 0x40) = scroll;

    void *choice = operator_new(0x5c);
    ChoiceWindow_ctor(choice);
    F<void *>(self, 0x50) = choice;

    void **gameText = g_dw_gameTextInit;
    void *button = operator_new(0xc8);
    String *label = GameText_getText(*gameText, 0xb3);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 5,
                     F<int>(self, 0x14) + margin,
                     F<int>(self, 0x18) - margin + F<int>(self, 0x20),
                     F<int>(layout, 0x50), 0x21, 4);
    F<void *>(self, 0x00) = button;

    button = operator_new(0xc8);
    label = GameText_getText(*gameText, 0xb4);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 6,
                     F<int>(self, 0x14) + F<int>(self, 0x1c) - margin,
                     F<int>(self, 0x18) - margin + F<int>(self, 0x20),
                     F<int>(layout, 0x50), 0x22, 4);
    F<void *>(self, 0x04) = button;

    button = operator_new(0xc8);
    label = GameText_getText(*gameText, 0x18b);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 0,
                     F<int>(self, 0x14) + half_round_to_zero(F<int>(self, 0x1c)),
                     F<int>(self, 0x18) + F<int>(self, 0x20) - margin,
                     F<int>(self, 0x1c) - margin * 4 - F<int>(layout, 0x50) * 2,
                     0x24, 4);
    F<uint8_t>(self, 0x54) = 0;
    F<void *>(self, 0x08) = button;
    return 0;
}

// ---- DialogueWindow_166fa0.cpp ----
__attribute__((visibility("hidden"))) extern const char g_dw_emptyString[];
__attribute__((visibility("hidden"))) extern void **g_dw_imageFactoryCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutCtor;

extern "C" DialogueWindow *DialogueWindow_ctor_text(DialogueWindow *self, String *text, String *agentName, int *parts)
{
    StringSlot blank;
    StringSlot copy;

    String_ctor((String *)((char *)self + 0x28));
    String_ctor((String *)((char *)self + 0x34));
    DialogueWindow_init(self);

    void *scroll = F<void *>(self, 0x40);
    String_ctor_literal(&blank, g_dw_emptyString, false);
    String_ctor_copy(&copy, text, false);
    ScrollTouchWindow_setText(scroll, &blank, &copy);
    String_dtor(&copy);
    String_dtor(&blank);

    TouchButton_setVisible(F<void *>(self, 0x08), false);
    TouchButton_setVisible(F<void *>(self, 0x00), false);

    F<void *>(self, 0x0c) = ImageFactory_loadChar(*g_dw_imageFactoryCtor, parts);
    void *old = F<void *>(self, 0x04);
    if (old != 0) {
        operator_delete(TouchButton_dtor(old));
    }
    F<void *>(self, 0x04) = 0;

    void *button = operator_new(0xc8);
    String *buttonText = GameText_getText(*g_dw_gameTextCtor, 0x20c);
    void *layout = *g_dw_layoutCtor;
    int margin = F<int>(layout, 0x4c);
    int x = F<int>(self, 0x14) + F<int>(self, 0x1c) / 2;
    int y = F<int>(self, 0x18) + F<int>(self, 0x20) - margin;
    int width = F<int>(self, 0x1c) - margin * 4 - F<int>(layout, 0x50) * 2;
    TouchButton_ctor(button, buttonText, 0, x, y, width, 0x24, 4);
    F<void *>(self, 0x04) = button;

    String_assign((String *)((char *)self + 0x34), agentName);
    F<int>(self, 0x64) = -1;
    F<int>(self, 0x48) = 0;
    F<int>(self, 0x6c) = 0;
    return self;
}

// ---- DialogueWindow_166bfc.cpp ----
extern "C" DialogueWindow *DialogueWindow_ctor_mission(DialogueWindow *self, Mission *mission, Level *level, int kind)
{
    String_ctor((String *)((char *)self + 0x28));
    String_ctor((String *)((char *)self + 0x34));
    DialogueWindow_init(self);
    F<Level *>(self, 0x58) = level;
    DialogueWindow_set(self, mission, kind, -1);
    return self;
}

// ---- pickGermanGenericTextBecauseWeSaved100EurosWithThat_1680e0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_dw_random;
__attribute__((visibility("hidden"))) extern int g_dw_germanBriefingTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanSuccessTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanOtherTexts[];

extern "C" int DialogueWindow_pickGermanGenericTextBecauseWeSaved100EurosWithThat(
    DialogueWindow *self, int kind, Agent *agent)
{
    (void)self;
    int race = Agent_getRace(agent);
    int male;
    if (race < 10) {
        race = Agent_getRace(agent);
        male = Agent_isMale(agent);
        if (race == 3) {
            if (Agent_getImageParts(agent) == 0) {
                race = 3;
            } else {
                int *parts = Agent_getImageParts(agent);
                race = 0;
                if (*parts == 2) race = 3;
            }
        }
    } else {
        male = Agent_isMale(agent);
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

extern "C" void DialogueWindow_draw(DialogueWindow *self)
{
    StringSlot title;
    Vec2 pos;

    PaintCanvas_SetColor(*g_dw_paintCanvas, -1);
    void *layout = *g_dw_layoutDraw;
    Layout_drawMask(layout);
    String_ctor_copy(&title, (String *)((char *)self + 0x34), false);
    Layout_drawBox(layout, 7, F<int>(self, 0x14), F<int>(self, 0x18),
                   F<int>(self, 0x1c), F<int>(self, 0x20), &title);
    String_dtor(&title);

    ScrollTouchWindow_draw(F<void *>(self, 0x40));

    layout = *g_dw_layoutDraw;
    int margin = F<int>(layout, 0x4c);
    ImageFactory_drawChar(*g_dw_imageFactoryDraw, F<void *>(self, 0x0c),
                          F<int>(self, 0x14) + margin,
                          F<int>(self, 0x18) + margin + F<int>(layout, 0x08),
                          F<uint8_t>(self, 0x70));

    ButtonDraw drawButton = g_dw_touchButtonDraw;
    drawButton(F<void *>(self, 0x00));
    drawButton(F<void *>(self, 0x04));
    drawButton(F<void *>(self, 0x08));

    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_draw(F<void *>(self, 0x50));
    }

    if (*g_dw_drawPositionsReady == 0) {
        if (F<void *>(self, 0x08) != 0) {
            TouchButton_getPosition(&pos, F<void *>(self, 0x08));
            F<int>(g_dw_moreButtonX, 0x08) = (int)pos.x;
            TouchButton_getPosition(&pos, F<void *>(self, 0x08));
            F<int>(g_dw_moreButtonY, 0x08) = (int)pos.y;
        }
        if (F<void *>(self, 0x04) != 0) {
            TouchButton_getPosition(&pos, F<void *>(self, 0x04));
            F<int>(g_dw_nextButtonX, 0x0c) = (int)pos.x;
            TouchButton_getPosition(&pos, F<void *>(self, 0x04));
            F<int>(g_dw_nextButtonY, 0x0c) = (int)pos.y;
        }
        *g_dw_drawReadyFlag = 1;
    }
}
