# StarMap::init
elf 0xc6bf0 body 2076
Sig: int __stdcall init(EVP_PKEY_CTX * ctx)

## decompile
```c

/* StarMap::init(bool, Mission*, bool, int) */

int StarMap::init(EVP_PKEY_CTX *ctx)

{
  EVP_PKEY_CTX EVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  EaseInOut *pEVar5;
  int *piVar6;
  int iVar7;
  Station *this;
  void *pvVar8;
  Array<Station*> *this_00;
  FileRead *this_01;
  TouchButton *this_02;
  int *piVar9;
  String *pSVar10;
  ChoiceWindow *this_03;
  SystemPathFinder *pSVar11;
  uint *puVar12;
  AEGeometry *pAVar13;
  int in_r1;
  int iVar14;
  Mission *in_r2;
  int in_r3;
  int *piVar15;
  int iVar16;
  code *pcVar17;
  uint uVar18;
  Array *pAVar19;
  uint uVar20;
  uint *puVar21;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float fVar22;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float extraout_s0_07;
  float extraout_s0_08;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s1_08;
  float extraout_s1_09;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar23;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  float extraout_s2_07;
  float extraout_s2_08;
  undefined4 in_stack_00000000;
  Matrix aMStack_e8 [44];
  undefined4 local_bc;
  Matrix aMStack_a8 [64];
  undefined4 local_68 [5];
  undefined4 local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined4 local_30;
  int local_28;
  
  piVar2 = *(int **)(DAT_000d6f20 + 0xd6c06);
  puVar21 = *(uint **)(DAT_000d6f24 + 0xd6c0a);
  local_28 = *piVar2;
  AbyssEngine::PaintCanvas::FogEnable(*puVar21,0,1);
  puVar3 = (undefined4 *)(DAT_000d6f28 + 0xd6c30);
  ctx[0x118] = SUB41(in_r3,0);
  ctx[0xa8] = SUB41(in_r1,0);
  *(undefined4 *)(ctx + 0x114) = in_stack_00000000;
  pcVar17 = (code *)*puVar3;
  (*pcVar17)(*puVar21,0x4a1,ctx + 0x124);
  (*pcVar17)(*puVar21,0x49c,ctx + 0x128);
  (*pcVar17)(*puVar21,0x49f,ctx + 300);
  (*pcVar17)(*puVar21,0x49e,ctx + 0x130);
  (*pcVar17)(*puVar21,0x452,ctx + 0x20);
  (*pcVar17)(*puVar21,0x4a2,ctx + 0x30);
  (*pcVar17)(*puVar21,0x453,ctx + 0x2c);
  (*pcVar17)(*puVar21,0x455,ctx + 0x28);
  (*pcVar17)(*puVar21,0x454,ctx + 0x24);
  (*pcVar17)(*puVar21,0x48c,ctx + 0x40);
  (*pcVar17)(*puVar21,0x48a,ctx + 0x44);
  (*pcVar17)(*puVar21,0x4fd,ctx + 0x48);
  (*pcVar17)(*puVar21,0x545,ctx + 0x134);
  uVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar21);
  *(undefined4 *)(ctx + 0x1a8) = uVar4;
  uVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar21);
  *(undefined4 *)(ctx + 0x1ac) = uVar4;
  *(undefined4 *)(ctx + 0x1d0) = 0;
  ctx[0x1d4] = (EVP_PKEY_CTX)0x0;
  *(undefined4 *)(ctx + 0x1bc) = 0;
  *(undefined4 *)(ctx + 0x1c0) = 0;
  *(undefined4 *)(ctx + 0x1c4) = 0xffffffff;
  *(undefined4 *)(ctx + 0x19c) = 0xffffffff;
  *(undefined4 *)(ctx + 0x1a0) = 0xffffffff;
  *(undefined4 *)(ctx + 0x1dc) = 0xffffffff;
  *(undefined4 *)(ctx + 0x1e0) = 0xffffffff;
  *(undefined4 *)(ctx + 0xe8) = 0;
  *(undefined4 *)(ctx + 0xec) = 0;
  *(undefined4 *)(ctx + 8) = 0;
  *(undefined4 *)(ctx + 0xc) = 0;
  ctx[1] = (EVP_PKEY_CTX)0x0;
  ctx[0x174] = (EVP_PKEY_CTX)0x0;
  *(undefined4 *)(ctx + 0x138) = 0;
  *(undefined4 *)(ctx + 0x13c) = 0;
  *(undefined4 *)(ctx + 0x140) = 0;
  pEVar5 = operator_new(0x10);
  AbyssEngine::EaseInOut::EaseInOut(pEVar5);
  *(EaseInOut **)(ctx + 0x17c) = pEVar5;
  pEVar5 = operator_new(0x10);
  AbyssEngine::EaseInOut::EaseInOut(pEVar5);
  *(EaseInOut **)(ctx + 0x180) = pEVar5;
  pEVar5 = operator_new(0x10);
  AbyssEngine::EaseInOut::EaseInOut(pEVar5);
  iVar7 = DAT_000d6f2c;
  *(EaseInOut **)(ctx + 0x184) = pEVar5;
  piVar6 = *(int **)(iVar7 + 0xd6d7a);
  *(undefined4 *)(ctx + 0x1cc) = *(undefined4 *)(*piVar6 + 0x90);
  uVar4 = AbyssEngine::PaintCanvas::CameraGetCurrent();
  *(undefined4 *)(ctx + 0x74) = uVar4;
  AbyssEngine::PaintCanvas::CameraCreate((PaintCanvas *)*puVar21,(uint *)(ctx + 0x70));
  AbyssEngine::PaintCanvas::CameraSetPerspective(*puVar21,extraout_s0,extraout_s1,extraout_s2);
  uStack_38 = DAT_000d7470;
  uStack_40 = DAT_000d7468;
  uStack_4c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_48 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar3 = (undefined4 *)((uint)local_68 | 4);
  *puVar3 = 0;
  puVar3[1] = uStack_4c;
  puVar3[2] = uStack_48;
  puVar3[3] = uStack_44;
  local_50 = 0;
  local_68[0] = 0x3f800000;
  local_54 = 0x3f800000;
  local_30 = 0x3f800000;
  AbyssEngine::AEMath::MatrixSetTranslation(aMStack_a8,extraout_s0_00,extraout_s1_00,extraout_s2_00)
  ;
  AbyssEngine::AEMath::MatrixSetRotation(aMStack_a8,extraout_s0_01,extraout_s1_01,extraout_s2_01);
  fVar22 = (float)VectorSignedToFloat((int)*(float *)(ctx + 8) * 0x14,(byte)(in_fpscr >> 0x16) & 3);
  fVar23 = (float)VectorSignedToFloat((int)*(float *)(ctx + 0xc) * 0x14,(byte)(in_fpscr >> 0x16) & 3
                                     );
  AbyssEngine::AEMath::MatrixSetTranslation(aMStack_a8,fVar22,extraout_s1_02,fVar23);
  AbyssEngine::PaintCanvas::CameraSetLocal(*puVar21,*(Matrix **)(ctx + 0x70));
  AbyssEngine::PaintCanvas::CameraSetCurrent((PaintCanvas *)*puVar21,*(uint *)(ctx + 0x70));
  piVar15 = *(int **)(DAT_000d6f44 + 0xd6e68);
  iVar7 = Status::getCurrentCampaignMission();
  uVar4 = 3;
  ctx[0xf4] = (EVP_PKEY_CTX)(0xf < iVar7);
  if (0xf < iVar7) {
    uVar4 = 0;
  }
  *(undefined4 *)(ctx + 4) = uVar4;
  Status::getSystem();
  uVar4 = SolarSystem::getIndex();
  *(undefined4 *)(ctx + 0x60) = uVar4;
  if (in_r3 == 0) {
    EVar1 = (EVP_PKEY_CTX)0x0;
    if (in_r1 != 0) {
      EVar1 = ctx[0xf4];
    }
    if (in_r1 != 0 && EVar1 != (EVP_PKEY_CTX)0x0) {
      iVar7 = **(int **)(DAT_000d6f48 + 0xd6ef0);
      Mission::getTargetStation();
      this = (Station *)Galaxy::getStation(iVar7);
      if ((this == (Station *)0x0) || (iVar7 = Station::getSystem(this), iVar7 < 0)) {
        if (-1 < *(int *)(*piVar15 + 0x7c)) goto LAB_000d6f60;
      }
      else {
        Station::getSystem(this);
LAB_000d6f60:
        AEGeometry::getPosition();
        AbyssEngine::AEMath::Vector::operator=((Vector *)(ctx + 0x78),(Vector *)aMStack_a8);
      }
      if (this != *(Station **)(*piVar15 + 0x78) && this != (Station *)0x0) {
        pvVar8 = (void *)Station::~Station(this);
        operator_delete(pvVar8);
      }
    }
    else {
      AEGeometry::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(ctx + 0x78),(Vector *)aMStack_a8);
    }
  }
  else {
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator=((Vector *)(ctx + 0x78),(Vector *)aMStack_a8);
  }
  uVar18 = *puVar21;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  uVar4 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar18);
  __aeabi_memcpy(aMStack_a8,uVar4,0x3c);
  AbyssEngine::AEMath::MatrixSetTranslation(aMStack_e8,extraout_s0_02,extraout_s1_03,extraout_s2_02)
  ;
  AbyssEngine::PaintCanvas::CameraSetLocal(*puVar21,*(Matrix **)(ctx + 0x70));
  *(float *)(ctx + 8) = *(float *)(ctx + 0x78) / 20.0;
  *(float *)(ctx + 0xc) = *(float *)(ctx + 0x7c) / 20.0;
  if (*(int *)(ctx + 4) == 3) {
    *(undefined4 *)(ctx + 100) = 0xffffffff;
    initStarSystem((StarMap *)ctx);
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator=((Vector *)(ctx + 0x78),(Vector *)aMStack_e8);
    *(float *)(ctx + 0x80) = *(float *)(ctx + 0x80) + DAT_000d6f4c;
    uVar18 = *puVar21;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar4 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar18);
    __aeabi_memcpy(aMStack_e8,uVar4,0x3c);
    local_bc = *(undefined4 *)(ctx + 0x80);
    AbyssEngine::PaintCanvas::CameraSetLocal(*puVar21,*(Matrix **)(ctx + 0x70));
  }
  else {
    *(undefined4 *)(ctx + 0x19c) = *(undefined4 *)(ctx + 0x60);
    if (*(Array **)(ctx + 0x58) != (Array *)0x0) {
      ArrayReleaseClasses<Station*>(*(Array **)(ctx + 0x58));
      if (*(Array<Station*> **)(ctx + 0x58) != (Array<Station*> *)0x0) {
        pvVar8 = (void *)Array<Station*>::~Array(*(Array<Station*> **)(ctx + 0x58));
        operator_delete(pvVar8);
      }
      *(undefined4 *)(ctx + 0x58) = 0;
    }
    this_00 = operator_new(0xc);
    Array<Station*>::Array(this_00);
    *(Array<Station*> **)(ctx + 0x58) = this_00;
    this_01 = operator_new(1);
    FileRead::FileRead(this_01);
    uVar4 = FileRead::loadStationsBinary
                      (this_01,*(SolarSystem **)
                                (*(int *)(*(int *)(ctx + 0x54) + 4) + *(int *)(ctx + 0x60) * 4));
    *(undefined4 *)(ctx + 0x58) = uVar4;
    pvVar8 = (void *)FileRead::~FileRead(this_01);
    operator_delete(pvVar8);
  }
  if (in_r3 != 0) {
    *(undefined4 *)(ctx + 0x19c) = 0xffffffff;
    *(undefined4 *)(ctx + 0x60) = 0xffffffff;
  }
  ctx[0x108] = (EVP_PKEY_CTX)0x0;
  *(undefined2 *)(ctx + 0xaa) = 0;
  *ctx = (EVP_PKEY_CTX)0x0;
  this_02 = operator_new(200);
  piVar9 = *(int **)(DAT_000d7488 + 0xd70f6);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton
            (this_02,pSVar10,0,**(int **)(DAT_000d748c + 0xd710c) - *(int *)(*piVar6 + 0x2c),
             **(int **)(DAT_000d7490 + 0xd7110) - *(int *)(*piVar6 + 0x2c),'\"');
  *(TouchButton **)(ctx + 0x4c) = this_02;
  this_03 = operator_new(0x5c);
  ChoiceWindow::ChoiceWindow(this_03);
  *(undefined4 *)(ctx + 0xa0) = 0;
  *(ChoiceWindow **)(ctx + 0x5c) = this_03;
  pSVar11 = operator_new(1);
  SystemPathFinder::SystemPathFinder(pSVar11);
  *(SystemPathFinder **)(ctx + 0x50) = pSVar11;
  if ((in_r1 == 0) || (*(int *)(ctx + 4) != 0)) goto LAB_000d72f0;
  *(undefined4 *)(ctx + 0x104) = 0xffffffff;
  *(undefined4 *)(ctx + 0x60) = 0xffffffff;
  iVar7 = Mission::isEmpty();
  if ((iVar7 == 0) &&
     ((iVar7 = Mission::isVisible(in_r2), iVar7 != 0 || (iVar7 = Mission::getType(), iVar7 == 0xe)))
     ) {
    if (*(int *)(ctx + 4) != 3) {
      Status::getSystem();
      iVar7 = SolarSystem::getRoutes();
      if (iVar7 == 0) goto LAB_000d725c;
    }
    for (uVar18 = 0; uVar18 < **(uint **)(ctx + 0x54); uVar18 = uVar18 + 1) {
      iVar7 = SolarSystem::getStations(*(SolarSystem **)((*(uint **)(ctx + 0x54))[1] + uVar18 * 4));
      if (iVar7 != 0) {
        for (uVar20 = 0;
            puVar12 = (uint *)SolarSystem::getStations
                                        (*(SolarSystem **)
                                          (*(int *)(*(int *)(ctx + 0x54) + 4) + uVar18 * 4)),
            uVar20 < *puVar12; uVar20 = uVar20 + 1) {
          iVar7 = Mission::getTargetStation();
          iVar16 = SolarSystem::getStations
                             (*(SolarSystem **)(*(int *)(*(int *)(ctx + 0x54) + 4) + uVar18 * 4));
          if (iVar7 == *(int *)(*(int *)(iVar16 + 4) + uVar20 * 4)) {
            *(uint *)(ctx + 0x104) = uVar18;
            if (*(int *)(ctx + 4) == 3) {
              *(uint *)(ctx + 0x1a0) = uVar20;
              *(uint *)(ctx + 100) = uVar20;
              ctx[0x13b] = (EVP_PKEY_CTX)0x1;
            }
            break;
          }
        }
      }
    }
    iVar7 = *(int *)(ctx + 0x104);
    if (iVar7 == -1) {
      iVar7 = Mission::getTargetStation();
      if ((iVar7 != -1) || (*(int *)(*piVar15 + 0x7c) < 0)) {
        iVar7 = *(int *)(ctx + 0x104);
        goto LAB_000d7234;
      }
      *(int *)(ctx + 0x104) = *(int *)(*piVar15 + 0x7c);
    }
    else {
LAB_000d7234:
      if (iVar7 < 0) goto LAB_000d72dc;
    }
    pSVar11 = *(SystemPathFinder **)(ctx + 0x50);
    pAVar19 = *(Array **)(ctx + 0x54);
    Status::getSystem();
    iVar7 = SolarSystem::getIndex();
    uVar4 = SystemPathFinder::getSystemPath(pSVar11,pAVar19,iVar7,*(int *)(ctx + 0x104));
    *(undefined4 *)(ctx + 0xa0) = uVar4;
  }
  else {
LAB_000d725c:
    iVar7 = Mission::isEmpty();
    if ((iVar7 == 0) &&
       ((iVar7 = Mission::isVisible(in_r2), iVar7 != 0 || (iVar7 = Mission::getType(), iVar7 == 0xe)
        ))) {
      Status::getSystem();
      iVar7 = SolarSystem::getRoutes();
      if (iVar7 == 0) {
        for (uVar18 = 0; uVar18 < **(uint **)(ctx + 0x54); uVar18 = uVar18 + 1) {
          iVar7 = SolarSystem::getStations
                            (*(SolarSystem **)((*(uint **)(ctx + 0x54))[1] + uVar18 * 4));
          if (iVar7 != 0) {
            uVar20 = 0;
            do {
              puVar12 = (uint *)SolarSystem::getStations
                                          (*(SolarSystem **)
                                            (*(int *)(*(int *)(ctx + 0x54) + 4) + uVar18 * 4));
              if (*puVar12 <= uVar20) goto LAB_000d72d0;
              iVar16 = Mission::getTargetStation();
              iVar14 = SolarSystem::getStations
                                 (*(SolarSystem **)(*(int *)(*(int *)(ctx + 0x54) + 4) + uVar18 * 4)
                                 );
              iVar7 = uVar20 * 4;
              uVar20 = uVar20 + 1;
            } while (iVar16 != *(int *)(*(int *)(iVar14 + 4) + iVar7));
            *(uint *)(ctx + 0x104) = uVar18;
          }
LAB_000d72d0:
        }
      }
    }
  }
LAB_000d72dc:
  uVar4 = DAT_000d7480;
  ctx[0x13a] = (EVP_PKEY_CTX)0x1;
  *(undefined4 *)(ctx + 0x168) = uVar4;
  *(undefined4 *)(ctx + 0x60) = *(undefined4 *)(ctx + 0x104);
LAB_000d72f0:
  *(undefined4 *)(ctx + 0x10c) = 0;
  iVar16 = 0;
  puVar3 = *(undefined4 **)(DAT_000d7494 + 0xd7302);
  iVar7 = 0;
  while (iVar16 != 6) {
    pSVar10 = (String *)*puVar3;
    uVar18 = *puVar21;
    GameText::getText(*piVar9);
    iVar14 = AbyssEngine::PaintCanvas::GetTextWidth(uVar18,pSVar10);
    iVar16 = iVar16 + 1;
    iVar7 = *(int *)(ctx + 0x10c);
    if (*(int *)(ctx + 0x10c) < iVar14) {
      *(int *)(ctx + 0x10c) = iVar14;
      iVar7 = iVar14;
    }
  }
  uVar4 = 0;
  iVar16 = *piVar6;
  *(int *)(ctx + 0x10c) = iVar7 + *(int *)(iVar16 + 0x8c);
  iVar14 = *(int *)(iVar16 + 4);
  iVar7 = *(int *)(iVar16 + 0x2c);
  *(undefined4 *)(ctx + 0x11c) = 0;
  *(int *)(ctx + 0x110) = iVar14 * 5 + iVar7 * 2;
  iVar7 = Status::getShip();
  iVar7 = Ship::getCargo(iVar7);
  if (iVar7 != 0) {
    uVar4 = Item::getAmount();
  }
  *(undefined4 *)(ctx + 0x1d8) = uVar4;
  pAVar13 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar13,0x41d2,(PaintCanvas *)*puVar21,false);
  *(AEGeometry **)(ctx + 0x1b0) = pAVar13;
  AEGeometry::setRotation(pAVar13,extraout_s0_03,extraout_s1_04,extraout_s2_03);
  AEGeometry::setPosition(extraout_s0_04,extraout_s1_05,extraout_s2_04);
  pAVar13 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar13,0x41d3,(PaintCanvas *)*puVar21,false);
  *(AEGeometry **)(ctx + 0x1b4) = pAVar13;
  AEGeometry::setRotation(pAVar13,extraout_s0_05,extraout_s1_06,extraout_s2_05);
  AEGeometry::setPosition(extraout_s0_06,extraout_s1_07,extraout_s2_06);
  pAVar13 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar13,0x41d4,(PaintCanvas *)*puVar21,false);
  *(AEGeometry **)(ctx + 0x1b8) = pAVar13;
  AEGeometry::setRotation(pAVar13,extraout_s0_07,extraout_s1_08,extraout_s2_07);
  AEGeometry::setPosition(extraout_s0_08,extraout_s1_09,extraout_s2_08);
  if (*piVar2 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}

```

