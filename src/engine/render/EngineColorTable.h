#ifndef GOF2_ENGINE_COLOR_TABLE_H
#define GOF2_ENGINE_COLOR_TABLE_H
#include <cstdint>
#include <cstddef>

// EngineColorTable — the renderer's per-object color/state table that the engine exposes through
// a single hidden global base pointer (Level.cpp's g_engineColorBase, loaded from a .data slot in
// the binary). Level::setPlayerEngineColor() walks this table writing a packed engine-color word
// into each ship/object record; records are 0xa0 (0x28 uint32) bytes apart and the engine-color
// word lives at byte 0x1254 of the table base.
//
// Only the field that source actually touches is named here. The rest of the table is opaque
// engine-private layout, kept as a leading pad so the named field lands at its exact binary
// offset without disturbing anything else.
struct EngineColorTable {
    uint8_t field_0x0[0x1254]; // opaque engine-private records preceding the first entry
    uint32_t engineColorEntry0; // +0x1254: packed engine-color word (BBGGRR<<8 | 0xff) for entry 0
};

static_assert(offsetof(struct EngineColorTable, engineColorEntry0) == 0x1254,
              "engineColorEntry0 must live at table offset 0x1254");

#endif // GOF2_ENGINE_COLOR_TABLE_H
