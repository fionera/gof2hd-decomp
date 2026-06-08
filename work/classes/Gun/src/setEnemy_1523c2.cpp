#include "class.h"

struct Player;

// ANOMALOUS INPUT: methods.tsv lists setEnemy at 0x1523c2 with body=8, but those 8 bytes
// disassemble (Thumb) to a *function prologue* (push {r4,r5,r6,r7,lr}; add r7,sp,#0xc;
// str.w fp,[sp,#-4]!) with no epilogue — i.e. the recorded boundary is a mis-identified /
// truncated function, not a leaf setter. The Ghidra decompile for it is flagged "bad
// instruction data". No 2-instruction C setter can match a 3-instruction prologue window;
// the natural setter (store enemy at +0xb4, matching getEnemies/removeAllEnemies) is kept.
extern "C" void Gun_setEnemy(Gun *self, Player *enemy)
{
    F<Player *>(self, 0xb4) = enemy;
}
