#include "class.h"

extern "C" String *GameText_getText(void *self, int id);
extern "C" void TouchButton_replaceTextKeepSize(void *self, String *text);
extern "C" void TouchButton_setPressProgress(void *self, int progress);
extern "C" void TouchButton_setVisible(void *self, bool visible);
extern "C" void ArrayReleaseClasses_ImagePartPtr(void *self);
extern "C" void *Array_ImagePartPtr_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" int FModSound_stop(void *self, int sound);
extern "C" void FModSound_play(void *self, int sound, void *pos, int volume, int extra);
extern "C" int FModSound_getEventPauseLength(void *self, int sound);
extern "C" int Mission_isCampaignMission(Mission *self);
extern "C" int Mission_getType(Mission *self);
extern "C" int Mission_getTargetStation(Mission *self);
extern "C" Agent *Mission_getAgent(Mission *self);
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
extern "C" int DialogueWindow_length(DialogueWindow *self);
extern "C" int DialogueWindow_isLastPage(DialogueWindow *self);
extern "C" int DialogueWindow_pickGermanGenericTextBecauseWeSaved100EurosWithThat(DialogueWindow *self, int kind, Agent *agent);
extern "C" int Globals_getDialogueSoundId(void *self, int textId, Agent *agent);

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
