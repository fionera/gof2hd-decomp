#include "gof2/engine/core/GameData.h"

// ---- GameData_d0c6c.cpp / _GameData_d06f4.cpp ----
// GameData manages 8 embedded AbyssEngine::String members. These are now real
// named members of the struct, so their construction/destruction is handled
// automatically by the compiler-generated member init/teardown.

GameData::GameData()
{
    // field_0x18, field_0x24, field_0x30, field_0x7c, field_0x88,
    // field_0x94, field_0xa8, field_0xb8 are default-constructed as members.
}

GameData::~GameData()
{
    // The 8 embedded String members are destroyed automatically (reverse order).
}
