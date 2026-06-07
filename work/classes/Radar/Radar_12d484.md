# Radar::Radar
elf 0x12d484 body 2308
Sig: undefined __thiscall Radar(Radar * this, Level * param_1)

## decompile
```c

/* Radar::Radar(Level*) */

void __thiscall Radar::Radar(Radar *this,Level *param_1)

{
  Radar RVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  Array *pAVar5;
  String *pSVar6;
  String *pSVar7;
  Station *this_00;
  ushort *puVar8;
  uint *puVar9;
  PlayerEgo *pPVar10;
  Sprite *pSVar11;
  void *pvVar12;
  Array<KIPlayer*> *this_01;
  int iVar13;
  int iVar14;
  int *piVar15;
  int iVar16;
  String *pSVar17;
  int *piVar18;
  int *piVar19;
  BombGun *this_02;
  MineGun *this_03;
  uint uVar20;
  uint *puVar21;
  undefined4 *puVar22;
  uint in_fpscr;
  float fVar23;
  float fVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  undefined4 uVar27;
  String aSStack_6c [12];
  uint local_60 [3];
  String aSStack_54 [12];
  uint local_48 [3];
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  uVar4 = 0;
  uVar25 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar26 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar27 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar2 = *(int **)(DAT_0013d810 + 0x13d4a0);
  local_3c = *piVar2;
  *(undefined4 *)(this + 0x174) = 0;
  *(undefined4 *)(this + 0x178) = uVar25;
  *(undefined4 *)(this + 0x17c) = uVar26;
  *(undefined4 *)(this + 0x180) = uVar27;
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x168) = uVar25;
  *(undefined4 *)(this + 0x16c) = uVar26;
  *(undefined4 *)(this + 0x170) = uVar27;
  *(undefined4 *)(this + 0x154) = 0;
  *(undefined4 *)(this + 0x158) = uVar25;
  *(undefined4 *)(this + 0x15c) = uVar26;
  *(undefined4 *)(this + 0x160) = uVar27;
  AbyssEngine::String::String((String *)(this + 0x18c));
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x1d0));
  iVar13 = DAT_0013d814;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  this[0x48] = (Radar)0x1;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x188) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x20c) = 0;
  *(undefined4 *)(this + 0x1b4) = 0;
  *(undefined4 *)(this + 0x1b8) = 0;
  *(undefined4 *)(this + 0x1bc) = 0;
  *(undefined4 *)(this + 0x28) = uVar4;
  *(undefined4 *)(this + 0x2c) = uVar25;
  *(undefined4 *)(this + 0x30) = uVar26;
  *(undefined4 *)(this + 0x34) = uVar27;
  *(undefined4 *)(this + 0x1c) = uVar4;
  *(undefined4 *)(this + 0x20) = uVar25;
  *(undefined4 *)(this + 0x24) = uVar26;
  *(undefined4 *)(this + 0x28) = uVar27;
  *(undefined4 *)(this + 0x144) = uVar4;
  *(undefined4 *)(this + 0x148) = uVar25;
  *(undefined4 *)(this + 0x14c) = uVar26;
  *(undefined4 *)(this + 0x150) = uVar27;
  *(undefined4 *)(this + 0x134) = uVar4;
  *(undefined4 *)(this + 0x138) = uVar25;
  *(undefined4 *)(this + 0x13c) = uVar26;
  *(undefined4 *)(this + 0x140) = uVar27;
  piVar15 = *(int **)(iVar13 + 0x13d4e4);
  *(undefined4 *)(this + 0x198) = uVar4;
  *(undefined4 *)(this + 0x19c) = uVar25;
  *(undefined4 *)(this + 0x1a0) = uVar26;
  *(undefined4 *)(this + 0x1a4) = uVar27;
  this[0x54] = (Radar)0x0;
  this[0x130] = (Radar)0x0;
  this[0x120] = (Radar)0x0;
  *(undefined4 *)(this + 0x11c) = 0;
  this[0x1a8] = (Radar)0x0;
  *(Level **)this = param_1;
  iVar3 = *piVar15;
  iVar13 = *(int *)(iVar3 + 0xac);
  *(int *)(this + 0x21c) = iVar13;
  *(int *)(this + 0x220) = iVar13 >> 1;
  iVar13 = DAT_0013d818;
  iVar14 = *(int *)(iVar3 + 0xa8);
  *(int *)(this + 0x224) = iVar14;
  *(int *)(this + 0x228) = iVar14 >> 1;
  *(undefined4 *)(this + 0x22c) = *(undefined4 *)(iVar3 + 0xa0);
  puVar21 = *(uint **)(iVar13 + 0x13d568);
  *(undefined4 *)(this + 0x230) = *(undefined4 *)(iVar3 + 0xa4);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4c7,(uint *)(this + 0x1c4));
  uVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar21);
  *(undefined4 *)(this + 0x4c) = uVar4;
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar21);
  iVar13 = DAT_0013d820;
  fVar23 = (float)VectorSignedToFloat(iVar3 * iVar3,(byte)(in_fpscr >> 0x16) & 3);
  iVar16 = *(int *)(this + 0x4c) * *(int *)(this + 0x4c);
  fVar24 = (float)VectorSignedToFloat(iVar16,(byte)(in_fpscr >> 0x16) & 3);
  piVar18 = *(int **)(DAT_0013d81c + 0x13d5ca);
  *(int *)(this + 0x50) = iVar3;
  iVar14 = *piVar18;
  iVar13 = **(int **)(iVar13 + 0x13d5d6);
  *(int *)(this + 0x114) = iVar16;
  *(int *)(this + 0x118) = iVar3 * iVar3;
  *(int *)(this + 0x104) = iVar14 / 2;
  *(int *)(this + 0x108) = iVar13 / 2;
  *(float *)(this + 0x10c) = 1.0 / fVar24;
  *(float *)(this + 0x110) = 1.0 / fVar23;
  pAVar5 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  *(Array **)(this + 0x188) = pAVar5;
  ArraySetLength<AbyssEngine::String*>(4,pAVar5);
  pSVar6 = operator_new(0xc);
  puVar22 = *(undefined4 **)(DAT_0013d824 + 0x13d61e);
  iVar13 = Status::inAlienOrbit();
  if (iVar13 == 0) {
    Status::getStation();
    Station::getName();
    AbyssEngine::String::String((String *)local_48,aSStack_54,false);
  }
  else {
    pSVar7 = (String *)GameText::getText(**(int **)(DAT_0013d828 + 0x13d630));
    AbyssEngine::String::String((String *)local_48,pSVar7,false);
  }
  this_00 = (Station *)Status::getStation();
  iVar3 = Station::getIndex(this_00);
  if (iVar3 != 0x65) {
    AbyssEngine::String::String(aSStack_6c,(char *)(DAT_0013d830 + 0x13d68e),false);
    GameText::getText(**(int **)(DAT_0013d834 + 0x13d69a));
    AbyssEngine::operator+((String *)local_60,aSStack_6c);
  }
  else {
    AbyssEngine::String::String((String *)local_60,(char *)(DAT_0013d82c + 0x13d680),false);
  }
  AbyssEngine::operator+(pSVar6,(String *)local_48);
  **(undefined4 **)(*(int *)(this + 0x188) + 4) = pSVar6;
  AbyssEngine::String::~String((String *)local_60);
  if (iVar3 != 0x65) {
    AbyssEngine::String::~String(aSStack_6c);
  }
  AbyssEngine::String::~String((String *)local_48);
  if (iVar13 == 0) {
    AbyssEngine::String::~String(aSStack_54);
  }
  iVar13 = Status::inAlienOrbit();
  if (((iVar13 != 0) && (iVar13 = Status::dlc1Won(), iVar13 != 0)) &&
     (iVar13 = Status::inEmptyOrbit((Status *)*puVar22), iVar13 == 0)) {
    pSVar17 = (String *)**(undefined4 **)(*(int *)(this + 0x188) + 4);
    iVar13 = Level::getLandmarks();
    puVar8 = AbyssEngine::String::operator_cast_to_unsigned_short_
                       ((String *)(**(int **)(iVar13 + 4) + 0x18));
    AbyssEngine::String::Set(pSVar17,puVar8);
  }
  pSVar17 = operator_new(0xc);
  piVar19 = *(int **)(DAT_0013d838 + 0x13d72e);
  pSVar6 = (String *)GameText::getText(*piVar19);
  AbyssEngine::String::String(pSVar17,pSVar6,false);
  *(String **)(*(int *)(*(int *)(this + 0x188) + 4) + 4) = pSVar17;
  pSVar17 = operator_new(0xc);
  AbyssEngine::String::String(pSVar17,(char *)(DAT_0013d83c + 0x13d758),false);
  *(String **)(*(int *)(*(int *)(this + 0x188) + 4) + 8) = pSVar17;
  pSVar17 = operator_new(0xc);
  pSVar6 = (String *)GameText::getText(*piVar19);
  AbyssEngine::String::String(pSVar17,pSVar6,false);
  *(String **)(*(int *)(*(int *)(this + 0x188) + 4) + 0xc) = pSVar17;
  puVar9 = (uint *)Level::getPlayerGuns(param_1);
  if (puVar9 != (uint *)0x0) {
    for (uVar20 = 0; uVar20 < *puVar9; uVar20 = uVar20 + 1) {
      iVar13 = (**(code **)(**(int **)(puVar9[1] + uVar20 * 4) + 0x20))();
      if (iVar13 != 0) {
        RocketGun::setRadar(*(Radar **)(puVar9[1] + uVar20 * 4));
      }
      iVar13 = (**(code **)(**(int **)(puVar9[1] + uVar20 * 4) + 0x24))();
      if (iVar13 != 0) {
        this_02 = *(BombGun **)(puVar9[1] + uVar20 * 4);
        pPVar10 = (PlayerEgo *)Level::getPlayer();
        BombGun::setPlayer(this_02,pPVar10);
      }
      iVar13 = (**(code **)(**(int **)(puVar9[1] + uVar20 * 4) + 0x28))();
      if (iVar13 != 0) {
        this_03 = *(MineGun **)(puVar9[1] + uVar20 * 4);
        pPVar10 = (PlayerEgo *)Level::getPlayer();
        MineGun::setPlayer(this_03,pPVar10);
      }
    }
  }
  puVar9 = (uint *)Level::getEnemyGuns(param_1);
  if (puVar9 != (uint *)0x0) {
    for (uVar20 = 0; uVar20 < *puVar9; uVar20 = uVar20 + 1) {
      iVar13 = (**(code **)(**(int **)(puVar9[1] + uVar20 * 4) + 0x20))();
      if (iVar13 != 0) {
        RocketGun::setRadar(*(Radar **)(puVar9[1] + uVar20 * 4));
      }
    }
  }
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d4,(uint *)(this + 0xd4));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d9,(uint *)(this + 0xd0));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d6,(uint *)(this + 0xd8));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d7,(uint *)(this + 0xdc));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d3,(uint *)(this + 0xe4));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4da,(uint *)(this + 0xe0));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d5,(uint *)(this + 0xe8));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d8,(uint *)(this + 0xec));
  uVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar21);
  *(undefined4 *)(this + 0x1c8) = uVar4;
  uVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar21);
  *(undefined4 *)(this + 0x1cc) = uVar4;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x454,(uint *)(this + 200));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x455,(uint *)(this + 0xc4));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4dc,(uint *)(this + 0x74));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4cb,(uint *)(this + 0x78));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4c9,(uint *)(this + 0x98));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4db,(uint *)(this + 0x5c));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4cc,(uint *)(this + 0x60));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4c8,(uint *)(this + 0x90));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4d2,(uint *)(this + 100));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4cd,(uint *)(this + 0x68));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4ca,(uint *)(this + 0x94));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4f0,(uint *)(this + 0x6c));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4ef,(uint *)(this + 0x70));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4f2,(uint *)(this + 0x88));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4f1,(uint *)(this + 0x8c));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4c8,(uint *)(this + 0x7c));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4ca,(uint *)(this + 0x80));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4c9,(uint *)(this + 0x84));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4f2,(uint *)(this + 0x9c));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4f1,(uint *)(this + 0xa4));
  *(undefined4 *)(this + 0xa0) = *(undefined4 *)(this + 0xa4);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x451,(uint *)(this + 0xa8));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x44f,(uint *)(this + 0xb0));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x44c,(uint *)(this + 0xac));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x44d,(uint *)(this + 0xb4));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x450,(uint *)(this + 0xb8));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x453,(uint *)(this + 0xbc));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x1f62,(uint *)(this + 0xc0));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4c4,(uint *)(this + 0xcc));
  iVar3 = *piVar18;
  iVar13 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar21);
  *(int *)(this + 0x210) = iVar3 / 2 - iVar13 / 2;
  *(undefined4 *)(this + 0x214) = *(undefined4 *)(*piVar15 + 0xb0);
  puVar9 = operator_new__(0x28);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4a1,puVar9);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x49c,puVar9 + 1);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x49e,puVar9 + 3);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x49f,puVar9 + 2);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x4a0,puVar9 + 8);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x49d,puVar9 + 9);
  pSVar11 = operator_new(0x40);
  iVar13 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar21);
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar21);
  Sprite::Sprite(pSVar11,puVar9,10,iVar13,iVar3);
  *(Sprite **)(this + 0xf0) = pSVar11;
  local_48[0] = 0;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x456,local_48);
  iVar13 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar21);
  pSVar11 = operator_new(0x40);
  Sprite::Sprite(pSVar11,local_48[0],iVar13,iVar13);
  *(Sprite **)(this + 0xf4) = pSVar11;
  Sprite::defineReferencePixel(pSVar11,iVar13 / 2,iVar13 / 2);
  local_60[0] = 0;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar21,0x44e,local_60);
  pSVar11 = operator_new(0x40);
  Sprite::Sprite(pSVar11,local_60[0],*(int *)(*piVar15 + 0xb8),*(int *)(*piVar15 + 0xb4));
  *(Sprite **)(this + 0xf8) = pSVar11;
  iVar13 = Status::getShip();
  iVar13 = Ship::getFirstEquipmentOfSort(iVar13);
  iVar3 = Status::getShip();
  iVar3 = Ship::getFirstEquipmentOfSort(iVar3);
  iVar14 = Status::getShip();
  iVar14 = Ship::getFirstEquipmentOfSort(iVar14);
  this[0x1ac] = (Radar)(iVar13 != 0);
  if (iVar3 == 0) {
    this[0x1ab] = (Radar)0x0;
    *(undefined2 *)(this + 0x1a9) = 0;
    uVar4 = 8000;
  }
  else {
    this[0x1ab] = (Radar)0x1;
    iVar13 = Item::getAttribute(iVar3);
    this[0x1a9] = (Radar)(iVar13 == 1);
    iVar13 = Item::getAttribute(iVar3);
    this[0x1aa] = (Radar)(iVar13 == 1);
    uVar4 = Item::getAttribute(iVar3);
  }
  *(undefined4 *)(this + 0x1b8) = uVar4;
  if (iVar14 == 0) {
    RVar1 = (Radar)0x0;
    *(undefined4 *)(this + 0x1b4) = 0;
    *(undefined2 *)(this + 0x1ad) = 0;
  }
  else {
    this[0x1ad] = (Radar)0x1;
    iVar13 = Item::getAttribute(iVar14);
    this[0x1ae] = (Radar)(iVar13 == 1);
    uVar4 = Item::getAttribute(iVar14);
    *(undefined4 *)(this + 0x1b4) = uVar4;
    iVar13 = Item::getAttribute(iVar14);
    RVar1 = (Radar)(iVar13 == 2);
  }
  this[0x1af] = RVar1;
  *(undefined4 *)(this + 0x1bc) = 0;
  this[0x1b0] = (Radar)0x1;
  this[0x48] = (Radar)0x1;
  pvVar12 = operator_new__(0x14);
  *(void **)(this + 0x58) = pvVar12;
  for (iVar13 = 0; iVar13 != 5; iVar13 = iVar13 + 1) {
    *(undefined4 *)((int)pvVar12 + iVar13 * 4) = 0xffffffff;
  }
  iVar13 = *piVar18;
  uVar4 = __aeabi_idiv(iVar13,6);
  *(int *)(this + 0x124) = (int)(iVar13 + ((uint)(iVar13 >> 0x1f) >> 0x1c)) >> 4;
  *(undefined4 *)(this + 0x128) = uVar4;
  *(int *)(this + 300) = (int)(iVar13 + ((uint)(iVar13 >> 0x1f) >> 0x1d)) >> 3;
  iVar13 = Status::getShip();
  iVar13 = Ship::getFirstEquipmentOfSort(iVar13);
  if ((iVar13 != 0) && (iVar13 = Item::getAttribute(iVar13), iVar13 != 0)) {
    iVar13 = Status::getShip();
    iVar13 = Ship::getFirstEquipmentOfSort(iVar13);
    uVar4 = Item::getAttribute(iVar13);
    fVar23 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    fVar24 = (float)VectorSignedToFloat(*(undefined4 *)(this + 300),(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 300) = (int)((fVar23 / DAT_0013de38) * fVar24);
  }
  iVar13 = Status::getShip();
  iVar13 = Ship::getFirstEquipmentOfSort(iVar13);
  if (iVar13 != 0) {
    this_01 = operator_new(0xc);
    Array<KIPlayer*>::Array(this_01);
    *(Array<KIPlayer*> **)(this + 0x34) = this_01;
    uVar20 = Item::getAttribute(iVar13);
    ArraySetLength<KIPlayer*>(uVar20,*(Array **)(this + 0x34));
  }
  iVar13 = *piVar2 - local_3c;
  if (iVar13 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar13);
  }
  return;
}

```

