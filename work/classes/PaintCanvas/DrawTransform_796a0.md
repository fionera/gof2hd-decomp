# PaintCanvas::DrawTransform
elf 0x796a0 body 550
Sig: undefined __stdcall DrawTransform(uint param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawTransform(unsigned int, AbyssEngine::AEMath::Matrix const*) */

void AbyssEngine::PaintCanvas::DrawTransform(uint param_1,Matrix *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  undefined4 *puVar6;
  Transform *pTVar7;
  Matrix *in_r2;
  Matrix *pMVar8;
  int *piVar9;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s2;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  Matrix aMStack_154 [60];
  undefined4 local_118 [5];
  undefined4 local_104;
  undefined4 local_100;
  undefined4 uStack_fc;
  undefined4 uStack_f8;
  undefined4 uStack_f4;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined4 local_e0;
  undefined4 local_d8;
  uint local_d4;
  uint local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined4 local_a0;
  undefined4 local_98 [5];
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined4 local_60;
  int local_5c;
  
  uVar2 = DAT_000898d0;
  uVar1 = DAT_000898c8;
  piVar9 = *(int **)(DAT_000898e0 + 0x896ba);
  local_5c = *piVar9;
  if ((*(Matrix **)(param_1 + 0x158) <= param_2) ||
     (*(char *)(*(int *)(*(int *)(param_1 + 0x15c) + (int)param_2 * 4) + 0xec) == '\0'))
  goto LAB_00089892;
  uVar10 = 0;
  uVar11 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar13 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar6 = (undefined4 *)((uint)local_98 | 4);
  puVar3 = (undefined4 *)(*(int *)(param_1 + 0x15c) + (int)param_2 * 4);
  *puVar6 = 0;
  puVar6[1] = uVar11;
  puVar6[2] = uVar12;
  puVar6[3] = uVar13;
  local_80 = 0;
  uStack_70 = uVar1;
  uStack_68 = uVar2;
  local_98[0] = 0x3f800000;
  local_84 = 0x3f800000;
  local_60 = 0x3f800000;
  uStack_7c = uVar11;
  uStack_78 = uVar12;
  uStack_74 = uVar13;
  if (*(uint *)(param_1 + 0x170) < *(uint *)(param_1 + 0x164)) {
    if (*(char *)param_1 == '\0') {
      iVar4 = Transform::InCameraVF
                        ((Transform *)*puVar3,(Matrix *)0x0,
                         *(Camera **)(*(int *)(param_1 + 0x168) + *(uint *)(param_1 + 0x170) * 4));
      if (iVar4 == 0) goto LAB_00089806;
      if (in_r2 == (Matrix *)0x0) {
        pMVar8 = (Matrix *)&local_d8;
      }
      else {
        pMVar8 = (Matrix *)&local_d8;
      }
    }
    else {
      uVar14 = 0;
      uVar15 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uVar16 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uVar17 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar3 = (undefined4 *)((uint)&local_d8 | 4);
      *puVar3 = 0;
      puVar3[1] = uVar15;
      puVar3[2] = uVar16;
      puVar3[3] = uVar17;
      local_c0 = 0;
      uStack_b0 = uVar1;
      uStack_a8 = uVar2;
      local_d8 = 0x3f800000;
      local_c4 = 0x3f800000;
      local_a0 = 0x3f800000;
      uStack_bc = uVar15;
      uStack_b8 = uVar16;
      uStack_b4 = uVar17;
      AEMath::MatrixIdentity((AEMath *)local_118,(Matrix *)&local_d8);
      iVar4 = Engine::GetGravValue(*(Engine **)(param_1 + 0x34));
      uVar5 = AEMath::Sinf((float)(*(double *)(iVar4 + 8) * DAT_000898d8));
      local_d8 = AEMath::Cosf(extraout_s0);
      local_d4 = uVar5 ^ 0x80000000;
      local_c8 = uVar5;
      local_c4 = local_d8;
      iVar4 = Transform::InCameraVF
                        (*(Transform **)(*(int *)(param_1 + 0x15c) + (int)param_2 * 4),
                         (Matrix *)&local_d8,
                         *(Camera **)(*(int *)(param_1 + 0x168) + *(int *)(param_1 + 0x170) * 4));
      if (iVar4 == 0) {
LAB_00089806:
        *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
        goto LAB_00089892;
      }
      puVar3 = (undefined4 *)((uint)local_118 | 4);
      *puVar3 = uVar14;
      puVar3[1] = uVar15;
      puVar3[2] = uVar16;
      puVar3[3] = uVar17;
      uStack_f0 = uVar1;
      uStack_e8 = uVar2;
      local_118[0] = 0x3f800000;
      local_104 = 0x3f800000;
      local_e0 = 0x3f800000;
      if (in_r2 == (Matrix *)0x0) {
        in_r2 = (Matrix *)
                (*(int *)(*(int *)(param_1 + 0x168) + *(int *)(param_1 + 0x170) * 4) + 0xc);
      }
      local_100 = uVar14;
      uStack_fc = uVar15;
      uStack_f8 = uVar16;
      uStack_f4 = uVar17;
      AEMath::Matrix::operator=((Matrix *)local_118,in_r2);
      AEMath::Matrix::operator*=((Matrix *)local_118,(Matrix *)&local_d8);
      pMVar8 = aMStack_154;
    }
    AEMath::MatrixGetInverse(pMVar8);
    AEMath::Matrix::operator=((Matrix *)local_98,pMVar8);
    Engine::SetEyePosition(*(Engine **)(param_1 + 0x34),extraout_s0_00,extraout_s1,extraout_s2);
    puVar3 = (undefined4 *)(*(int *)(param_1 + 0x15c) + (int)param_2 * 4);
  }
  pTVar7 = (Transform *)*puVar3;
  puVar3 = (undefined4 *)((uint)&local_d8 | 4);
  *puVar3 = uVar10;
  puVar3[1] = uVar11;
  puVar3[2] = uVar12;
  puVar3[3] = uVar13;
  uStack_b0 = uVar1;
  uStack_a8 = uVar2;
  local_d8 = 0x3f800000;
  local_c4 = 0x3f800000;
  local_a0 = 0x3f800000;
  local_c0 = uVar10;
  uStack_bc = uVar11;
  uStack_b8 = uVar12;
  uStack_b4 = uVar13;
  DrawTransform((PaintCanvas *)param_1,pTVar7,(Matrix *)&local_d8,(Matrix *)local_98);
LAB_00089892:
  if (*piVar9 != local_5c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000896a0: push {r4,r5,r6,r7,lr}
  000896a2: add r7,sp,#0xc
  000896a4: push {r7,r8,r9,r10,r11}
  000896a8: vpush {d8,d9,d10,d11,d12,d13}
  000896ac: sub sp,#0x100
  000896ae: mov r4,r0
  000896b0: ldr r0,[0x000898e0]
  000896b2: mov r10,r2
  000896b4: mov r5,r1
  000896b6: add r0,pc
  000896b8: ldr.w r8,[r0,#0x0]
  000896bc: ldr.w r0,[r8,#0x0]
  000896c0: str r0,[sp,#0xfc]
  000896c2: ldr.w r0,[r4,#0x158]
  000896c6: cmp r0,r1
  000896c8: bls.w 0x00089892
  000896cc: ldr.w r0,[r4,#0x15c]
  000896d0: ldr.w r1,[r0,r5,lsl #0x2]
  000896d4: ldrb.w r1,[r1,#0xec]
  000896d8: cmp r1,#0x0
  000896da: beq.w 0x00089892
  000896de: adr r1,[0x898c8]
  000896e0: vmov.i32 q4,#0x0
  000896e4: add r2,sp,#0xc0
  000896e6: vld1.64 {d10,d11},[r1]
  000896ea: orr r1,r2,#0x4
  000896ee: mov.w r9,#0x3f800000
  000896f2: add.w r0,r0,r5, lsl #0x2
  000896f6: vst1.32 {d8,d9},[r1]
  000896fa: add.w r1,r2,#0x18
  000896fe: vst1.64 {d8,d9},[r1]
  00089702: add.w r1,r2,#0x28
  00089706: vst1.64 {d10,d11},[r1]
  0008970a: str.w r9,[sp,#0xc0]
  0008970e: str.w r9,[sp,#0xd4]
  00089712: str.w r9,[sp,#0xf8]
  00089716: ldr.w r2,[r4,#0x164]
  0008971a: ldr.w r1,[r4,#0x170]
  0008971e: cmp r1,r2
  00089720: bcs.w 0x00089862
  00089724: ldrb r2,[r4,#0x0]
  00089726: cmp r2,#0x0
  00089728: beq 0x000897ea
  0008972a: add r1,sp,#0x80
  0008972c: vmov.i32 q6,#0x0
  00089730: orr r0,r1,#0x4
  00089734: vst1.32 {d12,d13},[r0]
  00089738: add.w r0,r1,#0x18
  0008973c: vst1.64 {d12,d13},[r0]
  00089740: add.w r0,r1,#0x28
  00089744: vst1.64 {d10,d11},[r0]
  00089748: mov.w r0,#0x3f800000
  0008974c: str r0,[sp,#0x80]
  0008974e: str r0,[sp,#0x94]
  00089750: str r0,[sp,#0xb8]
  00089752: add r0,sp,#0x40
  00089754: blx 0x0006fb2c
  00089758: ldr r0,[r4,#0x34]
  0008975a: blx 0x0006fb38
  0008975e: vldr.64 d17,[r0,#0x8]
  00089762: vldr.64 d16,[pc,#0x174]
  00089766: ldr r0,[r4,#0x30]
  00089768: vmul.f64 d16,d17,d16
  0008976c: cmp r0,#0x1
  0008976e: vcvt.f32.f64 s0,d16
  00089772: vneg.f32 s2,s0
  00089776: it eq
  00089778: vmov.eq.f32 s2,s0
  0008977c: vmov r6,s2
  00089780: mov r0,r6
  00089782: blx 0x0006f1a8
  00089786: mov r11,r0
  00089788: mov r0,r6
  0008978a: blx 0x0006f49c
  0008978e: str r0,[sp,#0x80]
  00089790: str r0,[sp,#0x94]
  00089792: eor r0,r11,#0x80000000
  00089796: str r0,[sp,#0x84]
  00089798: str.w r11,[sp,#0x90]
  0008979c: ldr.w r0,[r4,#0x15c]
  000897a0: ldr.w r1,[r4,#0x168]
  000897a4: ldr.w r2,[r4,#0x170]
  000897a8: ldr.w r0,[r0,r5,lsl #0x2]
  000897ac: ldr.w r2,[r1,r2,lsl #0x2]
  000897b0: add r1,sp,#0x80
  000897b2: blx 0x0006fb44
  000897b6: cbz r0,0x00089806
  000897b8: add r0,sp,#0x40
  000897ba: cmp.w r10,#0x0
  000897be: mov r1,r0
  000897c0: orr r0,r0,#0x4
  000897c4: vst1.32 {d12,d13},[r0]
  000897c8: add.w r0,r1,#0x18
  000897cc: vst1.64 {d12,d13},[r0]
  000897d0: add.w r0,r1,#0x28
  000897d4: vst1.64 {d10,d11},[r0]
  000897d8: mov.w r0,#0x3f800000
  000897dc: str r0,[sp,#0x40]
  000897de: str r0,[sp,#0x54]
  000897e0: str r0,[sp,#0x78]
  000897e2: beq 0x0008980e
  000897e4: add r0,sp,#0x40
  000897e6: mov r1,r10
  000897e8: b 0x00089820
  000897ea: ldr.w r2,[r4,#0x168]
  000897ee: ldr r0,[r0,#0x0]
  000897f0: ldr.w r2,[r2,r1,lsl #0x2]
  000897f4: movs r1,#0x0
  000897f6: blx 0x0006fb44
  000897fa: cbz r0,0x00089806
  000897fc: cmp.w r10,#0x0
  00089800: beq 0x000898b0
  00089802: add r6,sp,#0x80
  00089804: b 0x00089832
  00089806: ldr r0,[r4,#0x4]
  00089808: adds r0,#0x1
  0008980a: str r0,[r4,#0x4]
  0008980c: b 0x00089892
  0008980e: ldr.w r0,[r4,#0x168]
  00089812: ldr.w r1,[r4,#0x170]
  00089816: ldr.w r0,[r0,r1,lsl #0x2]
  0008981a: add.w r1,r0,#0xc
  0008981e: add r0,sp,#0x40
  00089820: blx 0x0006f148
  00089824: add.w r10,sp,#0x40
  00089828: add r1,sp,#0x80
  0008982a: mov r0,r10
  0008982c: blx 0x0006f4a8
  00089830: add r6,sp,#0x4
  00089832: mov r0,r6
  00089834: mov r1,r10
  00089836: blx 0x0006fb50
  0008983a: add r0,sp,#0xc0
  0008983c: mov r1,r6
  0008983e: blx 0x0006f148
  00089842: ldr.w r1,[r4,#0x168]
  00089846: ldr.w r2,[r4,#0x170]
  0008984a: ldr r0,[r4,#0x34]
  0008984c: ldr.w r3,[r1,r2,lsl #0x2]
  00089850: ldr r1,[r3,#0x18]
  00089852: ldr r2,[r3,#0x28]
  00089854: ldr r3,[r3,#0x38]
  00089856: blx 0x0006fb5c
  0008985a: ldr.w r0,[r4,#0x15c]
  0008985e: add.w r0,r0,r5, lsl #0x2
  00089862: add r2,sp,#0x80
  00089864: ldr r1,[r0,#0x0]
  00089866: orr r0,r2,#0x4
  0008986a: add r3,sp,#0xc0
  0008986c: vst1.32 {d8,d9},[r0]
  00089870: add.w r0,r2,#0x18
  00089874: vst1.64 {d8,d9},[r0]
  00089878: add.w r0,r2,#0x28
  0008987c: vst1.64 {d10,d11},[r0]
  00089880: mov r0,r4
  00089882: str.w r9,[sp,#0x80]
  00089886: str.w r9,[sp,#0x94]
  0008988a: str.w r9,[sp,#0xb8]
  0008988e: blx 0x0006fb68
  00089892: ldr r0,[sp,#0xfc]
  00089894: ldr.w r1,[r8,#0x0]
  00089898: subs r0,r1,r0
  0008989a: itttt eq
  0008989c: add.eq sp,#0x100
  0008989e: vpop.eq {d8,d9,d10,d11,d12,d13}
  000898a2: add.eq sp,#0x4
  000898a4: pop.eq.w {r8,r9,r10,r11}
  000898a8: it eq
  000898aa: pop.eq {r4,r5,r6,r7,pc}
  000898ac: blx 0x0006e824
  000898b0: ldr.w r0,[r4,#0x168]
  000898b4: add r6,sp,#0x80
  000898b6: ldr.w r1,[r4,#0x170]
  000898ba: ldr.w r0,[r0,r1,lsl #0x2]
  000898be: add.w r1,r0,#0xc
  000898c2: mov r0,r6
  000898c4: b 0x00089836
```
