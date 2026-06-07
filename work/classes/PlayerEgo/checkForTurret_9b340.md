# PlayerEgo::checkForTurret
elf 0x9b340 body 988
Sig: undefined __thiscall checkForTurret(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::checkForTurret() */

void __thiscall PlayerEgo::checkForTurret(PlayerEgo *this)

{
  int iVar1;
  Ship *this_00;
  undefined4 uVar2;
  int iVar3;
  AEGeometry *pAVar4;
  void *pvVar5;
  SingleLevel *this_01;
  Transform *this_02;
  ushort uVar6;
  ushort uVar7;
  uint *puVar8;
  int iVar9;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float fVar10;
  float extraout_s0_02;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar11;
  float extraout_s1_02;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar12;
  float extraout_s2_02;
  double dVar13;
  int local_48;
  int local_44;
  
  if (*(char *)&this[10].field_8 != '\0') {
    return;
  }
  iVar1 = Player::gunAvailable(this->m_pPlayer);
  *(char *)&this[10].field_8 = (char)iVar1;
  if (iVar1 == 0) {
    return;
  }
  *(undefined1 *)&this[10].field_18 = 0;
  this_00 = (Ship *)Status::getShip();
  iVar1 = Ship::getEquipment(this_00,2);
  uVar2 = Item::getAttribute(**(int **)(iVar1 + 4));
  dVar13 = (double)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
  this[0xe].m_pPlayer = (int)(longlong)(dVar13 * 1.5);
  iVar3 = Item::getIndex((Item *)**(undefined4 **)(iVar1 + 4));
  if (iVar3 == 0xe0) {
    uVar7 = 0x499a;
    iVar3 = 0x499d;
    iVar9 = 0x499c;
    uVar6 = 0x499b;
  }
  else {
    if (iVar3 == 0x30) {
      uVar7 = 0x1a74;
      uVar6 = 0x1a75;
    }
    else if (iVar3 == 0x31) {
      uVar7 = 0x1a76;
      uVar6 = 0x1a77;
    }
    else if (iVar3 == 0xb4) {
      *(undefined1 *)&this[10].field_18 = 1;
      uVar7 = 0x1a95;
      uVar6 = 0x1a96;
    }
    else if (iVar3 == 0xb5) {
      *(undefined1 *)&this[10].field_18 = 1;
      uVar7 = 0x1a97;
      uVar6 = 0x1a98;
    }
    else {
      if (iVar3 != 0xb6) {
        if (iVar3 == 0xc6) {
          local_48 = -1;
          uVar7 = 0x4963;
          iVar3 = 0x4966;
          iVar9 = 0x4964;
          uVar6 = 0x4967;
        }
        else if (iVar3 == 199) {
          local_48 = -1;
          uVar7 = 0x4968;
          iVar3 = 0x496a;
          iVar9 = 0x4969;
          uVar6 = 0x496b;
        }
        else {
          if (iVar3 != 200) {
            if (iVar3 == 0x2f) {
              uVar7 = 0x1a72;
              uVar6 = 0x1a73;
            }
            else {
              uVar6 = 0xffff;
              uVar7 = 0xffff;
            }
            goto LAB_000ab452;
          }
          local_48 = 0x4970;
          uVar7 = 0x496c;
          iVar3 = 0x496e;
          iVar9 = 0x496d;
          uVar6 = 0x496f;
        }
        local_44 = 0x4a7f;
        goto LAB_000ab504;
      }
      uVar7 = 0x1a99;
      *(undefined1 *)&this[10].field_18 = 1;
      uVar6 = 0x1a9a;
    }
LAB_000ab452:
    iVar9 = -1;
    iVar3 = -1;
  }
  local_48 = -1;
  local_44 = -1;
LAB_000ab504:
  pAVar4 = operator_new(0xc0);
  puVar8 = *(uint **)(DAT_000ab774 + 0xab512);
  AEGeometry::AEGeometry(pAVar4,uVar7,(PaintCanvas *)*puVar8,false);
  this[6].field_4 = (int *)pAVar4;
  pAVar4 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar4,uVar6,(PaintCanvas *)*puVar8,false);
  this[1].field_4 = (int *)pAVar4;
  AEGeometry::setRotationOrder(pAVar4,2);
  pAVar4 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar4,(PaintCanvas *)*puVar8);
  this[1].field_8 = (int *)pAVar4;
  if (iVar9 != -1) {
    pAVar4 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar4,(ushort)iVar9,(PaintCanvas *)*puVar8,false);
    AEGeometry::addChild((uint)this[6].field_4);
    pvVar5 = (void *)AEGeometry::~AEGeometry(pAVar4);
    operator_delete(pvVar5);
  }
  if (iVar3 != -1) {
    pAVar4 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar4,(ushort)iVar3,(PaintCanvas *)*puVar8,false);
    AEGeometry::addChild((uint)this[1].field_4);
    pvVar5 = (void *)AEGeometry::~AEGeometry(pAVar4);
    operator_delete(pvVar5);
  }
  if (local_48 != -1) {
    pAVar4 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar4,(ushort)local_48,(PaintCanvas *)*puVar8,false);
    AEGeometry::addChild((uint)this[1].field_4);
    pvVar5 = (void *)AEGeometry::~AEGeometry(pAVar4);
    operator_delete(pvVar5);
  }
  if (local_44 != -1) {
    this_01 = operator_new(0xc0);
    AEGeometry::AEGeometry((AEGeometry *)this_01,(ushort)local_44,(PaintCanvas *)*puVar8,false);
    this[1].m_pLevel = this_01;
    AEGeometry::addChild((uint)this[1].field_4);
    this_02 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
    AbyssEngine::Transform::SetVisible(this_02,*(bool *)&this[0xb].field_14);
  }
  iVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
  *(undefined4 *)(iVar3 + 0xe0) = DAT_000ab76c;
  uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
  AbyssEngine::Transform::SetAnimationState(uVar2,2,0);
  AEGeometry::setPosition((AEGeometry *)this[6].field_4,(Vector *)&this[0xf].field_8);
  AEGeometry::setPosition((AEGeometry *)this[1].field_4,(Vector *)&this[0xf].field_8);
  AEGeometry::translate(extraout_s0,extraout_s1,extraout_s2);
  iVar3 = Item::getIndex((Item *)**(undefined4 **)(iVar1 + 4));
  fVar10 = extraout_s0_00;
  fVar11 = extraout_s1_00;
  fVar12 = extraout_s2_00;
  if ((iVar3 < 0xc6) ||
     (iVar1 = Item::getIndex((Item *)**(undefined4 **)(iVar1 + 4)), fVar10 = extraout_s0_01,
     fVar11 = extraout_s1_01, fVar12 = extraout_s2_01, 200 < iVar1)) {
    AEGeometry::rotate(fVar10,fVar11,fVar12);
    AEGeometry::rotate(extraout_s0_02,extraout_s1_02,extraout_s2_02);
  }
  AEGeometry::addChild((uint)this[1].field_8);
  AEGeometry::addChild((uint)this[1].field_8);
  if ((this[0xc].m_pPlayer != 0) && (*(char *)&this[10].field_8 != '\0')) {
    AbyssEngine::PaintCanvas::MeshCloneMaterial
              ((PaintCanvas *)*puVar8,this[6].field_4[7],(uint *)&this[0x19].field_8);
    (*(code *)(DAT_001aba04 + 0x1aba08))(*puVar8,this[1].field_4[7],&this[0x19].m_pLevel);
    return;
  }
  return;
}

