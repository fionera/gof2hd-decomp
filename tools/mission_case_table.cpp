// Codegen device for Status::missionCompleted: mission type -> case-label rank.
//
// The original lays the switch bodies out in source order, but clang 7's
// MachineBlockPlacement follows the jump table's value-sorted successor list, so a
// direct switch emits ascending order.  This permutation keeps the ascending table
// order aligned with the original body layout.
//
// The table lives in its own translation unit that is linked *after* every archive,
// so it is the last .rodata input: appending it does not shift any existing .rodata
// byte (and therefore no existing literal-pool address), and it needs no extra ELF
// section.  It lands at 0x20b977, the first byte past the original .rodata image.
// Hidden visibility keeps it out of .dynsym (a visible symbol would change the
// dynamic symbol table).  Status.cpp reads it through that fixed address rather
// than through this symbol: a cross-TU reference would add an undefined symbol to
// gold's table mid-link and reorder .dynsym.
// See _work/reconstructions/status-missioncompleted-wave110-w5810.md.
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192] = {
      0, 190, 190, 190, 190, 190, 190, 190,   8, 190, 190,  11, 190,  13,  14, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 151, 152, 153, 154, 155, 156, 163, 164, 157, 190,
    165, 190, 158, 166, 167, 168, 159, 150, 169, 170, 160, 171, 181, 190, 150, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 161, 190, 190, 190, 190, 162, 190, 190,
};
#pragma GCC visibility pop
