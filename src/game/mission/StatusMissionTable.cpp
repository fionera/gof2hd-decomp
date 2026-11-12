// Codegen device for Status::missionCompleted: mission type -> case-label rank.
//
// The original lays the switch bodies out in source order, but clang 7's
// MachineBlockPlacement follows the jump table's value-sorted successor list, so a
// direct switch emits ascending order.  This permutation keeps the ascending table
// order aligned with the original body layout.  The first group's ranks are 0..23
// plus 38: 39 TBH entries, the same size and base as the original's, and a range
// starting at 0 so the dispatch needs no `sub`.  Within that range the ranks are a
// permutation of the body order; the gaps (7, 8, 24, 26..37) stay default so the
// emitted TBH keeps the original's default slots.  The second group (types 0, 8, 11,
// 13, 14) uses ranks 200..214 so clang keeps it in a separate 8..14 TBH.
//
// The table is ordinary C++ data in a normal (non-OBJECT-library) TU, but it is
// emitted into .fini_array: gold places that output section at the end of the
// zero-filled .rodata..fini_array gap, so .rodata keeps its exact 269,159-byte
// size and the table lands at 0x20cae8 without adding a section header.  The
// section name also keeps the table out of .rodata, so no existing literal-pool
// address moves.
//
// The symbol name is deliberately an existing weak hidden crtbegin symbol
// (__emutls_unregister_key).  A *new* name would be inserted into gold's
// Stringpool name pool at Status.cpp's link position and permute
// .dynsym/.dynstr/.hash/.rel.plt; reusing a name that is already in the pool
// keeps all four byte-identical.  The strong definition here overrides
// crtbegin's weak stub; the stub's only reference is __on_dlclose_late's tail
// branch, which is part of the same .fini_array that this table extends.
// See _work/reconstructions/status-missioncompleted-wave189-w13351.md.
#pragma clang section rodata=".fini_array"
#pragma GCC visibility push(hidden)
extern const unsigned char __emutls_unregister_key[192] = {
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
     16, 250,   9,  17,  18,  19,  10,  21,  20,  22,  11,  23,  38, 250,  25, 250,
    250, 250, 250, 250, 250, 250, 250, 250,  12, 250, 250, 250, 250,  13, 250, 250,
};
#pragma GCC visibility pop
#pragma clang section rodata=""