```

## target disasm
```
  000ab340: push {r4,r5,r6,r7,lr}
  000ab342: add r7,sp,#0xc
  000ab344: push {r7,r8,r9,r10,r11}
  000ab348: vpush {d6,d7,d8,d9,d10}
  000ab34c: mov r4,r0
  000ab34e: ldrb.w r0,[r0,#0x170]
  000ab352: cmp r0,#0x0
  000ab354: bne.w 0x000ab6f4
  000ab358: ldr r0,[r4,#0x0]
  000ab35a: movs r1,#0x2
  000ab35c: blx 0x00072520
  000ab360: cmp r0,#0x0
  000ab362: strb.w r0,[r4,#0x170]
  000ab366: beq.w 0x000ab6f4
  000ab36a: ldr r0,[0x000ab724]
  000ab36c: movs r1,#0x0
  000ab36e: strb.w r1,[r4,#0x180]
  000ab372: add r0,pc
  000ab374: ldr r0,[r0,#0x0]
  000ab376: ldr r0,[r0,#0x0]
  000ab378: blx 0x00071a58
  000ab37c: movs r1,#0x2
  000ab37e: blx 0x00072118
  000ab382: mov r5,r0
  000ab384: ldr r0,[r0,#0x4]
  000ab386: movs r1,#0x11
  000ab388: ldr r0,[r0,#0x0]
  000ab38a: blx 0x000718e4
  000ab38e: vmov s0,r0
  000ab392: vmov.f64 d17,0x3ff8000000000000
  000ab396: vcvt.f64.s32 d16,s0
  000ab39a: vmul.f64 d16,d16,d17
  000ab39e: vcvt.s32.f64 s0,d16
  000ab3a2: vstr.32 s0,[r4,#0x1f8]
  000ab3a6: ldr r0,[r5,#0x4]
  000ab3a8: ldr r0,[r0,#0x0]
  000ab3aa: blx 0x000718d8
  000ab3ae: vldr.32 s16,[pc,#0x390]
  000ab3b2: cmp r0,#0xe0
  000ab3b4: str r5,[sp,#0x4]
  000ab3b6: beq 0x000ab3f2
  000ab3b8: cmp r0,#0x30
  000ab3ba: beq 0x000ab410
  000ab3bc: cmp r0,#0x31
  000ab3be: beq 0x000ab422
  000ab3c0: cmp r0,#0xb4
  000ab3c2: beq 0x000ab434
  000ab3c4: cmp r0,#0xb5
  000ab3c6: beq 0x000ab460
  000ab3c8: cmp r0,#0xb6
  000ab3ca: beq 0x000ab47c
  000ab3cc: cmp r0,#0xc6
  000ab3ce: beq 0x000ab49c
  000ab3d0: cmp r0,#0xc7
  000ab3d2: beq 0x000ab4be
  000ab3d4: cmp r0,#0xc8
  000ab3d6: beq.w 0x000ab4e0
  000ab3da: cmp r0,#0x2f
  000ab3dc: bne.w 0x000ab700
  000ab3e0: vldr.32 s18,[pc,#0x380]
  000ab3e4: movw r9,#0x1a72
  000ab3e8: mov.w r11,#0xffffffff
  000ab3ec: movw r5,#0x1a73
  000ab3f0: b 0x000ab44a
  000ab3f2: vmov.f32 s20,s16
  000ab3f6: vldr.32 s18,[pc,#0x34c]
  000ab3fa: mov.w r0,#0xffffffff
  000ab3fe: movw r9,#0x499a
  000ab402: movw r8,#0x499d
  000ab406: movw r11,#0x499c
  000ab40a: movw r5,#0x499b
  000ab40e: b 0x000ab456
  000ab410: movw r9,#0x1a74
  000ab414: vldr.32 s18,[pc,#0x350]
  000ab418: mov.w r11,#0xffffffff
  000ab41c: movw r5,#0x1a75
  000ab420: b 0x000ab44a
  000ab422: movw r9,#0x1a76
  000ab426: vldr.32 s18,[pc,#0x33c]
  000ab42a: mov.w r11,#0xffffffff
  000ab42e: movw r5,#0x1a77
  000ab432: b 0x000ab44a
  000ab434: movs r0,#0x1
  000ab436: vldr.32 s18,[pc,#0x328]
  000ab43a: strb.w r0,[r4,#0x180]
  000ab43e: movw r9,#0x1a95
  000ab442: mov.w r11,#0xffffffff
  000ab446: movw r5,#0x1a96
  000ab44a: vmov.f32 s20,s16
  000ab44e: mov.w r8,#0xffffffff
  000ab452: mov.w r0,#0xffffffff
  000ab456: str r0,[sp,#0x8]
  000ab458: mov.w r0,#0xffffffff
  000ab45c: str r0,[sp,#0xc]
  000ab45e: b 0x000ab504
  000ab460: movs r0,#0x1
  000ab462: vldr.32 s18,[pc,#0x2f4]
  000ab466: vmov.f32 s20,0xc1300000
  000ab46a: strb.w r0,[r4,#0x180]
  000ab46e: movw r9,#0x1a97
  000ab472: mov.w r11,#0xffffffff
  000ab476: movw r5,#0x1a98
  000ab47a: b 0x000ab44e
  000ab47c: movs r0,#0x1
  000ab47e: vldr.32 s16,[pc,#0x2d4]
  000ab482: vldr.32 s18,[pc,#0x2d4]
  000ab486: movw r9,#0x1a99
  000ab48a: vldr.32 s20,[pc,#0x2d0]
  000ab48e: mov.w r11,#0xffffffff
  000ab492: strb.w r0,[r4,#0x180]
  000ab496: movw r5,#0x1a9a
  000ab49a: b 0x000ab44e
  000ab49c: movw r0,#0x4a7f
  000ab4a0: vldr.32 s18,[pc,#0x2ac]
  000ab4a4: str r0,[sp,#0xc]
  000ab4a6: mov.w r0,#0xffffffff
  000ab4aa: str r0,[sp,#0x8]
  000ab4ac: movw r9,#0x4963
  000ab4b0: movw r8,#0x4966
  000ab4b4: movw r11,#0x4964
  000ab4b8: movw r5,#0x4967
  000ab4bc: b 0x000ab500
  000ab4be: movw r0,#0x4a7f
  000ab4c2: vldr.32 s18,[pc,#0x288]
  000ab4c6: str r0,[sp,#0xc]
  000ab4c8: mov.w r0,#0xffffffff
  000ab4cc: str r0,[sp,#0x8]
  000ab4ce: movw r9,#0x4968
  000ab4d2: movw r8,#0x496a
  000ab4d6: movw r11,#0x4969
  000ab4da: movw r5,#0x496b
  000ab4de: b 0x000ab500
  000ab4e0: movw r0,#0x4a7f
  000ab4e4: vldr.32 s18,[pc,#0x260]
  000ab4e8: str r0,[sp,#0xc]
  000ab4ea: movw r0,#0x4970
  000ab4ee: str r0,[sp,#0x8]
  000ab4f0: movw r9,#0x496c
  000ab4f4: movw r8,#0x496e
  000ab4f8: movw r11,#0x496d
  000ab4fc: movw r5,#0x496f
  000ab500: vmov.f32 s20,s16
  000ab504: movs r0,#0xc0
  000ab506: blx 0x0006eb24
  000ab50a: mov r6,r0
  000ab50c: ldr r0,[0x000ab774]
  000ab50e: add r0,pc
  000ab510: ldr.w r10,[r0,#0x0]
  000ab514: ldr.w r2,[r10,#0x0]
  000ab518: mov r0,r6
  000ab51a: mov r1,r9
  000ab51c: movs r3,#0x0
  000ab51e: blx 0x0007207c
  000ab522: movs r0,#0xc0
  000ab524: str.w r6,[r4,#0xdc]
  000ab528: blx 0x0006eb24
  000ab52c: mov r6,r0
  000ab52e: ldr.w r2,[r10,#0x0]
  000ab532: mov r1,r5
  000ab534: movs r3,#0x0
  000ab536: blx 0x0007207c
  000ab53a: mov r0,r6
  000ab53c: movs r1,#0x2
  000ab53e: str r6,[r4,#0x28]
  000ab540: blx 0x00072100
  000ab544: movs r0,#0xc0
  000ab546: blx 0x0006eb24
  000ab54a: ldr.w r1,[r10,#0x0]
  000ab54e: mov r5,r0
  000ab550: blx 0x000720f4
  000ab554: adds.w r0,r11,#0x1
  000ab558: str r5,[r4,#0x2c]
  000ab55a: beq 0x000ab586
  000ab55c: movs r0,#0xc0
  000ab55e: blx 0x0006eb24
  000ab562: mov r5,r0
  000ab564: ldr.w r2,[r10,#0x0]
  000ab568: uxth.w r1,r11
  000ab56c: movs r3,#0x0
  000ab56e: blx 0x0007207c
  000ab572: ldr r1,[r5,#0xc]
  000ab574: ldr.w r0,[r4,#0xdc]
  000ab578: blx 0x000720e8
  000ab57c: mov r0,r5
  000ab57e: blx 0x00071fc8
  000ab582: blx 0x0006eb48
  000ab586: adds.w r0,r8,#0x1
  000ab58a: beq 0x000ab5b4
  000ab58c: movs r0,#0xc0
  000ab58e: blx 0x0006eb24
  000ab592: mov r5,r0
  000ab594: ldr.w r2,[r10,#0x0]
  000ab598: uxth.w r1,r8
  000ab59c: movs r3,#0x0
  000ab59e: blx 0x0007207c
  000ab5a2: ldr r1,[r5,#0xc]
  000ab5a4: ldr r0,[r4,#0x28]
  000ab5a6: blx 0x000720e8
  000ab5aa: mov r0,r5
  000ab5ac: blx 0x00071fc8
  000ab5b0: blx 0x0006eb48
  000ab5b4: ldr r6,[sp,#0x8]
  000ab5b6: ldr.w r8,[sp,#0xc]
  000ab5ba: adds r0,r6,#0x1
  000ab5bc: beq 0x000ab5e4
  000ab5be: movs r0,#0xc0
  000ab5c0: blx 0x0006eb24
  000ab5c4: mov r5,r0
  000ab5c6: ldr.w r2,[r10,#0x0]
  000ab5ca: uxth r1,r6
  000ab5cc: movs r3,#0x0
  000ab5ce: blx 0x0007207c
  000ab5d2: ldr r1,[r5,#0xc]
  000ab5d4: ldr r0,[r4,#0x28]
  000ab5d6: blx 0x000720e8
  000ab5da: mov r0,r5
  000ab5dc: blx 0x00071fc8
  000ab5e0: blx 0x0006eb48
  000ab5e4: adds.w r0,r8,#0x1
  000ab5e8: beq 0x000ab61e
  000ab5ea: movs r0,#0xc0
  000ab5ec: blx 0x0006eb24
  000ab5f0: mov r5,r0
  000ab5f2: ldr.w r2,[r10,#0x0]
  000ab5f6: uxth.w r1,r8
  000ab5fa: movs r3,#0x0
  000ab5fc: blx 0x0007207c
  000ab600: ldr r0,[r4,#0x28]
  000ab602: str r5,[r4,#0x30]
  000ab604: ldr r1,[r5,#0xc]
  000ab606: blx 0x000720e8
  000ab60a: ldr r1,[r4,#0x30]
  000ab60c: ldr.w r0,[r10,#0x0]
  000ab610: ldr r1,[r1,#0xc]
  000ab612: blx 0x00072088
  000ab616: ldrb.w r1,[r4,#0x1a0]
  000ab61a: blx 0x0007249c
  000ab61e: ldr r0,[r4,#0x2c]
  000ab620: ldr r1,[r0,#0xc]
  000ab622: ldr.w r0,[r10,#0x0]
  000ab626: blx 0x00072088
  000ab62a: ldr r1,[0x000ab76c]
  000ab62c: str.w r1,[r0,#0xe0]
  000ab630: ldr r1,[r4,#0x28]
  000ab632: ldr.w r0,[r10,#0x0]
  000ab636: ldr r1,[r1,#0xc]
  000ab638: blx 0x00072088
  000ab63c: movs r1,#0x2
  000ab63e: movs r2,#0x0
  000ab640: blx 0x0006fd18
  000ab644: add.w r5,r4,#0x224
  000ab648: ldr.w r0,[r4,#0xdc]
  000ab64c: mov r1,r5
  000ab64e: blx 0x00072148
  000ab652: ldr r0,[r4,#0x28]
  000ab654: mov r1,r5
  000ab656: blx 0x00072148
  000ab65a: vmov r1,s16
  000ab65e: ldr r0,[r4,#0x28]
  000ab660: vmov r2,s18
  000ab664: vmov r3,s20
  000ab668: blx 0x00072154
  000ab66c: ldr r5,[sp,#0x4]
  000ab66e: ldr r0,[r5,#0x4]
  000ab670: ldr r0,[r0,#0x0]
  000ab672: blx 0x000718d8
  000ab676: cmp r0,#0xc6
  000ab678: blt 0x000ab686
  000ab67a: ldr r0,[r5,#0x4]
  000ab67c: ldr r0,[r0,#0x0]
  000ab67e: blx 0x000718d8
  000ab682: cmp r0,#0xc9
  000ab684: blt 0x000ab6a2
  000ab686: ldr r5,[0x000ab770]
  000ab688: movs r1,#0x0
  000ab68a: ldr.w r0,[r4,#0xdc]
  000ab68e: movs r3,#0x0
  000ab690: mov r2,r5
  000ab692: blx 0x00072160
  000ab696: ldr r0,[r4,#0x28]
  000ab698: movs r1,#0x0
  000ab69a: mov r2,r5
  000ab69c: movs r3,#0x0
  000ab69e: blx 0x00072160
  000ab6a2: ldr.w r1,[r4,#0xdc]
  000ab6a6: ldr r0,[r4,#0x2c]
  000ab6a8: ldr r1,[r1,#0xc]
  000ab6aa: blx 0x000720e8
  000ab6ae: ldrd r1,r0,[r4,#0x28]
  000ab6b2: ldr r1,[r1,#0xc]
  000ab6b4: blx 0x000720e8
  000ab6b8: ldr.w r0,[r4,#0x1b0]
  000ab6bc: cbz r0,0x000ab6f4
  000ab6be: ldrb.w r0,[r4,#0x170]
  000ab6c2: cbz r0,0x000ab6f4
  000ab6c4: ldr.w r0,[r4,#0xdc]
  000ab6c8: add.w r2,r4,#0x38c
  000ab6cc: ldr r1,[r0,#0x1c]
  000ab6ce: ldr.w r0,[r10,#0x0]
  000ab6d2: blx 0x000723b8
  000ab6d6: ldr r1,[r4,#0x28]
  000ab6d8: add.w r2,r4,#0x390
  000ab6dc: ldr.w r0,[r10,#0x0]
  000ab6e0: ldr r1,[r1,#0x1c]
  000ab6e2: vpop {d6,d7,d8,d9,d10}
  000ab6e6: add sp,#0x4
  000ab6e8: pop.w {r8,r9,r10,r11}
  000ab6ec: pop.w {r4,r5,r6,r7,lr}
  000ab6f0: b.w 0x001ab9f8
  000ab6f4: vpop {d6,d7,d8,d9,d10}
  000ab6f8: add sp,#0x4
  000ab6fa: pop.w {r8,r9,r10,r11}
  000ab6fe: pop {r4,r5,r6,r7,pc}
  000ab700: mov.w r11,#0xffffffff
  000ab704: movw r5,#0xffff
  000ab708: vmov.f32 s20,s16
  000ab70c: mov.w r8,#0xffffffff
  000ab710: vmov.f32 s18,s16
  000ab714: movw r9,#0xffff
  000ab718: b 0x000ab452
  001ab9f8: bx pc
```
