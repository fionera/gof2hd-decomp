# Player::regenerateShield
elf 0xa2e1a body 42
Sig: undefined __thiscall regenerateShield(Player * this)

## decompile
```c

/* Player::regenerateShield() */

void __thiscall Player::regenerateShield(Player *this)

{
  uint in_fpscr;
  float fVar1;
  
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x94),(byte)(in_fpscr >> 0x16) & 3);
  if ((int)((uint)(*(float *)(this + 0x88) + 1.0 < fVar1) << 0x1f) < 0) {
    fVar1 = *(float *)(this + 0x88) + 1.0;
  }
  *(float *)(this + 0x88) = fVar1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2e1a: vmov.f32 s0,0x3f800000
  000b2e1e: vldr.32 s2,[r0,#0x88]
  000b2e22: vldr.32 s4,[r0,#0x94]
  000b2e26: vcvt.f32.s32 s4,s4
  000b2e2a: vadd.f32 s0,s2,s0
  000b2e2e: vcmpe.f32 s0,s4
  000b2e32: vmrs apsr,fpscr
  000b2e36: it mi
  000b2e38: vmov.mi.f32 s4,s0
  000b2e3c: vstr.32 s4,[r0,#0x88]
  000b2e40: b.w 0x001abcd8
```
