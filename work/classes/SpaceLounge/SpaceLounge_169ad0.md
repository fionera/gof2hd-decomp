# SpaceLounge::SpaceLounge
elf 0x169ad0 body 1076
Sig: undefined __thiscall SpaceLounge(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::SpaceLounge() */

void __thiscall SpaceLounge::SpaceLounge(SpaceLounge *this)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  EaseInOutMatrix *pEVar5;
  Matrix *pMVar6;
  uint *puVar7;
  uint *puVar8;
  undefined4 *puVar9;
  Agent *this_00;
  uint uVar10;
  CutScene *this_01;
  int iVar11;
  int *piVar12;
  uint uVar13;
  int iVar14;
  uint in_fpscr;
  float fVar15;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float fVar16;
  float extraout_s2;
  float extraout_s2_00;
  undefined4 uVar18;
  undefined8 uVar17;
  undefined4 uVar19;
  undefined4 uVar21;
  undefined8 uVar20;
  float local_10c [2];
  float local_104;
  Matrix aMStack_d0 [12];
  Matrix aMStack_c4 [12];
  float local_b8;
  undefined4 local_b4;
  float local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined4 local_80;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined8 local_50;
  undefined8 local_48;
  undefined4 local_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  uVar2 = 0;
  uVar18 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar19 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar21 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar12 = *(int **)(DAT_00179bf0 + 0x179aee);
  local_3c = *piVar12;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x90) = uVar18;
  *(undefined4 *)(this + 0x94) = uVar19;
  *(undefined4 *)(this + 0x98) = uVar21;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x9c) = 0;
  *(undefined4 *)(this + 0xa0) = 0;
  AbyssEngine::String::String((String *)(this + 0xa4));
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 200));
  this[0xb0] = (SpaceLounge)0x0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined2 *)(this + 0x34) = 0;
  this[0x36] = (SpaceLounge)0x0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0xc0) = 0;
  this[0x1c] = (SpaceLounge)0x0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x20) = uVar2;
  *(undefined4 *)(this + 0x24) = uVar18;
  *(undefined4 *)(this + 0x28) = uVar19;
  *(undefined4 *)(this + 0x2c) = uVar21;
  *(undefined4 *)(this + 0x38) = uVar2;
  *(undefined4 *)(this + 0x3c) = uVar18;
  *(undefined4 *)(this + 0x40) = uVar19;
  *(undefined4 *)(this + 0x44) = uVar21;
  init((EVP_PKEY_CTX *)this);
  puVar7 = *(uint **)(this + 0x24);
  if (puVar7 != (uint *)0x0) {
    for (uVar10 = 0; uVar10 < *puVar7; uVar10 = uVar10 + 1) {
      this_00 = *(Agent **)(puVar7[1] + uVar10 * 4);
      iVar1 = Agent::getOffer(this_00);
      if (((iVar1 == 6) || (iVar1 = Agent::getOffer(this_00), iVar1 == 0)) &&
         (iVar1 = Agent::getMission(this_00), iVar1 != 0)) {
        Agent::getMission(this_00);
        iVar1 = Mission::getType();
        if ((iVar1 == 0xc) && (iVar1 = Agent::hasAcceptedOffer(this_00), iVar1 != 0)) {
          ArrayRemove<Agent*>(*(Agent **)(*(int *)(*(Array **)(this + 0x24) + 4) + uVar10 * 4),
                              *(Array **)(this + 0x24));
          init((EVP_PKEY_CTX *)this);
          break;
        }
      }
      puVar7 = *(uint **)(this + 0x24);
    }
  }
  this_01 = *(CutScene **)(this + 0x44);
  if (this_01 == (CutScene *)0x0) {
    this_01 = operator_new(0xa0);
    CutScene::CutScene(this_01,4);
    *(CutScene **)(this + 0x44) = this_01;
  }
  while (iVar1 = CutScene::isInitialized(this_01), iVar1 == 0) {
    CutScene::initialize();
    this_01 = *(CutScene **)(this + 0x44);
  }
  iVar1 = Level::getEnemies();
  puVar7 = *(uint **)(this + 0x24);
  if (puVar7 != (uint *)0x0) {
    puVar8 = *(uint **)(DAT_00179f64 + 0x179be8);
    fVar15 = DAT_00179f60;
    for (uVar10 = 0; uVar10 < *puVar7; uVar10 = uVar10 + 1) {
      (**(code **)(**(int **)(*(int *)(iVar1 + 4) + uVar10 * 4) + 0x28))(local_10c);
      uVar13 = *puVar8;
      AbyssEngine::PaintCanvas::CameraGetCurrent();
      uVar2 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar13);
      __aeabi_memcpy((Matrix *)&local_78,uVar2,0x3c);
      AbyssEngine::AEMath::MatrixGetPosition(aMStack_c4);
      AbyssEngine::AEMath::MatrixGetUp(aMStack_d0);
      AbyssEngine::AEMath::MatrixGetLookAt
                ((AEMath *)&local_b8,(Vector *)aMStack_c4,(Vector *)local_10c,(Vector *)aMStack_d0);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_78,(Matrix *)&local_b8);
      AEGeometry::setMatrix(*(Matrix **)(*(int *)(*(int *)(iVar1 + 4) + uVar10 * 4) + 8));
      piVar3 = *(int **)(*(int *)(iVar1 + 4) + uVar10 * 4);
      (**(code **)(*piVar3 + 0x44))(piVar3,local_10c);
      AEGeometry::setMatrix
                (*(Matrix **)
                  (*(int *)(*(int *)(iVar1 + 4) + (**(int **)(this + 0x24) + uVar10) * 4) + 8));
      AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_b8);
      local_10c[0] = local_10c[0] + local_b8 * fVar15;
      AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_b8);
      local_104 = local_104 + local_b0 * fVar15;
      piVar3 = *(int **)(*(int *)(iVar1 + 4) + (**(int **)(this + 0x24) + uVar10) * 4);
      (**(code **)(*piVar3 + 0x44))(piVar3,local_10c);
      puVar7 = *(uint **)(this + 0x24);
    }
  }
  uVar20 = DAT_00179f58;
  uVar17 = DAT_00179f50;
  uStack_5c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  local_58 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  local_54 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar9 = (undefined4 *)((uint)&local_78 | 4);
  *puVar9 = 0;
  puVar9[1] = uStack_5c;
  puVar9[2] = local_58;
  puVar9[3] = local_54;
  local_60 = 0;
  local_50 = uVar17;
  local_48 = uVar20;
  local_78 = 0x3f800000;
  local_64 = 0x3f800000;
  local_40 = 0x3f800000;
  Status::getSystem();
  iVar1 = SolarSystem::getRace();
  iVar4 = DAT_00179f6c + 0x179d12;
  iVar11 = iVar1 * 3 + 1;
  iVar14 = iVar1 * 3 + 2;
  VectorSignedToFloat(*(undefined4 *)(iVar4 + iVar1 * 0xc),(byte)(in_fpscr >> 0x16) & 3);
  fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(iVar4 + iVar11 * 4),
                                      (byte)(in_fpscr >> 0x16) & 3);
  fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(iVar4 + iVar14 * 4),
                                      (byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_b8,fVar15,extraout_s1,fVar16);
  AbyssEngine::AEMath::MatrixSetRotation((Matrix *)&local_b8,extraout_s0,extraout_s1_00,extraout_s2)
  ;
  uStack_9c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  local_98 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_94 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar4 = DAT_00179f74 + 0x179d72;
  VectorSignedToFloat(*(undefined4 *)(iVar4 + iVar1 * 0xc),(byte)(in_fpscr >> 0x16) & 3);
  puVar9 = (undefined4 *)((uint)&local_b8 | 4);
  fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(iVar4 + iVar11 * 4),
                                      (byte)(in_fpscr >> 0x16) & 3);
  *puVar9 = 0;
  puVar9[1] = uStack_9c;
  puVar9[2] = local_98;
  puVar9[3] = uStack_94;
  local_a0 = 0;
  fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(iVar4 + iVar14 * 4),
                                      (byte)(in_fpscr >> 0x16) & 3);
  local_b8 = 1.0;
  local_a4 = 0x3f800000;
  local_80 = 0x3f800000;
  uStack_90 = uVar17;
  local_88 = uVar20;
  AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)local_10c,fVar15,extraout_s1_01,fVar16);
  AbyssEngine::AEMath::MatrixSetRotation
            ((Matrix *)local_10c,extraout_s0_00,extraout_s1_02,extraout_s2_00);
  pEVar5 = operator_new(0xf4);
  AbyssEngine::EaseInOutMatrix::EaseInOutMatrix
            (pEVar5,local_78,local_74,local_70,local_6c,local_68,local_64,local_60,uStack_5c,
             local_58,local_54,(undefined4)local_50,local_50._4_4_,(undefined4)local_48,
             local_48._4_4_,local_40,local_b8,local_b4,local_b0,local_ac,local_a8,local_a4,local_a0,
             uStack_9c,local_98,uStack_94,(undefined4)uStack_90,uStack_90._4_4_,(undefined4)local_88
             ,local_88._4_4_,local_80,3000);
  iVar1 = DAT_00179f7c;
  *(EaseInOutMatrix **)(this + 0x48) = pEVar5;
  uVar10 = **(uint **)(iVar1 + 0x179eb6);
  pMVar6 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraSetLocal(uVar10,pMVar6);
  this[0xb0] = (SpaceLounge)0x1;
  this[0xbd] = (SpaceLounge)0x0;
  iVar1 = *piVar12 - local_3c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  00179ad0: push {r4,r5,r6,r7,lr}
  00179ad2: add r7,sp,#0xc
  00179ad4: push {r7,r8,r9,r10,r11}
  00179ad8: vpush {d8,d9}
  00179adc: sub sp,#0x198
  00179ade: mov r5,r0
  00179ae0: ldr r0,[0x00179bf0]
  00179ae2: vmov.i32 q4,#0x0
  00179ae6: add.w r8,r5,#0xa4
  00179aea: add r0,pc
  00179aec: movs r4,#0x0
  00179aee: ldr.w r9,[r0,#0x0]
  00179af2: ldr.w r0,[r9,#0x0]
  00179af6: str r0,[sp,#0x194]
  00179af8: add.w r0,r5,#0x8c
  00179afc: vst1.32 {d8,d9},[r0]
  00179b00: mov r0,r8
  00179b02: strd r4,r4,[r5,#0x4c]
  00179b06: str r4,[r5,#0x54]
  00179b08: strd r4,r4,[r5,#0x9c]
  00179b0c: blx 0x0006efbc
  00179b10: add.w r0,r5,#0xc8
  00179b14: blx 0x0006f118
  00179b18: add.w r0,r5,#0x20
  00179b1c: strb.w r4,[r5,#0xb0]
  00179b20: strd r4,r4,[r5,#0x4]
  00179b24: str r4,[r5,#0xc]
  00179b26: strh r4,[r5,#0x34]
  00179b28: strb.w r4,[r5,#0x36]
  00179b2c: strd r4,r4,[r5,#0x58]
  00179b30: str.w r4,[r5,#0xc0]
  00179b34: strb r4,[r5,#0x1c]
  00179b36: strd r4,r4,[r5,#0x14]
  00179b3a: str r4,[r5,#0x48]
  00179b3c: vst1.32 {d8,d9},[r0]
  00179b40: add.w r0,r5,#0x38
  00179b44: vst1.32 {d8,d9},[r0]
  00179b48: mov r0,r5
  00179b4a: blx 0x000753dc
  00179b4e: ldr r1,[r5,#0x24]
  00179b50: cbz r1,0x00179b9a
  00179b52: movs r6,#0x0
  00179b54: b 0x00179b94
  00179b56: ldr r0,[r1,#0x4]
  00179b58: ldr.w r4,[r0,r6,lsl #0x2]
  00179b5c: mov r0,r4
  00179b5e: blx 0x00071ae8
  00179b62: cmp r0,#0x6
  00179b64: beq 0x00179b6e
  00179b66: mov r0,r4
  00179b68: blx 0x00071ae8
  00179b6c: cbnz r0,0x00179b90
  00179b6e: mov r0,r4
  00179b70: blx 0x00071bb4
  00179b74: cbz r0,0x00179b90
  00179b76: mov r0,r4
  00179b78: blx 0x00071bb4
  00179b7c: blx 0x00072874
  00179b80: cmp r0,#0xc
  00179b82: bne 0x00179b90
  00179b84: mov r0,r4
  00179b86: blx 0x00074bf0
  00179b8a: cmp r0,#0x0
  00179b8c: bne.w 0x00179ef4
  00179b90: ldr r1,[r5,#0x24]
  00179b92: adds r6,#0x1
  00179b94: ldr r0,[r1,#0x0]
  00179b96: cmp r6,r0
  00179b98: bcc 0x00179b56
  00179b9a: ldr r6,[r5,#0x44]
  00179b9c: cbnz r6,0x00179bb6
  00179b9e: movs r0,#0xa0
  00179ba0: blx 0x0006eb24
  00179ba4: mov r6,r0
  00179ba6: movs r1,#0x4
  00179ba8: blx 0x00074ef0
  00179bac: str r6,[r5,#0x44]
  00179bae: b 0x00179bb6
  00179bb0: blx 0x00074efc
  00179bb4: ldr r6,[r5,#0x44]
  00179bb6: mov r0,r6
  00179bb8: blx 0x00077cec
  00179bbc: mov r1,r0
  00179bbe: ldr r0,[r5,#0x44]
  00179bc0: cmp r1,#0x0
  00179bc2: beq 0x00179bb0
  00179bc4: strd r9,r8,[sp,#0xb8]
  00179bc8: ldr r0,[r0,#0x0]
  00179bca: blx 0x00071eb4
  00179bce: mov r6,r0
  00179bd0: ldr r0,[r5,#0x24]
  00179bd2: cmp r0,#0x0
  00179bd4: beq 0x00179cc8
  00179bd6: ldr r1,[0x00179f64]
  00179bd8: add.w r11,sp,#0x158
  00179bdc: add.w r9,sp,#0x118
  00179be0: vldr.32 s16,[pc,#0x37c]
  00179be4: add r1,pc
  00179be6: mov.w r8,#0x0
  00179bea: ldr r1,[r1,#0x0]
  00179bec: str r1,[sp,#0xc0]
  00179bee: b 0x00179cc2
  00179bf4: ldr r0,[r6,#0x4]
  00179bf6: ldr.w r1,[r0,r8,lsl #0x2]
  00179bfa: ldr r0,[r1,#0x0]
  00179bfc: ldr r2,[r0,#0x28]
  00179bfe: add r0,sp,#0xc4
  00179c00: blx r2
  00179c02: ldr r0,[sp,#0xc0]
  00179c04: ldr.w r10,[r0,#0x0]
  00179c08: mov r0,r10
  00179c0a: blx 0x000717f4
  00179c0e: mov r1,r0
  00179c10: mov r0,r10
  00179c12: blx 0x0006ff1c
  00179c16: mov r1,r0
  00179c18: mov r0,r11
  00179c1a: movs r2,#0x3c
  00179c1c: blx 0x0006f1e4
  00179c20: add r0,sp,#0x10c
  00179c22: mov r1,r11
  00179c24: blx 0x0006f16c
  00179c28: add r0,sp,#0x100
  00179c2a: mov r1,r11
  00179c2c: blx 0x0006f4d8
  00179c30: mov r4,r9
  00179c32: add r1,sp,#0x10c
  00179c34: add r2,sp,#0xc4
  00179c36: add r3,sp,#0x100
  00179c38: mov r0,r9
  00179c3a: blx 0x000725e0
  00179c3e: mov r0,r11
  00179c40: mov r1,r4
  00179c42: blx 0x0006f148
  00179c46: ldr r0,[r6,#0x4]
  00179c48: ldr.w r0,[r0,r8,lsl #0x2]
  00179c4c: ldr r0,[r0,#0x8]
  00179c4e: mov r1,r11
  00179c50: blx 0x000721e4
  00179c54: ldr r0,[r6,#0x4]
  00179c56: ldr.w r0,[r0,r8,lsl #0x2]
  00179c5a: ldr r1,[r0,#0x0]
  00179c5c: ldr r2,[r1,#0x44]
  00179c5e: add r1,sp,#0xc4
  00179c60: blx r2
  00179c62: ldr r0,[r5,#0x24]
  00179c64: ldr r1,[r6,#0x4]
  00179c66: ldr r0,[r0,#0x0]
  00179c68: add r0,r8
  00179c6a: ldr.w r0,[r1,r0,lsl #0x2]
  00179c6e: ldr r0,[r0,#0x8]
  00179c70: mov r1,r11
  00179c72: blx 0x000721e4
  00179c76: mov r0,r9
  00179c78: mov r1,r11
  00179c7a: mov r4,r9
  00179c7c: blx 0x0006f4c0
  00179c80: vldr.32 s0,[sp,#0x118]
  00179c84: vldr.32 s2,[sp,#0xc4]
  00179c88: vmla.f32 s2,s0,s16
  00179c8c: vstr.32 s2,[sp,#0xc4]
  00179c90: mov r0,r4
  00179c92: mov r1,r11
  00179c94: blx 0x0006f4c0
  00179c98: vldr.32 s0,[sp,#0x120]
  00179c9c: vldr.32 s2,[sp,#0xcc]
  00179ca0: vmla.f32 s2,s0,s16
  00179ca4: vstr.32 s2,[sp,#0xcc]
  00179ca8: ldr r0,[r5,#0x24]
  00179caa: ldr r1,[r6,#0x4]
  00179cac: ldr r0,[r0,#0x0]
  00179cae: add r0,r8
  00179cb0: ldr.w r0,[r1,r0,lsl #0x2]
  00179cb4: ldr r1,[r0,#0x0]
  00179cb6: ldr r2,[r1,#0x44]
  00179cb8: add r1,sp,#0xc4
  00179cba: blx r2
  00179cbc: ldr r0,[r5,#0x24]
  00179cbe: add.w r8,r8,#0x1
  00179cc2: ldr r0,[r0,#0x0]
  00179cc4: cmp r8,r0
  00179cc6: bcc 0x00179bf4
  00179cc8: ldr r0,[0x00179f68]
  00179cca: adr r1,[0x179f50]
  00179ccc: vmov.i32 q8,#0x0
  00179cd0: add r2,sp,#0x158
  00179cd2: add r0,pc
  00179cd4: vld1.64 {d8,d9},[r1]
  00179cd8: orr r1,r2,#0x4
  00179cdc: ldr r0,[r0,#0x0]
  00179cde: vst1.32 {d16,d17},[r1]
  00179ce2: add.w r1,r2,#0x18
  00179ce6: vst1.64 {d16,d17},[r1]
  00179cea: add.w r1,r2,#0x28
  00179cee: ldr r0,[r0,#0x0]
  00179cf0: vst1.64 {d8,d9},[r1]
  00179cf4: mov.w r1,#0x3f800000
  00179cf8: str r1,[sp,#0x158]
  00179cfa: str r1,[sp,#0x16c]
  00179cfc: str r1,[sp,#0x190]
  00179cfe: blx 0x00071a10
  00179d02: blx 0x00071950
  00179d06: mov r9,r0
  00179d08: ldr r0,[0x00179f6c]
  00179d0a: add.w r4,r9,r9, lsl #0x1
  00179d0e: add r0,pc
  00179d10: add.w r8,r4,#0x1
  00179d14: add.w r1,r0,r4, lsl #0x2
  00179d18: add.w r10,r4,#0x2
  00179d1c: vldr.32 s0,[r1]
  00179d20: add.w r1,r0,r8, lsl #0x2
  00179d24: add.w r0,r0,r10, lsl #0x2
  00179d28: vcvt.f32.s32 s0,s0
  00179d2c: vldr.32 s2,[r1]
  00179d30: vcvt.f32.s32 s2,s2
  00179d34: vmov r2,s0
  00179d38: vldr.32 s0,[r0]
  00179d3c: vmov r3,s2
  00179d40: vcvt.f32.s32 s0,s0
  00179d44: add r0,sp,#0x118
  00179d46: add r1,sp,#0x158
  00179d48: vstr.32 s0,[sp]
  00179d4c: blx 0x0006f820
  00179d50: ldr r0,[0x00179f70]
  00179d52: add r0,pc
  00179d54: ldr.w r3,[r0,r9,lsl #0x2]
  00179d58: movs r1,#0x0
  00179d5a: add r6,sp,#0x118
  00179d5c: str r1,[sp,#0x0]
  00179d5e: add r1,sp,#0x158
  00179d60: mov r0,r6
  00179d62: movs r2,#0x0
  00179d64: blx 0x00072094
  00179d68: ldr r0,[0x00179f74]
  00179d6a: vmov.i32 q8,#0x0
  00179d6e: add r0,pc
  00179d70: add.w r1,r0,r4, lsl #0x2
  00179d74: vldr.32 s0,[r1]
  00179d78: add.w r1,r0,r8, lsl #0x2
  00179d7c: add.w r0,r0,r10, lsl #0x2
  00179d80: vcvt.f32.s32 s0,s0
  00179d84: vldr.32 s2,[r1]
  00179d88: orr r1,r6,#0x4
  00179d8c: vcvt.f32.s32 s2,s2
  00179d90: vst1.32 {d16,d17},[r1]
  00179d94: add.w r1,r6,#0x18
  00179d98: vst1.64 {d16,d17},[r1]
  00179d9c: vmov r2,s0
  00179da0: vldr.32 s0,[r0]
  00179da4: add.w r0,r6,#0x28
  00179da8: vmov r3,s2
  00179dac: vst1.64 {d8,d9},[r0]
  00179db0: mov.w r0,#0x3f800000
  00179db4: vcvt.f32.s32 s0,s0
  00179db8: str r0,[sp,#0x118]
  00179dba: str r0,[sp,#0x12c]
  00179dbc: str r0,[sp,#0x150]
  00179dbe: add r0,sp,#0xc4
  00179dc0: mov r1,r6
  00179dc2: vstr.32 s0,[sp]
  00179dc6: blx 0x0006f820
  00179dca: ldr r0,[0x00179f78]
  00179dcc: add r0,pc
  00179dce: ldr.w r3,[r0,r9,lsl #0x2]
  00179dd2: movs r1,#0x0
  00179dd4: str r1,[sp,#0x0]
  00179dd6: add r0,sp,#0xc4
  00179dd8: add r1,sp,#0x118
  00179dda: movs r2,#0x0
  00179ddc: blx 0x00072094
  00179de0: movs r0,#0xf4
  00179de2: blx 0x0006eb24
  00179de6: str r0,[sp,#0xc0]
  00179de8: ldr r0,[sp,#0x158]
  00179dea: add.w r12,sp,#0x138
  00179dee: str r0,[sp,#0xb4]
  00179df0: ldr r0,[sp,#0x15c]
  00179df2: str r0,[sp,#0xb0]
  00179df4: ldr r0,[sp,#0x160]
  00179df6: str r0,[sp,#0xac]
  00179df8: ldr r0,[sp,#0x164]
  00179dfa: str r0,[sp,#0xa8]
  00179dfc: ldr r0,[sp,#0x118]
  00179dfe: str r0,[sp,#0x88]
  00179e00: ldr r0,[sp,#0x11c]
  00179e02: str r0,[sp,#0x84]
  00179e04: ldr r0,[sp,#0x120]
  00179e06: str r0,[sp,#0x80]
  00179e08: ldr r0,[sp,#0x124]
  00179e0a: str r0,[sp,#0x7c]
  00179e0c: ldr r0,[sp,#0x168]
  00179e0e: str r0,[sp,#0xa4]
  00179e10: ldr r0,[sp,#0x16c]
  00179e12: str r0,[sp,#0xa0]
  00179e14: ldr r0,[sp,#0x170]
  00179e16: str r0,[sp,#0x9c]
  00179e18: ldr r0,[sp,#0x174]
  00179e1a: str r0,[sp,#0x98]
  00179e1c: ldr r0,[sp,#0x178]
  00179e1e: str r0,[sp,#0x94]
  00179e20: ldr r0,[sp,#0x17c]
  00179e22: str r0,[sp,#0x90]
  00179e24: ldr r0,[sp,#0x180]
  00179e26: str r0,[sp,#0x8c]
  00179e28: ldr r0,[sp,#0x184]
  00179e2a: str r0,[sp,#0x78]
  00179e2c: ldr r0,[sp,#0x188]
  00179e2e: str r0,[sp,#0x74]
  00179e30: ldrd r4,r0,[sp,#0x18c]
  00179e34: str r0,[sp,#0x70]
  00179e36: ldrd r6,r10,[sp,#0x128]
  00179e3a: ldrd r8,lr,[sp,#0x130]
  00179e3e: ldm.w r12,{r1,r2,r3,r12}
  00179e42: ldrd r11,r9,[sp,#0x148]
  00179e46: ldr r0,[sp,#0x150]
  00179e48: strd r11,r9,[sp,#0x60]
  00179e4c: str r0,[sp,#0x68]
  00179e4e: movw r0,#0xbb8
  00179e52: str r0,[sp,#0x6c]
  00179e54: add r0,sp,#0x50
  00179e56: strd r6,r10,[sp,#0x40]
  00179e5a: strd r8,lr,[sp,#0x48]
  00179e5e: stm r0,{r1,r2,r3,r12}
  00179e62: ldr r0,[sp,#0x78]
  00179e64: str r0,[sp,#0x20]
  00179e66: ldr r0,[sp,#0x74]
  00179e68: strd r0,r4,[sp,#0x24]
  00179e6c: ldr r0,[sp,#0x70]
  00179e6e: str r0,[sp,#0x2c]
  00179e70: ldr r0,[sp,#0x88]
  00179e72: str r0,[sp,#0x30]
  00179e74: ldr r0,[sp,#0x84]
  00179e76: str r0,[sp,#0x34]
  00179e78: ldr r0,[sp,#0x80]
  00179e7a: str r0,[sp,#0x38]
  00179e7c: ldr r0,[sp,#0x7c]
  00179e7e: str r0,[sp,#0x3c]
  00179e80: ldr r0,[sp,#0xa8]
  00179e82: str r0,[sp,#0x0]
  00179e84: ldr r0,[sp,#0xa4]
  00179e86: str r0,[sp,#0x4]
  00179e88: ldr r0,[sp,#0xa0]
  00179e8a: str r0,[sp,#0x8]
  00179e8c: ldr r0,[sp,#0x9c]
  00179e8e: str r0,[sp,#0xc]
  00179e90: ldr r0,[sp,#0x98]
  00179e92: str r0,[sp,#0x10]
  00179e94: ldr r0,[sp,#0x94]
  00179e96: str r0,[sp,#0x14]
  00179e98: ldr r0,[sp,#0x90]
  00179e9a: str r0,[sp,#0x18]
  00179e9c: ldr r0,[sp,#0x8c]
  00179e9e: str r0,[sp,#0x1c]
  00179ea0: ldr r6,[sp,#0xc0]
  00179ea2: ldr r1,[sp,#0xb4]
  00179ea4: ldrd r3,r2,[sp,#0xac]
  00179ea8: mov r0,r6
  00179eaa: blx 0x00072b74
  00179eae: ldr r0,[0x00179f7c]
  00179eb0: str r6,[r5,#0x48]
  00179eb2: add r0,pc
  00179eb4: ldr r0,[r0,#0x0]
  00179eb6: ldr r4,[r0,#0x0]
  00179eb8: mov r0,r4
  00179eba: blx 0x000717f4
  00179ebe: mov r1,r0
  00179ec0: ldr r6,[sp,#0xb8]
  00179ec2: add r2,sp,#0x158
  00179ec4: mov r0,r4
  00179ec6: blx 0x000720a0
  00179eca: movs r0,#0x1
  00179ecc: strb.w r0,[r5,#0xb0]
  00179ed0: movs r0,#0x0
  00179ed2: strb.w r0,[r5,#0xbd]
  00179ed6: ldr r0,[sp,#0x194]
  00179ed8: ldr r1,[r6,#0x0]
  00179eda: subs r0,r1,r0
  00179edc: itttt eq
  00179ede: mov.eq r0,r5
  00179ee0: add.eq sp,#0x198
  00179ee2: vpop.eq {d8,d9}
  00179ee6: add.eq sp,#0x4
  00179ee8: itt eq
  00179eea: pop.eq.w {r8,r9,r10,r11}
  00179eee: pop.eq {r4,r5,r6,r7,pc}
  00179ef0: blx 0x0006e824
  00179ef4: ldr r1,[r5,#0x24]
  00179ef6: ldr r0,[r1,#0x4]
  00179ef8: ldr.w r0,[r0,r6,lsl #0x2]
  00179efc: blx 0x00077cf8
  00179f00: mov r0,r5
  00179f02: blx 0x000753dc
  00179f06: b 0x00179b9a
```
