#include "class.h"

extern "C" void ChoiceWindow_left(void *choice);
extern "C" void ChoiceWindow_right(void *choice);
extern "C" void SpaceLounge_startChat(SpaceLounge *self);
extern "C" int Agent_getOffer(void *agent);
extern "C" void *Agent_getMission(void *agent);
extern "C" void Agent_setEvent(void *agent, int event);
extern "C" int Agent_isGenericAgent(void *agent);
extern "C" void ScrollTouchWindow_scroll(void *scroll, int amount);

static inline void *key_agent(SpaceLounge *self)
{
    return ((void **)P(self->f_24, 0x4))[I(self, 0x20)];
}

extern "C" void SpaceLounge_onKeyPress(SpaceLounge *self, int key)
{
    char scratch[960];
    (void)scratch;

    if (UC(self, 0x1b) != 0) {
        if (key == 0x1000) {
            ChoiceWindow_left(self->f_8);
        } else if (key == 0x2000) {
            ChoiceWindow_right(self->f_8);
        } else if (key == 0x10000) {
            UC(self, 0x1b) = 0;
        }
        return;
    }

    int mode = I(self, 0x14);
    if (mode == 0) {
        if (key == 0x1000 || key == 0x8000) {
            unsigned next = I(self, 0x20) + 1;
            if (self->f_24 != 0 && next >= U(self->f_24, 0x0)) {
                next = 0;
            }
            I(self, 0x20) = next;
        } else if (key == 0x2000 || key == 0x4000) {
            int current = I(self, 0x20);
            if (current < 1 && self->f_24 != 0) {
                current = U(self->f_24, 0x0);
            }
            I(self, 0x20) = current - 1;
        } else if (key == 0x10000 || key == 0x20000) {
            SpaceLounge_startChat(self);
        }
        return;
    }

    if (mode == 1 || mode == 3) {
        if (key == 0x10000 || key == 0x20000) {
            I(self, 0x30) = 0;
            void *agent = key_agent(self);
            if (Agent_getOffer(agent) == 1) {
                I(self, 0x14) = 2;
            } else {
                I(self, 0x14) = mode == 1 ? 2 : 0;
            }
        } else if (key == 0x40000) {
            if (I(self, 0x2c) < 3) {
                if (mode == 1) {
                    I(self, 0x2c) = 0;
                    I(self, 0x14) = 0;
                }
            } else {
                I(self, 0x2c) -= 0x28;
            }
        }
        return;
    }

    if (mode == 2) {
        void *agent = key_agent(self);
        if (key == 0x40000) {
            I(self, 0x14) = 1;
        } else if (key == 0x10000 || key == 0x20000) {
            if (Agent_getMission(agent) == 0 && Agent_isGenericAgent(agent) != 0) {
                Agent_setEvent(agent, 1);
            }
            I(self, 0x14) = 0;
        } else if (key == 0x8000) {
            ScrollTouchWindow_scroll(self->f_60, 1);
        } else if (key == 0x4000) {
            ScrollTouchWindow_scroll(self->f_60, -1);
        }
    }
}
