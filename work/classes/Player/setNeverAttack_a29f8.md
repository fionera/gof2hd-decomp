# Player::setNeverAttack
elf 0xa29f8 body 6
Sig: undefined __thiscall setNeverAttack(Player * this, bool param_1)

## decompile
```c

/* Player::setNeverAttack(bool) */

void __thiscall Player::setNeverAttack(Player *this,bool param_1)

{
  this[0xee] = (Player)param_1;
  return;
}

```
## target disasm
```
  000b29f8: strb.w r1,[r0,#0xee]
  000b29fc: bx lr
```
