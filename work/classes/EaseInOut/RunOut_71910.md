# EaseInOut::RunOut
elf 0x71910 body 122
Sig: undefined __thiscall RunOut(EaseInOut * this, float param_1)

## decompile
```c

/* AbyssEngine::EaseInOut::RunOut(float) */

void __thiscall AbyssEngine::EaseInOut::RunOut(EaseInOut *this,float param_1)

{
  bool bVar1;
  byte bVar2;
  float in_r1;
  char cVar3;
  float fVar4;
  float fVar5;
  
  fVar5 = DAT_000819a8;
  fVar4 = *(float *)(this + 8);
  bVar2 = (byte)(((uint)(fVar4 == DAT_000819a8) << 0x1e) >> 0x18);
  cVar3 = -((char)((byte)(((uint)(fVar4 < DAT_000819a8) << 0x1f) >> 0x18) | bVar2) >> 7);
  if ((bool)(bVar2 >> 6) || (bool)cVar3 != (NAN(fVar4) || NAN(DAT_000819a8))) {
    if ((cVar3 == '\0') ||
       (fVar4 = (float)((double)fVar4 + (double)in_r1 * DAT_00081990 * DAT_00081998),
       *(float *)(this + 8) = fVar4, fVar4 <= fVar5)) goto LAB_001ab0e8;
  }
  else {
    fVar5 = (float)((double)fVar4 + (double)in_r1 * DAT_000819a0 * DAT_00081998);
    bVar1 = fVar5 < DAT_000819a8;
    *(float *)(this + 8) = fVar5;
    if (-1 < (int)((uint)bVar1 << 0x1f)) goto LAB_001ab0e8;
  }
  *(float *)(this + 8) = DAT_000819a8;
LAB_001ab0e8:
  (*(code *)(DAT_001ab0f4 + 0x1ab0f8))();
  return;
}

```

## target disasm
```
  00081910: vldr.32 s0,[pc,#0x94]
  00081914: vmov s4,r1
  00081918: vldr.32 s2,[r0,#0x8]
  0008191c: vcmpe.f32 s2,s0
  00081920: vmrs apsr,fpscr
  00081924: ble 0x00081952
  00081926: vcvt.f64.f32 d16,s4
  0008192a: vldr.64 d17,[pc,#0x74]
  0008192e: vldr.64 d18,[pc,#0x68]
  00081932: vmul.f64 d16,d16,d17
  00081936: vcvt.f64.f32 d17,s2
  0008193a: vmla.f64 d17,d16,d18
  0008193e: vcvt.f32.f64 s2,d17
  00081942: vcmpe.f32 s2,s0
  00081946: vstr.32 s2,[r0,#0x8]
  0008194a: vmrs apsr,fpscr
  0008194e: bmi 0x00081982
  00081950: b 0x00081986
  00081952: bpl 0x00081986
  00081954: vcvt.f64.f32 d16,s4
  00081958: vldr.64 d17,[pc,#0x34]
  0008195c: vldr.64 d18,[pc,#0x38]
  00081960: vmul.f64 d16,d16,d17
  00081964: vcvt.f64.f32 d17,s2
  00081968: vmla.f64 d17,d16,d18
  0008196c: vcvt.f32.f64 s2,d17
  00081970: vcmpe.f32 s2,s0
  00081974: vstr.32 s2,[r0,#0x8]
  00081978: vmrs apsr,fpscr
  0008197c: it le
  0008197e: b.le.w 0x001ab0e8
  00081982: ldr r1,[0x000819a8]
  00081984: str r1,[r0,#0x8]
  00081986: b.w 0x001ab0e8
```
