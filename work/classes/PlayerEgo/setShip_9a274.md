# PlayerEgo::setShip
elf 0x9a274 body 554
Sig: undefined __thiscall setShip(PlayerEgo * this, int param_1, int param_2)

## decompile
```c

/* PlayerEgo::setShip(int, int) */

void __thiscall PlayerEgo::setShip(PlayerEgo *this,int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  AEGeometry *pAVar3;
  Item *pIVar4;
  TractorBeam *this_00;
  Array<RepairBeam*> *this_01;
  RepairBeam *this_02;
  Ship *this_03;
  int iVar5;
  uint *puVar6;
  uint uVar7;
  int *piVar8;
  
  piVar8 = *(int **)(DAT_000aa4c0 + 0xaa286);
  piVar1 = (int *)Globals::getShipGroup((Globals *)*piVar8,param_1,param_2,true);
  iVar2 = DAT_000aa4c4;
  this->field_4 = piVar1;
  puVar6 = *(uint **)(iVar2 + 0xaa29a);
  iVar2 = AbyssEngine::PaintCanvas::MeshGetPointer((PaintCanvas *)*puVar6,piVar1[7]);
  this[0x19].field_10 = *(int **)(*(int *)(iVar2 + 0x30) + 0x20);
  pAVar3 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar3,(PaintCanvas *)*puVar6);
  this->field_8 = (int *)pAVar3;
  AEGeometry::addChild((uint)pAVar3);
  iVar2 = Status::getShip();
  iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
  if (iVar2 != 0) {
    iVar2 = Status::getShip();
    pIVar4 = (Item *)Ship::getFirstEquipmentOfSort(iVar2);
    iVar2 = Item::getIndex(pIVar4);
    iVar5 = 3;
    if (iVar2 < 0x48) {
      iVar5 = iVar2 + -0x44;
    }
    this_00 = operator_new(0x1c);
    TractorBeam::TractorBeam(this_00,(AEGeometry *)this->field_8,iVar5);
    this[0xc].field_4 = (int *)this_00;
    Globals::addSoundResourceToList(*piVar8);
    Globals::addSoundResourceToList(*piVar8);
  }
  uVar7 = 0;
  do {
    if (1 < uVar7) {
      iVar2 = Status::getShip();
      iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
      if (iVar2 != 0) {
        this_03 = (Ship *)Status::getShip();
        iVar2 = Ship::hasEmergencySystem(this_03);
        if (iVar2 != 0) {
          pAVar3 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar3,0x3826,(PaintCanvas *)*puVar6,false);
          this[4].field_1C = (int)pAVar3;
          iVar2 = Status::getShip();
          iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
          iVar2 = Item::getAttribute(iVar2);
          this[0x15].field_1C = iVar2;
          iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
          AbyssEngine::AEMath::Vector::operator=
                    ((Vector *)&this[0x15].field_20,(Vector *)(iVar2 + 0xd4));
          iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
          this[0x16].field_8 = (int *)(*(float *)(iVar2 + 0xe0) / DAT_000aa4b8 + DAT_000aa4bc);
        }
      }
      iVar2 = Status::inSupernovaSystem();
      if ((iVar2 != 0) || (iVar2 = Status::inSupernovaOrbit(), iVar2 != 0)) {
        iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
        this[1].field_18 = (int *)(*(float *)(iVar2 + 0xe0) * 1.75);
      }
      if (this[0xc].m_pPlayer != 0) {
        (*(code *)(DAT_001aba04 + 0x1aba08))(*puVar6,this->field_4[7],&this[0x19].field_4,puVar6);
        return;
      }
      return;
    }
    iVar2 = Status::getShip();
    pIVar4 = (Item *)Ship::getFirstEquipmentOfSort(iVar2);
    if (pIVar4 != (Item *)0x0) {
      if (this[0xc].field_8 == (int *)0x0) {
        this_01 = operator_new(0xc);
        Array<RepairBeam*>::Array(this_01);
        this[0xc].field_8 = (int *)this_01;
      }
      this_02 = operator_new(0x24);
      iVar2 = Item::getIndex(pIVar4);
      iVar5 = Item::getSort(pIVar4);
      RepairBeam::RepairBeam(this_02,iVar2,iVar5);
      iVar2 = Item::getIndex(pIVar4);
      if (iVar2 == 0xde) {
        iVar2 = *piVar8;
LAB_000aa3a6:
        Globals::addSoundResourceToList(iVar2);
      }
      else {
        iVar2 = Item::getIndex(pIVar4);
        if (iVar2 == 0xdf) {
          iVar2 = *piVar8;
          goto LAB_000aa3a6;
        }
      }
      ArrayAdd<RepairBeam*>(this_02,(Array *)this[0xc].field_8);
    }
    uVar7 = uVar7 + 1;
  } while( true );
}

```

