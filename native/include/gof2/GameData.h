#ifndef GOF2_GAMEDATA_H
#define GOF2_GAMEDATA_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- GameData (top-level class, Android libgof2hdaa.so, armv7 Thumb).
// Constructor and destructor manage 8 embedded AbyssEngine::String members.


namespace AbyssEngine {

} // namespace AbyssEngine

struct GameData { void* _opaque; };  // no offset accesses observed
#endif
