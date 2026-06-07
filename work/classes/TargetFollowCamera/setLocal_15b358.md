# TargetFollowCamera::setLocal
elf 0x15b358 body 118
Sig: undefined __stdcall setLocal(Matrix param_1)

## decompile
```c

/* TargetFollowCamera::setLocal(AbyssEngine::AEMath::Matrix) */

void TargetFollowCamera::setLocal
               (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
               undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
               undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  int *piVar1;
  undefined4 local_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  int local_28;
  
  piVar1 = *(int **)(DAT_0016b3d0 + 0x16b370);
  local_28 = *piVar1;
  local_58 = param_5;
  local_54 = param_6;
  local_50 = param_7;
  local_4c = param_8;
  local_48 = param_9;
  local_44 = param_10;
  uStack_40 = param_11;
  uStack_3c = param_12;
  local_38 = param_13;
  uStack_34 = param_14;
  local_30 = param_15;
  uStack_2c = param_16;
  local_64 = param_2;
  uStack_60 = param_3;
  local_5c = param_4;
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(param_1 + 0x13c),(Matrix *)&local_64);
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016b358: push {r4,r5,r6,r7,lr}
  0016b35a: add r7,sp,#0xc
  0016b35c: push {r8,r9,r10,r11}
  0016b360: sub sp,#0x44
  0016b362: ldr r4,[0x0016b3d0]
  0016b364: add.w r0,r0,#0x13c
  0016b368: ldrd r12,lr,[r7,#0x8]
  0016b36c: add r4,pc
  0016b36e: ldrd r5,r10,[r7,#0x10]
  0016b372: ldrd r8,r9,[r7,#0x18]
  0016b376: ldr r4,[r4,#0x0]
  0016b378: strd r1,r2,[sp,#0x4]
  0016b37c: ldr.w r11,[r7,#0x20]
  0016b380: ldr r1,[r4,#0x0]
  0016b382: str r3,[sp,#0xc]
  0016b384: str.w r12,[sp,#0x10]
  0016b388: str.w lr,[sp,#0x14]
  0016b38c: add.w lr,sp,#0x24
  0016b390: str r5,[sp,#0x18]
  0016b392: ldr.w r12,[r7,#0x24]
  0016b396: ldr r5,[r7,#0x2c]
  0016b398: ldr r3,[r7,#0x34]
  0016b39a: ldr r6,[r7,#0x28]
  0016b39c: ldr r2,[r7,#0x30]
  0016b39e: str.w r10,[sp,#0x1c]
  0016b3a2: str.w r8,[sp,#0x20]
  0016b3a6: stm lr,{r9,r11,r12}
  0016b3aa: strd r6,r5,[sp,#0x30]
  0016b3ae: strd r2,r3,[sp,#0x38]
  0016b3b2: str r1,[sp,#0x40]
  0016b3b4: add r1,sp,#0x4
  0016b3b6: blx 0x0006f148
  0016b3ba: ldr r0,[sp,#0x40]
  0016b3bc: ldr r1,[r4,#0x0]
  0016b3be: subs r0,r1,r0
  0016b3c0: ittt eq
  0016b3c2: add.eq sp,#0x44
  0016b3c4: pop.eq.w {r8,r9,r10,r11}
  0016b3c8: pop.eq {r4,r5,r6,r7,pc}
  0016b3ca: blx 0x0006e824
```
