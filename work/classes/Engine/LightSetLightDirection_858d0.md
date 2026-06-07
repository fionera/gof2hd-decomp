# Engine::LightSetLightDirection
elf 0x858d0 body 136
Sig: undefined __stdcall LightSetLightDirection(float param_1, float param_2, float param_3, uint param_4)

## decompile
```c

/* AbyssEngine::Engine::LightSetLightDirection(float, float, float, unsigned int) */

void AbyssEngine::Engine::LightSetLightDirection
               (float param_1,float param_2,float param_3,uint param_4)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  int in_stack_00000000;
  Vector local_34 [12];
  AEMath aAStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_00095958 + 0x958e4);
  local_1c = *piVar3;
  uVar2 = in_stack_00000000 - 0x4000;
  if (uVar2 < 8) {
    iVar1 = in_stack_00000000 + -0x3fff;
    if (in_stack_00000000 + -0x3fff < *(int *)(param_4 + 0x32c)) {
      iVar1 = *(int *)(param_4 + 0x32c);
    }
    *(int *)(param_4 + 0x32c) = iVar1;
    AEMath::VectorNormalize(aAStack_28,local_34);
    AEMath::Vector::operator=((Vector *)(param_4 + uVar2 * 0xc + 0x468),(Vector *)aAStack_28);
    *(undefined4 *)(param_4 + uVar2 * 4 + 0x378) = 0;
    param_1 = extraout_s0;
    param_2 = extraout_s1;
    param_3 = extraout_s2;
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_1,param_2,param_3);
  }
  return;
}

```

## target disasm
```
  000958d0: push {r4,r5,r6,r7,lr}
  000958d2: add r7,sp,#0xc
  000958d4: push.w r8
  000958d8: sub sp,#0x20
  000958da: ldr r5,[0x00095958]
  000958dc: mov r4,r0
  000958de: ldr r0,[r7,#0x8]
  000958e0: add r5,pc
  000958e2: ldr r6,[r5,#0x0]
  000958e4: ldr r5,[r6,#0x0]
  000958e6: str r5,[sp,#0x1c]
  000958e8: sub.w r5,r0,#0x4000
  000958ec: cmp r5,#0x7
  000958ee: bhi 0x00095944
  000958f0: vmov s2,r2
  000958f4: ldr.w r2,[r4,#0x32c]
  000958f8: vmov s4,r1
  000958fc: movw r1,#0x3fff
  00095900: subs r0,r0,r1
  00095902: add.w r8,sp,#0x10
  00095906: cmp r2,r0
  00095908: it gt
  0009590a: mov.gt r0,r2
  0009590c: vmov s0,r3
  00095910: add r1,sp,#0x4
  00095912: str.w r0,[r4,#0x32c]
  00095916: mov r0,r8
  00095918: vstr.32 s2,[sp,#0x8]
  0009591c: vstr.32 s4,[sp,#0x4]
  00095920: vstr.32 s0,[sp,#0xc]
  00095924: blx 0x0006ec80
  00095928: add.w r0,r5,r5, lsl #0x1
  0009592c: mov r1,r8
  0009592e: add.w r0,r4,r0, lsl #0x2
  00095932: add.w r0,r0,#0x468
  00095936: blx 0x0006eb3c
  0009593a: add.w r0,r4,r5, lsl #0x2
  0009593e: movs r1,#0x0
  00095940: str.w r1,[r0,#0x378]
  00095944: ldr r0,[sp,#0x1c]
  00095946: ldr r1,[r6,#0x0]
  00095948: subs r0,r1,r0
  0009594a: ittt eq
  0009594c: add.eq sp,#0x20
  0009594e: pop.eq.w r8
  00095952: pop.eq {r4,r5,r6,r7,pc}
  00095954: blx 0x0006e824
```