## target disasm
```
  0013d484: push {r4,r5,r6,r7,lr}
  0013d486: add r7,sp,#0xc
  0013d488: push {r7,r8,r9,r10,r11}
  0013d48c: vpush {d8,d9}
  0013d490: sub sp,#0x48
  0013d492: mov r5,r0
  0013d494: ldr r0,[0x0013d810]
  0013d496: vmov.i32 q4,#0x0
  0013d49a: mov r9,r1
  0013d49c: add r0,pc
  0013d49e: ldr r0,[r0,#0x0]
  0013d4a0: str r0,[sp,#0x4]
  0013d4a2: ldr r0,[r0,#0x0]
  0013d4a4: str r0,[sp,#0x44]
  0013d4a6: add.w r0,r5,#0x174
  0013d4aa: vst1.32 {d8,d9},[r0]
  0013d4ae: add.w r0,r5,#0x164
  0013d4b2: vst1.32 {d8,d9},[r0]
  0013d4b6: add.w r0,r5,#0x154
  0013d4ba: vst1.32 {d8,d9},[r0]
  0013d4be: add.w r0,r5,#0x18c
  0013d4c2: str r0,[sp,#0x10]
  0013d4c4: blx 0x0006efbc
  0013d4c8: add.w r0,r5,#0x1d0
  0013d4cc: blx 0x0006f118
  0013d4d0: movs r0,#0x0
  0013d4d2: movs r2,#0x1
  0013d4d4: ldr r1,[0x0013d814]
  0013d4d6: add.w r4,r5,#0x1c4
  0013d4da: strd r0,r0,[r5,#0x4]
  0013d4de: str r0,[r5,#0x14]
  0013d4e0: add r1,pc
  0013d4e2: strb.w r2,[r5,#0x48]
  0013d4e6: add.w r2,r5,#0x28
  0013d4ea: str r0,[r5,#0x58]
  0013d4ec: str.w r0,[r5,#0x188]
  0013d4f0: strd r0,r0,[r5,#0xc]
  0013d4f4: str r0,[r5,#0x18]
  0013d4f6: strd r0,r0,[r5,#0x38]
  0013d4fa: str r0,[r5,#0x40]
  0013d4fc: str.w r0,[r5,#0x20c]
  0013d500: strd r0,r0,[r5,#0x1b4]
  0013d504: str.w r0,[r5,#0x1bc]
  0013d508: vst1.32 {d8,d9},[r2]
  0013d50c: add.w r2,r5,#0x1c
  0013d510: vst1.32 {d8,d9},[r2]
  0013d514: add.w r2,r5,#0x144
  0013d518: vst1.32 {d8,d9},[r2]
  0013d51c: add.w r2,r5,#0x134
  0013d520: vst1.32 {d8,d9},[r2]
  0013d524: ldr r2,[r1,#0x0]
  0013d526: add.w r1,r5,#0x198
  0013d52a: str r2,[sp,#0xc]
  0013d52c: vst1.32 {d8,d9},[r1]
  0013d530: strb.w r0,[r5,#0x54]
  0013d534: strb.w r0,[r5,#0x130]
  0013d538: strb.w r0,[r5,#0x120]
  0013d53c: str.w r0,[r5,#0x11c]
  0013d540: strb.w r0,[r5,#0x1a8]
  0013d544: str.w r9,[r5,#0x0]
  0013d548: ldr r0,[r2,#0x0]
  0013d54a: ldr.w r1,[r0,#0xac]
  0013d54e: str.w r1,[r5,#0x21c]
  0013d552: asrs r1,r1,#0x1
  0013d554: str.w r1,[r5,#0x220]
  0013d558: ldr.w r1,[r0,#0xa8]
  0013d55c: ldr r2,[0x0013d818]
  0013d55e: str.w r1,[r5,#0x224]
  0013d562: asrs r1,r1,#0x1
  0013d564: add r2,pc
  0013d566: str.w r1,[r5,#0x228]
  0013d56a: ldr.w r1,[r0,#0xa0]
  0013d56e: str.w r1,[r5,#0x22c]
  0013d572: ldr.w r10,[r2,#0x0]
  0013d576: ldr.w r0,[r0,#0xa4]
  0013d57a: str.w r0,[r5,#0x230]
  0013d57e: ldr.w r0,[r10,#0x0]
  0013d582: movw r1,#0x4c7
  0013d586: mov r2,r4
  0013d588: blx 0x00071cf8
  0013d58c: ldr r1,[r4,#0x0]
  0013d58e: ldr.w r0,[r10,#0x0]
  0013d592: blx 0x00072d84
  0013d596: ldr.w r1,[r5,#0x1c4]
  0013d59a: str r0,[r5,#0x4c]
  0013d59c: ldr.w r0,[r10,#0x0]
  0013d5a0: blx 0x00072d90
  0013d5a4: mul r1,r0,r0
  0013d5a8: vmov.f32 s0,0x3f800000
  0013d5ac: vmov s2,r1
  0013d5b0: vcvt.f32.s32 s2,s2
  0013d5b4: ldr r2,[r5,#0x4c]
  0013d5b6: muls r2,r2
  0013d5b8: vdiv.f32 s2,s0,s2
  0013d5bc: vmov s4,r2
  0013d5c0: vcvt.f32.s32 s4,s4
  0013d5c4: ldr r3,[0x0013d81c]
  0013d5c6: add r3,pc
  0013d5c8: ldr r6,[r3,#0x0]
  0013d5ca: ldr r3,[0x0013d820]
  0013d5cc: str r0,[r5,#0x50]
  0013d5ce: vdiv.f32 s0,s0,s4
  0013d5d2: add r3,pc
  0013d5d4: ldr r0,[r6,#0x0]
  0013d5d6: ldr r3,[r3,#0x0]
  0013d5d8: str r6,[sp,#0x8]
  0013d5da: add.w r0,r0,r0, lsr #0x1f
  0013d5de: ldr r3,[r3,#0x0]
  0013d5e0: strd r2,r1,[r5,#0x114]
  0013d5e4: asrs r0,r0,#0x1
  0013d5e6: add.w r1,r3,r3, lsr #0x1f
  0013d5ea: asrs r1,r1,#0x1
  0013d5ec: strd r0,r1,[r5,#0x104]
  0013d5f0: vstr.32 s0,[r5,#0x10c]
  0013d5f4: vstr.32 s2,[r5,#0x110]
  0013d5f8: movs r0,#0xc
  0013d5fa: blx 0x0006eb24
  0013d5fe: mov r4,r0
  0013d600: blx 0x0006f628
  0013d604: str.w r4,[r5,#0x188]
  0013d608: movs r0,#0x4
  0013d60a: mov r1,r4
  0013d60c: blx 0x0006fe08
  0013d610: movs r0,#0xc
  0013d612: blx 0x0006eb24
  0013d616: mov r4,r0
  0013d618: ldr r0,[0x0013d824]
  0013d61a: add r0,pc
  0013d61c: ldr.w r11,[r0,#0x0]
  0013d620: ldr.w r0,[r11,#0x0]
  0013d624: blx 0x000723d0
  0013d628: cbz r0,0x0013d64a
  0013d62a: ldr r0,[0x0013d828]
  0013d62c: add r0,pc
  0013d62e: ldr r0,[r0,#0x0]
  0013d630: ldr r0,[r0,#0x0]
  0013d632: movw r1,#0x19f
  0013d636: blx 0x00072f70
  0013d63a: mov r1,r0
  0013d63c: mov.w r8,#0x0
  0013d640: add r0,sp,#0x38
  0013d642: movs r2,#0x0
  0013d644: blx 0x0006f028
  0013d648: b 0x0013d668
  0013d64a: ldr.w r0,[r11,#0x0]
  0013d64e: blx 0x00071c14
  0013d652: mov r1,r0
  0013d654: add r0,sp,#0x2c
  0013d656: blx 0x000736a8
  0013d65a: add r0,sp,#0x38
  0013d65c: add r1,sp,#0x2c
  0013d65e: movs r2,#0x0
  0013d660: blx 0x0006f028
  0013d664: mov.w r8,#0x1
  0013d668: ldr.w r0,[r11,#0x0]
  0013d66c: blx 0x00071c14
  0013d670: blx 0x00071824
  0013d674: cmp r0,#0x65
  0013d676: bne 0x0013d688
  0013d678: ldr r1,[0x0013d82c]
  0013d67a: movs r6,#0x0
  0013d67c: add r1,pc
  0013d67e: add r0,sp,#0x20
  0013d680: movs r2,#0x0
  0013d682: blx 0x0006ee18
  0013d686: b 0x0013d6ae
  0013d688: ldr r1,[0x0013d830]
  0013d68a: add r1,pc
  0013d68c: add r0,sp,#0x14
  0013d68e: movs r2,#0x0
  0013d690: blx 0x0006ee18
  0013d694: ldr r0,[0x0013d834]
  0013d696: add r0,pc
  0013d698: ldr r0,[r0,#0x0]
  0013d69a: ldr r0,[r0,#0x0]
  0013d69c: movs r1,#0x88
  0013d69e: blx 0x00072f70
  0013d6a2: mov r2,r0
  0013d6a4: add r0,sp,#0x20
  0013d6a6: add r1,sp,#0x14
  0013d6a8: blx 0x0006ef98
  0013d6ac: movs r6,#0x1
  0013d6ae: add r1,sp,#0x38
  0013d6b0: add r2,sp,#0x20
  0013d6b2: mov r0,r4
  0013d6b4: blx 0x0006ef98
  0013d6b8: ldr.w r0,[r5,#0x188]
  0013d6bc: ldr r0,[r0,#0x4]
  0013d6be: str r4,[r0,#0x0]
  0013d6c0: add r0,sp,#0x20
  0013d6c2: blx 0x0006ee30
  0013d6c6: cbz r6,0x0013d6ce
  0013d6c8: add r0,sp,#0x14
  0013d6ca: blx 0x0006ee30
  0013d6ce: add r0,sp,#0x38
  0013d6d0: blx 0x0006ee30
  0013d6d4: cmp.w r8,#0x0
  0013d6d8: itt ne
  0013d6da: add.ne.w r0,sp,#0x2c
  0013d6de: blx.ne 0x0006ee30
  0013d6e2: ldr.w r0,[r11,#0x0]
  0013d6e6: blx 0x000723d0
  0013d6ea: cbz r0,0x0013d720
  0013d6ec: ldr.w r0,[r11,#0x0]
  0013d6f0: blx 0x000719a4
  0013d6f4: cbz r0,0x0013d720
  0013d6f6: ldr.w r0,[r11,#0x0]
  0013d6fa: blx 0x00073d20
  0013d6fe: cbnz r0,0x0013d720
  0013d700: ldr.w r0,[r5,#0x188]
  0013d704: ldr r0,[r0,#0x4]
  0013d706: ldr r4,[r0,#0x0]
  0013d708: mov r0,r9
  0013d70a: blx 0x000725b0
  0013d70e: ldr r0,[r0,#0x4]
  0013d710: ldr r0,[r0,#0x0]
  0013d712: adds r0,#0x18
  0013d714: blx 0x0006f3ac
  0013d718: mov r1,r0
  0013d71a: mov r0,r4
  0013d71c: blx 0x0006f3f4
  0013d720: movs r0,#0xc
  0013d722: blx 0x0006eb24
  0013d726: mov r4,r0
  0013d728: ldr r0,[0x0013d838]
  0013d72a: add r0,pc
  0013d72c: ldr r6,[r0,#0x0]
  0013d72e: ldr r0,[r6,#0x0]
  0013d730: movw r1,#0x223
  0013d734: blx 0x00072f70
  0013d738: mov r1,r0
  0013d73a: mov r0,r4
  0013d73c: movs r2,#0x0
  0013d73e: blx 0x0006f028
  0013d742: ldr.w r0,[r5,#0x188]
  0013d746: ldr r0,[r0,#0x4]
  0013d748: str r4,[r0,#0x4]
  0013d74a: movs r0,#0xc
  0013d74c: blx 0x0006eb24
  0013d750: ldr r1,[0x0013d83c]
  0013d752: mov r4,r0
  0013d754: add r1,pc
  0013d756: movs r2,#0x0
  0013d758: blx 0x0006ee18
  0013d75c: ldr.w r0,[r5,#0x188]
  0013d760: ldr r0,[r0,#0x4]
  0013d762: str r4,[r0,#0x8]
  0013d764: movs r0,#0xc
  0013d766: blx 0x0006eb24
  0013d76a: mov r4,r0
  0013d76c: ldr r0,[r6,#0x0]
  0013d76e: movw r1,#0x221
  0013d772: blx 0x00072f70
  0013d776: mov r1,r0
  0013d778: mov r0,r4
  0013d77a: movs r2,#0x0
  0013d77c: blx 0x0006f028
  0013d780: ldr.w r0,[r5,#0x188]
  0013d784: ldr r0,[r0,#0x4]
  0013d786: str r4,[r0,#0xc]
  0013d788: mov r0,r9
  0013d78a: blx 0x00076738
  0013d78e: mov r4,r0
  0013d790: cbz r0,0x0013d800
  0013d792: mov.w r8,#0x0
  0013d796: b 0x0013d7fa
  0013d798: ldr r0,[r4,#0x4]
  0013d79a: ldr.w r0,[r0,r8,lsl #0x2]
  0013d79e: ldr r1,[r0,#0x0]
  0013d7a0: ldr r1,[r1,#0x20]
  0013d7a2: blx r1
  0013d7a4: cbz r0,0x0013d7b2
  0013d7a6: ldr r0,[r4,#0x4]
  0013d7a8: ldr.w r0,[r0,r8,lsl #0x2]
  0013d7ac: mov r1,r5
  0013d7ae: blx 0x00076744
  0013d7b2: ldr r0,[r4,#0x4]
  0013d7b4: ldr.w r0,[r0,r8,lsl #0x2]
  0013d7b8: ldr r1,[r0,#0x0]
  0013d7ba: ldr r1,[r1,#0x24]
  0013d7bc: blx r1
  0013d7be: cbz r0,0x0013d7d4
  0013d7c0: ldr r0,[r4,#0x4]
  0013d7c2: ldr.w r6,[r0,r8,lsl #0x2]
  0013d7c6: mov r0,r9
  0013d7c8: blx 0x00072034
  0013d7cc: mov r1,r0
  0013d7ce: mov r0,r6
  0013d7d0: blx 0x00076750
  0013d7d4: ldr r0,[r4,#0x4]
  0013d7d6: ldr.w r0,[r0,r8,lsl #0x2]
  0013d7da: ldr r1,[r0,#0x0]
  0013d7dc: ldr r1,[r1,#0x28]
  0013d7de: blx r1
  0013d7e0: cbz r0,0x0013d7f6
  0013d7e2: ldr r0,[r4,#0x4]
  0013d7e4: ldr.w r6,[r0,r8,lsl #0x2]
  0013d7e8: mov r0,r9
  0013d7ea: blx 0x00072034
  0013d7ee: mov r1,r0
  0013d7f0: mov r0,r6
  0013d7f2: blx 0x0007675c
  0013d7f6: add.w r8,r8,#0x1
  0013d7fa: ldr r0,[r4,#0x0]
  0013d7fc: cmp r8,r0
  0013d7fe: bcc 0x0013d798
  0013d800: mov r0,r9
  0013d802: blx 0x00076768
  0013d806: mov r4,r0
  0013d808: cbz r0,0x0013d862
  0013d80a: movs r6,#0x0
  0013d80c: b 0x0013d85c
  0013d840: ldr r0,[r4,#0x4]
  0013d842: ldr.w r0,[r0,r6,lsl #0x2]
  0013d846: ldr r1,[r0,#0x0]
  0013d848: ldr r1,[r1,#0x20]
  0013d84a: blx r1
  0013d84c: cbz r0,0x0013d85a
  0013d84e: ldr r0,[r4,#0x4]
  0013d850: ldr.w r0,[r0,r6,lsl #0x2]
  0013d854: mov r1,r5
  0013d856: blx 0x00076744
  0013d85a: adds r6,#0x1
  0013d85c: ldr r0,[r4,#0x0]
  0013d85e: cmp r6,r0
  0013d860: bcc 0x0013d840
  0013d862: ldr.w r0,[r10,#0x0]
  0013d866: add.w r4,r5,#0xd4
  0013d86a: movw r1,#0x4d4
  0013d86e: mov r2,r4
  0013d870: blx 0x00071cf8
  0013d874: ldr.w r0,[r10,#0x0]
  0013d878: add.w r2,r5,#0xd0
  0013d87c: movw r1,#0x4d9
  0013d880: blx 0x00071cf8
  0013d884: ldr.w r0,[r10,#0x0]
  0013d888: add.w r2,r5,#0xd8
  0013d88c: movw r1,#0x4d6
  0013d890: blx 0x00071cf8
  0013d894: ldr.w r0,[r10,#0x0]
  0013d898: add.w r2,r5,#0xdc
  0013d89c: movw r1,#0x4d7
  0013d8a0: blx 0x00071cf8
  0013d8a4: ldr.w r0,[r10,#0x0]
  0013d8a8: add.w r2,r5,#0xe4
  0013d8ac: movw r1,#0x4d3
  0013d8b0: blx 0x00071cf8
  0013d8b4: ldr.w r0,[r10,#0x0]
  0013d8b8: add.w r2,r5,#0xe0
  0013d8bc: movw r1,#0x4da
  0013d8c0: blx 0x00071cf8
  0013d8c4: ldr.w r0,[r10,#0x0]
  0013d8c8: add.w r2,r5,#0xe8
  0013d8cc: movw r1,#0x4d5
  0013d8d0: blx 0x00071cf8
  0013d8d4: ldr.w r0,[r10,#0x0]
  0013d8d8: add.w r2,r5,#0xec
  0013d8dc: mov.w r1,#0x4d8
  0013d8e0: blx 0x00071cf8
  0013d8e4: ldr r1,[r4,#0x0]
  0013d8e6: ldr.w r0,[r10,#0x0]
  0013d8ea: blx 0x00072d84
  0013d8ee: ldr.w r1,[r5,#0xd4]
  0013d8f2: str.w r0,[r5,#0x1c8]
  0013d8f6: ldr.w r0,[r10,#0x0]
  0013d8fa: blx 0x00072d90
  0013d8fe: str.w r0,[r5,#0x1cc]
  0013d902: ldr.w r0,[r10,#0x0]
  0013d906: add.w r2,r5,#0xc8
  0013d90a: movw r1,#0x454
  0013d90e: blx 0x00071cf8
  0013d912: ldr.w r0,[r10,#0x0]
  0013d916: add.w r2,r5,#0xc4
  0013d91a: movw r1,#0x455
  0013d91e: blx 0x00071cf8
  0013d922: ldr.w r0,[r10,#0x0]
  0013d926: add.w r2,r5,#0x74
  0013d92a: movw r1,#0x4dc
  0013d92e: blx 0x00071cf8
  0013d932: ldr.w r0,[r10,#0x0]
  0013d936: add.w r2,r5,#0x78
  0013d93a: movw r1,#0x4cb
  0013d93e: blx 0x00071cf8
  0013d942: ldr.w r0,[r10,#0x0]
  0013d946: add.w r2,r5,#0x98
  0013d94a: movw r1,#0x4c9
  0013d94e: blx 0x00071cf8
  0013d952: ldr.w r0,[r10,#0x0]
  0013d956: add.w r2,r5,#0x5c
  0013d95a: movw r1,#0x4db
  0013d95e: blx 0x00071cf8
  0013d962: ldr.w r0,[r10,#0x0]
  0013d966: add.w r2,r5,#0x60
  0013d96a: movw r1,#0x4cc
  0013d96e: blx 0x00071cf8
  0013d972: ldr.w r0,[r10,#0x0]
  0013d976: add.w r2,r5,#0x90
  0013d97a: mov.w r1,#0x4c8
  0013d97e: blx 0x00071cf8
  0013d982: ldr.w r0,[r10,#0x0]
  0013d986: add.w r2,r5,#0x64
  0013d98a: movw r1,#0x4d2
  0013d98e: blx 0x00071cf8
  0013d992: ldr.w r0,[r10,#0x0]
  0013d996: add.w r2,r5,#0x68
  0013d99a: movw r1,#0x4cd
  0013d99e: blx 0x00071cf8
  0013d9a2: ldr.w r0,[r10,#0x0]
  0013d9a6: add.w r2,r5,#0x94
  0013d9aa: movw r1,#0x4ca
  0013d9ae: blx 0x00071cf8
  0013d9b2: ldr.w r0,[r10,#0x0]
  0013d9b6: add.w r2,r5,#0x6c
  0013d9ba: mov.w r1,#0x4f0
  0013d9be: blx 0x00071cf8
  0013d9c2: ldr.w r0,[r10,#0x0]
  0013d9c6: add.w r2,r5,#0x70
  0013d9ca: movw r1,#0x4ef
  0013d9ce: blx 0x00071cf8
  0013d9d2: ldr.w r0,[r10,#0x0]
  0013d9d6: add.w r2,r5,#0x88
  0013d9da: movw r1,#0x4f2
  0013d9de: blx 0x00071cf8
  0013d9e2: ldr.w r0,[r10,#0x0]
  0013d9e6: add.w r2,r5,#0x8c
  0013d9ea: movw r1,#0x4f1
  0013d9ee: blx 0x00071cf8
  0013d9f2: ldr.w r0,[r10,#0x0]
  0013d9f6: add.w r2,r5,#0x7c
  0013d9fa: mov.w r1,#0x4c8
  0013d9fe: blx 0x00071cf8
  0013da02: ldr.w r0,[r10,#0x0]
  0013da06: add.w r2,r5,#0x80
  0013da0a: movw r1,#0x4ca
  0013da0e: blx 0x00071cf8
  0013da12: ldr.w r0,[r10,#0x0]
  0013da16: add.w r2,r5,#0x84
  0013da1a: movw r1,#0x4c9
  0013da1e: blx 0x00071cf8
  0013da22: ldr.w r0,[r10,#0x0]
  0013da26: add.w r2,r5,#0x9c
  0013da2a: movw r1,#0x4f2
  0013da2e: blx 0x00071cf8
  0013da32: ldr.w r0,[r10,#0x0]
  0013da36: add.w r2,r5,#0xa4
  0013da3a: movw r1,#0x4f1
  0013da3e: blx 0x00071cf8
  0013da42: ldr.w r0,[r5,#0xa4]
  0013da46: str.w r0,[r5,#0xa0]
  0013da4a: ldr.w r0,[r10,#0x0]
  0013da4e: add.w r2,r5,#0xa8
  0013da52: movw r1,#0x451
  0013da56: blx 0x00071cf8
  0013da5a: ldr.w r0,[r10,#0x0]
  0013da5e: add.w r2,r5,#0xb0
  0013da62: movw r1,#0x44f
  0013da66: blx 0x00071cf8
  0013da6a: ldr.w r0,[r10,#0x0]
  0013da6e: add.w r2,r5,#0xac
  0013da72: movw r1,#0x44c
  0013da76: blx 0x00071cf8
  0013da7a: ldr.w r0,[r10,#0x0]
  0013da7e: add.w r2,r5,#0xb4
  0013da82: movw r1,#0x44d
  0013da86: blx 0x00071cf8
  0013da8a: ldr.w r0,[r10,#0x0]
  0013da8e: add.w r2,r5,#0xb8
  0013da92: mov.w r1,#0x450
  0013da96: blx 0x00071cf8
  0013da9a: ldr.w r0,[r10,#0x0]
  0013da9e: add.w r2,r5,#0xbc
  0013daa2: movw r1,#0x453
  0013daa6: blx 0x00071cf8
  0013daaa: ldr.w r0,[r10,#0x0]
  0013daae: add.w r2,r5,#0xc0
  0013dab2: movw r1,#0x1f62
  0013dab6: blx 0x00071cf8
  0013daba: ldr.w r0,[r10,#0x0]
  0013dabe: add.w r4,r5,#0xcc
  0013dac2: movw r1,#0x4c4
  0013dac6: mov r2,r4
  0013dac8: blx 0x00071cf8
  0013dacc: ldr r0,[sp,#0x8]
  0013dace: ldr r1,[r4,#0x0]
  0013dad0: ldr r6,[r0,#0x0]
  0013dad2: ldr.w r0,[r10,#0x0]
  0013dad6: blx 0x00072d84
  0013dada: add.w r1,r6,r6, lsr #0x1f
  0013dade: add.w r0,r0,r0, lsr #0x1f
  0013dae2: asrs r1,r1,#0x1
  0013dae4: sub.w r0,r1,r0, asr #0x1
  0013dae8: str.w r0,[r5,#0x210]
  0013daec: ldr r0,[sp,#0xc]
  0013daee: ldr r0,[r0,#0x0]
  0013daf0: ldr.w r0,[r0,#0xb0]
  0013daf4: str.w r0,[r5,#0x214]
  0013daf8: movs r0,#0x28
  0013dafa: blx 0x0006ec08
  0013dafe: mov r4,r0
  0013db00: ldr.w r0,[r10,#0x0]
  0013db04: movw r1,#0x4a1
  0013db08: mov r2,r4
  0013db0a: blx 0x00071cf8
  0013db0e: ldr.w r0,[r10,#0x0]
  0013db12: adds r2,r4,#0x4
  0013db14: movw r1,#0x49c
  0013db18: blx 0x00071cf8
  0013db1c: ldr.w r0,[r10,#0x0]
  0013db20: add.w r2,r4,#0xc
  0013db24: movw r1,#0x49e
  0013db28: blx 0x00071cf8
  0013db2c: ldr.w r0,[r10,#0x0]
  0013db30: add.w r2,r4,#0x8
  0013db34: movw r1,#0x49f
  0013db38: blx 0x00071cf8
  0013db3c: ldr.w r0,[r10,#0x0]
  0013db40: add.w r2,r4,#0x20
  0013db44: mov.w r1,#0x4a0
  0013db48: blx 0x00071cf8
  0013db4c: ldr.w r0,[r10,#0x0]
  0013db50: add.w r2,r4,#0x24
  0013db54: movw r1,#0x49d
  0013db58: blx 0x00071cf8
  0013db5c: movs r0,#0x40
  0013db5e: blx 0x0006eb24
  0013db62: mov r6,r0
  0013db64: ldr r1,[r4,#0x0]
  0013db66: ldr.w r0,[r10,#0x0]
  0013db6a: blx 0x00072d90
  0013db6e: mov r9,r0
  0013db70: ldr r1,[r4,#0x0]
  0013db72: ldr.w r0,[r10,#0x0]
  0013db76: blx 0x00072d90
  0013db7a: str r0,[sp,#0x0]
  0013db7c: mov r0,r6
  0013db7e: mov r1,r4
  0013db80: movs r2,#0xa
  0013db82: mov r3,r9
  0013db84: blx 0x00076018
  0013db88: str.w r6,[r5,#0xf0]
  0013db8c: movs r1,#0x0
  0013db8e: ldr.w r0,[r10,#0x0]
  0013db92: str r1,[sp,#0x38]
  0013db94: add r2,sp,#0x38
  0013db96: movw r1,#0x456
  0013db9a: blx 0x00071cf8
  0013db9e: ldr r1,[sp,#0x38]
  0013dba0: ldr.w r0,[r10,#0x0]
  0013dba4: blx 0x00072d90
  0013dba8: mov r4,r0
  0013dbaa: movs r0,#0x40
  0013dbac: blx 0x0006eb24
  0013dbb0: ldr r1,[sp,#0x38]
  0013dbb2: mov r6,r0
  0013dbb4: mov r2,r4
  0013dbb6: mov r3,r4
  0013dbb8: blx 0x00076258
  0013dbbc: str.w r6,[r5,#0xf4]
  0013dbc0: add.w r0,r4,r4, lsr #0x1f
  0013dbc4: asrs r1,r0,#0x1
  0013dbc6: mov r0,r6
  0013dbc8: mov r2,r1
  0013dbca: blx 0x00076264
  0013dbce: ldr.w r0,[r10,#0x0]
  0013dbd2: movs r1,#0x0
  0013dbd4: str r1,[sp,#0x20]
  0013dbd6: add r2,sp,#0x20
  0013dbd8: movw r1,#0x44e
  0013dbdc: blx 0x00071cf8
  0013dbe0: movs r0,#0x40
  0013dbe2: blx 0x0006eb24
  0013dbe6: mov r4,r0
  0013dbe8: ldr r0,[sp,#0xc]
  0013dbea: ldr r1,[sp,#0x20]
  0013dbec: ldr r0,[r0,#0x0]
  0013dbee: ldrd r3,r2,[r0,#0xb4]
  0013dbf2: mov r0,r4
  0013dbf4: blx 0x00076258
  0013dbf8: str.w r4,[r5,#0xf8]
  0013dbfc: ldr.w r0,[r11,#0x0]
  0013dc00: blx 0x00071a58
  0013dc04: movs r1,#0x13
  0013dc06: blx 0x0007228c
  0013dc0a: mov r4,r0
  0013dc0c: ldr.w r0,[r11,#0x0]
  0013dc10: blx 0x00071a58
  0013dc14: movs r1,#0x11
  0013dc16: blx 0x0007228c
  0013dc1a: mov r6,r0
  0013dc1c: ldr.w r0,[r11,#0x0]
  0013dc20: blx 0x00071a58
  0013dc24: movs r1,#0xd
  0013dc26: blx 0x0007228c
  0013dc2a: cmp r4,#0x0
  0013dc2c: it ne
  0013dc2e: mov.ne r4,#0x1
  0013dc30: mov r9,r0
  0013dc32: cmp r6,#0x0
  0013dc34: strb.w r4,[r5,#0x1ac]
  0013dc38: beq 0x0013dc72
  0013dc3a: movs r0,#0x1
  0013dc3c: strb.w r0,[r5,#0x1ab]
  0013dc40: mov r0,r6
  0013dc42: movs r1,#0x1f
  0013dc44: blx 0x000718e4
  0013dc48: subs r0,#0x1
  0013dc4a: clz r0,r0
  0013dc4e: lsrs r0,r0,#0x5
  0013dc50: strb.w r0,[r5,#0x1a9]
  0013dc54: mov r0,r6
  0013dc56: movs r1,#0x1e
  0013dc58: blx 0x000718e4
  0013dc5c: subs r0,#0x1
  0013dc5e: clz r0,r0
  0013dc62: lsrs r0,r0,#0x5
  0013dc64: strb.w r0,[r5,#0x1aa]
  0013dc68: mov r0,r6
  0013dc6a: movs r1,#0x1d
  0013dc6c: blx 0x000718e4
  0013dc70: b 0x0013dc80
  0013dc72: movs r0,#0x0
  0013dc74: strb.w r0,[r5,#0x1ab]
  0013dc78: strh.w r0,[r5,#0x1a9]
  0013dc7c: mov.w r0,#0x1f40
  0013dc80: cmp.w r9,#0x0
  0013dc84: str.w r0,[r5,#0x1b8]
  0013dc88: beq 0x0013dcc2
  0013dc8a: movs r0,#0x1
  0013dc8c: strb.w r0,[r5,#0x1ad]
  0013dc90: mov r0,r9
  0013dc92: movs r1,#0x17
  0013dc94: blx 0x000718e4
  0013dc98: subs r0,#0x1
  0013dc9a: clz r0,r0
  0013dc9e: lsrs r0,r0,#0x5
  0013dca0: strb.w r0,[r5,#0x1ae]
  0013dca4: mov r0,r9
  0013dca6: movs r1,#0x18
  0013dca8: blx 0x000718e4
  0013dcac: str.w r0,[r5,#0x1b4]
  0013dcb0: mov r0,r9
  0013dcb2: movs r1,#0x17
  0013dcb4: blx 0x000718e4
  0013dcb8: subs r0,#0x2
  0013dcba: clz r0,r0
  0013dcbe: lsrs r0,r0,#0x5
  0013dcc0: b 0x0013dccc
  0013dcc2: movs r0,#0x0
  0013dcc4: str.w r0,[r5,#0x1b4]
  0013dcc8: strh.w r0,[r5,#0x1ad]
  0013dccc: strb.w r0,[r5,#0x1af]
  0013dcd0: movs r0,#0x0
  0013dcd2: str.w r0,[r5,#0x1bc]
  0013dcd6: movs r0,#0x1
  0013dcd8: strb.w r0,[r5,#0x1b0]
  0013dcdc: strb.w r0,[r5,#0x48]
  0013dce0: movs r0,#0x14
  0013dce2: blx 0x0006ec08
  0013dce6: movs r1,#0x0
  0013dce8: mov.w r2,#0xffffffff
  0013dcec: str r0,[r5,#0x58]
  0013dcee: b 0x0013dcf6
  0013dcf0: str.w r2,[r0,r1,lsl #0x2]
  0013dcf4: adds r1,#0x1
  0013dcf6: cmp r1,#0x5
  0013dcf8: bne 0x0013dcf0
  0013dcfa: ldr r0,[sp,#0x8]
  0013dcfc: movs r1,#0x6
  0013dcfe: ldr r4,[r0,#0x0]
  0013dd00: mov r0,r4
  0013dd02: blx 0x0007198c
  0013dd06: asrs r1,r4,#0x1f
  0013dd08: add.w r2,r4,r1, lsr #0x1c
  0013dd0c: add.w r1,r4,r1, lsr #0x1d
  0013dd10: asrs r2,r2,#0x4
  0013dd12: asrs r1,r1,#0x3
  0013dd14: strd r2,r0,[r5,#0x124]
  0013dd18: str.w r1,[r5,#0x12c]
  0013dd1c: ldr.w r0,[r11,#0x0]
  0013dd20: blx 0x00071a58
  0013dd24: movs r1,#0x23
  0013dd26: blx 0x0007228c
  0013dd2a: cbz r0,0x0013dd6c
  0013dd2c: movs r1,#0x32
  0013dd2e: blx 0x000718e4
  0013dd32: cbz r0,0x0013dd6c
  0013dd34: ldr.w r0,[r11,#0x0]
  0013dd38: blx 0x00071a58
  0013dd3c: movs r1,#0x23
  0013dd3e: blx 0x0007228c
  0013dd42: movs r1,#0x32
  0013dd44: blx 0x000718e4
  0013dd48: vmov s0,r0
  0013dd4c: vldr.32 s2,[pc,#0xe8]
  0013dd50: vcvt.f32.s32 s0,s0
  0013dd54: vdiv.f32 s0,s0,s2
  0013dd58: vldr.32 s2,[r5,#0x12c]
  0013dd5c: vcvt.f32.s32 s2,s2
  0013dd60: vmul.f32 s0,s0,s2
  0013dd64: vcvt.s32.f32 s0,s0
  0013dd68: vstr.32 s0,[r5,#0x12c]
  0013dd6c: ldr.w r0,[r11,#0x0]
  0013dd70: blx 0x00071a58
  0013dd74: movs r1,#0x25
  0013dd76: blx 0x0007228c
  0013dd7a: mov r4,r0
  0013dd7c: cbz r0,0x0013dd9a
  0013dd7e: movs r0,#0xc
  0013dd80: blx 0x0006eb24
  0013dd84: mov r6,r0
  0013dd86: blx 0x00073d08
  0013dd8a: str r6,[r5,#0x34]
  0013dd8c: mov r0,r4
  0013dd8e: movs r1,#0x37
  0013dd90: blx 0x000718e4
  0013dd94: ldr r1,[r5,#0x34]
  0013dd96: blx 0x00073d14
  0013dd9a: ldr r0,[sp,#0x44]
  0013dd9c: ldr r1,[sp,#0x4]
  0013dd9e: ldr r1,[r1,#0x0]
  0013dda0: subs r0,r1,r0
  0013dda2: itttt eq
  0013dda4: mov.eq r0,r5
  0013dda6: add.eq sp,#0x48
  0013dda8: vpop.eq {d8,d9}
  0013ddac: add.eq sp,#0x4
  0013ddae: itt eq
  0013ddb0: pop.eq.w {r8,r9,r10,r11}
  0013ddb4: pop.eq {r4,r5,r6,r7,pc}
  0013ddb6: blx 0x0006e824
```
