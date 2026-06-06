# Player::damageShip
elf 0xa2f04 body 12
Sig: undefined __thiscall damageShip(Player * this, int param_1)

## decompile
```c

/* Player::damageShip(int) */

void __thiscall Player::damageShip(Player *this,int param_1)

{
  *(int *)(this + 0x78) =
       *(int *)(this + 0x78) - param_1 & ~(*(int *)(this + 0x78) - param_1 >> 0x1f);
  return;
}

```
## target disasm
```
  000b2f04: ldr r2,[r0,#0x78]
  000b2f06: subs r1,r2,r1
  000b2f08: bic.w r1,r1,r1, asr #0x1f
  000b2f0c: str r1,[r0,#0x78]
  000b2f0e: bx lr
```
