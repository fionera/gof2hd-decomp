#include "class.h"

extern "C" int Agent_getOffer(void *agent);
extern "C" void *Agent_getMission(void *agent);
extern "C" int Agent_isKnown(void *agent);
extern "C" int Agent_isStoryAgent(void *agent);
extern "C" void Agent_setKnown(void *agent, bool known);
extern "C" void Agent_setEvent(void *agent, int event);
extern "C" int Agent_isGenericAgent(void *agent);
extern "C" void *GameText_getText(void *texts, int textId);
extern "C" void String_ctor_copy(void *dst, void *src, bool copy);
extern "C" void String_dtor(void *s);
extern "C" void ChoiceWindow_setText(void *choice, void *title, void *body);
extern "C" void ChoiceWindow_setButtonText(void *choice, void *left, void *right);
extern "C" int SpaceLounge_getSoundId(SpaceLounge *self, void *agent);

extern "C" void *SpaceLounge_start_text_slot;

extern "C" void SpaceLounge_startChat(SpaceLounge *self)
{
    char title[12];
    char body[12];
    char left[12];
    char right[12];
    char scratch[1900];
    (void)scratch;

    if (P(self, 0x24) == 0 || I(self, 0x20) < 0) {
        return;
    }

    void *agent = ((void **)P(P(self, 0x24), 0x4))[I(self, 0x20)];
    int offer = Agent_getOffer(agent);
    void *mission = Agent_getMission(agent);
    void *texts = *(void **)&SpaceLounge_start_text_slot;

    int titleId = 0x100;
    int bodyId = 0x101;
    if (mission != 0) {
        titleId = 0x120;
        bodyId = 0x121;
    } else if (offer == 1) {
        titleId = 0x130;
        bodyId = 0x131;
    } else if (offer == 6) {
        titleId = 0x132;
        bodyId = 0x133;
    }

    String_ctor_copy(title, GameText_getText(*(void **)texts, titleId), false);
    String_ctor_copy(body, GameText_getText(*(void **)texts, bodyId), false);
    String_ctor_copy(left, GameText_getText(*(void **)texts, 0x10), false);
    String_ctor_copy(right, GameText_getText(*(void **)texts, 0x11), false);

    ChoiceWindow_setText(P(self, 0x8), title, body);
    ChoiceWindow_setButtonText(P(self, 0x8), left, right);

    if (Agent_isKnown(agent) == 0 && Agent_isStoryAgent(agent) == 0) {
        Agent_setKnown(agent, true);
    }
    SpaceLounge_getSoundId(self, agent);

    UC(self, 0x1b) = 1;
    UC(self, 0x19) = 1;
    I(self, 0x14) = 1;

    if (offer == 1) {
        I(self, 0x14) = 3;
    } else if (Agent_isGenericAgent(agent) != 0) {
        Agent_setEvent(agent, 1);
    }

    String_dtor(right);
    String_dtor(left);
    String_dtor(body);
    String_dtor(title);
}
