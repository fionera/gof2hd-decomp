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
// section.  Hidden visibility keeps it out of .dynsym.
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