## target disasm
```
  000aa274: push {r4,r5,r6,r7,lr}
  000aa276: add r7,sp,#0xc
  000aa278: push {r7,r8,r9,r10,r11}
  000aa27c: mov r8,r0
  000aa27e: ldr r0,[0x000aa4c0]
  000aa280: movs r3,#0x1
  000aa282: add r0,pc
  000aa284: ldr.w r11,[r0,#0x0]
  000aa288: ldr.w r0,[r11,#0x0]
  000aa28c: blx 0x000721d8
  000aa290: ldr r1,[0x000aa4c4]
  000aa292: str.w r0,[r8,#0x4]
  000aa296: add r1,pc
  000aa298: ldr r6,[r1,#0x0]
  000aa29a: ldr r1,[r0,#0x1c]
  000aa29c: ldr r0,[r6,#0x0]
  000aa29e: blx 0x00072370
  000aa2a2: ldr r0,[r0,#0x30]
  000aa2a4: ldr r0,[r0,#0x20]
  000aa2a6: str.w r0,[r8,#0x394]
  000aa2aa: movs r0,#0xc0
  000aa2ac: blx 0x0006eb24
  000aa2b0: ldr r1,[r6,#0x0]
  000aa2b2: mov r5,r0
  000aa2b4: blx 0x000720f4
  000aa2b8: ldr.w r0,[r8,#0x4]
  000aa2bc: str.w r5,[r8,#0x8]
  000aa2c0: ldr r1,[r0,#0xc]
  000aa2c2: mov r0,r5
  000aa2c4: blx 0x000720e8
  000aa2c8: ldr r0,[0x000aa4c8]
  000aa2ca: add r0,pc
  000aa2cc: ldr.w r10,[r0,#0x0]
  000aa2d0: ldr.w r0,[r10,#0x0]
  000aa2d4: blx 0x00071a58
  000aa2d8: movs r1,#0xd
  000aa2da: blx 0x0007228c
  000aa2de: cbz r0,0x000aa326
  000aa2e0: ldr.w r0,[r10,#0x0]
  000aa2e4: blx 0x00071a58
  000aa2e8: movs r1,#0xd
  000aa2ea: blx 0x0007228c
  000aa2ee: blx 0x000718d8
  000aa2f2: movs r4,#0x3
  000aa2f4: cmp r0,#0x48
  000aa2f6: it lt
  000aa2f8: sub.lt.w r4,r0,#0x44
  000aa2fc: movs r0,#0x1c
  000aa2fe: blx 0x0006eb24
  000aa302: ldr.w r1,[r8,#0x8]
  000aa306: mov r5,r0
  000aa308: mov r2,r4
  000aa30a: blx 0x0007237c
  000aa30e: str.w r5,[r8,#0x1b4]
  000aa312: movs r1,#0x0
  000aa314: ldr.w r0,[r11,#0x0]
  000aa318: blx 0x00072298
  000aa31c: ldr.w r0,[r11,#0x0]
  000aa320: movs r1,#0x4
  000aa322: blx 0x00072298
  000aa326: mov.w r9,#0x0
  000aa32a: str r6,[sp,#0x0]
  000aa32c: b 0x000aa3b8
  000aa32e: ldr.w r0,[r10,#0x0]
  000aa332: movs r4,#0x29
  000aa334: cmp.w r9,#0x0
  000aa338: it eq
  000aa33a: mov.eq r4,#0x25
  000aa33c: blx 0x00071a58
  000aa340: mov r1,r4
  000aa342: blx 0x0007228c
  000aa346: mov r5,r0
  000aa348: cbz r0,0x000aa3b4
  000aa34a: ldr.w r0,[r8,#0x1b8]
  000aa34e: cbnz r0,0x000aa360
  000aa350: movs r0,#0xc
  000aa352: blx 0x0006eb24
  000aa356: mov r6,r0
  000aa358: blx 0x00072388
  000aa35c: str.w r6,[r8,#0x1b8]
  000aa360: movs r0,#0x24
  000aa362: blx 0x0006eb24
  000aa366: mov r6,r0
  000aa368: mov r0,r5
  000aa36a: blx 0x000718d8
  000aa36e: mov r4,r0
  000aa370: mov r0,r5
  000aa372: blx 0x00071908
  000aa376: mov r2,r0
  000aa378: mov r0,r6
  000aa37a: mov r1,r4
  000aa37c: blx 0x00072394
  000aa380: mov r0,r5
  000aa382: blx 0x000718d8
  000aa386: cmp r0,#0xde
  000aa388: bne 0x000aa394
  000aa38a: ldr.w r0,[r11,#0x0]
  000aa38e: movw r1,#0x8db
  000aa392: b 0x000aa3a6
  000aa394: mov r0,r5
  000aa396: blx 0x000718d8
  000aa39a: cmp r0,#0xdf
  000aa39c: bne 0x000aa3aa
  000aa39e: ldr.w r0,[r11,#0x0]
  000aa3a2: movw r1,#0x8dc
  000aa3a6: blx 0x00072298
  000aa3aa: ldr.w r1,[r8,#0x1b8]
  000aa3ae: mov r0,r6
  000aa3b0: blx 0x000723a0
  000aa3b4: add.w r9,r9,#0x1
  000aa3b8: cmp.w r9,#0x2
  000aa3bc: bcc 0x000aa32e
  000aa3be: ldr.w r0,[r10,#0x0]
  000aa3c2: blx 0x00071a58
  000aa3c6: movs r1,#0x1b
  000aa3c8: blx 0x0007228c
  000aa3cc: ldr r4,[sp,#0x0]
  000aa3ce: cbz r0,0x000aa44a
  000aa3d0: ldr.w r0,[r10,#0x0]
  000aa3d4: blx 0x00071a58
  000aa3d8: blx 0x000723ac
  000aa3dc: cbz r0,0x000aa44a
  000aa3de: movs r0,#0xc0
  000aa3e0: blx 0x0006eb24
  000aa3e4: mov r5,r0
  000aa3e6: ldr r2,[r4,#0x0]
  000aa3e8: movw r1,#0x3826
  000aa3ec: movs r3,#0x0
  000aa3ee: blx 0x0007207c
  000aa3f2: str.w r5,[r8,#0xac]
  000aa3f6: ldr.w r0,[r10,#0x0]
  000aa3fa: blx 0x00071a58
  000aa3fe: movs r1,#0x1b
  000aa400: blx 0x0007228c
  000aa404: movs r1,#0x29
  000aa406: blx 0x000718e4
  000aa40a: ldr.w r1,[r8,#0x4]
  000aa40e: str.w r0,[r8,#0x310]
  000aa412: ldr r0,[r4,#0x0]
  000aa414: ldr r1,[r1,#0xc]
  000aa416: blx 0x00072088
  000aa41a: add.w r1,r0,#0xd4
  000aa41e: add.w r0,r8,#0x314
  000aa422: blx 0x0006eb3c
  000aa426: ldr.w r1,[r8,#0x4]
  000aa42a: ldr r0,[r4,#0x0]
  000aa42c: ldr r1,[r1,#0xc]
  000aa42e: blx 0x00072088
  000aa432: vldr.32 s0,[pc,#0x84]
  000aa436: vldr.32 s2,[r0,#0xe0]
  000aa43a: vdiv.f32 s0,s2,s0
  000aa43e: vldr.32 s2,[pc,#0x7c]
  000aa442: vadd.f32 s0,s0,s2
  000aa446: vstr.32 s0,[r8,#0x320]
  000aa44a: ldr.w r0,[r10,#0x0]
  000aa44e: blx 0x00071878
  000aa452: cbnz r0,0x000aa45e
  000aa454: ldr.w r0,[r10,#0x0]
  000aa458: blx 0x000722f8
  000aa45c: cbz r0,0x000aa47a
  000aa45e: ldr.w r0,[r8,#0x4]
  000aa462: ldr r1,[r0,#0xc]
  000aa464: ldr r0,[r4,#0x0]
  000aa466: blx 0x00072088
  000aa46a: vmov.f32 s0,0x3fe00000
  000aa46e: vldr.32 s2,[r0,#0xe0]
  000aa472: vmul.f32 s0,s2,s0
  000aa476: vstr.32 s0,[r8,#0x3c]
  000aa47a: ldr.w r0,[r8,#0x1b0]
  000aa47e: cbz r0,0x000aa498
  000aa480: ldr.w r0,[r8,#0x4]
  000aa484: add.w r2,r8,#0x388
  000aa488: ldr r1,[r0,#0x1c]
  000aa48a: ldr r0,[r4,#0x0]
  000aa48c: pop.w {r3,r8,r9,r10,r11}
  000aa490: pop.w {r4,r5,r6,r7,lr}
  000aa494: b.w 0x001ab9f8
  000aa498: pop.w {r3,r8,r9,r10,r11}
  000aa49c: pop {r4,r5,r6,r7,pc}
```
