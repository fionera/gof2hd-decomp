#ifndef GOF2_ENGINE_COLOR_TABLE_H
#define GOF2_ENGINE_COLOR_TABLE_H
#include <cstdint>
#include <cstddef>

struct EngineColorTable {
    uint8_t field_0x0[0x1254];
    uint32_t engineColorEntry0;
};

static_assert(offsetof(struct EngineColorTable, engineColorEntry0) == 0x1254,
              "engineColorEntry0 must live at table offset 0x1254");

#endif
