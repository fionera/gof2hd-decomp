# Layout::getPulseValue
elf 0xd4bf0 body 108
Sig: undefined __thiscall getPulseValue(Layout * this, float param_1)

## decompile
```c

/* Layout::getPulseValue(float) */

float __thiscall Layout::getPulseValue(Layout *this,float param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float in_r1;
  
  Status::getPlayingTime();
  fVar1 = (float)__aeabi_l2f();
  fVar2 = (float)AbyssEngine::AEMath::Sinf(fVar1 * in_r1);
  Status::getPlayingTime();
  fVar1 = (float)__aeabi_l2f();
  fVar3 = (float)AbyssEngine::AEMath::Sinf(fVar1 * in_r1);
  fVar1 = -fVar3;
  if (0.0 < fVar2) {
    fVar1 = fVar3;
  }
  return fVar1;
}

```

## target disasm
```
  000e4bf0: push {r4,r5,r7,lr}
  000e4bf2: add r7,sp,#0x8
  000e4bf4: vpush {d8}
  000e4bf8: ldr r0,[0x000e4c5c]
  000e4bfa: vmov s16,r1
  000e4bfe: add r0,pc
  000e4c00: ldr r5,[r0,#0x0]
  000e4c02: ldr r0,[r5,#0x0]
  000e4c04: blx 0x00071710
  000e4c08: blx 0x0006f7d8
  000e4c0c: vmov s0,r0
  000e4c10: vmul.f32 s0,s0,s16
  000e4c14: vmov r0,s0
  000e4c18: blx 0x0006f1a8
  000e4c1c: mov r4,r0
  000e4c1e: ldr r0,[r5,#0x0]
  000e4c20: blx 0x00071710
  000e4c24: blx 0x0006f7d8
  000e4c28: vmov s0,r0
  000e4c2c: vmul.f32 s0,s0,s16
  000e4c30: vmov r0,s0
  000e4c34: blx 0x0006f1a8
  000e4c38: vmov s4,r4
  000e4c3c: vmov s0,r0
  000e4c40: vcmpe.f32 s4,#0
  000e4c44: vmrs apsr,fpscr
  000e4c48: vneg.f32 s2,s0
  000e4c4c: it gt
  000e4c4e: vmov.gt.f32 s2,s0
  000e4c52: vmov r0,s2
  000e4c56: vpop {d8}
  000e4c5a: pop {r4,r5,r7,pc}
```
