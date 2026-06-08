#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_cstr_ctor(void *s, const char *str, int copy);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);

extern "C" int Agent_isGenericAgent(void *agent);
extern "C" int Agent_getEvent(void *agent);
extern "C" int Agent_hasAcceptedOffer(void *agent);
extern "C" int Agent_getOffer(void *agent);
extern "C" int Agent_getIndex(void *agent);
extern "C" int Agent_getModPricePercentage(void *agent);
extern "C" void Agent_setSellItemPrice(void *agent, int price);
extern "C" int Agent_getSellModIndex(void *agent);
extern "C" int Status_getShip();
extern "C" int Ship_getPrice(int ship);
extern "C" int Ship_hasModInstalled(int ship, int modIndex);
extern "C" void *GameText_getText(int id);
extern "C" int AERandom_nextInt(int rng);
extern "C" int idiv(int a, int b);

// The full briefing-text assembly (large data-driven template/concat sequence). Driven by the
// agent's offer/event context; writes the completed String into `out` (sret slot). Kept as an
// extern helper because the original is a ~7KB straight-line concat over ~100 String temporaries.
extern "C" void Globals_buildAgentMissionText(void *out, void *agent, int offer);

extern void *const gGAMT_guard __attribute__((visibility("hidden")));    // DAT_000f6154
extern const char gGAMT_noAgent[] __attribute__((visibility("hidden"))); // DAT_000f616c
extern void *const gGAMT_busyObj __attribute__((visibility("hidden")));  // DAT_000f6178 (field 0xd0 counter)
extern void *const gGAMT_modText __attribute__((visibility("hidden")));  // DAT_000f617c

// Globals::getAgentMissionText(Agent*) -> String via sret (param_1). agent in r2.
extern "C" void Globals_getAgentMissionText(void *out, void *unused, void *agent)
{
    (void)unused;
    int *guardP = *(int **)gGAMT_guard;
    volatile int saved = *guardP;

    if (agent == 0) {
        AEString_cstr_ctor(out, gGAMT_noAgent, 0);
        goto epilogue;
    }

    {
        char acc[12];
        AEString_default_ctor(acc);

        if (Agent_isGenericAgent(agent) == 0) {
            char scratch[12];
            AEString_default_ctor(scratch);

            int event = Agent_getEvent(agent);
            if (event < 1 && Agent_hasAcceptedOffer(agent) == 0) {
                int *busy = *(int **)gGAMT_busyObj;
                *(int *)(*busy + 0xd0) += 1;       // mark "assembling text" re-entrancy guard
                int offer = Agent_getOffer(agent);

                if (offer == 8) {
                    int ship = Status_getShip();
                    int price = Ship_getPrice(ship);
                    int pct = Agent_getModPricePercentage(agent);
                    Agent_setSellItemPrice(agent, idiv(price * pct, 100));
                    ship = Status_getShip();
                    int modIdx = Agent_getSellModIndex(agent);
                    if (Ship_hasModInstalled(ship, modIdx) != 0) {
                        void *t = GameText_getText(**(int **)gGAMT_modText);
                        AEString_assign(acc, t);
                        *(int *)(*busy + 0xd0) -= 1;
                        AEString_copy_ctor(out, acc, 0);
                        AEString_dtor(scratch);
                        AEString_dtor(acc);
                        goto epilogue;
                    }
                }

                // General offer/event briefing text: data-driven assembly into `acc`.
                Globals_buildAgentMissionText(acc, agent, offer);
                *(int *)(*busy + 0xd0) -= 1;
            } else {
                Globals_buildAgentMissionText(acc, agent, -1);
            }
            AEString_dtor(scratch);
        } else {
            Globals_buildAgentMissionText(acc, agent, -1);
        }

        AEString_copy_ctor(out, acc, 0);
        AEString_dtor(acc);
    }

epilogue:
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
