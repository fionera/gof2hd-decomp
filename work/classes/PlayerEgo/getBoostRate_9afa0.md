# PlayerEgo::getBoostRate
elf 0x9afa0 body 48
Sig: undefined __thiscall getBoostRate(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getBoostRate() */

float __thiscall PlayerEgo::getBoostRate(PlayerEgo *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  float fVar3;
  
  fVar1 = (float)VectorSignedToFloat(this[5].field_1C,(byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat(this[8].field_18,(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = fVar2 / fVar1 + 1.0;
  if (0.0 < fVar2 / fVar1) {
    fVar3 = 1.0;
  }
  return fVar3;
}

```

## target disasm
```
  000aafa0: vldr.32 s0,[r0,#0xd0]
  000aafa4: vldr.32 s2,[r0,#0x138]
  000aafa8: vcvt.f32.s32 s0,s0
  000aafac: vcvt.f32.s32 s2,s2
  000aafb0: vdiv.f32 s0,s2,s0
  000aafb4: vcmpe.f32 s0,#0
  000aafb8: vmrs apsr,fpscr
  000aafbc: vmov.f32 s2,0x3f800000
  000aafc0: vadd.f32 s4,s0,s2
  000aafc4: it gt
  000aafc6: vmov.gt.f32 s4,s2
  000aafca: vmov r0,s4
  000aafce: bx lr
```
