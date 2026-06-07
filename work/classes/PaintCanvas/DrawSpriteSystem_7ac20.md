# PaintCanvas::DrawSpriteSystem
elf 0x7ac20 body 362
Sig: undefined __stdcall DrawSpriteSystem(uint param_1, Matrix param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawSpriteSystem(unsigned int, AbyssEngine::AEMath::Matrix) */

void AbyssEngine::PaintCanvas::DrawSpriteSystem
               (char *param_1,uint param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
               undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
               undefined4 param_10,undefined4 param_11,undefined4 param_12,undefined4 param_13,
               undefined4 param_14,undefined4 param_15,undefined4 param_16,undefined4 param_17)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  Engine *pEVar6;
  Matrix *pMVar7;
  int *piVar8;
  float extraout_s0;
  AEMath aAStack_dc [60];
  undefined4 local_a0;
  uint local_9c;
  uint local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  
  uVar2 = DAT_0008ad98;
  uVar1 = DAT_0008ad90;
  piVar8 = *(int **)(DAT_0008ada8 + 0x8ac34);
  local_28 = *piVar8;
  if ((param_2 < *(uint *)(param_1 + 0x180)) &&
     (*(int *)(*(int *)(param_1 + 0x184) + param_2 * 4) != 0)) {
    local_5c = param_5;
    uStack_58 = param_6;
    uStack_54 = param_7;
    uStack_50 = param_8;
    local_4c = param_9;
    uStack_48 = param_10;
    uStack_44 = param_11;
    uStack_40 = param_12;
    local_3c = param_13;
    local_38 = param_14;
    local_34 = param_15;
    local_30 = param_16;
    local_2c = param_17;
    local_64 = param_3;
    uStack_60 = param_4;
    if (*param_1 == '\0') {
      pMVar7 = (Matrix *)&local_a0;
      AEMath::MatrixGetInverse(pMVar7);
    }
    else {
      uStack_84 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uStack_80 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_7c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar3 = (undefined4 *)((uint)&local_a0 | 4);
      *puVar3 = 0;
      puVar3[1] = uStack_84;
      puVar3[2] = uStack_80;
      puVar3[3] = uStack_7c;
      local_88 = 0;
      uStack_78 = uVar1;
      uStack_70 = uVar2;
      local_a0 = 0x3f800000;
      local_8c = 0x3f800000;
      local_68 = 0x3f800000;
      AEMath::MatrixIdentity(aAStack_dc,(Matrix *)&local_a0);
      iVar4 = Engine::GetGravValue(*(Engine **)(param_1 + 0x34));
      uVar5 = AEMath::Sinf((float)(*(double *)(iVar4 + 8) * DAT_0008ada0));
      local_a0 = AEMath::Cosf(extraout_s0);
      local_9c = uVar5 ^ 0x80000000;
      local_90 = uVar5;
      local_8c = local_a0;
      AEMath::Matrix::operator*=((Matrix *)&local_64,(Matrix *)&local_a0);
      pMVar7 = (Matrix *)aAStack_dc;
      AEMath::MatrixGetInverse(pMVar7);
    }
    AEMath::Matrix::operator=((Matrix *)&local_64,pMVar7);
    uStack_70 = DAT_0008ad98;
    uStack_78 = DAT_0008ad90;
    uStack_84 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_80 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_7c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar3 = (undefined4 *)((uint)&local_a0 | 4);
    pEVar6 = *(Engine **)(param_1 + 0x34);
    *puVar3 = 0;
    puVar3[1] = uStack_84;
    puVar3[2] = uStack_80;
    puVar3[3] = uStack_7c;
    local_88 = 0;
    local_a0 = 0x3f800000;
    local_8c = 0x3f800000;
    local_68 = 0x3f800000;
    SpriteSystemDraw(pEVar6,(Matrix *)&local_a0,(Matrix *)&local_64,
                     *(SpriteSystem **)(*(int *)(param_1 + 0x184) + param_2 * 4));
  }
  if (*piVar8 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008ac20: push {r4,r5,r6,r7,lr}
  0008ac22: add r7,sp,#0xc
  0008ac24: push {r8,r9,r10,r11}
  0008ac28: sub sp,#0xbc
  0008ac2a: mov r5,r0
  0008ac2c: ldr r0,[0x0008ada8]
  0008ac2e: mov r9,r1
  0008ac30: add r0,pc
  0008ac32: ldr.w r10,[r0,#0x0]
  0008ac36: ldr.w r0,[r10,#0x0]
  0008ac3a: str r0,[sp,#0xb8]
  0008ac3c: ldr.w r0,[r5,#0x180]
  0008ac40: cmp r0,r1
  0008ac42: bls.w 0x0008ad74
  0008ac46: ldr.w r0,[r5,#0x184]
  0008ac4a: ldr.w r0,[r0,r9,lsl #0x2]
  0008ac4e: cmp r0,#0x0
  0008ac50: beq.w 0x0008ad74
  0008ac54: add.w r11,r7,#0x18
  0008ac58: add.w lr,r7,#0x8
  0008ac5c: ldm.w r11,{r4,r6,r8,r11}
  0008ac60: ldm.w lr,{r0,r1,r12,lr}
  0008ac64: strd r2,r3,[sp,#0x7c]
  0008ac68: add r2,sp,#0x84
  0008ac6a: stm r2,{r0,r1,r12,lr}
  0008ac6e: add r0,sp,#0x94
  0008ac70: stm r0,{r4,r6,r8,r11}
  0008ac74: ldr r0,[r7,#0x28]
  0008ac76: str r0,[sp,#0xa4]
  0008ac78: ldr r0,[r7,#0x2c]
  0008ac7a: str r0,[sp,#0xa8]
  0008ac7c: ldr r0,[r7,#0x30]
  0008ac7e: str r0,[sp,#0xac]
  0008ac80: ldr r0,[r7,#0x34]
  0008ac82: str r0,[sp,#0xb0]
  0008ac84: ldr r0,[r7,#0x38]
  0008ac86: str r0,[sp,#0xb4]
  0008ac88: ldrb r0,[r5,#0x0]
  0008ac8a: cmp r0,#0x0
  0008ac8c: beq 0x0008ad22
  0008ac8e: adr r0,[0x8ad90]
  0008ac90: add.w r8,sp,#0x40
  0008ac94: vmov.i32 q8,#0x0
  0008ac98: vld1.64 {d18,d19},[r0]
  0008ac9c: orr r0,r8,#0x4
  0008aca0: mov r1,r8
  0008aca2: vst1.32 {d16,d17},[r0]
  0008aca6: add.w r0,r8,#0x18
  0008acaa: vst1.64 {d16,d17},[r0]
  0008acae: add.w r0,r8,#0x28
  0008acb2: vst1.64 {d18,d19},[r0]
  0008acb6: mov.w r0,#0x3f800000
  0008acba: str r0,[sp,#0x40]
  0008acbc: str r0,[sp,#0x54]
  0008acbe: str r0,[sp,#0x78]
  0008acc0: add r0,sp,#0x4
  0008acc2: blx 0x0006fb2c
  0008acc6: ldr r0,[r5,#0x34]
  0008acc8: blx 0x0006fb38
  0008accc: vldr.64 d17,[r0,#0x8]
  0008acd0: vldr.64 d16,[pc,#0xcc]
  0008acd4: ldr r0,[r5,#0x30]
  0008acd6: vmul.f64 d16,d17,d16
  0008acda: cmp r0,#0x1
  0008acdc: vcvt.f32.f64 s0,d16
  0008ace0: vneg.f32 s2,s0
  0008ace4: it eq
  0008ace6: vmov.eq.f32 s2,s0
  0008acea: vmov r6,s2
  0008acee: mov r0,r6
  0008acf0: blx 0x0006f1a8
  0008acf4: mov r4,r0
  0008acf6: mov r0,r6
  0008acf8: blx 0x0006f49c
  0008acfc: str r0,[sp,#0x40]
  0008acfe: mov r1,r8
  0008ad00: str r0,[sp,#0x54]
  0008ad02: eor r0,r4,#0x80000000
  0008ad06: str r0,[sp,#0x44]
  0008ad08: str r4,[sp,#0x50]
  0008ad0a: add r4,sp,#0x7c
  0008ad0c: mov r0,r4
  0008ad0e: blx 0x0006f4a8
  0008ad12: add r6,sp,#0x4
  0008ad14: mov r1,r4
  0008ad16: mov r0,r6
  0008ad18: blx 0x0006fb50
  0008ad1c: mov r0,r4
  0008ad1e: mov r1,r6
  0008ad20: b 0x0008ad32
  0008ad22: add r4,sp,#0x40
  0008ad24: add r6,sp,#0x7c
  0008ad26: mov r0,r4
  0008ad28: mov r1,r6
  0008ad2a: blx 0x0006fb50
  0008ad2e: mov r0,r6
  0008ad30: mov r1,r4
  0008ad32: blx 0x0006f148
  0008ad36: adr r1,[0x8ad90]
  0008ad38: vmov.i32 q8,#0x0
  0008ad3c: vld1.64 {d18,d19},[r1]
  0008ad40: add r1,sp,#0x40
  0008ad42: orr r2,r1,#0x4
  0008ad46: ldr r0,[r5,#0x34]
  0008ad48: vst1.32 {d16,d17},[r2]
  0008ad4c: add.w r2,r1,#0x18
  0008ad50: vst1.64 {d16,d17},[r2]
  0008ad54: add.w r2,r1,#0x28
  0008ad58: vst1.64 {d18,d19},[r2]
  0008ad5c: mov.w r2,#0x3f800000
  0008ad60: str r2,[sp,#0x40]
  0008ad62: str r2,[sp,#0x54]
  0008ad64: str r2,[sp,#0x78]
  0008ad66: ldr.w r2,[r5,#0x184]
  0008ad6a: ldr.w r3,[r2,r9,lsl #0x2]
  0008ad6e: add r2,sp,#0x7c
  0008ad70: blx 0x0006fce8
  0008ad74: ldr r0,[sp,#0xb8]
  0008ad76: ldr.w r1,[r10,#0x0]
  0008ad7a: subs r0,r1,r0
  0008ad7c: ittt eq
  0008ad7e: add.eq sp,#0xbc
  0008ad80: pop.eq.w {r8,r9,r10,r11}
  0008ad84: pop.eq {r4,r5,r6,r7,pc}
  0008ad86: blx 0x0006e824
```
