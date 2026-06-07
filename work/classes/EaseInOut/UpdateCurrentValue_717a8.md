# EaseInOut::UpdateCurrentValue
elf 0x717a8 body 96
Sig: undefined __thiscall UpdateCurrentValue(EaseInOut * this)

## decompile
```c

/* AbyssEngine::EaseInOut::UpdateCurrentValue() */

void __thiscall AbyssEngine::EaseInOut::UpdateCurrentValue(EaseInOut *this)

{
  float fVar1;
  
  if ((double)*(float *)(this + 8) == DAT_00081808) {
    fVar1 = *(float *)this + *(float *)(this + 4);
  }
  else {
    fVar1 = (float)AEMath::Sinf(*(float *)(this + 8));
    fVar1 = *(float *)this + (fVar1 * 0.5 + 0.5) * *(float *)(this + 4);
  }
  *(float *)(this + 0xc) = fVar1;
  return;
}

```

## target disasm
```
  000817a8: push {r4,r6,r7,lr}
  000817aa: add r7,sp,#0x8
  000817ac: vldr.32 s0,[r0,#0x8]
  000817b0: mov r4,r0
  000817b2: vldr.64 d16,[pc,#0x54]
  000817b6: vcvt.f64.f32 d17,s0
  000817ba: vcmpe.f64 d17,d16
  000817be: vmrs apsr,fpscr
  000817c2: bne 0x000817d2
  000817c4: vldr.32 s0,[r4]
  000817c8: vldr.32 s2,[r4,#0x4]
  000817cc: vadd.f32 s0,s0,s2
  000817d0: b 0x00081802
  000817d2: vmov r0,s0
  000817d6: blx 0x0006f1a8
  000817da: vmov s0,r0
  000817de: vldr.32 s2,[r4,#0x4]
  000817e2: vmov.f64 d17,0x3fe0000000000000
  000817e6: vcvt.f64.f32 d16,s0
  000817ea: vmla.f64 d17,d16,d17
  000817ee: vldr.32 s0,[r4]
  000817f2: vcvt.f64.f32 d16,s2
  000817f6: vcvt.f64.f32 d18,s0
  000817fa: vmla.f64 d18,d17,d16
  000817fe: vcvt.f32.f64 s0,d18
  00081802: vstr.32 s0,[r4,#0xc]
  00081806: pop {r4,r6,r7,pc}
```
