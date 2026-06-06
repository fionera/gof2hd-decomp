# Player::setShieldHP
elf 0xa2cf6 body 38
Sig: undefined __stdcall setShieldHP(int param_1)

## decompile
```c

/* Player::setShieldHP(int) */

void Player::setShieldHP(int param_1)

{
  undefined4 in_r1;
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar2 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x94),(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_1 + 0x88) = fVar2;
  if (fVar1 < fVar2) {
    *(float *)(param_1 + 0x88) = fVar1;
  }
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2cf6: vldr.32 s0,[r0,#0x94]
  000b2cfa: vmov s2,r1
  000b2cfe: vcvt.f32.s32 s2,s2
  000b2d02: vcvt.f32.s32 s0,s0
  000b2d06: vstr.32 s2,[r0,#0x88]
  000b2d0a: vcmpe.f32 s2,s0
  000b2d0e: vmrs apsr,fpscr
  000b2d12: it gt
  000b2d14: vstr.gt.32 s0,[r0,#0x88]
  000b2d18: b.w 0x001abcd8
```
