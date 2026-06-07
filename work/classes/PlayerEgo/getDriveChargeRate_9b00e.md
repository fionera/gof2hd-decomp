# PlayerEgo::getDriveChargeRate
elf 0x9b00e body 44
Sig: undefined __thiscall getDriveChargeRate(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getDriveChargeRate() */

float __thiscall PlayerEgo::getDriveChargeRate(PlayerEgo *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  float fVar3;
  
  fVar2 = (float)VectorSignedToFloat(this[0xe].field_8,(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(this[0xe].field_4,(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = 1.0;
  if (0.0 <= fVar1 / fVar2) {
    fVar3 = fVar1 / fVar2;
  }
  return fVar3;
}

```

## target disasm
```
  000ab00e: vldr.32 s0,[r0,#0x1fc]
  000ab012: vldr.32 s2,[r0,#0x200]
  000ab016: vcvt.f32.s32 s2,s2
  000ab01a: vcvt.f32.s32 s0,s0
  000ab01e: vdiv.f32 s0,s0,s2
  000ab022: vcmpe.f32 s0,#0
  000ab026: vmrs apsr,fpscr
  000ab02a: vmov.f32 s2,0x3f800000
  000ab02e: it ge
  000ab030: vmov.ge.f32 s2,s0
  000ab034: vmov r0,s2
  000ab038: bx lr
```
