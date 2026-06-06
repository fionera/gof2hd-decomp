# Player::setPlayShootSound
elf 0xa2a56 body 10
Sig: undefined __thiscall setPlayShootSound(Player * this, bool param_1, int param_2)

## decompile
```c

/* Player::setPlayShootSound(bool, int) */

void __thiscall Player::setPlayShootSound(Player *this,bool param_1,int param_2)

{
  *(int *)(this + 0x10c) = param_2;
  this[0x70] = (Player)param_1;
  return;
}

```
## target disasm
```
  000b2a56: str.w r2,[r0,#0x10c]
  000b2a5a: strb.w r1,[r0,#0x70]
  000b2a5e: bx lr
```
