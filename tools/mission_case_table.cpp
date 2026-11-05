// Codegen device for Status::missionCompleted: mission type -> case-label rank.
//
// The original lays the switch bodies out in source order, but clang 7's
// MachineBlockPlacement follows the jump table's value-sorted successor list, so a
// direct switch emits ascending order.  This permutation keeps the ascending table
// order aligned with the original body layout.  The first group's ranks are 0..23
// plus 39: 40 TBH entries, the same size and base as the original's, and a range
// starting at 0 so the dispatch needs no `sub`.  Within that range the ranks are a
// permutation of the body order; the gaps (7, 8, 24, 26..38) stay default so the
// emitted TBH keeps the original's default slots.  The second group (types 0, 8, 11,
// 13, 14) uses ranks 200..214 so clang keeps it in a separate 8..14 TBH.
//
// The table lives in its own translation unit that is linked *after* every archive,
// so it is the last .rodata input: appending it does not shift any existing .rodata
// byte (and therefore no existing literal-pool address), and it needs no extra ELF
// section.  It lands at 0x20b977, the first byte past the original .rodata image.
// Hidden visibility keeps it out of .dynsym (a visible symbol would change the
// dynamic symbol table).  Status.cpp reads it through that fixed address rather
// than through this symbol: a cross-TU reference would add an undefined symbol to
// gold's table mid-link and reorder .dynsym.
// See _work/reconstructions/status-missioncompleted-wave117-w6513.md,
// status-missioncompleted-wave110-w5810.md and
// status-missioncompleted-wave115-w6311.md.
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192] = {
    200, 250, 250, 250, 250, 250, 250, 250, 208, 250, 250, 211, 250, 213, 214, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250,   0,   1,   2,   3,   4,   5,  14,  15,   6, 250,
     16, 250,   9,  17,  18,  19,  10,  21,  20,  22,  11,  23,  39, 250,  25, 250,
    250, 250, 250, 250, 250, 250, 250, 250,  12, 250, 250, 250, 250,  13, 250, 250,
};
#pragma GCC visibility pop
