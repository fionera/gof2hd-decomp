# CutScene::initialize
elf 0x98628 body 972
Sig: undefined __stdcall initialize(void)

## decompile
```c

/* CutScene::initialize() */

void CutScene::initialize(void)

{
  ulonglong uVar1;
  CutScene *in_r0;
  int iVar2;
  int iVar3;
  void *pvVar4;
  AEGeometry *pAVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  Level *this;
  int *piVar9;
  uint *puVar10;
  int *piVar11;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float fVar12;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  ulonglong in_d16;
  undefined8 uVar13;
  float local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined1 auStack_68 [64];
  int local_28;
  
  piVar11 = *(int **)(DAT_000a89fc + 0xa863a);
  local_28 = *piVar11;
  this = *(Level **)in_r0;
  if (this == (Level *)0x0) {
    this = operator_new(0x2a0);
    Level::Level(this,*(int *)(in_r0 + 0x88));
    *(Level **)in_r0 = this;
  }
  do {
    iVar2 = Level::init((EVP_PKEY_CTX *)this);
    this = *(Level **)in_r0;
  } while (iVar2 == 0);
  iVar2 = Level::getPlayer();
  *(int *)(in_r0 + 0x60) = iVar2;
  if (iVar2 != 0) {
    PlayerEgo::setActive(SUB41(iVar2,0));
  }
  Level::initParticleSystems(*(Level **)in_r0);
  iVar2 = DAT_000a8a5c;
  iVar3 = *(int *)(in_r0 + 0x88);
  if (iVar3 == 2) {
    puVar10 = *(uint **)(DAT_000a8a50 + 0xa8718);
    AbyssEngine::PaintCanvas::CameraCreate((PaintCanvas *)*puVar10,(uint *)(in_r0 + 0x74));
    AbyssEngine::PaintCanvas::CameraSetPerspective
              (*puVar10,extraout_s0_00,extraout_s1_00,extraout_s2_00);
    AbyssEngine::PaintCanvas::CameraSetCurrent((PaintCanvas *)*puVar10,*(uint *)(in_r0 + 0x74));
    uVar6 = AbyssEngine::PaintCanvas::CameraGetLocal(*puVar10);
    __aeabi_memcpy(auStack_68,uVar6,0x3c);
    piVar9 = *(int **)(DAT_000a8a54 + 0xa875a);
    iVar2 = AbyssEngine::AERandom::nextInt(*piVar9);
    iVar3 = AbyssEngine::AERandom::nextInt(*piVar9);
    VectorSignedToFloat(iVar2 + -20000,(byte)(in_fpscr >> 0x16) & 3);
    fVar12 = (float)VectorSignedToFloat(iVar3 + 40000,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::MatrixSetTranslation
              ((Matrix *)&local_a4,fVar12,extraout_s1_01,extraout_s2_01);
    *(undefined4 *)(in_r0 + 4) = DAT_000a8a34;
    AbyssEngine::AEMath::MatrixSetRotation
              ((Matrix *)&local_a4,extraout_s0_01,extraout_s1_02,extraout_s2_02);
    AbyssEngine::PaintCanvas::CameraSetLocal(*puVar10,*(Matrix **)(in_r0 + 0x74));
    uVar13 = Status::getPlayingTime();
    if ((((int)(uint)((int)uVar13 == 0) <= (int)((ulonglong)uVar13 >> 0x20)) &&
        (puVar10 = (uint *)Level::getEnemies(), puVar10 != (uint *)0x0)) && (1 < *puVar10)) {
      iVar8 = puVar10[1] + *puVar10 * 4;
      piVar9 = *(int **)(iVar8 + -8);
      if ((piVar9 != (int *)0x0) && (*(int *)(iVar8 + -4) != 0)) {
        local_a4 = VectorSignedToFloat(iVar2 + -24000,(byte)(in_fpscr >> 0x16) & 3);
        local_9c = VectorSignedToFloat(iVar3 + 0x9a4c,(byte)(in_fpscr >> 0x16) & 3);
        local_a0 = DAT_000a8a38;
        (**(code **)(*piVar9 + 0x44))(piVar9,&local_a4);
        local_a4 = VectorSignedToFloat(iVar2 + -0x5b68,(byte)(in_fpscr >> 0x16) & 3);
        local_9c = VectorSignedToFloat(iVar3 + 0x96c8,(byte)(in_fpscr >> 0x16) & 3);
        piVar9 = *(int **)(puVar10[1] + *puVar10 * 4 + -4);
        local_a0 = DAT_000a8a3c;
        (**(code **)(*piVar9 + 0x44))(piVar9,&local_a4);
      }
    }
  }
  else if (iVar3 == 0x17) {
    *(undefined4 *)(in_r0 + 100) = 0;
    puVar10 = *(uint **)(iVar2 + 0xa8882);
    AbyssEngine::PaintCanvas::CameraCreate((PaintCanvas *)*puVar10,(uint *)(in_r0 + 0x70));
    AbyssEngine::PaintCanvas::CameraSetPerspective
              (*puVar10,extraout_s0_02,extraout_s1_03,extraout_s2_03);
    AbyssEngine::PaintCanvas::CameraSetCurrent((PaintCanvas *)*puVar10,*(uint *)(in_r0 + 0x70));
    uVar6 = AbyssEngine::PaintCanvas::CameraGetLocal(*puVar10);
    __aeabi_memcpy(auStack_68,uVar6,0x3c);
    AbyssEngine::AEMath::MatrixSetRotation
              ((Matrix *)&local_a4,extraout_s0_03,extraout_s1_04,extraout_s2_04);
    AbyssEngine::AEMath::MatrixSetTranslation
              ((Matrix *)&local_a4,extraout_s0_04,extraout_s1_05,extraout_s2_05);
    AbyssEngine::PaintCanvas::CameraSetLocal(*puVar10,*(Matrix **)(in_r0 + 0x70));
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar10,(uint *)(in_r0 + 0x78));
    Level::getEnemies();
    AEGeometry::getPosition();
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar10);
    AbyssEngine::AEMath::MatrixSetTranslation
              ((Matrix *)&local_a4,local_a8,extraout_s1_06,extraout_s2_06);
    AbyssEngine::PaintCanvas::TransformAddChild
              ((PaintCanvas *)*puVar10,*(uint *)(in_r0 + 0x78),*(uint *)(in_r0 + 0x70));
    resetCamera();
    checkForTurret(in_r0);
  }
  else if (iVar3 == 4) {
    puVar10 = *(uint **)(DAT_000a8a48 + 0xa869c);
    AbyssEngine::PaintCanvas::CameraCreate((PaintCanvas *)*puVar10,(uint *)(in_r0 + 0x6c));
    AbyssEngine::PaintCanvas::CameraSetPerspective(*puVar10,extraout_s0,extraout_s1,extraout_s2);
    if (*(TargetFollowCamera **)(in_r0 + 0x68) != (TargetFollowCamera *)0x0) {
      pvVar4 = (void *)TargetFollowCamera::~TargetFollowCamera
                                 (*(TargetFollowCamera **)(in_r0 + 0x68));
      operator_delete(pvVar4);
      *(undefined4 *)(in_r0 + 0x68) = 0;
    }
    AbyssEngine::PaintCanvas::CameraSetCurrent((PaintCanvas *)*puVar10,*(uint *)(in_r0 + 0x6c));
    resetCamera();
    Status::getSystem();
    iVar2 = SolarSystem::getRace();
    if (iVar2 == 3) {
      pAVar5 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar5,0x36d6,(PaintCanvas *)*puVar10,false);
      *(AEGeometry **)(in_r0 + 0x2c) = pAVar5;
      uVar6 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
      AbyssEngine::Transform::SetAnimationState(uVar6,0,0);
    }
    else {
      Status::getSystem();
      iVar2 = SolarSystem::getRace();
      if (iVar2 == 0) {
        pAVar5 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar5,0x37c8,(PaintCanvas *)*puVar10,false);
        *(AEGeometry **)(in_r0 + 0x30) = pAVar5;
        pAVar5 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar5,0x37c7,(PaintCanvas *)*puVar10,false);
        *(AEGeometry **)(in_r0 + 0x34) = pAVar5;
        AEGeometry::addChild(*(uint *)(in_r0 + 0x30));
        if (*(AEGeometry **)(in_r0 + 0x34) != (AEGeometry *)0x0) {
          pvVar4 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(in_r0 + 0x34));
          operator_delete(pvVar4);
        }
        *(undefined4 *)(in_r0 + 0x34) = 0;
      }
      else {
        Status::getSystem();
        SolarSystem::getRace();
      }
    }
  }
  pAVar5 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar5,(PaintCanvas *)**(undefined4 **)(DAT_000a8a60 + 0xa8998));
  *(AEGeometry **)(in_r0 + 0x20) = pAVar5;
  AEGeometry::setRotationOrder(pAVar5,2);
  *(undefined4 *)(in_r0 + 0x50) = 0;
  *(undefined4 *)(in_r0 + 0x54) = 0;
  *(undefined4 *)(in_r0 + 0x7c) = 0;
  *(undefined4 *)(in_r0 + 0x80) = 0;
  *(undefined4 *)(in_r0 + 0x84) = 0;
  uVar7 = AbyssEngine::ApplicationManager::GetCurrentTimeMillis();
  in_r0[0x5c] = (CutScene)0x1;
  uVar1 = in_d16 & 0xffff0000ffff0000 | (ulonglong)uVar7 & 0xffff;
  *(ulonglong *)(in_r0 + 0x40) = uVar1;
  *(ulonglong *)(in_r0 + 0x48) = uVar1;
  if (*piVar11 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a8628: push {r4,r5,r6,r7,lr}
  000a862a: add r7,sp,#0xc
  000a862c: push {r8,r9,r10,r11}
  000a8630: sub sp,#0x94
  000a8632: mov r4,r0
  000a8634: ldr r0,[0x000a89fc]
  000a8636: add r0,pc
  000a8638: ldr.w r10,[r0,#0x0]
  000a863c: ldr.w r0,[r10,#0x0]
  000a8640: str r0,[sp,#0x90]
  000a8642: ldr r5,[r4,#0x0]
  000a8644: cbnz r5,0x000a865a
  000a8646: mov.w r0,#0x2a0
  000a864a: blx 0x0006eb24
  000a864e: ldr.w r1,[r4,#0x88]
  000a8652: mov r5,r0
  000a8654: blx 0x0007201c
  000a8658: str r5,[r4,#0x0]
  000a865a: mov r0,r5
  000a865c: blx 0x00072028
  000a8660: ldr r5,[r4,#0x0]
  000a8662: cmp r0,#0x0
  000a8664: beq 0x000a865a
  000a8666: mov r0,r5
  000a8668: blx 0x00072034
  000a866c: cmp r0,#0x0
  000a866e: str r0,[r4,#0x60]
  000a8670: beq 0x000a8678
  000a8672: movs r1,#0x0
  000a8674: blx 0x00072040
  000a8678: ldr r0,[r4,#0x0]
  000a867a: blx 0x0007204c
  000a867e: ldr.w r0,[r4,#0x88]
  000a8682: cmp r0,#0x2
  000a8684: beq 0x000a870e
  000a8686: cmp r0,#0x17
  000a8688: beq.w 0x000a8870
  000a868c: cmp r0,#0x4
  000a868e: bne.w 0x000a898a
  000a8692: ldr r0,[0x000a8a48]
  000a8694: add.w r1,r4,#0x6c
  000a8698: add r0,pc
  000a869a: ldr r6,[r0,#0x0]
  000a869c: ldr r0,[r6,#0x0]
  000a869e: blx 0x00072058
  000a86a2: ldr r1,[r4,#0x6c]
  000a86a4: ldr r2,[0x000a8a40]
  000a86a6: ldr r3,[0x000a8a14]
  000a86a8: ldr r0,[r6,#0x0]
  000a86aa: ldr r5,[0x000a8a44]
  000a86ac: str r5,[sp,#0x0]
  000a86ae: blx 0x00072004
  000a86b2: ldr r0,[r4,#0x68]
  000a86b4: cbz r0,0x000a86c2
  000a86b6: blx 0x00072064
  000a86ba: blx 0x0006eb48
  000a86be: movs r0,#0x0
  000a86c0: str r0,[r4,#0x68]
  000a86c2: ldr r1,[r4,#0x6c]
  000a86c4: ldr r0,[r6,#0x0]
  000a86c6: blx 0x0006fd9c
  000a86ca: mov r0,r4
  000a86cc: blx 0x00072070
  000a86d0: ldr r0,[0x000a8a4c]
  000a86d2: add r0,pc
  000a86d4: ldr r5,[r0,#0x0]
  000a86d6: ldr r0,[r5,#0x0]
  000a86d8: blx 0x00071a10
  000a86dc: blx 0x00071950
  000a86e0: cmp r0,#0x3
  000a86e2: bne.w 0x000a892e
  000a86e6: movs r0,#0xc0
  000a86e8: blx 0x0006eb24
  000a86ec: mov r5,r0
  000a86ee: ldr r2,[r6,#0x0]
  000a86f0: movw r1,#0x36d6
  000a86f4: movs r3,#0x0
  000a86f6: blx 0x0007207c
  000a86fa: str r5,[r4,#0x2c]
  000a86fc: ldr r1,[r5,#0xc]
  000a86fe: ldr r0,[r6,#0x0]
  000a8700: blx 0x00072088
  000a8704: movs r1,#0x0
  000a8706: movs r2,#0x0
  000a8708: blx 0x0006fd18
  000a870c: b 0x000a898a
  000a870e: ldr r0,[0x000a8a50]
  000a8710: add.w r1,r4,#0x74
  000a8714: add r0,pc
  000a8716: ldr r6,[r0,#0x0]
  000a8718: ldr r0,[r6,#0x0]
  000a871a: blx 0x00072058
  000a871e: ldr r1,[r4,#0x74]
  000a8720: ldr r2,[0x000a8a2c]
  000a8722: ldr r3,[0x000a8a14]
  000a8724: ldr r0,[r6,#0x0]
  000a8726: ldr r5,[0x000a8a30]
  000a8728: str r5,[sp,#0x0]
  000a872a: blx 0x00072004
  000a872e: ldr r1,[r4,#0x74]
  000a8730: ldr r0,[r6,#0x0]
  000a8732: blx 0x0006fd9c
  000a8736: ldr r1,[r4,#0x74]
  000a8738: ldr r0,[r6,#0x0]
  000a873a: blx 0x0006ff1c
  000a873e: add.w r11,sp,#0x50
  000a8742: mov r1,r0
  000a8744: movs r2,#0x3c
  000a8746: mov r0,r11
  000a8748: blx 0x0006f1e4
  000a874c: ldr r0,[0x000a8a54]
  000a874e: movw r1,#0x4e20
  000a8752: movw r9,#0x4e20
  000a8756: add r0,pc
  000a8758: ldr r5,[r0,#0x0]
  000a875a: ldr r0,[r5,#0x0]
  000a875c: blx 0x00071848
  000a8760: mov r8,r0
  000a8762: ldr r0,[r5,#0x0]
  000a8764: movw r1,#0xea60
  000a8768: sub.w r5,r8,r9
  000a876c: blx 0x00071848
  000a8770: vmov s0,r5
  000a8774: mov r9,r0
  000a8776: movw r0,#0x9c40
  000a877a: mov r1,r11
  000a877c: vcvt.f32.s32 s0,s0
  000a8780: add r0,r9
  000a8782: movs r3,#0x0
  000a8784: movs r5,#0x0
  000a8786: vmov r2,s0
  000a878a: vmov s0,r0
  000a878e: add r0,sp,#0x14
  000a8790: vcvt.f32.s32 s0,s0
  000a8794: vstr.32 s0,[sp]
  000a8798: blx 0x0006f820
  000a879c: ldr r3,[0x000a8a34]
  000a879e: add r0,sp,#0x14
  000a87a0: mov r1,r11
  000a87a2: movs r2,#0x0
  000a87a4: str r3,[r4,#0x4]
  000a87a6: str r5,[sp,#0x0]
  000a87a8: blx 0x00072094
  000a87ac: ldr r1,[r4,#0x74]
  000a87ae: mov r2,r11
  000a87b0: ldr r0,[r6,#0x0]
  000a87b2: blx 0x000720a0
  000a87b6: ldr r0,[0x000a8a58]
  000a87b8: add r0,pc
  000a87ba: ldr r0,[r0,#0x0]
  000a87bc: ldr r0,[r0,#0x0]
  000a87be: blx 0x00071710
  000a87c2: subs r0,#0x1
  000a87c4: sbcs r0,r1,#0x0
  000a87c8: blt.w 0x000a898a
  000a87cc: ldr r0,[r4,#0x0]
  000a87ce: blx 0x00071eb4
  000a87d2: mov r5,r0
  000a87d4: cmp r0,#0x0
  000a87d6: beq.w 0x000a898a
  000a87da: ldr r0,[r5,#0x0]
  000a87dc: cmp r0,#0x2
  000a87de: bcc.w 0x000a898a
  000a87e2: ldr r1,[r5,#0x4]
  000a87e4: add.w r1,r1,r0, lsl #0x2
  000a87e8: ldr.w r0,[r1,#-0x8]
  000a87ec: cmp r0,#0x0
  000a87ee: beq.w 0x000a898a
  000a87f2: ldr.w r1,[r1,#-0x4]
  000a87f6: cmp r1,#0x0
  000a87f8: beq.w 0x000a898a
  000a87fc: movw r1,#0x5dc0
  000a8800: movw r2,#0x9a4c
  000a8804: sub.w r1,r8,r1
  000a8808: vmov s0,r1
  000a880c: add.w r1,r9,r2
  000a8810: vmov s2,r1
  000a8814: ldr r1,[0x000a8a38]
  000a8816: vcvt.f32.s32 s0,s0
  000a881a: vcvt.f32.s32 s2,s2
  000a881e: str r1,[sp,#0x18]
  000a8820: vstr.32 s0,[sp,#0x14]
  000a8824: vstr.32 s2,[sp,#0x1c]
  000a8828: ldr r1,[r0,#0x0]
  000a882a: ldr r2,[r1,#0x44]
  000a882c: add r1,sp,#0x14
  000a882e: blx r2
  000a8830: movw r0,#0x5b68
  000a8834: movw r1,#0x96c8
  000a8838: sub.w r0,r8,r0
  000a883c: add r1,r9
  000a883e: vmov s0,r0
  000a8842: ldrd r0,r2,[r5,#0x0]
  000a8846: vmov s2,r1
  000a884a: ldr r1,[0x000a8a3c]
  000a884c: vcvt.f32.s32 s0,s0
  000a8850: vcvt.f32.s32 s2,s2
  000a8854: add.w r0,r2,r0, lsl #0x2
  000a8858: ldr.w r0,[r0,#-0x4]
  000a885c: str r1,[sp,#0x18]
  000a885e: vstr.32 s0,[sp,#0x14]
  000a8862: vstr.32 s2,[sp,#0x1c]
  000a8866: ldr r1,[r0,#0x0]
  000a8868: ldr r2,[r1,#0x44]
  000a886a: add r1,sp,#0x14
  000a886c: blx r2
  000a886e: b 0x000a898a
  000a8870: ldr r0,[0x000a8a5c]
  000a8872: mov.w r8,#0x0
  000a8876: str.w r8,[r4,#0x64]
  000a887a: add.w r1,r4,#0x70
  000a887e: add r0,pc
  000a8880: ldr r6,[r0,#0x0]
  000a8882: ldr r0,[r6,#0x0]
  000a8884: blx 0x00072058
  000a8888: ldr r1,[r4,#0x70]
  000a888a: ldr r2,[0x000a8a10]
  000a888c: ldr r3,[0x000a8a14]
  000a888e: ldr r0,[r6,#0x0]
  000a8890: ldr r5,[0x000a8a18]
  000a8892: str r5,[sp,#0x0]
  000a8894: blx 0x00072004
  000a8898: ldr r1,[r4,#0x70]
  000a889a: ldr r0,[r6,#0x0]
  000a889c: blx 0x0006fd9c
  000a88a0: ldr r1,[r4,#0x70]
  000a88a2: ldr r0,[r6,#0x0]
  000a88a4: blx 0x0006ff1c
  000a88a8: add r5,sp,#0x50
  000a88aa: mov r1,r0
  000a88ac: movs r2,#0x3c
  000a88ae: mov r0,r5
  000a88b0: blx 0x0006f1e4
  000a88b4: ldr r3,[0x000a8a1c]
  000a88b6: add r0,sp,#0x14
  000a88b8: mov r1,r5
  000a88ba: movs r2,#0x0
  000a88bc: str.w r8,[sp,#0x0]
  000a88c0: blx 0x00072094
  000a88c4: ldr r0,[0x000a8a28]
  000a88c6: mov r1,r5
  000a88c8: ldr r2,[0x000a8a20]
  000a88ca: ldr r3,[0x000a8a24]
  000a88cc: str r0,[sp,#0x0]
  000a88ce: add r0,sp,#0x14
  000a88d0: blx 0x0006f820
  000a88d4: ldr r1,[r4,#0x70]
  000a88d6: mov r2,r5
  000a88d8: ldr r0,[r6,#0x0]
  000a88da: blx 0x000720a0
  000a88de: ldr r0,[r6,#0x0]
  000a88e0: add.w r1,r4,#0x78
  000a88e4: blx 0x000720ac
  000a88e8: ldr r0,[r4,#0x0]
  000a88ea: blx 0x00071eb4
  000a88ee: ldr r0,[r0,#0x4]
  000a88f0: ldr r0,[r0,#0x0]
  000a88f2: ldr r1,[r0,#0x8]
  000a88f4: add r0,sp,#0x8
  000a88f6: blx 0x000720b8
  000a88fa: ldr r1,[r4,#0x78]
  000a88fc: ldr r0,[r6,#0x0]
  000a88fe: blx 0x000720c4
  000a8902: ldrd r2,r3,[sp,#0x8]
  000a8906: mov r1,r0
  000a8908: vldr.32 s0,[sp,#0x10]
  000a890c: add r0,sp,#0x14
  000a890e: vstr.32 s0,[sp]
  000a8912: blx 0x0006f820
  000a8916: ldr r2,[r4,#0x70]
  000a8918: ldr r1,[r4,#0x78]
  000a891a: ldr r0,[r6,#0x0]
  000a891c: blx 0x000720d0
  000a8920: mov r0,r4
  000a8922: blx 0x00072070
  000a8926: mov r0,r4
  000a8928: blx 0x000720dc
  000a892c: b 0x000a898a
  000a892e: ldr r0,[r5,#0x0]
  000a8930: blx 0x00071a10
  000a8934: blx 0x00071950
  000a8938: cbz r0,0x000a8946
  000a893a: ldr r0,[r5,#0x0]
  000a893c: blx 0x00071a10
  000a8940: blx 0x00071950
  000a8944: b 0x000a898a
  000a8946: movs r0,#0xc0
  000a8948: blx 0x0006eb24
  000a894c: mov r5,r0
  000a894e: ldr r2,[r6,#0x0]
  000a8950: movw r1,#0x37c8
  000a8954: movs r3,#0x0
  000a8956: blx 0x0007207c
  000a895a: movs r0,#0xc0
  000a895c: str r5,[r4,#0x30]
  000a895e: blx 0x0006eb24
  000a8962: mov r5,r0
  000a8964: ldr r2,[r6,#0x0]
  000a8966: movw r1,#0x37c7
  000a896a: movs r3,#0x0
  000a896c: blx 0x0007207c
  000a8970: ldr r0,[r4,#0x30]
  000a8972: str r5,[r4,#0x34]
  000a8974: ldr r1,[r5,#0xc]
  000a8976: blx 0x000720e8
  000a897a: ldr r0,[r4,#0x34]
  000a897c: cbz r0,0x000a8986
  000a897e: blx 0x00071fc8
  000a8982: blx 0x0006eb48
  000a8986: movs r0,#0x0
  000a8988: str r0,[r4,#0x34]
  000a898a: movs r0,#0xc0
  000a898c: blx 0x0006eb24
  000a8990: mov r5,r0
  000a8992: ldr r0,[0x000a8a60]
  000a8994: add r0,pc
  000a8996: ldr r0,[r0,#0x0]
  000a8998: ldr r1,[r0,#0x0]
  000a899a: mov r0,r5
  000a899c: blx 0x000720f4
  000a89a0: mov r0,r5
  000a89a2: movs r1,#0x2
  000a89a4: str r5,[r4,#0x20]
  000a89a6: blx 0x00072100
  000a89aa: ldr r0,[0x000a8a64]
  000a89ac: movs r1,#0x0
  000a89ae: strd r1,r1,[r4,#0x50]
  000a89b2: add r0,pc
  000a89b4: strd r1,r1,[r4,#0x7c]
  000a89b8: str.w r1,[r4,#0x84]
  000a89bc: ldr r0,[r0,#0x0]
  000a89be: ldr r0,[r0,#0x0]
  000a89c0: blx 0x000713d4
  000a89c4: vmov.32 d16[0x0],r0
  000a89c8: movs r0,#0x1
  000a89ca: strb.w r0,[r4,#0x5c]
  000a89ce: add.w r0,r4,#0x40
  000a89d2: vmov.32 d16[0x1],r1
  000a89d6: vmov d17,d16
  000a89da: vst1.64 {d16,d17},[r0]
  000a89de: ldr r0,[sp,#0x90]
  000a89e0: ldr.w r1,[r10,#0x0]
  000a89e4: subs r0,r1,r0
  000a89e6: ittt eq
  000a89e8: add.eq sp,#0x94
  000a89ea: pop.eq.w {r8,r9,r10,r11}
  000a89ee: pop.eq {r4,r5,r6,r7,pc}
  000a89f0: blx 0x0006e824
```
