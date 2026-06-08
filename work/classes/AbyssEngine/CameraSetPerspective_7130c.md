# AbyssEngine::CameraSetPerspective
elf 0x7130c body 174
Sig: undefined __stdcall CameraSetPerspective(float param_1, float param_2, float param_3, float param_4, float param_5, Camera * param_6)

## decompile
```c

/* AbyssEngine::CameraSetPerspective(float, float, float, float, float, AbyssEngine::Camera*) */

float AbyssEngine::CameraSetPerspective
                (float param_1,float param_2,float param_3,float param_4,float param_5,
                Camera *param_6)

{
  float fVar1;
  float fVar2;
  undefined4 in_r1;
  float in_r2;
  float in_r3;
  code *pcVar3;
  float in_stack_00000000;
  undefined4 *in_stack_00000004;
  
  if (in_stack_00000004 != (undefined4 *)0x0) {
    *in_stack_00000004 = param_6;
    in_stack_00000004[1] = in_r1;
    in_stack_00000004[2] = in_r2;
    fVar1 = (float)AEMath::Sinf(in_r2);
    pcVar3 = *(code **)(DAT_000813bc + 0x8135c);
    fVar2 = (float)(*pcVar3)((float)param_6 * 0.5);
    in_stack_00000004[0x12] = fVar1 / fVar2;
    in_stack_00000004[0x13] = (in_r3 / in_stack_00000000) * (fVar1 / fVar2);
    in_stack_00000004[0x14] = in_r3 / in_stack_00000000;
    fVar1 = (float)(*pcVar3)((float)param_6 * 0.5);
    in_stack_00000004[0x16] = 1.0 / fVar1;
    AEMath::ATanf((float)in_stack_00000004[0x12] * (float)in_stack_00000004[0x14]);
    fVar1 = (float)(*pcVar3)();
    param_1 = 1.0 / fVar1;
    in_stack_00000004[0x15] = param_1;
  }
  return param_1;
}

```

## target disasm
```
  0008130c: push {r4,r5,r6,r7,lr}
  0008130e: add r7,sp,#0xc
  00081310: push.w r11
  00081314: vpush {d8,d9,d10}
  00081318: ldr r5,[r7,#0xc]
  0008131a: cmp r5,#0x0
  0008131c: beq 0x000813b0
  0008131e: vmov.f32 s6,0x3f000000
  00081322: vldr.32 s16,[r7,#0x8]
  00081326: vmov s4,r0
  0008132a: vmov s0,r2
  0008132e: vmov s2,r1
  00081332: vstr.32 s4,[r5]
  00081336: vmov s18,r3
  0008133a: vstr.32 s2,[r5,#0x4]
  0008133e: vstr.32 s0,[r5,#0x8]
  00081342: vmul.f32 s6,s4,s6
  00081346: vmov r4,s6
  0008134a: mov r0,r4
  0008134c: blx 0x0006f1a8
  00081350: ldr r1,[0x000813bc]
  00081352: vmov s20,r0
  00081356: mov r0,r4
  00081358: add r1,pc
  0008135a: ldr r6,[r1,#0x0]
  0008135c: blx r6
  0008135e: vmov s0,r0
  00081362: mov r0,r4
  00081364: vdiv.f32 s2,s18,s16
  00081368: vdiv.f32 s0,s20,s0
  0008136c: vmul.f32 s4,s2,s0
  00081370: vstr.32 s0,[r5,#0x48]
  00081374: vstr.32 s4,[r5,#0x4c]
  00081378: vstr.32 s2,[r5,#0x50]
  0008137c: blx r6
  0008137e: vldr.32 s0,[r5,#0x48]
  00081382: vmov.f32 s16,0x3f800000
  00081386: vldr.32 s2,[r5,#0x50]
  0008138a: vmov s4,r0
  0008138e: vmul.f32 s0,s0,s2
  00081392: vdiv.f32 s2,s16,s4
  00081396: vmov r0,s0
  0008139a: vstr.32 s2,[r5,#0x58]
  0008139e: blx 0x0006f490
  000813a2: blx r6
  000813a4: vmov s0,r0
  000813a8: vdiv.f32 s0,s16,s0
  000813ac: vstr.32 s0,[r5,#0x54]
  000813b0: vpop {d8,d9,d10}
  000813b4: pop.w r11
  000813b8: pop {r4,r5,r6,r7,pc}
```
