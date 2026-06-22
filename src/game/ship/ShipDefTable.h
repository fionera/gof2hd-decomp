#ifndef GOF2_SHIPDEFTABLE_H
#define GOF2_SHIPDEFTABLE_H
#include <cstdint>

// One row of the ship-definition table.
//
// The table lives behind the gValShipTable / gSnShipTable holder objects:
//   holder      = *(void**)gShipTable           // table object
//   defs        = *(int*)holder + 4             // base of the row-pointer array
//   row         = ((int*)defs)[0]               // first ship-def row (this struct)
//
// Every named field below is a 32-bit value read out of a single row and fed
// straight into Ship::makeShip / Item::makeItem / Item::makeItemQty as an id.
// The layout is a flat column table: offsets are sparse and the unnamed gaps
// are columns that the decompiled call-sites never touch.  Padding members
// only exist to keep each named field at its exact byte offset; do NOT renumber
// them, other translation units pin these offsets via raw casts.
//
// Some offsets are reached under more than one spelling at different call
// sites (a row id used both as a "ship def" and as an "item def"); those carry
// an anonymous-union alias so either name resolves to the same memory.
struct ShipDefTable {
    uint8_t  _pad_0x00[0x8];     // 0x00

    int32_t  itemDef_primary;    // 0x08  primary weapon item-def id

    uint8_t  _pad_0x0c[0x8];     // 0x0c
    union {                      // 0x14
        int32_t shipDefId;       //   read as ship-def id (defs[0]+0x14)
        int32_t itemDef_secondary; // read as secondary weapon item-def id
    };

    uint8_t  _pad_0x18[0x38];    // 0x18

    int32_t  itemDef_0x50;       // 0x50  item-def id

    uint8_t  _pad_0x54[0x24];    // 0x54

    int32_t  shipDefId_0x78;     // 0x78  ship-def id (Supernova start row)

    uint8_t  _pad_0x7c[0x14];    // 0x7c

    int32_t  itemDef_missile;    // 0x90  missile item-def id (makeItemQty)

    uint8_t  _pad_0x94[0x1c];    // 0x94

    int32_t  itemDef_0xb0;       // 0xb0  item-def id (makeItemQty)

    uint8_t  _pad_0xb4[0x18];    // 0xb4
    union {                      // 0xcc
        int32_t itemDef_equip0xcc;
        int32_t itemDef_0xcc;
    };

    uint8_t  _pad_0xd0[0x10];    // 0xd0

    int32_t  itemDef_0xe0;       // 0xe0  item-def id

    uint8_t  _pad_0xe4[0x2c];    // 0xe4

    int32_t  itemDef_0x110;      // 0x110 item-def id

    uint8_t  _pad_0x114[0x30];   // 0x114
    union {                      // 0x144
        int32_t itemDef_equip0x144;
        int32_t itemDef_0x144;
    };

    uint8_t  _pad_0x148[0xc];    // 0x148
    union {                      // 0x154
        int32_t itemDef_equip0x154;
        int32_t itemDef_0x154;
    };
    union {                      // 0x158
        int32_t itemDef_equip0x158;
        int32_t itemDef_0x158;
    };

    uint8_t  _pad_0x15c[0x8c];   // 0x15c

    int32_t  itemDef_cargo0x1e8;  // 0x1e8 cargo item-def id (makeItemQty)

    uint8_t  _pad_0x1ec[0xd4];   // 0x1ec

    int32_t  itemDef_0x2c0;      // 0x2c0 item-def id
};

static_assert(sizeof(struct ShipDefTable) == 0x2c4, "ShipDefTable row size");

#endif // GOF2_SHIPDEFTABLE_H
