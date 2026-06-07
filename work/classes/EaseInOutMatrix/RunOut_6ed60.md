# EaseInOutMatrix::RunOut
elf 0x6ed60 body 108
Sig: undefined __thiscall RunOut(EaseInOutMatrix * this, float param_1)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::RunOut(float) */

void __thiscall AbyssEngine::EaseInOutMatrix::RunOut(EaseInOutMatrix *this,float param_1)

{
  byte bVar1;
  float in_r1;
  char cVar2;
  float fVar3;
  
  fVar3 = *(float *)(this + 0x74);
  bVar1 = (byte)(((uint)(fVar3 == 1.0) << 0x1e) >> 0x18);
  cVar2 = -((char)((byte)(((uint)(fVar3 < 1.0) << 0x1f) >> 0x18) | bVar1) >> 7);
  if ((bool)(bVar1 >> 6) || (bool)cVar2 != NAN(fVar3)) {
    if ((cVar2 == '\0') ||
       (fVar3 = fVar3 + (in_r1 * 0.5) / *(float *)(this + 0xf0), *(float *)(this + 0x74) = fVar3,
       fVar3 <= 1.0)) goto LAB_001ab0a8;
  }
  else {
    fVar3 = fVar3 + (in_r1 * -0.5) / *(float *)(this + 0xf0);
    *(float *)(this + 0x74) = fVar3;
    if (-1 < (int)((uint)(fVar3 < 1.0) << 0x1f)) goto LAB_001ab0a8;
  }
  *(undefined4 *)(this + 0x74) = 0x3f800000;
LAB_001ab0a8:
  (*(code *)(DAT_001ab0b4 + 0x1ab0b8))();
  return;
}

```

## target disasm
```
  0007ed60: vmov.f32 s0,0x3f800000
  0007ed64: vldr.32 s2,[r0,#0x74]
  0007ed68: vmov s4,r1
  0007ed6c: vcmpe.f32 s2,s0
  0007ed70: vmrs apsr,fpscr
  0007ed74: ble 0x0007ed9a
  0007ed76: vmov.f32 s6,0xbf000000
  0007ed7a: vmul.f32 s4,s4,s6
  0007ed7e: vldr.32 s6,[r0,#0xf0]
  0007ed82: vdiv.f32 s4,s4,s6
  0007ed86: vadd.f32 s2,s2,s4
  0007ed8a: vcmpe.f32 s2,s0
  0007ed8e: vstr.32 s2,[r0,#0x74]
  0007ed92: vmrs apsr,fpscr
  0007ed96: bmi 0x0007edc2
  0007ed98: b 0x0007edc8
  0007ed9a: bpl 0x0007edc8
  0007ed9c: vmov.f32 s6,0x3f000000
  0007eda0: vmul.f32 s4,s4,s6
  0007eda4: vldr.32 s6,[r0,#0xf0]
  0007eda8: vdiv.f32 s4,s4,s6
  0007edac: vadd.f32 s2,s2,s4
  0007edb0: vcmpe.f32 s2,s0
  0007edb4: vstr.32 s2,[r0,#0x74]
  0007edb8: vmrs apsr,fpscr
  0007edbc: it le
  0007edbe: b.le.w 0x001ab0a8
  0007edc2: mov.w r1,#0x3f800000
  0007edc6: str r1,[r0,#0x74]
  0007edc8: b.w 0x001ab0a8
```
