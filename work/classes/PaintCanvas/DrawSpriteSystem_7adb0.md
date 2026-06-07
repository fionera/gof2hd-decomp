# PaintCanvas::DrawSpriteSystem
elf 0x7adb0 body 406
Sig: undefined __stdcall DrawSpriteSystem(uint param_1, Matrix param_2, Matrix param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawSpriteSystem(unsigned int, AbyssEngine::AEMath::Matrix,
   AbyssEngine::AEMath::Matrix) */

void AbyssEngine::PaintCanvas::DrawSpriteSystem
               (char *param_1,uint param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
               undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
               undefined4 param_10,undefined4 param_11,undefined4 param_12,undefined4 param_13,
               undefined4 param_14,undefined4 param_15,undefined4 param_16,undefined4 param_17,
               undefined4 param_18,undefined4 param_19,undefined4 param_20,undefined4 param_21,
               undefined4 param_22,undefined4 param_23,undefined4 param_24,undefined4 param_25,
               undefined4 param_26,undefined4 param_27,undefined4 param_28,undefined4 param_29,
               undefined4 param_30,undefined4 param_31,undefined4 param_32)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  uint in_fpscr;
  float fVar7;
  float extraout_s0;
  AEMath aAStack_11c [60];
  undefined4 local_e0;
  uint local_dc;
  uint local_d0;
  undefined4 local_cc;
  undefined4 local_c8;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined4 local_a8;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 local_94;
  undefined4 uStack_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  
  uVar2 = DAT_0008af50;
  uVar1 = DAT_0008af48;
  piVar6 = *(int **)(DAT_0008af60 + 0x8adc4);
  local_28 = *piVar6;
  if ((param_2 < *(uint *)(param_1 + 0x180)) &&
     (*(int *)(*(int *)(param_1 + 0x184) + param_2 * 4) != 0)) {
    local_64 = param_18;
    uStack_60 = param_19;
    local_5c = param_20;
    uStack_58 = param_21;
    local_54 = param_22;
    local_50 = param_23;
    local_4c = param_24;
    local_48 = param_25;
    local_44 = param_26;
    local_40 = param_27;
    local_3c = param_28;
    local_38 = param_29;
    local_34 = param_30;
    local_30 = param_31;
    local_2c = param_32;
    uStack_98 = param_5;
    local_94 = param_6;
    uStack_90 = param_7;
    local_8c = param_8;
    local_88 = param_9;
    local_84 = param_10;
    local_80 = param_11;
    local_7c = param_12;
    local_78 = param_13;
    local_74 = param_14;
    local_70 = param_15;
    local_6c = param_16;
    local_68 = param_17;
    local_a0 = param_3;
    uStack_9c = param_4;
    if (*param_1 == '\0') {
      AEMath::MatrixGetInverse((Matrix *)&local_e0);
      AEMath::Matrix::operator=((Matrix *)&local_64,(Matrix *)&local_e0);
    }
    else {
      uStack_c4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uStack_c0 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_bc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar3 = (undefined4 *)((uint)&local_e0 | 4);
      *puVar3 = 0;
      puVar3[1] = uStack_c4;
      puVar3[2] = uStack_c0;
      puVar3[3] = uStack_bc;
      local_c8 = 0;
      uStack_b8 = uVar1;
      uStack_b0 = uVar2;
      local_e0 = 0x3f800000;
      local_cc = 0x3f800000;
      local_a8 = 0x3f800000;
      AEMath::MatrixIdentity(aAStack_11c,(Matrix *)&local_e0);
      iVar4 = Engine::GetGravValue(*(Engine **)(param_1 + 0x34));
      iVar4 = (int)(longlong)(*(double *)(iVar4 + 8) * DAT_0008af58);
      if (*(int *)(param_1 + 0x30) == 1) {
        iVar4 = -iVar4;
      }
      fVar7 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
      uVar5 = AEMath::Sinf(fVar7);
      local_e0 = AEMath::Cosf(extraout_s0);
      local_dc = uVar5 ^ 0x80000000;
      local_d0 = uVar5;
      local_cc = local_e0;
      AEMath::Matrix::operator*=((Matrix *)&local_64,(Matrix *)&local_e0);
      AEMath::MatrixGetInverse((Matrix *)aAStack_11c);
      AEMath::Matrix::operator=((Matrix *)&local_64,(Matrix *)aAStack_11c);
      AEMath::Matrix::operator*=((Matrix *)&local_a0,(Matrix *)&local_e0);
    }
    SpriteSystemDraw(*(Engine **)(param_1 + 0x34),(Matrix *)&local_a0,(Matrix *)&local_64,
                     *(SpriteSystem **)(*(int *)(param_1 + 0x184) + param_2 * 4));
  }
  if (*piVar6 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008adb0: push {r4,r5,r6,r7,lr}
  0008adb2: add r7,sp,#0xc
  0008adb4: push {r8,r9,r10,r11}
  0008adb8: sub sp,#0xfc
  0008adba: mov r5,r0
  0008adbc: ldr r0,[0x0008af60]
  0008adbe: mov r9,r1
  0008adc0: add r0,pc
  0008adc2: ldr.w r10,[r0,#0x0]
  0008adc6: ldr.w r0,[r10,#0x0]
  0008adca: str r0,[sp,#0xf8]
  0008adcc: ldr.w r0,[r5,#0x180]
  0008add0: cmp r0,r1
  0008add2: bls.w 0x0008af30
  0008add6: ldr.w r0,[r5,#0x184]
  0008adda: ldr.w r0,[r0,r9,lsl #0x2]
  0008adde: cmp r0,#0x0
  0008ade0: beq.w 0x0008af30
  0008ade4: ldrd r12,r6,[r7,#0x44]
  0008ade8: ldrd r1,r0,[r7,#0x3c]
  0008adec: ldrd lr,r4,[r7,#0x8]
  0008adf0: strd r1,r0,[sp,#0xbc]
  0008adf4: ldr.w r8,[r7,#0x4c]
  0008adf8: ldr r0,[r7,#0x50]
  0008adfa: strd r12,r6,[sp,#0xc4]
  0008adfe: str.w r8,[sp,#0xcc]
  0008ae02: str r0,[sp,#0xd0]
  0008ae04: ldr r0,[r7,#0x54]
  0008ae06: str r0,[sp,#0xd4]
  0008ae08: ldr r0,[r7,#0x58]
  0008ae0a: str r0,[sp,#0xd8]
  0008ae0c: ldr r0,[r7,#0x5c]
  0008ae0e: str r0,[sp,#0xdc]
  0008ae10: ldr r0,[r7,#0x60]
  0008ae12: str r0,[sp,#0xe0]
  0008ae14: ldr r0,[r7,#0x64]
  0008ae16: str r0,[sp,#0xe4]
  0008ae18: ldr r0,[r7,#0x68]
  0008ae1a: str r0,[sp,#0xe8]
  0008ae1c: ldr r0,[r7,#0x6c]
  0008ae1e: str r0,[sp,#0xec]
  0008ae20: ldr r0,[r7,#0x70]
  0008ae22: str r0,[sp,#0xf0]
  0008ae24: ldr r0,[r7,#0x74]
  0008ae26: str r0,[sp,#0xf4]
  0008ae28: add r0,sp,#0x80
  0008ae2a: ldr.w r11,[r7,#0x10]
  0008ae2e: stm.w r0!,{r2,r3,lr}
  0008ae32: ldr r0,[r7,#0x14]
  0008ae34: strd r4,r11,[sp,#0x8c]
  0008ae38: str r0,[sp,#0x94]
  0008ae3a: ldr r0,[r7,#0x18]
  0008ae3c: str r0,[sp,#0x98]
  0008ae3e: ldr r0,[r7,#0x1c]
  0008ae40: str r0,[sp,#0x9c]
  0008ae42: ldr r0,[r7,#0x20]
  0008ae44: str r0,[sp,#0xa0]
  0008ae46: ldr r0,[r7,#0x24]
  0008ae48: str r0,[sp,#0xa4]
  0008ae4a: ldr r0,[r7,#0x28]
  0008ae4c: str r0,[sp,#0xa8]
  0008ae4e: ldr r0,[r7,#0x2c]
  0008ae50: str r0,[sp,#0xac]
  0008ae52: ldr r0,[r7,#0x30]
  0008ae54: str r0,[sp,#0xb0]
  0008ae56: ldr r0,[r7,#0x34]
  0008ae58: str r0,[sp,#0xb4]
  0008ae5a: ldr r0,[r7,#0x38]
  0008ae5c: str r0,[sp,#0xb8]
  0008ae5e: ldrb r0,[r5,#0x0]
  0008ae60: cmp r0,#0x0
  0008ae62: beq 0x0008af0a
  0008ae64: adr r0,[0x8af48]
  0008ae66: add.w r8,sp,#0x40
  0008ae6a: vmov.i32 q8,#0x0
  0008ae6e: vld1.64 {d18,d19},[r0]
  0008ae72: orr r0,r8,#0x4
  0008ae76: mov r1,r8
  0008ae78: vst1.32 {d16,d17},[r0]
  0008ae7c: add.w r0,r8,#0x18
  0008ae80: vst1.64 {d16,d17},[r0]
  0008ae84: add.w r0,r8,#0x28
  0008ae88: vst1.64 {d18,d19},[r0]
  0008ae8c: mov.w r0,#0x3f800000
  0008ae90: str r0,[sp,#0x40]
  0008ae92: str r0,[sp,#0x54]
  0008ae94: str r0,[sp,#0x78]
  0008ae96: add r0,sp,#0x4
  0008ae98: blx 0x0006fb2c
  0008ae9c: ldr r0,[r5,#0x34]
  0008ae9e: blx 0x0006fb38
  0008aea2: vldr.64 d16,[pc,#0xb4]
  0008aea6: vldr.64 d17,[r0,#0x8]
  0008aeaa: vmul.f64 d16,d17,d16
  0008aeae: vcvt.s32.f64 s0,d16
  0008aeb2: ldr r0,[r5,#0x30]
  0008aeb4: cmp r0,#0x1
  0008aeb6: vmov r1,s0
  0008aeba: it eq
  0008aebc: rsb.eq r1,r1
  0008aebe: vmov s0,r1
  0008aec2: vcvt.f32.s32 s0,s0
  0008aec6: vmov r6,s0
  0008aeca: mov r0,r6
  0008aecc: blx 0x0006f1a8
  0008aed0: mov r4,r0
  0008aed2: mov r0,r6
  0008aed4: blx 0x0006f49c
  0008aed8: str r0,[sp,#0x40]
  0008aeda: mov r1,r8
  0008aedc: str r0,[sp,#0x54]
  0008aede: eor r0,r4,#0x80000000
  0008aee2: str r0,[sp,#0x44]
  0008aee4: str r4,[sp,#0x50]
  0008aee6: add r4,sp,#0xbc
  0008aee8: mov r0,r4
  0008aeea: blx 0x0006f4a8
  0008aeee: add r6,sp,#0x4
  0008aef0: mov r1,r4
  0008aef2: mov r0,r6
  0008aef4: blx 0x0006fb50
  0008aef8: mov r0,r4
  0008aefa: mov r1,r6
  0008aefc: blx 0x0006f148
  0008af00: add r0,sp,#0x80
  0008af02: mov r1,r8
  0008af04: blx 0x0006f4a8
  0008af08: b 0x0008af1e
  0008af0a: add r4,sp,#0x40
  0008af0c: add r6,sp,#0xbc
  0008af0e: mov r0,r4
  0008af10: mov r1,r6
  0008af12: blx 0x0006fb50
  0008af16: mov r0,r6
  0008af18: mov r1,r4
  0008af1a: blx 0x0006f148
  0008af1e: ldr.w r1,[r5,#0x184]
  0008af22: add r2,sp,#0xbc
  0008af24: ldr r0,[r5,#0x34]
  0008af26: ldr.w r3,[r1,r9,lsl #0x2]
  0008af2a: add r1,sp,#0x80
  0008af2c: blx 0x0006fce8
  0008af30: ldr r0,[sp,#0xf8]
  0008af32: ldr.w r1,[r10,#0x0]
  0008af36: subs r0,r1,r0
  0008af38: ittt eq
  0008af3a: add.eq sp,#0xfc
  0008af3c: pop.eq.w {r8,r9,r10,r11}
  0008af40: pop.eq {r4,r5,r6,r7,pc}
  0008af42: blx 0x0006e824
```