## target disasm
```
  000d6bf0: push {r4,r5,r6,r7,lr}
  000d6bf2: add r7,sp,#0xc
  000d6bf4: push {r8,r9,r10,r11}
  000d6bf8: sub sp,#0xe4
  000d6bfa: mov r11,r0
  000d6bfc: ldr r0,[0x000d6f20]
  000d6bfe: mov r4,r1
  000d6c00: ldr r1,[0x000d6f24]
  000d6c02: add r0,pc
  000d6c04: mov r8,r2
  000d6c06: add r1,pc
  000d6c08: movs r2,#0x1
  000d6c0a: ldr r0,[r0,#0x0]
  000d6c0c: mov r9,r3
  000d6c0e: ldr.w r10,[r1,#0x0]
  000d6c12: movs r6,#0x0
  000d6c14: str r0,[sp,#0xc]
  000d6c16: ldr r1,[r0,#0x0]
  000d6c18: ldr.w r0,[r10,#0x0]
  000d6c1c: str r1,[sp,#0xe0]
  000d6c1e: movs r1,#0x0
  000d6c20: blx 0x00071fe0
  000d6c24: ldr r0,[0x000d6f28]
  000d6c26: add.w r2,r11,#0x124
  000d6c2a: ldr r1,[r7,#0x8]
  000d6c2c: add r0,pc
  000d6c2e: strb.w r9,[r11,#0x118]
  000d6c32: strb.w r4,[r11,#0xa8]
  000d6c36: str.w r1,[r11,#0x114]
  000d6c3a: movw r1,#0x4a1
  000d6c3e: ldr r5,[r0,#0x0]
  000d6c40: ldr.w r0,[r10,#0x0]
  000d6c44: str r4,[sp,#0x14]
  000d6c46: blx r5
  000d6c48: ldr.w r0,[r10,#0x0]
  000d6c4c: add.w r2,r11,#0x128
  000d6c50: movw r1,#0x49c
  000d6c54: blx r5
  000d6c56: ldr.w r0,[r10,#0x0]
  000d6c5a: add.w r2,r11,#0x12c
  000d6c5e: movw r1,#0x49f
  000d6c62: blx r5
  000d6c64: ldr.w r0,[r10,#0x0]
  000d6c68: add.w r2,r11,#0x130
  000d6c6c: movw r1,#0x49e
  000d6c70: blx r5
  000d6c72: ldr.w r0,[r10,#0x0]
  000d6c76: add.w r2,r11,#0x20
  000d6c7a: movw r1,#0x452
  000d6c7e: blx r5
  000d6c80: ldr.w r0,[r10,#0x0]
  000d6c84: add.w r2,r11,#0x30
  000d6c88: movw r1,#0x4a2
  000d6c8c: blx r5
  000d6c8e: ldr.w r0,[r10,#0x0]
  000d6c92: add.w r2,r11,#0x2c
  000d6c96: movw r1,#0x453
  000d6c9a: blx r5
  000d6c9c: ldr.w r0,[r10,#0x0]
  000d6ca0: add.w r2,r11,#0x28
  000d6ca4: movw r1,#0x455
  000d6ca8: blx r5
  000d6caa: ldr.w r0,[r10,#0x0]
  000d6cae: add.w r2,r11,#0x24
  000d6cb2: movw r1,#0x454
  000d6cb6: blx r5
  000d6cb8: ldr.w r0,[r10,#0x0]
  000d6cbc: add.w r2,r11,#0x40
  000d6cc0: movw r1,#0x48c
  000d6cc4: blx r5
  000d6cc6: ldr.w r0,[r10,#0x0]
  000d6cca: add.w r2,r11,#0x44
  000d6cce: movw r1,#0x48a
  000d6cd2: blx r5
  000d6cd4: ldr.w r0,[r10,#0x0]
  000d6cd8: add.w r2,r11,#0x48
  000d6cdc: movw r1,#0x4fd
  000d6ce0: blx r5
  000d6ce2: ldr.w r0,[r10,#0x0]
  000d6ce6: add.w r2,r11,#0x134
  000d6cea: movw r1,#0x545
  000d6cee: blx r5
  000d6cf0: ldr.w r1,[r11,#0x40]
  000d6cf4: ldr.w r0,[r10,#0x0]
  000d6cf8: blx 0x00072d84
  000d6cfc: ldr.w r1,[r11,#0x28]
  000d6d00: str.w r0,[r11,#0x1a8]
  000d6d04: ldr.w r0,[r10,#0x0]
  000d6d08: blx 0x00072d84
  000d6d0c: str.w r0,[r11,#0x1ac]
  000d6d10: mov.w r0,#0xffffffff
  000d6d14: str.w r6,[r11,#0x1d0]
  000d6d18: strb.w r6,[r11,#0x1d4]
  000d6d1c: strd r6,r6,[r11,#0x1bc]
  000d6d20: str.w r0,[r11,#0x1c4]
  000d6d24: strd r0,r0,[r11,#0x19c]
  000d6d28: strd r0,r0,[r11,#0x1dc]
  000d6d2c: movs r0,#0x10
  000d6d2e: strd r6,r6,[r11,#0xe8]
  000d6d32: strd r6,r6,[r11,#0x8]
  000d6d36: strb.w r6,[r11,#0x1]
  000d6d3a: strb.w r6,[r11,#0x174]
  000d6d3e: strd r6,r6,[r11,#0x138]
  000d6d42: str.w r6,[r11,#0x140]
  000d6d46: blx 0x0006eb24
  000d6d4a: mov r6,r0
  000d6d4c: blx 0x00074560
  000d6d50: movs r0,#0x10
  000d6d52: str.w r6,[r11,#0x17c]
  000d6d56: blx 0x0006eb24
  000d6d5a: mov r6,r0
  000d6d5c: blx 0x00074560
  000d6d60: movs r0,#0x10
  000d6d62: str.w r6,[r11,#0x180]
  000d6d66: blx 0x0006eb24
  000d6d6a: mov r6,r0
  000d6d6c: blx 0x00074560
  000d6d70: ldr r0,[0x000d6f2c]
  000d6d72: str.w r6,[r11,#0x184]
  000d6d76: add r0,pc
  000d6d78: ldr r0,[r0,#0x0]
  000d6d7a: str r0,[sp,#0x18]
  000d6d7c: ldr r0,[r0,#0x0]
  000d6d7e: ldr.w r0,[r0,#0x90]
  000d6d82: str.w r0,[r11,#0x1cc]
  000d6d86: ldr.w r0,[r10,#0x0]
  000d6d8a: blx 0x000717f4
  000d6d8e: str.w r0,[r11,#0x74]
  000d6d92: add.w r1,r11,#0x70
  000d6d96: ldr.w r0,[r10,#0x0]
  000d6d9a: blx 0x00072058
  000d6d9e: ldr.w r1,[r11,#0x70]
  000d6da2: ldr r2,[0x000d6f30]
  000d6da4: ldr r3,[0x000d6f34]
  000d6da6: ldr.w r0,[r10,#0x0]
  000d6daa: ldr r4,[0x000d6f38]
  000d6dac: str r4,[sp,#0x0]
  000d6dae: blx 0x00072004
  000d6db2: adr.w r1,0xd7468
  000d6db6: vmov.i32 q8,#0x0
  000d6dba: add r6,sp,#0xa0
  000d6dbc: vld1.64 {d18,d19},[r1]
  000d6dc0: orr r1,r6,#0x4
  000d6dc4: movs r2,#0x0
  000d6dc6: ldr r0,[0x000d6f3c]
  000d6dc8: movs r3,#0x0
  000d6dca: vst1.32 {d16,d17},[r1]
  000d6dce: add.w r1,r6,#0x18
  000d6dd2: movs r5,#0x0
  000d6dd4: vst1.64 {d16,d17},[r1]
  000d6dd8: add.w r1,r6,#0x28
  000d6ddc: vst1.64 {d18,d19},[r1]
  000d6de0: mov.w r1,#0x3f800000
  000d6de4: str r0,[sp,#0x0]
  000d6de6: add r0,sp,#0x60
  000d6de8: str r1,[sp,#0xa0]
  000d6dea: str r1,[sp,#0xb4]
  000d6dec: str r1,[sp,#0xd8]
  000d6dee: mov r1,r6
  000d6df0: blx 0x0006f820
  000d6df4: ldr r3,[0x000d6f40]
  000d6df6: add r0,sp,#0x60
  000d6df8: mov r1,r6
  000d6dfa: movs r2,#0x0
  000d6dfc: str r5,[sp,#0x0]
  000d6dfe: blx 0x00072094
  000d6e02: vldr.32 s0,[r11,#0x8]
  000d6e06: vldr.32 s2,[r11,#0xc]
  000d6e0a: vcvt.s32.f32 s2,s2
  000d6e0e: vcvt.s32.f32 s0,s0
  000d6e12: vmov r0,s2
  000d6e16: vmov r1,s0
  000d6e1a: add.w r0,r0,r0, lsl #0x2
  000d6e1e: add.w r1,r1,r1, lsl #0x2
  000d6e22: lsls r0,r0,#0x2
  000d6e24: lsls r1,r1,#0x2
  000d6e26: vmov s2,r0
  000d6e2a: vmov s0,r1
  000d6e2e: add r0,sp,#0x60
  000d6e30: mov r1,r6
  000d6e32: vcvt.f32.s32 s0,s0
  000d6e36: vcvt.f32.s32 s2,s2
  000d6e3a: str r5,[sp,#0x0]
  000d6e3c: vmov r2,s0
  000d6e40: vmov r3,s2
  000d6e44: blx 0x0006f820
  000d6e48: ldr.w r1,[r11,#0x70]
  000d6e4c: mov r2,r6
  000d6e4e: ldr.w r0,[r10,#0x0]
  000d6e52: blx 0x000720a0
  000d6e56: ldr.w r1,[r11,#0x70]
  000d6e5a: ldr.w r0,[r10,#0x0]
  000d6e5e: blx 0x0006fd9c
  000d6e62: ldr r0,[0x000d6f44]
  000d6e64: add r0,pc
  000d6e66: ldr r4,[r0,#0x0]
  000d6e68: ldr r0,[r4,#0x0]
  000d6e6a: blx 0x00071770
  000d6e6e: cmp r0,#0xf
  000d6e70: it gt
  000d6e72: mov.gt r5,#0x1
  000d6e74: mov.w r0,#0x3
  000d6e78: strb.w r5,[r11,#0xf4]
  000d6e7c: it gt
  000d6e7e: mov.gt r0,#0x0
  000d6e80: str r4,[sp,#0x10]
  000d6e82: str.w r0,[r11,#0x4]
  000d6e86: ldr r0,[r4,#0x0]
  000d6e88: blx 0x00071a10
  000d6e8c: blx 0x00071a7c
  000d6e90: cmp.w r9,#0x0
  000d6e94: str.w r0,[r11,#0x60]
  000d6e98: beq 0x000d6ebc
  000d6e9a: ldr.w r0,[r11,#0x68]
  000d6e9e: add r6,sp,#0x60
  000d6ea0: ldr r1,[r7,#0x8]
  000d6ea2: ldr r0,[r0,#0x4]
  000d6ea4: ldr.w r1,[r0,r1,lsl #0x2]
  000d6ea8: mov r0,r6
  000d6eaa: blx 0x000720b8
  000d6eae: add.w r0,r11,#0x78
  000d6eb2: mov r1,r6
  000d6eb4: blx 0x0006eb3c
  000d6eb8: mov r4,r8
  000d6eba: b 0x000d6f8e
  000d6ebc: ldr r1,[sp,#0x14]
  000d6ebe: mov r4,r8
  000d6ec0: cmp r1,#0x0
  000d6ec2: itt ne
  000d6ec4: ldrb.ne.w r1,[r11,#0xf4]
  000d6ec8: cmp.ne r1,#0x0
  000d6eca: bne 0x000d6eea
  000d6ecc: ldr.w r1,[r11,#0x68]
  000d6ed0: add r6,sp,#0x60
  000d6ed2: ldr r1,[r1,#0x4]
  000d6ed4: ldr.w r1,[r1,r0,lsl #0x2]
  000d6ed8: mov r0,r6
  000d6eda: blx 0x000720b8
  000d6ede: add.w r0,r11,#0x78
  000d6ee2: mov r1,r6
  000d6ee4: blx 0x0006eb3c
  000d6ee8: b 0x000d6f8e
  000d6eea: ldr r0,[0x000d6f48]
  000d6eec: add r0,pc
  000d6eee: ldr r0,[r0,#0x0]
  000d6ef0: ldr r6,[r0,#0x0]
  000d6ef2: mov r0,r4
  000d6ef4: blx 0x00073738
  000d6ef8: mov r1,r0
  000d6efa: mov r0,r6
  000d6efc: blx 0x00071c44
  000d6f00: mov r6,r0
  000d6f02: cbz r0,0x000d6f50
  000d6f04: mov r0,r6
  000d6f06: blx 0x0007189c
  000d6f0a: cmp r0,#0x0
  000d6f0c: blt 0x000d6f50
  000d6f0e: mov r0,r6
  000d6f10: ldr.w r4,[r11,#0x68]
  000d6f14: blx 0x0007189c
  000d6f18: ldr r1,[r4,#0x4]
  000d6f1a: mov r4,r8
  000d6f1c: b 0x000d6f60
  000d6f50: ldr r0,[sp,#0x10]
  000d6f52: ldr r0,[r0,#0x0]
  000d6f54: ldr r0,[r0,#0x7c]
  000d6f56: cmp r0,#0x0
  000d6f58: blt 0x000d6f76
  000d6f5a: ldr.w r1,[r11,#0x68]
  000d6f5e: ldr r1,[r1,#0x4]
  000d6f60: ldr.w r1,[r1,r0,lsl #0x2]
  000d6f64: add r5,sp,#0x60
  000d6f66: mov r0,r5
  000d6f68: blx 0x000720b8
  000d6f6c: add.w r0,r11,#0x78
  000d6f70: mov r1,r5
  000d6f72: blx 0x0006eb3c
  000d6f76: ldr r0,[sp,#0x10]
  000d6f78: ldr r0,[r0,#0x0]
  000d6f7a: ldr r0,[r0,#0x78]
  000d6f7c: cmp r6,r0
  000d6f7e: it ne
  000d6f80: cmp.ne r6,#0x0
  000d6f82: beq 0x000d6f8e
  000d6f84: mov r0,r6
  000d6f86: blx 0x0007360c
  000d6f8a: blx 0x0006eb48
  000d6f8e: ldr.w r6,[r10,#0x0]
  000d6f92: mov r0,r6
  000d6f94: blx 0x000717f4
  000d6f98: mov r1,r0
  000d6f9a: mov r0,r6
  000d6f9c: blx 0x0006ff1c
  000d6fa0: add r5,sp,#0x60
  000d6fa2: mov r1,r0
  000d6fa4: movs r2,#0x3c
  000d6fa6: mov r0,r5
  000d6fa8: blx 0x0006f1e4
  000d6fac: mov r6,r11
  000d6fae: movs r0,#0x0
  000d6fb0: ldr.w r2,[r6,#0x78]!
  000d6fb4: mov r1,r5
  000d6fb6: ldr r3,[r6,#0x4]
  000d6fb8: str r0,[sp,#0x0]
  000d6fba: add r0,sp,#0x20
  000d6fbc: blx 0x0006f820
  000d6fc0: ldr.w r1,[r6,#-0x8]
  000d6fc4: mov r2,r5
  000d6fc6: ldr.w r0,[r10,#0x0]
  000d6fca: blx 0x000720a0
  000d6fce: vmov.f32 s0,0x41a00000
  000d6fd2: vldr.32 s2,[r6]
  000d6fd6: vldr.32 s4,[r6,#0x4]
  000d6fda: ldr.w r0,[r6,#-0x74]
  000d6fde: cmp r0,#0x3
  000d6fe0: vdiv.f32 s2,s2,s0
  000d6fe4: vdiv.f32 s0,s4,s0
  000d6fe8: vstr.32 s2,[r6,#-0x70]
  000d6fec: vstr.32 s0,[r6,#-0x6c]
  000d6ff0: bne 0x000d7062
  000d6ff2: mov.w r0,#0xffffffff
  000d6ff6: str.w r0,[r11,#0x64]
  000d6ffa: mov r0,r11
  000d6ffc: blx 0x0007456c
  000d7000: ldr.w r1,[r11,#0x68]
  000d7004: add r5,sp,#0x20
  000d7006: ldr.w r0,[r11,#0x60]
  000d700a: ldr r1,[r1,#0x4]
  000d700c: ldr.w r1,[r1,r0,lsl #0x2]
  000d7010: mov r0,r5
  000d7012: blx 0x000720b8
  000d7016: mov r0,r6
  000d7018: mov r1,r5
  000d701a: blx 0x0006eb3c
  000d701e: vldr.32 s0,[pc,#-0xd4]
  000d7022: vldr.32 s2,[r11,#0x80]
  000d7026: vadd.f32 s0,s2,s0
  000d702a: vstr.32 s0,[r11,#0x80]
  000d702e: ldr.w r5,[r10,#0x0]
  000d7032: mov r0,r5
  000d7034: blx 0x000717f4
  000d7038: mov r1,r0
  000d703a: mov r0,r5
  000d703c: blx 0x0006ff1c
  000d7040: add r5,sp,#0x20
  000d7042: mov r1,r0
  000d7044: movs r2,#0x3c
  000d7046: mov r0,r5
  000d7048: blx 0x0006f1e4
  000d704c: ldr.w r1,[r11,#0x80]
  000d7050: mov r2,r5
  000d7052: str r1,[sp,#0x4c]
  000d7054: ldr.w r0,[r10,#0x0]
  000d7058: ldr.w r1,[r11,#0x70]
  000d705c: blx 0x000720a0
  000d7060: b 0x000d70c8
  000d7062: ldr.w r0,[r11,#0x58]
  000d7066: ldr.w r1,[r11,#0x60]
  000d706a: cmp r0,#0x0
  000d706c: str.w r1,[r11,#0x19c]
  000d7070: beq 0x000d708a
  000d7072: blx 0x000735dc
  000d7076: ldr.w r0,[r11,#0x58]
  000d707a: cbz r0,0x000d7084
  000d707c: blx 0x000735e8
  000d7080: blx 0x0006eb48
  000d7084: movs r0,#0x0
  000d7086: str.w r0,[r11,#0x58]
  000d708a: movs r0,#0xc
  000d708c: blx 0x0006eb24
  000d7090: mov r6,r0
  000d7092: blx 0x00073594
  000d7096: movs r0,#0x1
  000d7098: str.w r6,[r11,#0x58]
  000d709c: blx 0x0006eb24
  000d70a0: mov r6,r0
  000d70a2: blx 0x00072124
  000d70a6: ldr.w r0,[r11,#0x54]
  000d70aa: ldr.w r1,[r11,#0x60]
  000d70ae: ldr r0,[r0,#0x4]
  000d70b0: ldr.w r1,[r0,r1,lsl #0x2]
  000d70b4: mov r0,r6
  000d70b6: blx 0x00073690
  000d70ba: str.w r0,[r11,#0x58]
  000d70be: mov r0,r6
  000d70c0: blx 0x0007213c
  000d70c4: blx 0x0006eb48
  000d70c8: cmp.w r9,#0x0
  000d70cc: ittt ne
  000d70ce: mov.ne.w r0,#0xffffffff
  000d70d2: str.w.ne r0,[r11,#0x19c]
  000d70d6: str.w.ne r0,[r11,#0x60]
  000d70da: movs r0,#0x0
  000d70dc: strb.w r0,[r11,#0x108]
  000d70e0: strh.w r0,[r11,#0xaa]
  000d70e4: strb.w r0,[r11,#0x0]
  000d70e8: movs r0,#0xc8
  000d70ea: blx 0x0006eb24
  000d70ee: mov r6,r0
  000d70f0: ldr r0,[0x000d7488]
  000d70f2: add r0,pc
  000d70f4: ldr r0,[r0,#0x0]
  000d70f6: str r0,[sp,#0x1c]
  000d70f8: ldr r0,[r0,#0x0]
  000d70fa: mov.w r1,#0x190
  000d70fe: blx 0x00072f70
  000d7102: mov r1,r0
  000d7104: ldr r0,[0x000d748c]
  000d7106: ldr r2,[0x000d7490]
  000d7108: add r0,pc
  000d710a: ldr r3,[sp,#0x18]
  000d710c: add r2,pc
  000d710e: ldr r0,[r0,#0x0]
  000d7110: ldr r2,[r2,#0x0]
  000d7112: ldr r3,[r3,#0x0]
  000d7114: ldr r0,[r0,#0x0]
  000d7116: ldr r2,[r2,#0x0]
  000d7118: ldr r3,[r3,#0x2c]
  000d711a: subs r2,r2,r3
  000d711c: movs r5,#0x22
  000d711e: subs r3,r0,r3
  000d7120: strd r2,r5,[sp,#0x0]
  000d7124: mov r0,r6
  000d7126: movs r2,#0x0
  000d7128: blx 0x00074578
  000d712c: movs r0,#0x5c
  000d712e: str.w r6,[r11,#0x4c]
  000d7132: blx 0x0006eb24
  000d7136: mov r6,r0
  000d7138: blx 0x00074584
  000d713c: movs r0,#0x0
  000d713e: str.w r0,[r11,#0xa0]
  000d7142: movs r0,#0x1
  000d7144: str.w r6,[r11,#0x5c]
  000d7148: blx 0x0006eb24
  000d714c: mov r6,r0
  000d714e: blx 0x00073798
  000d7152: ldr r0,[sp,#0x14]
  000d7154: str.w r6,[r11,#0x50]
  000d7158: cmp r0,#0x0
  000d715a: beq.w 0x000d72f0
  000d715e: ldr.w r0,[r11,#0x4]
  000d7162: cmp r0,#0x0
  000d7164: bne.w 0x000d72f0
  000d7168: mov.w r0,#0xffffffff
  000d716c: str.w r0,[r11,#0x104]
  000d7170: str.w r0,[r11,#0x60]
  000d7174: mov r0,r4
  000d7176: blx 0x00072868
  000d717a: cmp r0,#0x0
  000d717c: bne 0x000d725c
  000d717e: mov r0,r4
  000d7180: blx 0x00074590
  000d7184: cbnz r0,0x000d7190
  000d7186: mov r0,r4
  000d7188: blx 0x00072874
  000d718c: cmp r0,#0xe
  000d718e: bne 0x000d725c
  000d7190: ldr.w r0,[r11,#0x4]
  000d7194: cmp r0,#0x3
  000d7196: beq 0x000d71a8
  000d7198: ldr r0,[sp,#0x10]
  000d719a: ldr r0,[r0,#0x0]
  000d719c: blx 0x00071a10
  000d71a0: blx 0x00071aac
  000d71a4: cmp r0,#0x0
  000d71a6: beq 0x000d725c
  000d71a8: movs r6,#0x0
  000d71aa: b 0x000d7212
  000d71ac: ldr r0,[r0,#0x4]
  000d71ae: ldr.w r0,[r0,r6,lsl #0x2]
  000d71b2: blx 0x00071a88
  000d71b6: cbz r0,0x000d7210
  000d71b8: mov.w r8,#0x0
  000d71bc: b 0x000d71c2
  000d71be: add.w r8,r8,#0x1
  000d71c2: ldr.w r0,[r11,#0x54]
  000d71c6: ldr r0,[r0,#0x4]
  000d71c8: ldr.w r0,[r0,r6,lsl #0x2]
  000d71cc: blx 0x00071a88
  000d71d0: ldr r0,[r0,#0x0]
  000d71d2: cmp r8,r0
  000d71d4: bcs 0x000d7210
  000d71d6: mov r0,r4
  000d71d8: blx 0x00073738
  000d71dc: mov r5,r0
  000d71de: ldr.w r0,[r11,#0x54]
  000d71e2: ldr r0,[r0,#0x4]
  000d71e4: ldr.w r0,[r0,r6,lsl #0x2]
  000d71e8: blx 0x00071a88
  000d71ec: ldr r0,[r0,#0x4]
  000d71ee: ldr.w r0,[r0,r8,lsl #0x2]
  000d71f2: cmp r5,r0
  000d71f4: bne 0x000d71be
  000d71f6: ldr.w r0,[r11,#0x4]
  000d71fa: str.w r6,[r11,#0x104]
  000d71fe: cmp r0,#0x3
  000d7200: itttt eq
  000d7202: str.w.eq r8,[r11,#0x1a0]
  000d7206: str.w.eq r8,[r11,#0x64]
  000d720a: mov.eq r0,#0x1
  000d720c: strb.eq.w r0,[r11,#0x13b]
  000d7210: adds r6,#0x1
  000d7212: ldr.w r0,[r11,#0x54]
  000d7216: ldr r1,[r0,#0x0]
  000d7218: cmp r6,r1
  000d721a: bcc 0x000d71ac
  000d721c: ldr.w r0,[r11,#0x104]
  000d7220: adds r1,r0,#0x1
  000d7222: bne 0x000d7234
  000d7224: mov r0,r4
  000d7226: blx 0x00073738
  000d722a: adds r0,#0x1
  000d722c: beq.w 0x000d742c
  000d7230: ldr.w r0,[r11,#0x104]
  000d7234: cmp r0,#0x0
  000d7236: blt 0x000d72dc
  000d7238: ldr r0,[sp,#0x10]
  000d723a: ldr r0,[r0,#0x0]
  000d723c: ldrd r5,r6,[r11,#0x50]
  000d7240: blx 0x00071a10
  000d7244: blx 0x00071a7c
  000d7248: ldr.w r3,[r11,#0x104]
  000d724c: mov r2,r0
  000d724e: mov r0,r5
  000d7250: mov r1,r6
  000d7252: blx 0x000737e0
  000d7256: str.w r0,[r11,#0xa0]
  000d725a: b 0x000d72dc
  000d725c: mov r0,r4
  000d725e: blx 0x00072868
  000d7262: cbnz r0,0x000d72dc
  000d7264: mov r0,r4
  000d7266: blx 0x00074590
  000d726a: cbnz r0,0x000d7276
  000d726c: mov r0,r4
  000d726e: blx 0x00072874
  000d7272: cmp r0,#0xe
  000d7274: bne 0x000d72dc
  000d7276: ldr r0,[sp,#0x10]
  000d7278: ldr r0,[r0,#0x0]
  000d727a: blx 0x00071a10
  000d727e: blx 0x00071aac
  000d7282: cbnz r0,0x000d72dc
  000d7284: movs r6,#0x0
  000d7286: b 0x000d72d2
  000d7288: ldr r0,[r0,#0x4]
  000d728a: ldr.w r0,[r0,r6,lsl #0x2]
  000d728e: blx 0x00071a88
  000d7292: cbz r0,0x000d72d0
  000d7294: movs r4,#0x0
  000d7296: ldr.w r0,[r11,#0x54]
  000d729a: ldr r0,[r0,#0x4]
  000d729c: ldr.w r0,[r0,r6,lsl #0x2]
  000d72a0: blx 0x00071a88
  000d72a4: ldr r0,[r0,#0x0]
  000d72a6: cmp r4,r0
  000d72a8: bcs 0x000d72d0
  000d72aa: mov r0,r8
  000d72ac: blx 0x00073738
  000d72b0: mov r5,r0
  000d72b2: ldr.w r0,[r11,#0x54]
  000d72b6: ldr r0,[r0,#0x4]
  000d72b8: ldr.w r0,[r0,r6,lsl #0x2]
  000d72bc: blx 0x00071a88
  000d72c0: ldr r0,[r0,#0x4]
  000d72c2: ldr.w r0,[r0,r4,lsl #0x2]
  000d72c6: adds r4,#0x1
  000d72c8: cmp r5,r0
  000d72ca: bne 0x000d7296
  000d72cc: str.w r6,[r11,#0x104]
  000d72d0: adds r6,#0x1
  000d72d2: ldr.w r0,[r11,#0x54]
  000d72d6: ldr r1,[r0,#0x0]
  000d72d8: cmp r6,r1
  000d72da: bcc 0x000d7288
  000d72dc: ldr r0,[0x000d7480]
  000d72de: movs r2,#0x1
  000d72e0: ldr.w r1,[r11,#0x104]
  000d72e4: strb.w r2,[r11,#0x13a]
  000d72e8: str.w r0,[r11,#0x168]
  000d72ec: str.w r1,[r11,#0x60]
  000d72f0: movs r1,#0x0
  000d72f2: mov r9,r10
  000d72f4: str.w r1,[r11,#0x10c]
  000d72f8: movs r4,#0x0
  000d72fa: ldr r0,[0x000d7494]
  000d72fc: ldr r2,[0x000d7498]
  000d72fe: add r0,pc
  000d7300: add r2,pc
  000d7302: ldr.w r8,[r0,#0x0]
  000d7306: mov r10,r2
  000d7308: b 0x000d7338
  000d730a: ldr r0,[sp,#0x1c]
  000d730c: ldr.w r1,[r10,r4,lsl #0x2]
  000d7310: ldr.w r5,[r8,#0x0]
  000d7314: ldr r0,[r0,#0x0]
  000d7316: ldr.w r6,[r9,#0x0]
  000d731a: blx 0x00072f70
  000d731e: mov r2,r0
  000d7320: mov r0,r6
  000d7322: mov r1,r5
  000d7324: blx 0x0006faa8
  000d7328: ldr.w r1,[r11,#0x10c]
  000d732c: adds r4,#0x1
  000d732e: cmp r0,r1
  000d7330: itt gt
  000d7332: str.w.gt r0,[r11,#0x10c]
  000d7336: mov.gt r1,r0
  000d7338: cmp r4,#0x6
  000d733a: bne 0x000d730a
  000d733c: ldr r0,[sp,#0x18]
  000d733e: movs r5,#0x0
  000d7340: ldr r0,[r0,#0x0]
  000d7342: ldr.w r2,[r0,#0x8c]
  000d7346: add r1,r2
  000d7348: str.w r1,[r11,#0x10c]
  000d734c: ldr r1,[r0,#0x4]
  000d734e: ldr r0,[r0,#0x2c]
  000d7350: str.w r5,[r11,#0x11c]
  000d7354: add.w r1,r1,r1, lsl #0x2
  000d7358: add.w r0,r1,r0, lsl #0x1
  000d735c: str.w r0,[r11,#0x110]
  000d7360: ldr r0,[sp,#0x10]
  000d7362: ldr r0,[r0,#0x0]
  000d7364: blx 0x00071a58
  000d7368: movs r1,#0x7a
  000d736a: blx 0x00072424
  000d736e: cbz r0,0x000d7376
  000d7370: blx 0x0007183c
  000d7374: mov r5,r0
  000d7376: movs r0,#0xc0
  000d7378: str.w r5,[r11,#0x1d8]
  000d737c: blx 0x0006eb24
  000d7380: mov r5,r0
  000d7382: ldr.w r2,[r9,#0x0]
  000d7386: movw r1,#0x41d2
  000d738a: movs r3,#0x0
  000d738c: blx 0x0007207c
  000d7390: ldr r2,[0x000d747c]
  000d7392: mov r0,r5
  000d7394: movs r1,#0x0
  000d7396: movs r3,#0x0
  000d7398: str.w r5,[r11,#0x1b0]
  000d739c: blx 0x00073054
  000d73a0: ldr r1,[0x000d7484]
  000d73a2: movs r3,#0x0
  000d73a4: ldr.w r0,[r11,#0x1b0]
  000d73a8: ldr r2,[0x000d7478]
  000d73aa: blx 0x00073048
  000d73ae: movs r0,#0xc0
  000d73b0: blx 0x0006eb24
  000d73b4: mov r5,r0
  000d73b6: ldr.w r2,[r9,#0x0]
  000d73ba: movw r1,#0x41d3
  000d73be: movs r3,#0x0
  000d73c0: blx 0x0007207c
  000d73c4: ldr r2,[0x000d747c]
  000d73c6: mov r0,r5
  000d73c8: movs r1,#0x0
  000d73ca: movs r3,#0x0
  000d73cc: str.w r5,[r11,#0x1b4]
  000d73d0: blx 0x00073054
  000d73d4: ldr r1,[0x000d7484]
  000d73d6: movs r3,#0x0
  000d73d8: ldr.w r0,[r11,#0x1b4]
  000d73dc: ldr r2,[0x000d7478]
  000d73de: blx 0x00073048
  000d73e2: movs r0,#0xc0
  000d73e4: blx 0x0006eb24
  000d73e8: mov r5,r0
  000d73ea: ldr.w r2,[r9,#0x0]
  000d73ee: movw r1,#0x41d4
  000d73f2: movs r3,#0x0
  000d73f4: blx 0x0007207c
  000d73f8: ldr r2,[0x000d747c]
  000d73fa: mov r0,r5
  000d73fc: movs r1,#0x0
  000d73fe: movs r3,#0x0
  000d7400: str.w r5,[r11,#0x1b8]
  000d7404: blx 0x00073054
  000d7408: ldr r1,[0x000d7484]
  000d740a: movs r3,#0x0
  000d740c: ldr.w r0,[r11,#0x1b8]
  000d7410: ldr r2,[0x000d7478]
  000d7412: blx 0x00073048
  000d7416: ldr r0,[sp,#0xe0]
  000d7418: ldr r1,[sp,#0xc]
  000d741a: ldr r1,[r1,#0x0]
  000d741c: subs r0,r1,r0
  000d741e: ittt eq
  000d7420: add.eq sp,#0xe4
  000d7422: pop.eq.w {r8,r9,r10,r11}
  000d7426: pop.eq {r4,r5,r6,r7,pc}
  000d7428: blx 0x0006e824
  000d742c: ldr r0,[sp,#0x10]
  000d742e: ldr r0,[r0,#0x0]
  000d7430: ldr r1,[r0,#0x7c]
  000d7432: cmp r1,#0x0
  000d7434: blt.w 0x000d7230
  000d7438: str.w r1,[r11,#0x104]
  000d743c: b 0x000d723c
```
