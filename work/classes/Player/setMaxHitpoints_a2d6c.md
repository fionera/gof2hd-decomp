# Player::setMaxHitpoints
elf 0xa2d6c body 10
Sig: undefined __thiscall setMaxHitpoints(Player * this, int param_1)

## decompile
```c

/* Player::setMaxHitpoints(int) */

void __thiscall Player::setMaxHitpoints(Player *this,int param_1)

{
  *(int *)(this + 0x78) = param_1;
  *(int *)(this + 0x84) = param_1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2d6c: str r1,[r0,#0x78]
  000b2d6e: str.w r1,[r0,#0x84]
  000b2d72: b.w 0x001abcd8
```
