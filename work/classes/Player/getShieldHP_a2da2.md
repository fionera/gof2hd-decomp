# Player::getShieldHP
elf 0xa2da2 body 14
Sig: undefined __thiscall getShieldHP(Player * this)

## decompile
```c

/* Player::getShieldHP() */

int __thiscall Player::getShieldHP(Player *this)

{
  return (int)*(float *)(this + 0x88);
}

```
## target disasm
```
  000b2da2: vldr.32 s0,[r0,#0x88]
  000b2da6: vcvt.s32.f32 s0,s0
  000b2daa: vmov r0,s0
  000b2dae: bx lr
```
