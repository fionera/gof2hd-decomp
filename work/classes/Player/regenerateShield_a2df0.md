# Player::regenerateShield
elf 0xa2df0 body 42
Sig: undefined __thiscall regenerateShield(Player * this, float param_1)

## decompile
```c

/* Player::regenerateShield(float) */

void __thiscall Player::regenerateShield(Player *this,float param_1)

{
  float in_r1;
  uint in_fpscr;
  float fVar1;
  
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x94),(byte)(in_fpscr >> 0x16) & 3);
  if ((int)((uint)(*(float *)(this + 0x88) + in_r1 < fVar1) << 0x1f) < 0) {
    fVar1 = *(float *)(this + 0x88) + in_r1;
  }
  *(float *)(this + 0x88) = fVar1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2df0: vldr.32 s0,[r0,#0x88]
  000b2df4: vmov s4,r1
  000b2df8: vldr.32 s2,[r0,#0x94]
  000b2dfc: vadd.f32 s0,s0,s4
  000b2e00: vcvt.f32.s32 s2,s2
  000b2e04: vcmpe.f32 s0,s2
  000b2e08: vmrs apsr,fpscr
  000b2e0c: it mi
  000b2e0e: vmov.mi.f32 s2,s0
  000b2e12: vstr.32 s2,[r0,#0x88]
  000b2e16: b.w 0x001abcd8
```
