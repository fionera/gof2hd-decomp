# StarMap::StarMap
elf 0xc67a4 body 930
Sig: undefined __thiscall StarMap(StarMap * this, bool param_1, Mission * param_2, bool param_3, int param_4)

## decompile
```c

/* StarMap::StarMap(bool, Mission*, bool, int) */

void __thiscall
StarMap::StarMap(StarMap *this,bool param_1,Mission *param_2,bool param_3,int param_4)

{
  float fVar1;
  float fVar2;
  short sVar3;
  int *piVar4;
  undefined4 uVar5;
  void *pvVar6;
  AEGeometry *pAVar7;
  Array<AEGeometry*> *this_00;
  Array<AbyssEngine::AEMath::Vector*> *this_01;
  int iVar8;
  undefined4 *puVar9;
  int iVar10;
  int *piVar11;
  ushort uVar12;
  code *pcVar13;
  uint *puVar14;
  uint uVar15;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float fVar16;
  float extraout_s0_02;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  undefined4 in_s14;
  float fVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  float fVar20;
  undefined4 uVar21;
  float fVar22;
  float fVar23;
  Vector aVStack_68 [8];
  undefined4 uStack_60;
  int local_5c;
  
  uVar5 = 0;
  uVar18 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar19 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar21 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar11 = *(int **)(DAT_000d6b80 + 0xd67bc);
  pcVar13 = *(code **)(DAT_000d6b84 + 0xd67c2);
  _uStack_60 = CONCAT44(*piVar11,in_s14);
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = uVar18;
  *(undefined4 *)(this + 0x80) = uVar19;
  *(undefined4 *)(this + 0x84) = uVar21;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  (*pcVar13)(this + 0xac);
  (*pcVar13)(this + 0xbc);
  (*pcVar13)(this + 0xcc);
  iVar10 = DAT_000d6b88;
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x154) = uVar5;
  *(undefined4 *)(this + 0x158) = uVar18;
  *(undefined4 *)(this + 0x15c) = uVar19;
  *(undefined4 *)(this + 0x160) = uVar21;
  piVar4 = *(int **)(iVar10 + 0xd67fa);
  *(undefined4 *)(this + 0x144) = uVar5;
  *(undefined4 *)(this + 0x148) = uVar18;
  *(undefined4 *)(this + 0x14c) = uVar19;
  *(undefined4 *)(this + 0x150) = uVar21;
  *(undefined4 *)(this + 0x188) = 0;
  *(undefined4 *)(this + 0x18c) = 0;
  *(undefined4 *)(this + 400) = 0;
  uVar5 = *(undefined4 *)(*piVar4 + 0x88);
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 0x1c8) = uVar5;
  *(undefined4 *)(this + 0x60) = 0xffffffff;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x194) = 0;
  *(undefined4 *)(this + 0x198) = 0;
  this[0x1e4] = (StarMap)0x0;
  this[1] = (StarMap)0x0;
  this[0x120] = (StarMap)0x0;
  *(undefined4 *)(this + 0x17c) = 0;
  *(undefined4 *)(this + 0x180) = 0;
  *(undefined4 *)(this + 0x168) = 0;
  *(undefined4 *)(this + 0x16c) = 0;
  *(undefined4 *)(this + 0x184) = 0;
  *(undefined4 *)(this + 0x171) = 0;
  *(undefined4 *)(this + 0x16d) = 0;
  pvVar6 = operator_new__(0x14);
  iVar10 = DAT_000d6b8c;
  *(void **)(this + 0xfc) = pvVar6;
  uVar5 = Galaxy::getSystems((Galaxy *)**(undefined4 **)(iVar10 + 0xd6860));
  *(undefined4 *)(this + 0x54) = uVar5;
  *(undefined4 *)(this + 0x10) = 500;
  *(undefined4 *)(this + 0x14) = 500;
  pAVar7 = operator_new(0xc0);
  puVar14 = *(uint **)(DAT_000d6b90 + 0xd6880);
  AEGeometry::AEGeometry(pAVar7,(PaintCanvas *)*puVar14);
  *(AEGeometry **)(this + 0x6c) = pAVar7;
  this_00 = operator_new(0xc);
  Array<AEGeometry*>::Array(this_00);
  *(Array<AEGeometry*> **)(this + 0x68) = this_00;
  this_01 = operator_new(0xc);
  Array<AbyssEngine::AEMath::Vector*>::Array(this_01);
  *(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x194) = this_01;
  ArraySetLength<AEGeometry*>(0x22,*(Array **)(this + 0x68));
  ArraySetLength<AbyssEngine::AEMath::Vector*>(0x22,*(Array **)(this + 0x194));
  fVar2 = DAT_000d6b7c;
  fVar1 = DAT_000d6b78;
  fVar17 = DAT_000d6b70;
  fVar20 = DAT_000d6b74;
  for (uVar15 = 0; uVar15 < **(uint **)(this + 0x68); uVar15 = uVar15 + 1) {
    sVar3 = SolarSystem::getTextureIndex();
    iVar10 = Status::getCurrentCampaignMission();
    uVar12 = sVar3 + 0x4696U;
    if (0x9d < iVar10) {
      uVar12 = 0x469b;
    }
    if (uVar15 != 0x1b) {
      uVar12 = sVar3 + 0x4696U;
    }
    pAVar7 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar7,uVar12,(PaintCanvas *)*puVar14,false);
    *(AEGeometry **)(*(int *)(*(int *)(this + 0x68) + 4) + uVar15 * 4) = pAVar7;
    iVar10 = SolarSystem::getTextureIndex();
    fVar16 = extraout_s0;
    fVar22 = extraout_s1;
    fVar23 = extraout_s2;
    if ((iVar10 == 0xf) &&
       (iVar10 = Status::getCurrentCampaignMission(), fVar16 = extraout_s0_00,
       fVar22 = extraout_s1_00, fVar23 = extraout_s2_00, iVar10 < 0x9e)) {
      Status::getCurrentCampaignMission();
      fVar16 = extraout_s0_01;
      fVar22 = extraout_s1_01;
      fVar23 = extraout_s2_01;
    }
    AEGeometry::setScaling(fVar16,fVar22,fVar23);
    iVar10 = SolarSystem::getX(*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + uVar15 * 4));
    iVar8 = SolarSystem::getY(*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + uVar15 * 4));
    fVar22 = (float)VectorSignedToFloat(100 - iVar10,(byte)(in_fpscr >> 0x16) & 3);
    fVar23 = (float)VectorSignedToFloat(100 - iVar8,(byte)(in_fpscr >> 0x16) & 3);
    iVar10 = SolarSystem::getZ(*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + uVar15 * 4));
    fVar16 = (float)VectorSignedToFloat(100 - iVar10,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat((int)((fVar22 / fVar17) * fVar20) + -10000,(byte)(in_fpscr >> 0x16) & 3);
    fVar22 = (float)VectorSignedToFloat((int)((fVar23 / fVar17) * fVar1) + -9000,
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar16 = (float)VectorSignedToFloat((int)((fVar16 / fVar17) * fVar2) + 1000,
                                        (byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::setPosition(fVar16,extraout_s1_02,fVar22);
    AEGeometry::addChild(*(uint *)(this + 0x6c));
    puVar9 = operator_new(0xc);
    *puVar9 = 0;
    puVar9[1] = 0;
    puVar9[2] = 0;
    *(undefined4 **)(*(int *)(*(int *)(this + 0x194) + 4) + uVar15 * 4) = puVar9;
  }
  AbyssEngine::AERandom::reset();
  iVar10 = DAT_000d6b98;
  *(undefined4 *)(this + 0xf8) = 0;
  this[0xa9] = (StarMap)0x0;
  piVar4 = *(int **)(iVar10 + 0xd6a98);
  iVar10 = Status::getCurrentCampaignMission();
  if ((0x1f < iVar10) && (-1 < *(int *)(*piVar4 + 0x7c))) {
    pAVar7 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar7,0x4262,(PaintCanvas *)*puVar14,false);
    *(AEGeometry **)(this + 0xf8) = pAVar7;
    AEGeometry::getPosition();
    AEGeometry::setPosition(pAVar7,aVStack_68);
    uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
    AbyssEngine::Transform::SetAnimationState(uVar5,2,0);
    AEGeometry::setRotation
              (*(AEGeometry **)(this + 0xf8),extraout_s0_02,extraout_s1_03,extraout_s2_02);
  }
  init((EVP_PKEY_CTX *)this);
  iVar10 = *piVar11 - local_5c;
  if (iVar10 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar10);
}

```

