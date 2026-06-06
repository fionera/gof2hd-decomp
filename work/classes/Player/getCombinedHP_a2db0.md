# Player::getCombinedHP
elf 0xa2db0 body 38
Sig: undefined __thiscall getCombinedHP(Player * this)

## decompile
```c

/* Player::getCombinedHP() */

int __thiscall Player::getCombinedHP(Player *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x8c),(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x78),(byte)(in_fpscr >> 0x16) & 3);
  return (int)(*(float *)(this + 0x88) + fVar2 + fVar1);
}

```
## target disasm
```
  000b2db0: vldr.32 s4,[r0,#0x8c]
  000b2db4: vldr.32 s0,[r0,#0x78]
  000b2db8: vldr.32 s2,[r0,#0x88]
  000b2dbc: vcvt.f32.s32 s4,s4
  000b2dc0: vcvt.f32.s32 s0,s0
  000b2dc4: vadd.f32 s2,s2,s4
  000b2dc8: vadd.f32 s0,s2,s0
  000b2dcc: vcvt.s32.f32 s0,s0
  000b2dd0: vmov r0,s0
  000b2dd4: bx lr
```
