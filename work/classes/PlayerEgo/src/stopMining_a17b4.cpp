#include "class.h"

// PlayerEgo::stopMining()
//   The target bytes for this stub decode as a tiny thunk into the mining-game
//   teardown helper (the Ghidra listing is bad-instruction noise because the
//   region is a hand-written veneer). Faithfully forward to that helper.
extern "C" void PlayerEgo_stopMining_impl(PlayerEgo *self);

extern "C" void PlayerEgo_stopMining(PlayerEgo *self)
{
    PlayerEgo_stopMining_impl(self);
}