## target disasm
```
  000d67a4: push {r4,r5,r6,r7,lr}
  000d67a6: add r7,sp,#0xc
  000d67a8: push {r7,r8,r9,r10,r11}
  000d67ac: vpush {d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13}
  000d67b0: mov r8,r0
  000d67b2: ldr r0,[0x000d6b80]
  000d67b4: mov r11,r1
  000d67b6: ldr r1,[0x000d6b84]
  000d67b8: add r0,pc
  000d67ba: vmov.i32 q4,#0x0
  000d67be: add r1,pc
  000d67c0: movs r5,#0x0
  000d67c2: ldr r4,[r0,#0x0]
  000d67c4: mov r9,r3
  000d67c6: ldr r6,[r1,#0x0]
  000d67c8: mov r10,r2
  000d67ca: ldr r0,[r4,#0x0]
  000d67cc: str r0,[sp,#0x24]
  000d67ce: add.w r0,r8,#0x78
  000d67d2: vst1.32 {d8,d9},[r0]
  000d67d6: add.w r0,r8,#0xac
  000d67da: strd r5,r5,[r8,#0x88]
  000d67de: blx r6
  000d67e0: add.w r0,r8,#0xbc
  000d67e4: blx r6
  000d67e6: add.w r0,r8,#0xcc
  000d67ea: blx r6
  000d67ec: ldr r0,[0x000d6b88]
  000d67ee: add.w r1,r8,#0x154
  000d67f2: str.w r5,[r8,#0x164]
  000d67f6: add r0,pc
  000d67f8: vst1.32 {d8,d9},[r1]
  000d67fc: add.w r1,r8,#0x144
  000d6800: ldr r0,[r0,#0x0]
  000d6802: vst1.32 {d8,d9},[r1]
  000d6806: strd r5,r5,[r8,#0x188]
  000d680a: str.w r5,[r8,#0x190]
  000d680e: ldr r0,[r0,#0x0]
  000d6810: ldr.w r0,[r0,#0x88]
  000d6814: str.w r5,[r8,#0x4]
  000d6818: str.w r0,[r8,#0x1c8]
  000d681c: mov.w r0,#0xffffffff
  000d6820: str.w r0,[r8,#0x60]
  000d6824: movs r0,#0x14
  000d6826: str.w r5,[r8,#0xa4]
  000d682a: str.w r5,[r8,#0x58]
  000d682e: strd r5,r5,[r8,#0x194]
  000d6832: strb.w r5,[r8,#0x1e4]
  000d6836: strb.w r5,[r8,#0x1]
  000d683a: strb.w r5,[r8,#0x120]
  000d683e: strd r5,r5,[r8,#0x17c]
  000d6842: strd r5,r5,[r8,#0x168]
  000d6846: str.w r5,[r8,#0x184]
  000d684a: str.w r5,[r8,#0x171]
  000d684e: str.w r5,[r8,#0x16d]
  000d6852: blx 0x0006ec08
  000d6856: ldr r1,[0x000d6b8c]
  000d6858: str.w r0,[r8,#0xfc]
  000d685c: add r1,pc
  000d685e: ldr r1,[r1,#0x0]
  000d6860: ldr r0,[r1,#0x0]
  000d6862: blx 0x00071884
  000d6866: str.w r0,[r8,#0x54]
  000d686a: mov.w r0,#0x1f4
  000d686e: strd r0,r0,[r8,#0x10]
  000d6872: movs r0,#0xc0
  000d6874: blx 0x0006eb24
  000d6878: mov r5,r0
  000d687a: ldr r0,[0x000d6b90]
  000d687c: add r0,pc
  000d687e: ldr r6,[r0,#0x0]
  000d6880: ldr r1,[r6,#0x0]
  000d6882: mov r0,r5
  000d6884: blx 0x000720f4
  000d6888: movs r0,#0xc
  000d688a: str.w r5,[r8,#0x6c]
  000d688e: blx 0x0006eb24
  000d6892: mov r5,r0
  000d6894: blx 0x000717b8
  000d6898: movs r0,#0xc
  000d689a: str r6,[sp,#0x14]
  000d689c: strd r11,r4,[sp,#0x4]
  000d68a0: strd r10,r9,[sp,#0xc]
  000d68a4: str.w r5,[r8,#0x68]
  000d68a8: blx 0x0006eb24
  000d68ac: mov r5,r0
  000d68ae: blx 0x00074530
  000d68b2: ldr.w r1,[r8,#0x68]
  000d68b6: movs r0,#0x22
  000d68b8: str.w r5,[r8,#0x194]
  000d68bc: blx 0x00073504
  000d68c0: ldr.w r1,[r8,#0x194]
  000d68c4: movs r0,#0x22
  000d68c6: blx 0x0007453c
  000d68ca: vldr.32 s16,[pc,#0x2a4]
  000d68ce: mov.w r9,#0x0
  000d68d2: vldr.32 s18,[pc,#0x2a0]
  000d68d6: mov.w r11,#0x0
  000d68da: vldr.32 s20,[pc,#0x29c]
  000d68de: vldr.32 s22,[pc,#0x29c]
  000d68e2: b 0x000d6a74
  000d68e4: ldr.w r0,[r8,#0x54]
  000d68e8: ldr r0,[r0,#0x4]
  000d68ea: ldr.w r0,[r0,r11,lsl #0x2]
  000d68ee: blx 0x0007390c
  000d68f2: ldr r1,[0x000d6b9c]
  000d68f4: movw r2,#0x4696
  000d68f8: adds r4,r0,r2
  000d68fa: add r1,pc
  000d68fc: ldr r6,[r1,#0x0]
  000d68fe: ldr r1,[r6,#0x0]
  000d6900: mov r0,r1
  000d6902: blx 0x00071770
  000d6906: mov r5,r4
  000d6908: cmp r0,#0x9d
  000d690a: it gt
  000d690c: movw.gt r5,#0x469b
  000d6910: movs r0,#0xc0
  000d6912: cmp.w r11,#0x1b
  000d6916: it ne
  000d6918: mov.ne r5,r4
  000d691a: blx 0x0006eb24
  000d691e: mov r10,r0
  000d6920: ldr r0,[sp,#0x14]
  000d6922: uxth r1,r5
  000d6924: ldr r2,[r0,#0x0]
  000d6926: mov r0,r10
  000d6928: movs r3,#0x0
  000d692a: blx 0x0007207c
  000d692e: ldr.w r0,[r8,#0x68]
  000d6932: ldr r0,[r0,#0x4]
  000d6934: str.w r10,[r0,r11,lsl #0x2]
  000d6938: ldr.w r0,[r8,#0x54]
  000d693c: ldr r0,[r0,#0x4]
  000d693e: ldr.w r0,[r0,r11,lsl #0x2]
  000d6942: blx 0x0007390c
  000d6946: cmp r0,#0xf
  000d6948: bne 0x000d6970
  000d694a: ldr r0,[r6,#0x0]
  000d694c: blx 0x00071770
  000d6950: cmp r0,#0x9d
  000d6952: bgt 0x000d6970
  000d6954: ldr r0,[r6,#0x0]
  000d6956: blx 0x00071770
  000d695a: mov r1,r0
  000d695c: ldr.w r0,[r8,#0x68]
  000d6960: cmp r1,#0x69
  000d6962: ldr r0,[r0,#0x4]
  000d6964: ldr.w r0,[r0,r11,lsl #0x2]
  000d6968: ite gt
  000d696a: ldr.gt r1,[0x000d6b68]
  000d696c: ldr.le r1,[0x000d6b6c]
  000d696e: b 0x000d697c
  000d6970: ldr.w r0,[r8,#0x68]
  000d6974: ldr r1,[0x000d6b64]
  000d6976: ldr r0,[r0,#0x4]
  000d6978: ldr.w r0,[r0,r11,lsl #0x2]
  000d697c: mov r2,r1
  000d697e: mov r3,r1
  000d6980: blx 0x000727b4
  000d6984: ldr.w r0,[r8,#0x54]
  000d6988: ldr.w r1,[r8,#0x68]
  000d698c: ldr r0,[r0,#0x4]
  000d698e: ldr r1,[r1,#0x4]
  000d6990: ldr.w r0,[r0,r11,lsl #0x2]
  000d6994: ldr.w r5,[r1,r11,lsl #0x2]
  000d6998: blx 0x00071968
  000d699c: mov r6,r0
  000d699e: ldr.w r0,[r8,#0x54]
  000d69a2: ldr r0,[r0,#0x4]
  000d69a4: ldr.w r0,[r0,r11,lsl #0x2]
  000d69a8: blx 0x00071974
  000d69ac: ldr.w r1,[r8,#0x54]
  000d69b0: rsb.w r2,r6,#0x64
  000d69b4: rsb.w r0,r0,#0x64
  000d69b8: vmov s0,r2
  000d69bc: ldr r1,[r1,#0x4]
  000d69be: vcvt.f32.s32 s24,s0
  000d69c2: vmov s0,r0
  000d69c6: ldr.w r0,[r1,r11,lsl #0x2]
  000d69ca: vcvt.f32.s32 s26,s0
  000d69ce: blx 0x00074548
  000d69d2: vdiv.f32 s2,s24,s16
  000d69d6: rsb.w r0,r0,#0x64
  000d69da: movw r1,#0x2710
  000d69de: vmul.f32 s2,s2,s18
  000d69e2: vdiv.f32 s4,s26,s16
  000d69e6: vmov s0,r0
  000d69ea: vmul.f32 s4,s4,s20
  000d69ee: vcvt.f32.s32 s0,s0
  000d69f2: vcvt.s32.f32 s2,s2
  000d69f6: vdiv.f32 s0,s0,s16
  000d69fa: vmov r0,s2
  000d69fe: vcvt.s32.f32 s2,s4
  000d6a02: vmul.f32 s0,s0,s22
  000d6a06: subs r0,r0,r1
  000d6a08: movw r1,#0x2328
  000d6a0c: vmov s4,r0
  000d6a10: vmov r0,s2
  000d6a14: vcvt.f32.s32 s4,s4
  000d6a18: vcvt.s32.f32 s0,s0
  000d6a1c: subs r0,r0,r1
  000d6a1e: vmov r1,s4
  000d6a22: vmov s2,r0
  000d6a26: vmov r0,s0
  000d6a2a: vcvt.f32.s32 s2,s2
  000d6a2e: vmov r2,s2
  000d6a32: add.w r0,r0,#0x3e8
  000d6a36: vmov s0,r0
  000d6a3a: mov r0,r5
  000d6a3c: vcvt.f32.s32 s0,s0
  000d6a40: vmov r3,s0
  000d6a44: blx 0x00073048
  000d6a48: ldrd r1,r0,[r8,#0x68]
  000d6a4c: ldr r1,[r1,#0x4]
  000d6a4e: ldr.w r1,[r1,r11,lsl #0x2]
  000d6a52: ldr r1,[r1,#0xc]
  000d6a54: blx 0x000720e8
  000d6a58: movs r0,#0xc
  000d6a5a: blx 0x0006eb24
  000d6a5e: strd r9,r9,[r0,#0x0]
  000d6a62: str.w r9,[r0,#0x8]
  000d6a66: ldr.w r1,[r8,#0x194]
  000d6a6a: ldr r1,[r1,#0x4]
  000d6a6c: str.w r0,[r1,r11,lsl #0x2]
  000d6a70: add.w r11,r11,#0x1
  000d6a74: ldr.w r0,[r8,#0x68]
  000d6a78: ldr r0,[r0,#0x0]
  000d6a7a: cmp r11,r0
  000d6a7c: bcc.w 0x000d68e4
  000d6a80: ldr r0,[0x000d6b94]
  000d6a82: add r0,pc
  000d6a84: ldr r0,[r0,#0x0]
  000d6a86: ldr r0,[r0,#0x0]
  000d6a88: blx 0x000718cc
  000d6a8c: ldr r0,[0x000d6b98]
  000d6a8e: movs r1,#0x0
  000d6a90: str.w r1,[r8,#0xf8]
  000d6a94: add r0,pc
  000d6a96: strb.w r1,[r8,#0xa9]
  000d6a9a: ldr r6,[r0,#0x0]
  000d6a9c: ldr r0,[r6,#0x0]
  000d6a9e: blx 0x00071770
  000d6aa2: add.w r11,sp,#0x4
  000d6aa6: ldr.w r9,[sp,#0x10]
  000d6aaa: cmp r0,#0x1f
  000d6aac: ldm.w r11,{r4,r10,r11}
  000d6ab0: ble 0x000d6b18
  000d6ab2: ldr r0,[r6,#0x0]
  000d6ab4: ldr r0,[r0,#0x7c]
  000d6ab6: cmp r0,#0x0
  000d6ab8: blt 0x000d6b18
  000d6aba: movs r0,#0xc0
  000d6abc: blx 0x0006eb24
  000d6ac0: mov r5,r0
  000d6ac2: ldr r0,[sp,#0x14]
  000d6ac4: ldr r2,[r0,#0x0]
  000d6ac6: mov r0,r5
  000d6ac8: movw r1,#0x4262
  000d6acc: movs r3,#0x0
  000d6ace: blx 0x0007207c
  000d6ad2: str.w r5,[r8,#0xf8]
  000d6ad6: ldr r1,[r6,#0x0]
  000d6ad8: add r6,sp,#0x18
  000d6ada: ldr.w r0,[r8,#0x68]
  000d6ade: ldr r1,[r1,#0x7c]
  000d6ae0: ldr r0,[r0,#0x4]
  000d6ae2: ldr.w r1,[r0,r1,lsl #0x2]
  000d6ae6: mov r0,r6
  000d6ae8: blx 0x000720b8
  000d6aec: mov r0,r5
  000d6aee: mov r1,r6
  000d6af0: blx 0x00072148
  000d6af4: ldr.w r1,[r8,#0xf8]
  000d6af8: ldr r0,[sp,#0x14]
  000d6afa: ldr r1,[r1,#0xc]
  000d6afc: ldr r0,[r0,#0x0]
  000d6afe: blx 0x00072088
  000d6b02: movs r1,#0x2
  000d6b04: movs r2,#0x0
  000d6b06: blx 0x0006fd18
  000d6b0a: ldr.w r0,[r8,#0xf8]
  000d6b0e: movs r1,#0x0
  000d6b10: ldr r2,[0x000d6b60]
  000d6b12: movs r3,#0x0
  000d6b14: blx 0x00073054
  000d6b18: ldr r0,[r7,#0x8]
  000d6b1a: mov r1,r4
  000d6b1c: str r0,[sp,#0x0]
  000d6b1e: mov r0,r8
  000d6b20: mov r2,r11
  000d6b22: mov r3,r9
  000d6b24: blx 0x00074554
  000d6b28: ldr r0,[sp,#0x24]
  000d6b2a: ldr.w r1,[r10,#0x0]
  000d6b2e: subs r0,r1,r0
  000d6b30: itttt eq
  000d6b32: mov.eq r0,r8
  000d6b34: vpop.eq {d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13}
  000d6b38: add.eq sp,#0x4
  000d6b3a: pop.eq.w {r8,r9,r10,r11}
  000d6b3e: it eq
  000d6b40: pop.eq {r4,r5,r6,r7,pc}
  000d6b42: blx 0x0006e824
```
