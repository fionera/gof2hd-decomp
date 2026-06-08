# AbyssEngine::CameraSetPerspective
elf 0x713c0 body 140
Sig: undefined __stdcall CameraSetPerspective(float param_1, float param_2, float param_3, float param_4, Camera * param_5)

## decompile
```c

/* AbyssEngine::CameraSetPerspective(float, float, float, float, AbyssEngine::Camera*) */

float AbyssEngine::CameraSetPerspective
                (float param_1,float param_2,float param_3,float param_4,Camera *param_5)

{
  float fVar1;
  float fVar2;
  float in_r1;
  float in_r2;
  float in_r3;
  float *in_stack_00000000;
  
  if (in_stack_00000000 != (float *)0x0) {
    in_stack_00000000[1] = (float)param_5;
    in_stack_00000000[2] = in_r1;
    fVar1 = (float)AEMath::Sinf(in_r1);
    fVar2 = (float)AEMath::Cosf(*in_stack_00000000 * 0.5);
    fVar1 = fVar1 / fVar2;
    in_stack_00000000[0x12] = fVar1;
    in_stack_00000000[0x13] = (in_r2 / in_r3) * fVar1;
    in_stack_00000000[0x14] = in_r2 / in_r3;
    fVar1 = (float)AEMath::ATanf(fVar1);
    fVar1 = (float)AEMath::Cosf(fVar1);
    param_1 = 1.0 / fVar1;
    in_stack_00000000[0x15] = param_1;
  }
  return param_1;
}

```

## target disasm
```
  000813c0: push {r4,r6,r7,lr}
  000813c2: add r7,sp,#0x8
  000813c4: vpush {d8,d9,d10}
  000813c8: ldr r4,[r7,#0x8]
  000813ca: cbz r4,0x00081446
  000813cc: vmov.f32 s20,0x3f000000
  000813d0: vldr.32 s4,[r4]
  000813d4: vmov s2,r0
  000813d8: vmov s0,r1
  000813dc: vstr.32 s2,[r4,#0x4]
  000813e0: vmov s16,r3
  000813e4: vmov s18,r2
  000813e8: vstr.32 s0,[r4,#0x8]
  000813ec: vmul.f32 s4,s4,s20
  000813f0: vmov r0,s4
  000813f4: blx 0x0006f1a8
  000813f8: vldr.32 s0,[r4]
  000813fc: vmul.f32 s0,s0,s20
  00081400: vmov s20,r0
  00081404: vmov r1,s0
  00081408: mov r0,r1
  0008140a: blx 0x0006f49c
  0008140e: vmov s0,r0
  00081412: vdiv.f32 s2,s18,s16
  00081416: vdiv.f32 s0,s20,s0
  0008141a: vmul.f32 s4,s2,s0
  0008141e: vstr.32 s0,[r4,#0x48]
  00081422: vmov r0,s4
  00081426: vstr.32 s4,[r4,#0x4c]
  0008142a: vstr.32 s2,[r4,#0x50]
  0008142e: blx 0x0006f490
  00081432: blx 0x0006f49c
  00081436: vmov.f32 s0,0x3f800000
  0008143a: vmov s2,r0
  0008143e: vdiv.f32 s0,s0,s2
  00081442: vstr.32 s0,[r4,#0x54]
  00081446: vpop {d8,d9,d10}
  0008144a: pop {r4,r6,r7,pc}
```
