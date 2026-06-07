# PlayerEgo::getCloakingPercentage
elf 0x9c170 body 118
Sig: undefined __thiscall getCloakingPercentage(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getCloakingPercentage() */

float __thiscall PlayerEgo::getCloakingPercentage(PlayerEgo *this)

{
  int *piVar1;
  uint in_fpscr;
  float fVar2;
  float fVar3;
  
  fVar2 = DAT_000ac1e8;
  if (((char)this[0xb].field_20 != '\0') && (piVar1 = this[0xe].field_10, -1 < (int)piVar1)) {
    if ((int)piVar1 < 2000) {
      fVar2 = (float)VectorSignedToFloat(piVar1,(byte)(in_fpscr >> 0x16) & 3);
      fVar2 = (fVar2 * DAT_000ac1ec) / DAT_000ac1f4;
    }
    else {
      fVar2 = DAT_000ac1ec;
      if ((int)(this[0xe].field_18 + -500) < (int)piVar1) {
        fVar2 = (float)VectorSignedToFloat(this[0xe].field_18 + -500,(byte)(in_fpscr >> 0x16) & 3);
        fVar3 = (float)VectorSignedToFloat(piVar1,(byte)(in_fpscr >> 0x16) & 3);
        fVar2 = ((fVar3 - fVar2) / DAT_000ac1f0 + 1.0) * DAT_000ac1ec;
      }
    }
  }
  return fVar2;
}

```

## target disasm
```
  000ac170: ldrb.w r1,[r0,#0x1ac]
  000ac174: vldr.32 s0,[pc,#0x70]
  000ac178: cbz r1,0x000ac1e0
  000ac17a: ldr.w r1,[r0,#0x208]
  000ac17e: cmp r1,#0x0
  000ac180: blt 0x000ac1e0
  000ac182: cmp.w r1,#0x7d0
  000ac186: bge 0x000ac1a2
  000ac188: vmov s0,r1
  000ac18c: vldr.32 s2,[pc,#0x5c]
  000ac190: vcvt.f32.s32 s0,s0
  000ac194: vmul.f32 s0,s0,s2
  000ac198: vldr.32 s2,[pc,#0x58]
  000ac19c: vdiv.f32 s0,s0,s2
  000ac1a0: b 0x000ac1e0
  000ac1a2: ldr.w r0,[r0,#0x210]
  000ac1a6: sub.w r0,r0,#0x7d0
  000ac1aa: cmp r1,r0
  000ac1ac: ble 0x000ac1dc
  000ac1ae: vmov s0,r0
  000ac1b2: vmov s2,r1
  000ac1b6: vcvt.f32.s32 s0,s0
  000ac1ba: vcvt.f32.s32 s2,s2
  000ac1be: vsub.f32 s0,s2,s0
  000ac1c2: vldr.32 s2,[pc,#0x2c]
  000ac1c6: vdiv.f32 s0,s0,s2
  000ac1ca: vmov.f32 s2,0x3f800000
  000ac1ce: vadd.f32 s0,s0,s2
  000ac1d2: vldr.32 s2,[pc,#0x18]
  000ac1d6: vmul.f32 s0,s0,s2
  000ac1da: b 0x000ac1e0
  000ac1dc: vldr.32 s0,[pc,#0xc]
  000ac1e0: vmov r0,s0
  000ac1e4: bx lr
```
