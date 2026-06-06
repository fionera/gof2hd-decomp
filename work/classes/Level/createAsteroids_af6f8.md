# Level::createAsteroids
elf 0xaf6f8 body 1450
Sig: undefined __thiscall createAsteroids(Level * this)

## decompile
```c

/* Level::createAsteroids() */

void __thiscall Level::createAsteroids(Level *this)

{
  ushort uVar1;
  float fVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  int iVar6;
  Array<KIPlayer*> *this_00;
  Station *pSVar7;
  void *pvVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  Waypoint *this_01;
  BoundingSphere *this_02;
  uint uVar13;
  int iVar14;
  undefined4 uVar15;
  AEGeometry *this_03;
  PlayerAsteroid *this_04;
  int *piVar16;
  bool bVar17;
  undefined4 extraout_r1;
  int iVar18;
  Galaxy *this_05;
  int iVar19;
  code *pcVar20;
  int iVar21;
  uint uVar22;
  int *piVar23;
  uint uVar24;
  int *piVar25;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  float extraout_s4;
  float extraout_s5;
  float extraout_s6;
  float fVar26;
  float fVar27;
  double dVar28;
  float local_80;
  float local_7c;
  float local_78;
  ushort local_74;
  short local_72;
  short local_70;
  undefined8 local_68;
  undefined4 local_60;
  int local_5c;
  
  piVar25 = *(int **)(DAT_000bf97c + 0xbf710);
  local_5c = *piVar25;
  iVar6 = Status::inAlienOrbit();
  if (iVar6 == 0) {
    Status::getSystem();
    iVar6 = SolarSystem::getIndex();
    iVar6 = (uint)(iVar6 == 0x16) << 1;
  }
  else {
    iVar6 = 0;
  }
  this_00 = operator_new(0xc);
  Array<KIPlayer*>::Array(this_00);
  iVar19 = DAT_000bf984;
  *(Array<KIPlayer*> **)(this + 0xfc) = this_00;
  this_05 = (Galaxy *)**(undefined4 **)(iVar19 + 0xbf75a);
  pSVar7 = (Station *)Status::getStation();
  pvVar8 = (void *)Galaxy::getAsteroidProbabilities(this_05,pSVar7);
  piVar23 = *(int **)(DAT_000bf988 + 0xbf774);
  iVar19 = *piVar23;
  pSVar7 = (Station *)Status::getStation();
  Station::getIndex(pSVar7);
  AbyssEngine::AERandom::setSeed(CONCAT44(extraout_r1,iVar19));
  pcVar20 = *(code **)(DAT_000bf98c + 0xbf794);
  iVar19 = (*pcVar20)(*piVar23,0x28);
  ArraySetLength<KIPlayer*>(iVar19 + 0x28,*(Array **)(this + 0xfc));
  uVar15 = DAT_000bf990;
  iVar9 = (*pcVar20)(*piVar23,DAT_000bf990);
  iVar19 = (*pcVar20)(*piVar23,uVar15);
  iVar10 = (*pcVar20)(*piVar23,uVar15);
  iVar11 = Status::inAlienOrbit();
  iVar12 = Status::getCurrentCampaignMission();
  if (iVar11 != 0) {
    iVar19 = 0;
    iVar11 = 30000;
    iVar21 = DAT_000bf994;
    if (iVar12 == 0x9a) {
      iVar21 = -70000;
    }
    goto LAB_000bf8a6;
  }
  if (iVar12 == 0x72) {
    pSVar7 = (Station *)Status::getStation();
    iVar11 = Station::getIndex(pSVar7);
    if (iVar11 != 0x53) goto LAB_000bf80c;
    iVar11 = DAT_000bf998;
    iVar21 = 30000;
  }
  else {
LAB_000bf80c:
    iVar11 = Status::getCurrentCampaignMission();
    if ((iVar11 != 0x59) ||
       (iVar12 = Status::inSupernovaOrbit(), iVar11 = DAT_000bf9a0, iVar21 = DAT_000bf99c,
       iVar12 == 0)) {
      iVar11 = Status::getCurrentCampaignMission();
      if (iVar11 == 0x5b) {
        pSVar7 = (Station *)Status::getStation();
        iVar11 = Station::getIndex(pSVar7);
        if (iVar11 == 0x6e) {
          iVar19 = 0;
          iVar11 = 50000;
          iVar21 = 60000;
          goto LAB_000bf8a6;
        }
      }
      iVar11 = Status::getCurrentCampaignMission();
      if (iVar11 == 0x91) {
        pSVar7 = (Station *)Status::getStation();
        iVar11 = Station::getIndex(pSVar7);
        if (iVar11 == 0x70) {
          iVar11 = DAT_000bf9a4;
          iVar21 = 50000;
          goto LAB_000bf874;
        }
      }
      iVar19 = iVar19 + -50000;
      iVar11 = iVar10 + 20000;
      iVar10 = Status::getCurrentCampaignMission();
      iVar21 = iVar9 + -50000;
      if ((iVar10 == 0) && (*(int *)(this + 0xc0) == 3)) {
        iVar11 = 0;
        iVar19 = 0;
        iVar21 = 0;
      }
      goto LAB_000bf8a6;
    }
  }
LAB_000bf874:
  iVar19 = 0;
LAB_000bf8a6:
  AbyssEngine::AERandom::reset();
  local_7c = (float)VectorSignedToFloat(iVar19,(byte)(in_fpscr >> 0x16) & 3);
  local_80 = (float)VectorSignedToFloat(iVar21,(byte)(in_fpscr >> 0x16) & 3);
  local_78 = (float)VectorSignedToFloat(iVar11,(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 200),(Vector *)&local_80);
  this_01 = operator_new(0x138);
  Waypoint::Waypoint(this_01,iVar21,iVar19,iVar11,(Route *)0x0);
  *(Waypoint **)(this + 0xd8) = this_01;
  this_02 = operator_new(0x48);
  BoundingSphere::BoundingSphere
            (this_02,extraout_s0,extraout_s1,extraout_s2,extraout_s3,extraout_s4,extraout_s5,
             extraout_s6);
  *(BoundingSphere **)(this + 0xc4) = this_02;
  iVar9 = AbyssEngine::AERandom::nextInt(*piVar23);
  iVar19 = Status::inAlienOrbit();
  dVar5 = DAT_000bfd08;
  dVar4 = DAT_000bfd00;
  dVar3 = DAT_000bfcf8;
  fVar2 = DAT_000bfcf4;
  uVar22 = 0;
  iVar11 = 0;
  iVar10 = 0x9a;
  iVar9 = iVar9 + 2;
  local_80 = 0.0;
  local_7c = 0.0;
  local_78 = 0.0;
  do {
    if (**(uint **)(this + 0xfc) <= uVar22) {
      if (pvVar8 != (void *)0x0) {
        operator_delete__(pvVar8);
      }
      if (*piVar25 == local_5c) {
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
    if (iVar19 == 0) {
      bVar17 = false;
      iVar12 = iVar11;
      while (iVar11 = iVar12, !bVar17) {
        iVar21 = AbyssEngine::AERandom::nextInt(*piVar23);
        bVar17 = false;
        iVar12 = 0;
        if (iVar21 < *(int *)((int)pvVar8 + iVar11 * 4 + 4)) {
          iVar10 = *(int *)((int)pvVar8 + iVar11 * 4);
          iVar12 = iVar11 + 2;
          if (8 < iVar11) {
            iVar12 = 0;
          }
          bVar17 = iVar10 == 0xd9 || iVar10 < 0xa4;
        }
      }
    }
    else {
      iVar10 = 0xa4;
    }
    iVar12 = DAT_000bfd14 + 0xbf9fe;
    uVar13 = DAT_000bfd10;
    if ((int)uVar22 < iVar9) {
      uVar13 = 60000;
    }
    fVar26 = (float)VectorSignedToFloat(uVar13 >> 1,(byte)(in_fpscr >> 0x16) & 3);
    iVar14 = Status::inAlienOrbit();
    iVar21 = iVar6;
    if (iVar14 != 0) {
      iVar21 = 1;
    }
    if (iVar10 == 0xd9) {
      iVar21 = 3;
    }
    iVar12 = *(int *)(iVar12 + iVar21 * 4);
    while( true ) {
      fVar27 = *(float *)(this + 200);
      uVar15 = (*pcVar20)(*piVar23,uVar13);
      local_80 = (float)VectorSignedToFloat(uVar15,(byte)(in_fpscr >> 0x16) & 3);
      local_80 = (fVar27 - fVar26) + local_80;
      fVar27 = *(float *)(this + 0xcc);
      uVar15 = (*pcVar20)(*piVar23,uVar13);
      local_7c = (float)VectorSignedToFloat(uVar15,(byte)(in_fpscr >> 0x16) & 3);
      local_7c = (fVar27 - fVar26) + local_7c;
      fVar27 = *(float *)(this + 0xd0);
      uVar15 = (*pcVar20)(*piVar23,uVar13);
      local_78 = (float)VectorSignedToFloat(uVar15,(byte)(in_fpscr >> 0x16) & 3);
      local_78 = (fVar27 - fVar26) + local_78;
      if (uVar22 == 0 || (int)uVar22 >= iVar9) break;
      bVar17 = false;
      uVar24 = 0;
      while (uVar24 < uVar22) {
        (**(code **)(**(int **)(*(int *)(*(int *)(this + 0xfc) + 4) + uVar24 * 4) + 0x28))
                  ((Vector *)&local_74);
        AbyssEngine::AEMath::operator-((Vector *)&local_68,(Vector *)&local_74);
        fVar27 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_68);
        uVar24 = uVar24 + 1;
        bVar17 = (bool)(bVar17 | 8000 < (int)fVar27);
        if (8000 < (int)fVar27) goto LAB_000bfb10;
      }
      if (bVar17) break;
    }
LAB_000bfb10:
    this_03 = operator_new(0xc0);
    uVar1 = (ushort)iVar12;
    AEGeometry::AEGeometry
              (this_03,uVar1,(PaintCanvas *)**(undefined4 **)(DAT_000bfd18 + 0xbfb1e),false);
    local_72 = uVar1 + 2;
    local_74 = uVar1 + 1;
    local_70 = uVar1 + 3;
    if (**(char **)(DAT_000bfd1c + 0xbfb3a) == '\0') {
      if ((int)uVar22 < iVar9) {
        local_68 = *(undefined8 *)(DAT_000bfd20 + 0xbfb8e);
        local_60 = *(undefined4 *)(DAT_000bfd20 + 0xbfb96);
        AEGeometry::setLodMeshes(this_03,&local_74,(int *)&local_68,3);
        LODManager::addObject(*(LODManager **)this,this_03);
        iVar14 = *piVar23;
        goto LAB_000bfbaa;
      }
      local_68 = *(undefined8 *)(DAT_000bfd28 + 0xbfbc0);
      local_60 = *(undefined4 *)(DAT_000bfd28 + 0xbfbc8);
      AEGeometry::setLodMeshes(this_03,&local_74,(int *)&local_68,3);
      LODManager::addObject(*(LODManager **)this,this_03);
      iVar14 = *piVar23;
LAB_000bfbdc:
      iVar14 = AbyssEngine::AERandom::nextInt(iVar14);
      iVar18 = 0x1e;
    }
    else {
      local_68 = *(undefined8 *)(DAT_000bfd24 + 0xbfb54);
      local_60 = *(undefined4 *)(DAT_000bfd24 + 0xbfb5c);
      AEGeometry::setLodMeshes(this_03,&local_74,(int *)&local_68,3);
      LODManager::addObject(*(LODManager **)this,this_03);
      iVar14 = *piVar23;
      if (iVar9 <= (int)uVar22) goto LAB_000bfbdc;
LAB_000bfbaa:
      iVar14 = AbyssEngine::AERandom::nextInt(iVar14);
      iVar18 = 0x78;
    }
    fVar26 = (float)VectorSignedToFloat(iVar14 + iVar18,(byte)(in_fpscr >> 0x16) & 3);
    dVar28 = (double)(fVar26 * fVar2);
    in_fpscr = in_fpscr & 0xfffffff;
    if ((((dVar3 <= dVar28) && (dVar4 <= dVar28)) && (dVar5 <= dVar28)) &&
       (iVar14 = AbyssEngine::AERandom::nextInt(*piVar23), iVar14 != 0)) {
      AbyssEngine::AERandom::nextInt(*piVar23);
    }
    this_04 = operator_new(0x170);
    PlayerAsteroid::PlayerAsteroid
              (this_04,iVar12,this_03,iVar21,iVar10,(Vector *)&local_80,extraout_s0_00,
               (int)(fVar26 * fVar2));
    *(PlayerAsteroid **)(*(int *)(*(int *)(this + 0xfc) + 4) + uVar22 * 4) = this_04;
    piVar16 = *(int **)(*(int *)(*(int *)(this + 0xfc) + 4) + uVar22 * 4);
    (**(code **)(*piVar16 + 0x14))(piVar16,this);
    PlayerAsteroid::setAsteroidCenter
              (*(undefined4 *)(*(int *)(*(int *)(this + 0xfc) + 4) + uVar22 * 4),
               *(undefined4 *)(this + 200),*(undefined4 *)(this + 0xcc),*(undefined4 *)(this + 0xd0)
              );
    uVar22 = uVar22 + 1;
  } while( true );
}

```
## target disasm
```
  000bf6f8: push {r4,r5,r6,r7,lr}
  000bf6fa: add r7,sp,#0xc
  000bf6fc: push {r7,r8,r9,r10,r11}
  000bf700: vpush {d8,d9,d10,d11,d12,d13}
  000bf704: sub sp,#0x78
  000bf706: mov r5,r0
  000bf708: ldr r0,[0x000bf97c]
  000bf70a: ldr r1,[0x000bf980]
  000bf70c: add r0,pc
  000bf70e: add r1,pc
  000bf710: ldr.w r10,[r0,#0x0]
  000bf714: ldr.w r11,[r1,#0x0]
  000bf718: ldr.w r1,[r10,#0x0]
  000bf71c: ldr.w r0,[r11,#0x0]
  000bf720: str r1,[sp,#0x74]
  000bf722: blx 0x000723d0
  000bf726: cbz r0,0x000bf72c
  000bf728: movs r0,#0x0
  000bf72a: b 0x000bf742
  000bf72c: ldr.w r0,[r11,#0x0]
  000bf730: blx 0x00071a10
  000bf734: blx 0x00071a7c
  000bf738: subs r0,#0x16
  000bf73a: clz r0,r0
  000bf73e: lsrs r0,r0,#0x5
  000bf740: lsls r0,r0,#0x1
  000bf742: str r0,[sp,#0x20]
  000bf744: movs r0,#0xc
  000bf746: blx 0x0006eb24
  000bf74a: mov r4,r0
  000bf74c: blx 0x00073d08
  000bf750: ldr r0,[0x000bf984]
  000bf752: str.w r4,[r5,#0xfc]
  000bf756: add r0,pc
  000bf758: ldr r0,[r0,#0x0]
  000bf75a: ldr r4,[r0,#0x0]
  000bf75c: ldr.w r0,[r11,#0x0]
  000bf760: blx 0x00071c14
  000bf764: mov r1,r0
  000bf766: mov r0,r4
  000bf768: blx 0x00071c38
  000bf76c: str r0,[sp,#0x24]
  000bf76e: ldr r0,[0x000bf988]
  000bf770: add r0,pc
  000bf772: ldr r6,[r0,#0x0]
  000bf774: ldr.w r0,[r11,#0x0]
  000bf778: ldr r4,[r6,#0x0]
  000bf77a: blx 0x00071c14
  000bf77e: blx 0x00071824
  000bf782: mov r2,r0
  000bf784: asrs r3,r0,#0x1f
  000bf786: mov r0,r4
  000bf788: blx 0x000739c0
  000bf78c: ldr r0,[0x000bf98c]
  000bf78e: movs r1,#0x28
  000bf790: add r0,pc
  000bf792: ldr r4,[r0,#0x0]
  000bf794: ldr r0,[r6,#0x0]
  000bf796: blx r4
  000bf798: ldr.w r1,[r5,#0xfc]
  000bf79c: adds r0,#0x28
  000bf79e: str r5,[sp,#0x28]
  000bf7a0: blx 0x00073d14
  000bf7a4: ldr r5,[0x000bf990]
  000bf7a6: ldr r0,[r6,#0x0]
  000bf7a8: mov r1,r5
  000bf7aa: blx r4
  000bf7ac: mov r8,r0
  000bf7ae: ldr r0,[r6,#0x0]
  000bf7b0: mov r1,r5
  000bf7b2: blx r4
  000bf7b4: mov r9,r0
  000bf7b6: ldr r0,[r6,#0x0]
  000bf7b8: mov r1,r5
  000bf7ba: str r6,[sp,#0x48]
  000bf7bc: str r4,[sp,#0x34]
  000bf7be: blx r4
  000bf7c0: mov r4,r0
  000bf7c2: ldr.w r0,[r11,#0x0]
  000bf7c6: blx 0x000723d0
  000bf7ca: mov r5,r0
  000bf7cc: ldr.w r0,[r11,#0x0]
  000bf7d0: blx 0x00071770
  000bf7d4: cmp r5,#0x0
  000bf7d6: beq 0x000bf7f0
  000bf7d8: ldr r5,[0x000bf994]
  000bf7da: cmp r0,#0x9a
  000bf7dc: mov.w r8,#0x0
  000bf7e0: movw r4,#0x7530
  000bf7e4: itt eq
  000bf7e6: movw.eq r5,#0xee90
  000bf7ea: movt.eq r5,#0xfffe
  000bf7ee: b 0x000bf8a6
  000bf7f0: cmp r0,#0x72
  000bf7f2: bne 0x000bf80c
  000bf7f4: ldr.w r0,[r11,#0x0]
  000bf7f8: blx 0x00071c14
  000bf7fc: blx 0x00071824
  000bf800: cmp r0,#0x53
  000bf802: bne 0x000bf80c
  000bf804: ldr r4,[0x000bf998]
  000bf806: movw r5,#0x7530
  000bf80a: b 0x000bf874
  000bf80c: ldr.w r0,[r11,#0x0]
  000bf810: blx 0x00071770
  000bf814: cmp r0,#0x59
  000bf816: bne 0x000bf828
  000bf818: ldr.w r0,[r11,#0x0]
  000bf81c: blx 0x000722f8
  000bf820: cbz r0,0x000bf828
  000bf822: ldr r5,[0x000bf99c]
  000bf824: ldr r4,[0x000bf9a0]
  000bf826: b 0x000bf874
  000bf828: ldr.w r0,[r11,#0x0]
  000bf82c: blx 0x00071770
  000bf830: cmp r0,#0x5b
  000bf832: bne 0x000bf852
  000bf834: ldr.w r0,[r11,#0x0]
  000bf838: blx 0x00071c14
  000bf83c: blx 0x00071824
  000bf840: cmp r0,#0x6e
  000bf842: bne 0x000bf852
  000bf844: movw r5,#0xea60
  000bf848: mov.w r8,#0x0
  000bf84c: movw r4,#0xc350
  000bf850: b 0x000bf8a6
  000bf852: ldr.w r0,[r11,#0x0]
  000bf856: blx 0x00071770
  000bf85a: cmp r0,#0x91
  000bf85c: bne 0x000bf87a
  000bf85e: ldr.w r0,[r11,#0x0]
  000bf862: blx 0x00071c14
  000bf866: blx 0x00071824
  000bf86a: cmp r0,#0x70
  000bf86c: bne 0x000bf87a
  000bf86e: movw r5,#0xc350
  000bf872: ldr r4,[0x000bf9a4]
  000bf874: mov.w r8,#0x0
  000bf878: b 0x000bf8a6
  000bf87a: movw r0,#0xc350
  000bf87e: sub.w r5,r8,r0
  000bf882: sub.w r8,r9,r0
  000bf886: movw r0,#0x4e20
  000bf88a: add r4,r0
  000bf88c: ldr.w r0,[r11,#0x0]
  000bf890: blx 0x00071770
  000bf894: cbnz r0,0x000bf8a6
  000bf896: ldr r0,[sp,#0x28]
  000bf898: ldr.w r0,[r0,#0xc0]
  000bf89c: subs r0,#0x3
  000bf89e: ittt eq
  000bf8a0: mov.eq r5,r0
  000bf8a2: mov.eq r8,r0
  000bf8a4: mov.eq r4,r0
  000bf8a6: ldr r0,[sp,#0x48]
  000bf8a8: ldr r0,[r0,#0x0]
  000bf8aa: blx 0x000718cc
  000bf8ae: vmov s0,r8
  000bf8b2: add r1,sp,#0x50
  000bf8b4: vcvt.f32.s32 s16,s0
  000bf8b8: vmov s0,r5
  000bf8bc: vcvt.f32.s32 s18,s0
  000bf8c0: vmov s0,r4
  000bf8c4: vcvt.f32.s32 s20,s0
  000bf8c8: ldr.w r9,[sp,#0x28]
  000bf8cc: vstr.32 s16,[sp,#0x54]
  000bf8d0: add.w r0,r9,#0xc8
  000bf8d4: vstr.32 s18,[sp,#0x50]
  000bf8d8: vstr.32 s20,[sp,#0x58]
  000bf8dc: blx 0x0006eb3c
  000bf8e0: mov.w r0,#0x138
  000bf8e4: blx 0x0006eb24
  000bf8e8: mov r3,r4
  000bf8ea: mov r4,r0
  000bf8ec: movs r6,#0x0
  000bf8ee: mov r1,r5
  000bf8f0: mov r2,r8
  000bf8f2: str r6,[sp,#0x0]
  000bf8f4: blx 0x00073e4c
  000bf8f8: movs r0,#0x48
  000bf8fa: str.w r10,[sp,#0x14]
  000bf8fe: str.w r4,[r9,#0xd8]
  000bf902: blx 0x0006eb24
  000bf906: vmov r1,s18
  000bf90a: mov r5,r0
  000bf90c: ldr r0,[0x000bf9a8]
  000bf90e: vmov r2,s16
  000bf912: vmov r3,s20
  000bf916: strd r6,r6,[sp,#0x0]
  000bf91a: strd r6,r0,[sp,#0x8]
  000bf91e: mov r0,r5
  000bf920: blx 0x000730e4
  000bf924: ldr r0,[sp,#0x48]
  000bf926: movs r1,#0x8
  000bf928: str.w r5,[r9,#0xc4]
  000bf92c: ldr r0,[r0,#0x0]
  000bf92e: blx 0x00071848
  000bf932: mov r4,r0
  000bf934: ldr.w r0,[r11,#0x0]
  000bf938: str.w r11,[sp,#0x1c]
  000bf93c: blx 0x000723d0
  000bf940: vldr.32 s16,[pc,#0x3b0]
  000bf944: mov r10,r9
  000bf946: vldr.64 d9,[pc,#0x3b0]
  000bf94a: movs r5,#0x0
  000bf94c: vldr.64 d10,[pc,#0x3b0]
  000bf950: mov.w r11,#0x0
  000bf954: vldr.64 d11,[pc,#0x3b0]
  000bf958: mov.w r9,#0x9a
  000bf95c: str r0,[sp,#0x18]
  000bf95e: adds r0,r4,#0x2
  000bf960: strd r5,r5,[sp,#0x50]
  000bf964: str r5,[sp,#0x58]
  000bf966: str r0,[sp,#0x38]
  000bf968: b 0x000bfc9e
  000bf96a: ldr r0,[sp,#0x18]
  000bf96c: cbz r0,0x000bf974
  000bf96e: mov.w r9,#0xa4
  000bf972: b 0x000bf9f2
  000bf974: ldr r6,[sp,#0x48]
  000bf976: movs r1,#0x0
  000bf978: b 0x000bf9ea
  000bf9ac: ldr r0,[r6,#0x0]
  000bf9ae: movs r1,#0x64
  000bf9b0: blx 0x00071848
  000bf9b4: add.w r1,r5,r4, lsl #0x2
  000bf9b8: mov.w r11,#0x0
  000bf9bc: ldr r2,[r1,#0x4]
  000bf9be: movs r1,#0x0
  000bf9c0: cmp r0,r2
  000bf9c2: bge 0x000bf9ea
  000bf9c4: ldr.w r9,[r5,r4,lsl #0x2]
  000bf9c8: movs r0,#0x0
  000bf9ca: add.w r11,r4,#0x2
  000bf9ce: sub.w r1,r9,#0xd9
  000bf9d2: cmp.w r9,#0xa4
  000bf9d6: clz r1,r1
  000bf9da: it lt
  000bf9dc: mov.lt r0,#0x1
  000bf9de: cmp r4,#0x8
  000bf9e0: it gt
  000bf9e2: mov.gt.w r11,#0x0
  000bf9e6: lsrs r1,r1,#0x5
  000bf9e8: orrs r1,r0
  000bf9ea: mov r4,r11
  000bf9ec: lsls r0,r1,#0x1f
  000bf9ee: beq 0x000bf9ac
  000bf9f0: mov r11,r4
  000bf9f2: ldr r6,[0x000bfd14]
  000bf9f4: movs r5,#0x0
  000bf9f6: ldr r0,[sp,#0x38]
  000bf9f8: ldr r4,[sp,#0x4c]
  000bf9fa: add r6,pc
  000bf9fc: cmp r4,r0
  000bf9fe: ldr r0,[0x000bfd10]
  000bfa00: it lt
  000bfa02: mov.lt r5,#0x1
  000bfa04: it lt
  000bfa06: movw.lt r0,#0xea60
  000bfa0a: str r0,[sp,#0x40]
  000bfa0c: lsrs r0,r0,#0x1
  000bfa0e: vmov s0,r0
  000bfa12: ldr r0,[sp,#0x1c]
  000bfa14: ldr r0,[r0,#0x0]
  000bfa16: vcvt.f32.s32 s24,s0
  000bfa1a: blx 0x000723d0
  000bfa1e: ldr r1,[sp,#0x20]
  000bfa20: cmp r0,#0x0
  000bfa22: mov r0,r4
  000bfa24: it ne
  000bfa26: mov.ne r1,#0x1
  000bfa28: cmp.w r9,#0xd9
  000bfa2c: str.w r9,[sp,#0x30]
  000bfa30: it eq
  000bfa32: mov.eq r1,#0x3
  000bfa34: cmp r4,#0x0
  000bfa36: it ne
  000bfa38: mov.ne r0,#0x1
  000bfa3a: ldr.w r10,[sp,#0x28]
  000bfa3e: ands r0,r5
  000bfa40: str r1,[sp,#0x2c]
  000bfa42: ldr.w r1,[r6,r1,lsl #0x2]
  000bfa46: str r0,[sp,#0x3c]
  000bfa48: str r1,[sp,#0x44]
  000bfa4a: ldr r6,[sp,#0x48]
  000bfa4c: ldr r5,[sp,#0x40]
  000bfa4e: ldr r4,[sp,#0x34]
  000bfa50: ldr r0,[r6,#0x0]
  000bfa52: mov r1,r5
  000bfa54: vldr.32 s26,[r10,#0xc8]
  000bfa58: blx r4
  000bfa5a: vmov s2,r0
  000bfa5e: mov r1,r5
  000bfa60: vsub.f32 s0,s26,s24
  000bfa64: vcvt.f32.s32 s2,s2
  000bfa68: ldr r0,[r6,#0x0]
  000bfa6a: vadd.f32 s0,s0,s2
  000bfa6e: vstr.32 s0,[sp,#0x50]
  000bfa72: vldr.32 s26,[r10,#0xcc]
  000bfa76: blx r4
  000bfa78: vmov s2,r0
  000bfa7c: mov r1,r5
  000bfa7e: vsub.f32 s0,s26,s24
  000bfa82: vcvt.f32.s32 s2,s2
  000bfa86: ldr r0,[r6,#0x0]
  000bfa88: ldr r5,[sp,#0x4c]
  000bfa8a: vadd.f32 s0,s0,s2
  000bfa8e: vstr.32 s0,[sp,#0x54]
  000bfa92: vldr.32 s26,[r10,#0xd0]
  000bfa96: blx r4
  000bfa98: vmov s2,r0
  000bfa9c: add r4,sp,#0x5c
  000bfa9e: vsub.f32 s0,s26,s24
  000bfaa2: vcvt.f32.s32 s2,s2
  000bfaa6: ldr r0,[sp,#0x3c]
  000bfaa8: cmp r0,#0x0
  000bfaaa: vadd.f32 s0,s0,s2
  000bfaae: vstr.32 s0,[sp,#0x58]
  000bfab2: beq 0x000bfb10
  000bfab4: mov.w r8,#0x0
  000bfab8: mov.w r9,#0x0
  000bfabc: cmp r9,r5
  000bfabe: bcs 0x000bfb0a
  000bfac0: ldr.w r0,[r10,#0xfc]
  000bfac4: ldr r0,[r0,#0x4]
  000bfac6: ldr.w r1,[r0,r9,lsl #0x2]
  000bfaca: ldr r0,[r1,#0x0]
  000bfacc: ldr r2,[r0,#0x28]
  000bface: mov r0,r4
  000bfad0: blx r2
  000bfad2: add r6,sp,#0x68
  000bfad4: add r2,sp,#0x50
  000bfad6: mov r1,r4
  000bfad8: mov r0,r6
  000bfada: blx 0x0006ec38
  000bfade: mov r0,r6
  000bfae0: blx 0x0006ec44
  000bfae4: vmov s0,r0
  000bfae8: movs r1,#0x0
  000bfaea: add.w r9,r9,#0x1
  000bfaee: vcvt.s32.f32 s0,s0
  000bfaf2: vmov r0,s0
  000bfaf6: cmp.w r0,#0x1f40
  000bfafa: it gt
  000bfafc: mov.gt r1,#0x1
  000bfafe: orr.w r8,r8,r1
  000bfb02: cmp.w r0,#0x1f40
  000bfb06: ble 0x000bfabc
  000bfb08: b 0x000bfb10
  000bfb0a: lsls.w r0,r8,#0x1f
  000bfb0e: beq 0x000bfa4a
  000bfb10: movs r0,#0xc0
  000bfb12: blx 0x0006eb24
  000bfb16: mov r8,r0
  000bfb18: ldr r0,[0x000bfd18]
  000bfb1a: add r0,pc
  000bfb1c: ldr r0,[r0,#0x0]
  000bfb1e: ldr r2,[r0,#0x0]
  000bfb20: ldr r6,[sp,#0x44]
  000bfb22: mov r0,r8
  000bfb24: movs r3,#0x0
  000bfb26: uxth r1,r6
  000bfb28: blx 0x0007207c
  000bfb2c: ldr r0,[0x000bfd1c]
  000bfb2e: adds r1,r6,#0x2
  000bfb30: strh.w r1,[sp,#0x5e]
  000bfb34: adds r1,r6,#0x1
  000bfb36: add r0,pc
  000bfb38: strh.w r1,[sp,#0x5c]
  000bfb3c: adds r1,r6,#0x3
  000bfb3e: strh.w r1,[sp,#0x60]
  000bfb42: ldr r0,[r0,#0x0]
  000bfb44: ldrb r0,[r0,#0x0]
  000bfb46: cbz r0,0x000bfb7a
  000bfb48: ldr r0,[0x000bfd24]
  000bfb4a: add r2,sp,#0x68
  000bfb4c: mov r1,r4
  000bfb4e: movs r3,#0x3
  000bfb50: add r0,pc
  000bfb52: vldr.64 d16,[r0]
  000bfb56: ldr r0,[r0,#0x8]
  000bfb58: str r0,[sp,#0x70]
  000bfb5a: mov r0,r8
  000bfb5c: vstr.64 d16,[sp,#0x68]
  000bfb60: blx 0x00073d5c
  000bfb64: ldr.w r0,[r10,#0x0]
  000bfb68: mov r1,r8
  000bfb6a: blx 0x00073d38
  000bfb6e: ldr r4,[sp,#0x48]
  000bfb70: ldr r1,[sp,#0x38]
  000bfb72: ldr r0,[r4,#0x0]
  000bfb74: cmp r5,r1
  000bfb76: blt 0x000bfbaa
  000bfb78: b 0x000bfbdc
  000bfb7a: ldr r0,[sp,#0x38]
  000bfb7c: ldr r4,[sp,#0x48]
  000bfb7e: cmp r5,r0
  000bfb80: bge 0x000bfbb4
  000bfb82: ldr r0,[0x000bfd20]
  000bfb84: add r1,sp,#0x5c
  000bfb86: add r2,sp,#0x68
  000bfb88: movs r3,#0x3
  000bfb8a: add r0,pc
  000bfb8c: vldr.64 d16,[r0]
  000bfb90: ldr r0,[r0,#0x8]
  000bfb92: str r0,[sp,#0x70]
  000bfb94: mov r0,r8
  000bfb96: vstr.64 d16,[sp,#0x68]
  000bfb9a: blx 0x00073d5c
  000bfb9e: ldr.w r0,[r10,#0x0]
  000bfba2: mov r1,r8
  000bfba4: blx 0x00073d38
  000bfba8: ldr r0,[r4,#0x0]
  000bfbaa: movs r1,#0x64
  000bfbac: blx 0x00071848
  000bfbb0: movs r1,#0x78
  000bfbb2: b 0x000bfbe4
  000bfbb4: ldr r0,[0x000bfd28]
  000bfbb6: add r1,sp,#0x5c
  000bfbb8: add r2,sp,#0x68
  000bfbba: movs r3,#0x3
  000bfbbc: add r0,pc
  000bfbbe: vldr.64 d16,[r0]
  000bfbc2: ldr r0,[r0,#0x8]
  000bfbc4: str r0,[sp,#0x70]
  000bfbc6: mov r0,r8
  000bfbc8: vstr.64 d16,[sp,#0x68]
  000bfbcc: blx 0x00073d5c
  000bfbd0: ldr.w r0,[r10,#0x0]
  000bfbd4: mov r1,r8
  000bfbd6: blx 0x00073d38
  000bfbda: ldr r0,[r4,#0x0]
  000bfbdc: movs r1,#0x46
  000bfbde: blx 0x00071848
  000bfbe2: movs r1,#0x1e
  000bfbe4: add r0,r1
  000bfbe6: vmov s0,r0
  000bfbea: vcvt.f32.s32 s0,s0
  000bfbee: vmul.f32 s24,s0,s16
  000bfbf2: vcvt.f64.f32 d16,s24
  000bfbf6: vcmpe.f64 d16,d9
  000bfbfa: vmrs apsr,fpscr
  000bfbfe: bpl 0x000bfc06
  000bfc00: mov.w r9,#0x4
  000bfc04: b 0x000bfc42
  000bfc06: vcmpe.f64 d16,d10
  000bfc0a: vmrs apsr,fpscr
  000bfc0e: bpl 0x000bfc16
  000bfc10: mov.w r9,#0x5
  000bfc14: b 0x000bfc42
  000bfc16: vcmpe.f64 d16,d11
  000bfc1a: vmrs apsr,fpscr
  000bfc1e: bpl 0x000bfc26
  000bfc20: mov.w r9,#0x6
  000bfc24: b 0x000bfc42
  000bfc26: ldr r0,[r4,#0x0]
  000bfc28: movs r1,#0x2
  000bfc2a: blx 0x00071848
  000bfc2e: cbz r0,0x000bfc3e
  000bfc30: ldr r0,[r4,#0x0]
  000bfc32: movs r1,#0x3
  000bfc34: blx 0x00071848
  000bfc38: add.w r9,r0,#0x4
  000bfc3c: b 0x000bfc42
  000bfc3e: mov.w r9,#0x7
  000bfc42: mov.w r0,#0x170
  000bfc46: blx 0x0006eb24
  000bfc4a: mov r4,r0
  000bfc4c: ldr r1,[sp,#0x44]
  000bfc4e: add r0,sp,#0x50
  000bfc50: ldr r3,[sp,#0x2c]
  000bfc52: mov r2,r8
  000bfc54: str.w r9,[sp,#0xc]
  000bfc58: ldr.w r9,[sp,#0x30]
  000bfc5c: str r0,[sp,#0x4]
  000bfc5e: mov r0,r4
  000bfc60: vstr.32 s24,[sp,#0x8]
  000bfc64: str.w r9,[sp,#0x0]
  000bfc68: blx 0x00073e58
  000bfc6c: ldr.w r0,[r10,#0xfc]
  000bfc70: ldr r0,[r0,#0x4]
  000bfc72: str.w r4,[r0,r5,lsl #0x2]
  000bfc76: ldr.w r0,[r10,#0xfc]
  000bfc7a: ldr r0,[r0,#0x4]
  000bfc7c: ldr.w r0,[r0,r5,lsl #0x2]
  000bfc80: ldr r1,[r0,#0x0]
  000bfc82: ldr r2,[r1,#0x14]
  000bfc84: mov r1,r10
  000bfc86: blx r2
  000bfc88: add.w r3,r10,#0xc8
  000bfc8c: ldmia r3,{r1,r2,r3}
  000bfc8e: ldr.w r0,[r10,#0xfc]
  000bfc92: ldr r0,[r0,#0x4]
  000bfc94: ldr.w r0,[r0,r5,lsl #0x2]
  000bfc98: blx 0x00073e64
  000bfc9c: adds r5,#0x1
  000bfc9e: ldr.w r0,[r10,#0xfc]
  000bfca2: str r5,[sp,#0x4c]
  000bfca4: ldr r0,[r0,#0x0]
  000bfca6: cmp r5,r0
  000bfca8: ldr r5,[sp,#0x24]
  000bfcaa: bcc.w 0x000bf96a
  000bfcae: cbz r5,0x000bfcb6
  000bfcb0: mov r0,r5
  000bfcb2: blx 0x0006ebfc
  000bfcb6: ldr r0,[sp,#0x74]
  000bfcb8: ldr r1,[sp,#0x14]
  000bfcba: ldr r1,[r1,#0x0]
  000bfcbc: subs r0,r1,r0
  000bfcbe: itttt eq
  000bfcc0: add.eq sp,#0x78
  000bfcc2: vpop.eq {d8,d9,d10,d11,d12,d13}
  000bfcc6: add.eq sp,#0x4
  000bfcc8: pop.eq.w {r8,r9,r10,r11}
  000bfccc: it eq
  000bfcce: pop.eq {r4,r5,r6,r7,pc}
  000bfcd0: blx 0x0006e824
```
