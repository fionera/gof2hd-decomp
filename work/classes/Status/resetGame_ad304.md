# Status::resetGame
elf 0xad304 body 1592
Sig: undefined __stdcall resetGame(void)

## decompile
```c

/* Status::resetGame() */

void Status::resetGame(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  Status *in_r0;
  int *piVar5;
  void *pvVar6;
  Station *pSVar7;
  Array *pAVar8;
  Array<bool> *pAVar9;
  Array<BluePrint*> *this;
  BluePrint *this_00;
  Standing *this_01;
  Mission *this_02;
  undefined4 uVar10;
  uint *puVar11;
  uint uVar12;
  undefined4 *puVar13;
  int *piVar14;
  int iVar15;
  uint uVar16;
  undefined4 *puVar17;
  undefined1 *puVar18;
  int iVar19;
  int iVar20;
  undefined4 *puVar21;
  int iVar22;
  code *pcVar23;
  uint uVar24;
  int iVar25;
  code *pcVar26;
  int iVar27;
  int *piVar28;
  
  piVar5 = (int *)(DAT_000bd694 + 0xbd31a);
  *(undefined4 *)(in_r0 + 0x8c) = 0;
  iVar27 = *piVar5;
  *(undefined4 *)(in_r0 + 0x1e4) = 0;
  *(undefined4 *)(in_r0 + 0x100) = 0;
  *(undefined4 *)(in_r0 + 0x104) = 0;
  *(undefined4 *)(in_r0 + 0x1b0) = 0;
  *(undefined4 *)(in_r0 + 0x1b4) = 0;
  *(undefined4 *)(in_r0 + 0x1b8) = 0;
  *(undefined4 *)(in_r0 + 0x1bc) = 0;
  *(undefined4 *)(in_r0 + 0x1c0) = 0;
  *(undefined4 *)(in_r0 + 0x1c4) = 0;
  *(undefined4 *)(in_r0 + 0x1c8) = 1;
  *(undefined4 *)(in_r0 + 0x1cc) = 0;
  *(undefined4 *)(in_r0 + 0x1d0) = 0;
  *(undefined4 *)(in_r0 + 0x10c) = 0;
  in_r0[0x110] = (Status)0x0;
  *(undefined4 *)(in_r0 + 0x118) = 0;
  *(undefined4 *)(in_r0 + 0x11c) = 0;
  *(undefined4 *)(in_r0 + 0x124) = 0;
  *(undefined4 *)(in_r0 + 300) = 0;
  *(undefined4 *)(in_r0 + 0x1d4) = 0;
  *(undefined4 *)(in_r0 + 0x1d8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(in_r0 + 0x1dc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(in_r0 + 0x1e0) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(in_r0 + 0x13c) = 0;
  *(undefined4 *)(in_r0 + 0x134) = 0;
  *(undefined4 *)(in_r0 + 0x144) = 0;
  in_r0[0x120] = (Status)0x0;
  in_r0[0x128] = (Status)0x0;
  in_r0[0x130] = (Status)0x0;
  in_r0[0x138] = (Status)0x0;
  in_r0[0x140] = (Status)0x0;
  in_r0[0x148] = (Status)0x0;
  cVar1 = *(char *)(iVar27 + 0x36);
  in_r0[0x111] = (Status)0x0;
  uVar10 = 0;
  if (cVar1 != '\0') {
    uVar10 = 3;
  }
  *(undefined4 *)(in_r0 + 0x114) = uVar10;
  if (*(Station **)(in_r0 + 0x14c) != (Station *)0x0) {
    pvVar6 = (void *)Station::~Station(*(Station **)(in_r0 + 0x14c));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x14c) = 0;
  }
  pSVar7 = operator_new(0x34);
  Station::Station(pSVar7);
  *(Station **)(in_r0 + 0x14c) = pSVar7;
  if (*(void **)(in_r0 + 0x28) != (void *)0x0) {
    operator_delete__(*(void **)(in_r0 + 0x28));
    *(undefined4 *)(in_r0 + 0x28) = 0;
  }
  if (*(Array **)(in_r0 + 0x24) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(in_r0 + 0x24));
    if (*(Array<AbyssEngine::String*> **)(in_r0 + 0x24) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar6 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(in_r0 + 0x24));
      operator_delete(pvVar6);
    }
  }
  piVar5 = *(int **)(in_r0 + 0x94);
  *(undefined4 *)(in_r0 + 0x30) = 0;
  *(undefined4 *)(in_r0 + 0x24) = 0;
  iVar15 = *piVar5;
  for (iVar20 = 0; iVar15 != iVar20; iVar20 = iVar20 + 1) {
    *(undefined1 *)(piVar5[1] + iVar20) = 0;
  }
  piVar5 = *(int **)(in_r0 + 0x98);
  iVar15 = *piVar5;
  for (iVar20 = 0; iVar15 != iVar20; iVar20 = iVar20 + 1) {
    *(undefined1 *)(piVar5[1] + iVar20) = 0;
  }
  piVar5 = *(int **)(in_r0 + 0xac);
  *(undefined4 *)(in_r0 + 0x9c) = 0;
  *(undefined4 *)(in_r0 + 0xa0) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(in_r0 + 0xa4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(in_r0 + 0xa8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar15 = *piVar5;
  for (iVar20 = 0; iVar15 != iVar20; iVar20 = iVar20 + 1) {
    *(undefined1 *)(piVar5[1] + iVar20) = 0;
  }
  piVar5 = *(int **)(in_r0 + 0xb4);
  *(undefined4 *)(in_r0 + 0xb0) = 0;
  iVar15 = *piVar5;
  for (iVar20 = 0; iVar15 != iVar20; iVar20 = iVar20 + 1) {
    *(undefined1 *)(piVar5[1] + iVar20) = 0;
  }
  puVar11 = *(uint **)(in_r0 + 0x1a0);
  if (puVar11 != (uint *)0x0) {
    for (uVar24 = 0; uVar24 < *puVar11; uVar24 = uVar24 + 1) {
      pSVar7 = *(Station **)(puVar11[1] + uVar24 * 4);
      if (pSVar7 != (Station *)0x0) {
        pvVar6 = (void *)Station::~Station(pSVar7);
        operator_delete(pvVar6);
        *(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x1a0) + 4) + uVar24 * 4) = 0;
        puVar11 = *(uint **)(in_r0 + 0x1a0);
      }
    }
  }
  Globals::resetHints();
  piVar28 = *(int **)(DAT_000bd69c + 0xbd490);
  Galaxy::reset((Galaxy *)*piVar28);
  piVar5 = *(int **)(in_r0 + 0x50);
  iVar15 = *piVar5;
  for (iVar20 = 0; iVar15 != iVar20; iVar20 = iVar20 + 1) {
    *(undefined1 *)(piVar5[1] + iVar20) = 0;
  }
  puVar11 = *(uint **)(in_r0 + 0x54);
  uVar24 = puVar11[1];
  for (iVar15 = 0; iVar15 != 0xb0; iVar15 = iVar15 + 1) {
    *(undefined1 *)(uVar24 + iVar15) = 1;
  }
  uVar12 = *puVar11;
  for (uVar16 = 0xb0; iVar20 = DAT_000bd6ac, iVar15 = DAT_000bd6a8, uVar16 < uVar12;
      uVar16 = uVar16 + 1) {
    *(undefined1 *)(uVar24 + uVar16) = 0;
  }
  uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar13 = (undefined4 *)(DAT_000bd6a0 + 0xbd4e4);
  puVar17 = (undefined4 *)(DAT_000bd6a4 + 0xbd4e8);
  *(undefined1 *)(uVar24 + 0x3e) = 0;
  *(undefined2 *)(uVar24 + 0x3c) = 0;
  *(undefined4 *)(in_r0 + 0x34) = 0;
  *(undefined4 *)(in_r0 + 0xb8) = 0;
  *(undefined4 *)(in_r0 + 0xe0) = 0;
  *(undefined4 *)(in_r0 + 0xe4) = uVar10;
  *(undefined4 *)(in_r0 + 0xe8) = uVar2;
  *(undefined4 *)(in_r0 + 0xec) = uVar3;
  puVar13 = (undefined4 *)*puVar13;
  puVar17 = (undefined4 *)*puVar17;
  *(undefined4 *)(in_r0 + 0xc0) = 0;
  *(undefined4 *)(in_r0 + 0xc4) = uVar10;
  *(undefined4 *)(in_r0 + 200) = uVar2;
  *(undefined4 *)(in_r0 + 0xcc) = uVar3;
  *(undefined4 *)(in_r0 + 0xd0) = 0;
  *(undefined4 *)(in_r0 + 0xd4) = uVar10;
  *(undefined4 *)(in_r0 + 0xd8) = uVar2;
  *(undefined4 *)(in_r0 + 0xdc) = uVar3;
  puVar18 = *(undefined1 **)(iVar15 + 0xbd4ee);
  puVar21 = *(undefined4 **)(iVar20 + 0xbd4fa);
  *puVar17 = 0;
  *puVar18 = 0;
  *puVar21 = 0;
  *(undefined4 *)(in_r0 + 0x160) = 0;
  *(undefined4 *)(in_r0 + 0x164) = 0;
  in_r0[0x108] = (Status)0x0;
  *(undefined4 *)(in_r0 + 0x174) = 0;
  *(undefined2 *)(in_r0 + 0xf0) = 0;
  *(undefined4 *)(in_r0 + 0xf4) = 0xffffffff;
  *(undefined2 *)(in_r0 + 0xf8) = 1;
  Achievements::init((Achievements *)*puVar13,(EVP_PKEY_CTX *)0x1);
  if (*(Array<int> **)(in_r0 + 0x40) != (Array<int> *)0x0) {
    pvVar6 = (void *)Array<int>::~Array(*(Array<int> **)(in_r0 + 0x40));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x40) = 0;
  }
  if (*(Array<int> **)(in_r0 + 0x3c) != (Array<int> *)0x0) {
    pvVar6 = (void *)Array<int>::~Array(*(Array<int> **)(in_r0 + 0x3c));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x3c) = 0;
  }
  if (*(Array<int> **)(in_r0 + 0x48) != (Array<int> *)0x0) {
    pvVar6 = (void *)Array<int>::~Array(*(Array<int> **)(in_r0 + 0x48));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x48) = 0;
  }
  if (*(Array<int> **)(in_r0 + 0x44) != (Array<int> *)0x0) {
    pvVar6 = (void *)Array<int>::~Array(*(Array<int> **)(in_r0 + 0x44));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x44) = 0;
  }
  pAVar8 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(in_r0 + 0x40) = pAVar8;
  ArraySetLength<int>(0xe9,pAVar8);
  pAVar8 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(in_r0 + 0x3c) = pAVar8;
  ArraySetLength<int>(0xe9,pAVar8);
  pAVar8 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(in_r0 + 0x48) = pAVar8;
  ArraySetLength<int>(0xe9,pAVar8);
  pAVar8 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(in_r0 + 0x44) = pAVar8;
  ArraySetLength<int>(0xe9,pAVar8);
  iVar20 = *(int *)(*(int *)(in_r0 + 0x48) + 4);
  iVar19 = *(int *)(*(int *)(in_r0 + 0x3c) + 4);
  iVar15 = *(int *)(*(int *)(in_r0 + 0x44) + 4);
  iVar22 = *(int *)(*(int *)(in_r0 + 0x40) + 4);
  for (iVar25 = 0; iVar25 != 0xe9; iVar25 = iVar25 + 1) {
    *(undefined4 *)(iVar22 + iVar25 * 4) = 0;
    *(undefined4 *)(iVar19 + iVar25 * 4) = 0;
    *(undefined4 *)(iVar20 + iVar25 * 4) = 0;
    *(undefined4 *)(iVar15 + iVar25 * 4) = 0;
  }
  if (*(Array<bool> **)(in_r0 + 0x4c) != (Array<bool> *)0x0) {
    pvVar6 = (void *)Array<bool>::~Array(*(Array<bool> **)(in_r0 + 0x4c));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x4c) = 0;
  }
  pAVar9 = operator_new(0xc);
  Array<bool>::Array(pAVar9);
  *(Array<bool> **)(in_r0 + 0x4c) = pAVar9;
  ArraySetLength<bool>(4,(Array *)pAVar9);
  iVar15 = *(int *)(*(int *)(in_r0 + 0x4c) + 4);
  for (iVar20 = 0; iVar20 != 4; iVar20 = iVar20 + 1) {
    *(undefined1 *)(iVar15 + iVar20) = 0;
  }
  if (*(Array<bool> **)(in_r0 + 0x58) != (Array<bool> *)0x0) {
    pvVar6 = (void *)Array<bool>::~Array(*(Array<bool> **)(in_r0 + 0x58));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x58) = 0;
  }
  pAVar9 = operator_new(0xc);
  Array<bool>::Array(pAVar9);
  *(Array<bool> **)(in_r0 + 0x58) = pAVar9;
  ArraySetLength<bool>(5,(Array *)pAVar9);
  iVar15 = *(int *)(*(int *)(in_r0 + 0x58) + 4);
  for (iVar20 = 0; iVar20 != 5; iVar20 = iVar20 + 1) {
    *(undefined1 *)(iVar15 + iVar20) = 0;
  }
  puVar11 = (uint *)Galaxy::getSystems((Galaxy *)*piVar28);
  iVar15 = *(int *)(in_r0 + 0x38);
  for (uVar24 = 0; uVar24 < *puVar11; uVar24 = uVar24 + 1) {
    uVar4 = SolarSystem::isVisible(*(SolarSystem **)(puVar11[1] + uVar24 * 4));
    *(undefined1 *)(*(int *)(iVar15 + 4) + uVar24) = uVar4;
  }
  uVar24 = 0;
  piVar5 = *(int **)(DAT_000bd980 + 0xbd6ee);
  puVar11 = (uint *)*piVar5;
  for (uVar12 = 0; uVar12 < *puVar11; uVar12 = uVar12 + 1) {
    iVar15 = Item::getIngredients();
    if (iVar15 != 0) {
      uVar24 = uVar24 + 1;
    }
  }
  if (*(Array **)(in_r0 + 0x18) != (Array *)0x0) {
    ArrayReleaseClasses<BluePrint*>(*(Array **)(in_r0 + 0x18));
    if (*(Array<BluePrint*> **)(in_r0 + 0x18) != (Array<BluePrint*> *)0x0) {
      pvVar6 = (void *)Array<BluePrint*>::~Array(*(Array<BluePrint*> **)(in_r0 + 0x18));
      operator_delete(pvVar6);
    }
  }
  *(undefined4 *)(in_r0 + 0x18) = 0;
  if (uVar24 != 0) {
    this = operator_new(0xc);
    Array<BluePrint*>::Array(this);
    *(Array<BluePrint*> **)(in_r0 + 0x18) = this;
    ArraySetLength<BluePrint*>(uVar24,(Array *)this);
    iVar15 = 0;
    for (uVar24 = 0; uVar24 < *puVar11; uVar24 = uVar24 + 1) {
      iVar20 = Item::getIngredients();
      if (iVar20 != 0) {
        this_00 = operator_new(0x2c);
        BluePrint::BluePrint(this_00,uVar24);
        *(BluePrint **)(*(int *)(*(int *)(in_r0 + 0x18) + 4) + iVar15 * 4) = this_00;
        iVar15 = iVar15 + 1;
      }
    }
  }
  if (*(Array **)(in_r0 + 0x1c) != (Array *)0x0) {
    ArrayReleaseClasses<PendingProduct*>(*(Array **)(in_r0 + 0x1c));
    if (*(Array<PendingProduct*> **)(in_r0 + 0x1c) != (Array<PendingProduct*> *)0x0) {
      pvVar6 = (void *)Array<PendingProduct*>::~Array(*(Array<PendingProduct*> **)(in_r0 + 0x1c));
      operator_delete(pvVar6);
    }
  }
  *(undefined4 *)(in_r0 + 0x1c) = 0;
  loadAgents(in_r0);
  loadWanted(in_r0);
  if (*(Standing **)(in_r0 + 0x14) != (Standing *)0x0) {
    pvVar6 = (void *)Standing::~Standing(*(Standing **)(in_r0 + 0x14));
    operator_delete(pvVar6);
    *(undefined4 *)(in_r0 + 0x14) = 0;
  }
  this_01 = operator_new(8);
  Standing::Standing(this_01);
  iVar15 = DAT_000bd984;
  *(Standing **)(in_r0 + 0x14) = this_01;
  *(undefined4 *)(in_r0 + 0x7c) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x80) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x84) = 0;
  *(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x198) + 4) + 4) = **(undefined4 **)(iVar15 + 0xbd7d8);
  *(undefined4 *)(in_r0 + 0x1e8) = 0;
  this_02 = operator_new(0x78);
  Mission::Mission(this_02,4,0,0x4e);
  setCampaignMission(in_r0,this_02);
  piVar14 = *(int **)(DAT_000bd988 + 0xbd816);
  *(undefined4 *)(in_r0 + 0x194) = **(undefined4 **)(*(int *)(in_r0 + 0x198) + 4);
  Ship::makeShip(*(int *)(*(int *)(*piVar14 + 4) + 0x28));
  setShip((Ship *)in_r0);
  Ship::priceDecline(*(Ship **)(in_r0 + 400));
  Galaxy::getStation(*piVar28);
  setStation((Station *)in_r0);
  Ship::setCargo(*(Ship **)(in_r0 + 400),(Array *)0x0);
  pcVar26 = *(code **)(DAT_000bd98c + 0xbd864);
  uVar10 = (*pcVar26)(*(undefined4 *)(*(int *)(*piVar5 + 4) + 8));
  pcVar23 = *(code **)(DAT_000bd990 + 0xbd874);
  (*pcVar23)(*(undefined4 *)(in_r0 + 400),uVar10,0);
  uVar10 = (*pcVar26)(*(undefined4 *)(*(int *)(*piVar5 + 4) + 8));
  (*pcVar23)(*(undefined4 *)(in_r0 + 400),uVar10,1);
  uVar10 = (*pcVar26)(*(undefined4 *)(*(int *)(*piVar5 + 4) + 0xd8));
  (*pcVar23)(*(undefined4 *)(in_r0 + 400),uVar10,0);
  uVar10 = (*pcVar26)(*(undefined4 *)(*(int *)(*piVar5 + 4) + 0xec));
  (*pcVar23)(*(undefined4 *)(in_r0 + 400),uVar10,1);
  uVar10 = (*pcVar26)(*(undefined4 *)(*(int *)(*piVar5 + 4) + 0x148));
  (*pcVar23)(*(undefined4 *)(in_r0 + 400),uVar10,2);
  uVar10 = (*pcVar26)(*(undefined4 *)(*(int *)(*piVar5 + 4) + 0x124));
  (*pcVar23)(*(undefined4 *)(in_r0 + 400),uVar10,3);
  uVar10 = Item::makeItem(*(int *)(*(int *)(*piVar5 + 4) + 0x90));
  (*pcVar23)(*(undefined4 *)(**(int **)(DAT_000bd994 + 0xbd902) + 400),uVar10,0);
  if (*(char *)(iVar27 + 0x35) != '\0') {
    *(undefined1 *)(*(int *)(*(int *)(in_r0 + 0x38) + 4) + 0x19) = 1;
  }
  uVar10 = Ship::getMaxHP(*(Ship **)(in_r0 + 400));
  *(undefined4 *)(in_r0 + 100) = uVar10;
  uVar10 = Ship::getMaxShieldHP();
  *(undefined4 *)(in_r0 + 0x5c) = uVar10;
  uVar10 = Ship::getMaxArmorHP(*(Ship **)(in_r0 + 400));
  *(undefined4 *)(in_r0 + 0x60) = uVar10;
  *(undefined4 *)(in_r0 + 0x68) = 100;
  *(undefined4 *)(in_r0 + 0x150) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x154) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x158) = 0xffffffff;
  return;
}

```
## target disasm
```
  000bd304: push {r4,r5,r6,r7,lr}
  000bd306: add r7,sp,#0xc
  000bd308: push {r7,r8,r9,r10,r11}
  000bd30c: mov r11,r0
  000bd30e: ldr r0,[0x000bd694]
  000bd310: vmov.i32 q8,#0x0
  000bd314: movs r4,#0x0
  000bd316: add r0,pc
  000bd318: movs r1,#0x1
  000bd31a: str.w r4,[r11,#0x8c]
  000bd31e: ldr.w r8,[r0,#0x0]
  000bd322: add.w r0,r11,#0x1d4
  000bd326: str.w r4,[r11,#0x1e4]
  000bd32a: strd r4,r4,[r11,#0x100]
  000bd32e: strd r4,r4,[r11,#0x1b0]
  000bd332: strd r4,r4,[r11,#0x1b8]
  000bd336: strd r4,r4,[r11,#0x1c0]
  000bd33a: strd r1,r4,[r11,#0x1c8]
  000bd33e: str.w r4,[r11,#0x1d0]
  000bd342: str.w r4,[r11,#0x10c]
  000bd346: strb.w r4,[r11,#0x110]
  000bd34a: strd r4,r4,[r11,#0x118]
  000bd34e: str.w r4,[r11,#0x124]
  000bd352: str.w r4,[r11,#0x12c]
  000bd356: vst1.32 {d16,d17},[r0]
  000bd35a: str.w r4,[r11,#0x13c]
  000bd35e: str.w r4,[r11,#0x134]
  000bd362: str.w r4,[r11,#0x144]
  000bd366: strb.w r4,[r11,#0x120]
  000bd36a: strb.w r4,[r11,#0x128]
  000bd36e: strb.w r4,[r11,#0x130]
  000bd372: strb.w r4,[r11,#0x138]
  000bd376: strb.w r4,[r11,#0x140]
  000bd37a: strb.w r4,[r11,#0x148]
  000bd37e: ldrb.w r1,[r8,#0x36]
  000bd382: ldr.w r0,[r11,#0x14c]
  000bd386: cmp r1,#0x0
  000bd388: strb.w r4,[r11,#0x111]
  000bd38c: it ne
  000bd38e: mov.ne r1,#0x3
  000bd390: cmp r0,#0x0
  000bd392: str.w r1,[r11,#0x114]
  000bd396: beq 0x000bd3a4
  000bd398: blx 0x0007360c
  000bd39c: blx 0x0006eb48
  000bd3a0: str.w r4,[r11,#0x14c]
  000bd3a4: movs r0,#0x34
  000bd3a6: blx 0x0006eb24
  000bd3aa: mov r5,r0
  000bd3ac: blx 0x000735d0
  000bd3b0: ldr.w r0,[r11,#0x28]
  000bd3b4: str.w r5,[r11,#0x14c]
  000bd3b8: cbz r0,0x000bd3c4
  000bd3ba: blx 0x0006ebfc
  000bd3be: movs r0,#0x0
  000bd3c0: str.w r0,[r11,#0x28]
  000bd3c4: ldr.w r0,[r11,#0x24]
  000bd3c8: cbz r0,0x000bd3dc
  000bd3ca: blx 0x0006facc
  000bd3ce: ldr.w r0,[r11,#0x24]
  000bd3d2: cbz r0,0x000bd3dc
  000bd3d4: blx 0x0006f64c
  000bd3d8: blx 0x0006eb48
  000bd3dc: ldr.w r0,[r11,#0x94]
  000bd3e0: movs r1,#0x0
  000bd3e2: str.w r1,[r11,#0x30]
  000bd3e6: movs r3,#0x0
  000bd3e8: str.w r1,[r11,#0x24]
  000bd3ec: ldr r2,[r0,#0x0]
  000bd3ee: b 0x000bd3f6
  000bd3f0: ldr r6,[r0,#0x4]
  000bd3f2: strb r1,[r6,r3]
  000bd3f4: adds r3,#0x1
  000bd3f6: cmp r2,r3
  000bd3f8: bne 0x000bd3f0
  000bd3fa: ldr.w r0,[r11,#0x98]
  000bd3fe: movs r2,#0x0
  000bd400: movs r3,#0x0
  000bd402: ldr r1,[r0,#0x0]
  000bd404: b 0x000bd40c
  000bd406: ldr r6,[r0,#0x4]
  000bd408: strb r2,[r6,r3]
  000bd40a: adds r3,#0x1
  000bd40c: cmp r1,r3
  000bd40e: bne 0x000bd406
  000bd410: vmov.i32 q8,#0x0
  000bd414: ldr.w r0,[r11,#0xac]
  000bd418: add.w r1,r11,#0x9c
  000bd41c: movs r2,#0x0
  000bd41e: movs r3,#0x0
  000bd420: vst1.32 {d16,d17},[r1]
  000bd424: ldr r1,[r0,#0x0]
  000bd426: b 0x000bd42e
  000bd428: ldr r6,[r0,#0x4]
  000bd42a: strb r2,[r6,r3]
  000bd42c: adds r3,#0x1
  000bd42e: cmp r1,r3
  000bd430: bne 0x000bd428
  000bd432: ldr.w r0,[r11,#0xb4]
  000bd436: movs r1,#0x0
  000bd438: str.w r1,[r11,#0xb0]
  000bd43c: movs r3,#0x0
  000bd43e: ldr r2,[r0,#0x0]
  000bd440: b 0x000bd448
  000bd442: ldr r6,[r0,#0x4]
  000bd444: strb r1,[r6,r3]
  000bd446: adds r3,#0x1
  000bd448: cmp r2,r3
  000bd44a: bne 0x000bd442
  000bd44c: ldr.w r1,[r11,#0x1a0]
  000bd450: cbz r1,0x000bd47e
  000bd452: movs r4,#0x0
  000bd454: movs r5,#0x0
  000bd456: b 0x000bd478
  000bd458: ldr r0,[r1,#0x4]
  000bd45a: ldr.w r0,[r0,r5,lsl #0x2]
  000bd45e: cbz r0,0x000bd476
  000bd460: blx 0x0007360c
  000bd464: blx 0x0006eb48
  000bd468: ldr.w r0,[r11,#0x1a0]
  000bd46c: ldr r0,[r0,#0x4]
  000bd46e: str.w r4,[r0,r5,lsl #0x2]
  000bd472: ldr.w r1,[r11,#0x1a0]
  000bd476: adds r5,#0x1
  000bd478: ldr r0,[r1,#0x0]
  000bd47a: cmp r5,r0
  000bd47c: bcc 0x000bd458
  000bd47e: ldr r0,[0x000bd698]
  000bd480: add r0,pc
  000bd482: ldr r0,[r0,#0x0]
  000bd484: ldr r0,[r0,#0x0]
  000bd486: blx 0x00073a38
  000bd48a: ldr r0,[0x000bd69c]
  000bd48c: add r0,pc
  000bd48e: ldr.w r9,[r0,#0x0]
  000bd492: ldr.w r0,[r9,#0x0]
  000bd496: blx 0x00073a44
  000bd49a: ldr.w r0,[r11,#0x50]
  000bd49e: movs r2,#0x0
  000bd4a0: movs r3,#0x0
  000bd4a2: ldr r1,[r0,#0x0]
  000bd4a4: b 0x000bd4ac
  000bd4a6: ldr r6,[r0,#0x4]
  000bd4a8: strb r2,[r6,r3]
  000bd4aa: adds r3,#0x1
  000bd4ac: cmp r1,r3
  000bd4ae: bne 0x000bd4a6
  000bd4b0: ldr.w r1,[r11,#0x54]
  000bd4b4: movs r2,#0x0
  000bd4b6: movs r3,#0x1
  000bd4b8: ldr r0,[r1,#0x4]
  000bd4ba: b 0x000bd4c0
  000bd4bc: strb r3,[r0,r2]
  000bd4be: adds r2,#0x1
  000bd4c0: cmp r2,#0xb0
  000bd4c2: bne 0x000bd4bc
  000bd4c4: ldr r1,[r1,#0x0]
  000bd4c6: movs r2,#0xb0
  000bd4c8: movs r3,#0x0
  000bd4ca: b 0x000bd4d0
  000bd4cc: strb r3,[r0,r2]
  000bd4ce: adds r2,#0x1
  000bd4d0: cmp r2,r1
  000bd4d2: bcc 0x000bd4cc
  000bd4d4: ldr r1,[0x000bd6a0]
  000bd4d6: vmov.i32 q8,#0x0
  000bd4da: ldr r2,[0x000bd6a4]
  000bd4dc: movs r4,#0x0
  000bd4de: ldr r3,[0x000bd6a8]
  000bd4e0: add r1,pc
  000bd4e2: ldr r5,[0x000bd6ac]
  000bd4e4: add r2,pc
  000bd4e6: strb.w r4,[r0,#0x3e]
  000bd4ea: add r3,pc
  000bd4ec: strh r4,[r0,#0x3c]
  000bd4ee: add.w r0,r11,#0xe0
  000bd4f2: str.w r4,[r11,#0x34]
  000bd4f6: add r5,pc
  000bd4f8: str.w r4,[r11,#0xb8]
  000bd4fc: add.w r6,r11,#0xc0
  000bd500: vst1.64 {d16,d17},[r0]
  000bd504: ldr r0,[r1,#0x0]
  000bd506: ldr r1,[r2,#0x0]
  000bd508: add.w r2,r11,#0xd0
  000bd50c: vst1.64 {d16,d17},[r6]
  000bd510: vst1.64 {d16,d17},[r2]
  000bd514: ldr r2,[r3,#0x0]
  000bd516: ldr r3,[r5,#0x0]
  000bd518: str r4,[r1,#0x0]
  000bd51a: mov.w r1,#0xffffffff
  000bd51e: strb r4,[r2,#0x0]
  000bd520: str r4,[r3,#0x0]
  000bd522: strd r4,r4,[r11,#0x160]
  000bd526: strb.w r4,[r11,#0x108]
  000bd52a: str.w r4,[r11,#0x174]
  000bd52e: strh.w r4,[r11,#0xf0]
  000bd532: str.w r1,[r11,#0xf4]
  000bd536: movs r1,#0x1
  000bd538: strh.w r1,[r11,#0xf8]
  000bd53c: ldr r0,[r0,#0x0]
  000bd53e: blx 0x00073a50
  000bd542: ldr.w r0,[r11,#0x40]
  000bd546: cbz r0,0x000bd554
  000bd548: blx 0x00070204
  000bd54c: blx 0x0006eb48
  000bd550: str.w r4,[r11,#0x40]
  000bd554: ldr.w r0,[r11,#0x3c]
  000bd558: cbz r0,0x000bd568
  000bd55a: blx 0x00070204
  000bd55e: blx 0x0006eb48
  000bd562: movs r0,#0x0
  000bd564: str.w r0,[r11,#0x3c]
  000bd568: ldr.w r0,[r11,#0x48]
  000bd56c: cbz r0,0x000bd57c
  000bd56e: blx 0x00070204
  000bd572: blx 0x0006eb48
  000bd576: movs r0,#0x0
  000bd578: str.w r0,[r11,#0x48]
  000bd57c: ldr.w r0,[r11,#0x44]
  000bd580: cbz r0,0x000bd590
  000bd582: blx 0x00070204
  000bd586: blx 0x0006eb48
  000bd58a: movs r0,#0x0
  000bd58c: str.w r0,[r11,#0x44]
  000bd590: movs r0,#0xc
  000bd592: blx 0x0006eb24
  000bd596: mov r5,r0
  000bd598: blx 0x000701f8
  000bd59c: movs r0,#0xe9
  000bd59e: mov r1,r5
  000bd5a0: str.w r5,[r11,#0x40]
  000bd5a4: blx 0x00071a4c
  000bd5a8: movs r0,#0xc
  000bd5aa: blx 0x0006eb24
  000bd5ae: mov r5,r0
  000bd5b0: blx 0x000701f8
  000bd5b4: movs r0,#0xe9
  000bd5b6: mov r1,r5
  000bd5b8: str.w r5,[r11,#0x3c]
  000bd5bc: blx 0x00071a4c
  000bd5c0: movs r0,#0xc
  000bd5c2: blx 0x0006eb24
  000bd5c6: mov r5,r0
  000bd5c8: blx 0x000701f8
  000bd5cc: movs r0,#0xe9
  000bd5ce: mov r1,r5
  000bd5d0: str.w r5,[r11,#0x48]
  000bd5d4: blx 0x00071a4c
  000bd5d8: movs r0,#0xc
  000bd5da: blx 0x0006eb24
  000bd5de: mov r5,r0
  000bd5e0: blx 0x000701f8
  000bd5e4: movs r0,#0xe9
  000bd5e6: mov r1,r5
  000bd5e8: str.w r5,[r11,#0x44]
  000bd5ec: blx 0x00071a4c
  000bd5f0: ldrd r2,r3,[r11,#0x3c]
  000bd5f4: movs r6,#0x0
  000bd5f6: ldrd r0,r1,[r11,#0x44]
  000bd5fa: movs r5,#0x0
  000bd5fc: ldr r1,[r1,#0x4]
  000bd5fe: ldr r2,[r2,#0x4]
  000bd600: ldr r0,[r0,#0x4]
  000bd602: ldr r3,[r3,#0x4]
  000bd604: b 0x000bd618
  000bd606: str.w r6,[r3,r5,lsl #0x2]
  000bd60a: str.w r6,[r2,r5,lsl #0x2]
  000bd60e: str.w r6,[r1,r5,lsl #0x2]
  000bd612: str.w r6,[r0,r5,lsl #0x2]
  000bd616: adds r5,#0x1
  000bd618: cmp r5,#0xe9
  000bd61a: bne 0x000bd606
  000bd61c: ldr.w r0,[r11,#0x4c]
  000bd620: cbz r0,0x000bd630
  000bd622: blx 0x00073a5c
  000bd626: blx 0x0006eb48
  000bd62a: movs r0,#0x0
  000bd62c: str.w r0,[r11,#0x4c]
  000bd630: movs r0,#0xc
  000bd632: blx 0x0006eb24
  000bd636: mov r5,r0
  000bd638: blx 0x000735a0
  000bd63c: movs r0,#0x4
  000bd63e: mov r1,r5
  000bd640: str.w r5,[r11,#0x4c]
  000bd644: blx 0x000735c4
  000bd648: ldr.w r0,[r11,#0x4c]
  000bd64c: movs r1,#0x0
  000bd64e: movs r2,#0x0
  000bd650: ldr r0,[r0,#0x4]
  000bd652: b 0x000bd658
  000bd654: strb r1,[r0,r2]
  000bd656: adds r2,#0x1
  000bd658: cmp r2,#0x4
  000bd65a: bne 0x000bd654
  000bd65c: ldr.w r0,[r11,#0x58]
  000bd660: cbz r0,0x000bd670
  000bd662: blx 0x00073a5c
  000bd666: blx 0x0006eb48
  000bd66a: movs r0,#0x0
  000bd66c: str.w r0,[r11,#0x58]
  000bd670: movs r0,#0xc
  000bd672: blx 0x0006eb24
  000bd676: mov r5,r0
  000bd678: blx 0x000735a0
  000bd67c: movs r0,#0x5
  000bd67e: mov r1,r5
  000bd680: str.w r5,[r11,#0x58]
  000bd684: blx 0x000735c4
  000bd688: ldr.w r0,[r11,#0x58]
  000bd68c: movs r1,#0x0
  000bd68e: movs r2,#0x0
  000bd690: ldr r0,[r0,#0x4]
  000bd692: b 0x000bd6b4
  000bd6b0: strb r1,[r0,r2]
  000bd6b2: adds r2,#0x1
  000bd6b4: cmp r2,#0x5
  000bd6b6: bne 0x000bd6b0
  000bd6b8: ldr.w r0,[r9,#0x0]
  000bd6bc: blx 0x00071884
  000bd6c0: mov r5,r0
  000bd6c2: ldr.w r4,[r11,#0x38]
  000bd6c6: movs r6,#0x0
  000bd6c8: b 0x000bd6da
  000bd6ca: ldr r0,[r5,#0x4]
  000bd6cc: ldr.w r0,[r0,r6,lsl #0x2]
  000bd6d0: blx 0x00073a68
  000bd6d4: ldr r1,[r4,#0x4]
  000bd6d6: strb r0,[r1,r6]
  000bd6d8: adds r6,#0x1
  000bd6da: ldr r0,[r5,#0x0]
  000bd6dc: cmp r6,r0
  000bd6de: bcc 0x000bd6ca
  000bd6e0: str.w r8,[sp,#0x0]
  000bd6e4: movs r5,#0x0
  000bd6e6: ldr r0,[0x000bd980]
  000bd6e8: movs r6,#0x0
  000bd6ea: add r0,pc
  000bd6ec: ldr.w r10,[r0,#0x0]
  000bd6f0: ldr.w r4,[r10,#0x0]
  000bd6f4: b 0x000bd708
  000bd6f6: ldr r0,[r4,#0x4]
  000bd6f8: ldr.w r0,[r0,r6,lsl #0x2]
  000bd6fc: blx 0x00071914
  000bd700: cmp r0,#0x0
  000bd702: it ne
  000bd704: add.ne r5,#0x1
  000bd706: adds r6,#0x1
  000bd708: ldr r0,[r4,#0x0]
  000bd70a: cmp r6,r0
  000bd70c: bcc 0x000bd6f6
  000bd70e: ldr.w r0,[r11,#0x18]
  000bd712: cbz r0,0x000bd726
  000bd714: blx 0x00073a74
  000bd718: ldr.w r0,[r11,#0x18]
  000bd71c: cbz r0,0x000bd726
  000bd71e: blx 0x00073a80
  000bd722: blx 0x0006eb48
  000bd726: movs r0,#0x0
  000bd728: cmp r5,#0x0
  000bd72a: str.w r0,[r11,#0x18]
  000bd72e: beq 0x000bd780
  000bd730: movs r0,#0xc
  000bd732: blx 0x0006eb24
  000bd736: mov r6,r0
  000bd738: blx 0x00073a8c
  000bd73c: mov r0,r5
  000bd73e: mov r1,r6
  000bd740: str.w r6,[r11,#0x18]
  000bd744: blx 0x00073a98
  000bd748: mov.w r8,#0x0
  000bd74c: movs r5,#0x0
  000bd74e: b 0x000bd77a
  000bd750: ldr r0,[r4,#0x4]
  000bd752: ldr.w r0,[r0,r5,lsl #0x2]
  000bd756: blx 0x00071914
  000bd75a: cbz r0,0x000bd778
  000bd75c: movs r0,#0x2c
  000bd75e: blx 0x0006eb24
  000bd762: mov r6,r0
  000bd764: mov r1,r5
  000bd766: blx 0x00073aa4
  000bd76a: ldr.w r0,[r11,#0x18]
  000bd76e: ldr r0,[r0,#0x4]
  000bd770: str.w r6,[r0,r8,lsl #0x2]
  000bd774: add.w r8,r8,#0x1
  000bd778: adds r5,#0x1
  000bd77a: ldr r0,[r4,#0x0]
  000bd77c: cmp r5,r0
  000bd77e: bcc 0x000bd750
  000bd780: ldr.w r0,[r11,#0x1c]
  000bd784: cbz r0,0x000bd798
  000bd786: blx 0x00073ab0
  000bd78a: ldr.w r0,[r11,#0x1c]
  000bd78e: cbz r0,0x000bd798
  000bd790: blx 0x00073abc
  000bd794: blx 0x0006eb48
  000bd798: movs r4,#0x0
  000bd79a: mov r0,r11
  000bd79c: str.w r4,[r11,#0x1c]
  000bd7a0: blx 0x00073ac8
  000bd7a4: mov r0,r11
  000bd7a6: blx 0x00073ad4
  000bd7aa: ldr.w r0,[r11,#0x14]
  000bd7ae: cbz r0,0x000bd7bc
  000bd7b0: blx 0x00073ae0
  000bd7b4: blx 0x0006eb48
  000bd7b8: str.w r4,[r11,#0x14]
  000bd7bc: movs r0,#0x8
  000bd7be: blx 0x0006eb24
  000bd7c2: mov r5,r0
  000bd7c4: blx 0x00073aec
  000bd7c8: ldr r0,[0x000bd984]
  000bd7ca: mov.w r1,#0xffffffff
  000bd7ce: str.w r5,[r11,#0x14]
  000bd7d2: movs r3,#0x0
  000bd7d4: add r0,pc
  000bd7d6: ldr.w r2,[r11,#0x198]
  000bd7da: strd r1,r1,[r11,#0x7c]
  000bd7de: str.w r3,[r11,#0x84]
  000bd7e2: ldr r0,[r0,#0x0]
  000bd7e4: ldr r1,[r2,#0x4]
  000bd7e6: ldr r0,[r0,#0x0]
  000bd7e8: str r0,[r1,#0x4]
  000bd7ea: movs r0,#0x78
  000bd7ec: str.w r3,[r11,#0x1e8]
  000bd7f0: blx 0x0006eb24
  000bd7f4: mov r5,r0
  000bd7f6: movs r1,#0x4
  000bd7f8: movs r2,#0x0
  000bd7fa: movs r3,#0x4e
  000bd7fc: blx 0x00073834
  000bd800: mov r0,r11
  000bd802: mov r1,r5
  000bd804: blx 0x00073840
  000bd808: ldr.w r0,[r11,#0x198]
  000bd80c: mov.w r5,#0xffffffff
  000bd810: ldr r1,[0x000bd988]
  000bd812: add r1,pc
  000bd814: ldr r0,[r0,#0x4]
  000bd816: ldr r1,[r1,#0x0]
  000bd818: ldr r0,[r0,#0x0]
  000bd81a: str.w r0,[r11,#0x194]
  000bd81e: ldr r0,[r1,#0x0]
  000bd820: mov.w r1,#0xffffffff
  000bd824: ldr r0,[r0,#0x4]
  000bd826: ldr r0,[r0,#0x28]
  000bd828: blx 0x000719d4
  000bd82c: mov r1,r0
  000bd82e: mov r0,r11
  000bd830: blx 0x00073870
  000bd834: ldr.w r0,[r11,#0x190]
  000bd838: blx 0x00073af8
  000bd83c: ldr.w r0,[r9,#0x0]
  000bd840: movs r1,#0x4e
  000bd842: blx 0x00071c44
  000bd846: mov r1,r0
  000bd848: mov r0,r11
  000bd84a: blx 0x0007366c
  000bd84e: ldr.w r0,[r11,#0x190]
  000bd852: movs r1,#0x0
  000bd854: blx 0x000738b8
  000bd858: ldr.w r0,[r10,#0x0]
  000bd85c: ldr r1,[0x000bd98c]
  000bd85e: ldr r0,[r0,#0x4]
  000bd860: add r1,pc
  000bd862: ldr r6,[r1,#0x0]
  000bd864: ldr r0,[r0,#0x8]
  000bd866: blx r6
  000bd868: ldr r2,[0x000bd990]
  000bd86a: mov r1,r0
  000bd86c: ldr.w r0,[r11,#0x190]
  000bd870: add r2,pc
  000bd872: ldr r4,[r2,#0x0]
  000bd874: movs r2,#0x0
  000bd876: blx r4
  000bd878: ldr.w r0,[r10,#0x0]
  000bd87c: ldr r0,[r0,#0x4]
  000bd87e: ldr r0,[r0,#0x8]
  000bd880: blx r6
  000bd882: mov r1,r0
  000bd884: ldr.w r0,[r11,#0x190]
  000bd888: movs r2,#0x1
  000bd88a: mov.w r9,#0x1
  000bd88e: blx r4
  000bd890: ldr.w r0,[r10,#0x0]
  000bd894: ldr r0,[r0,#0x4]
  000bd896: ldr.w r0,[r0,#0xd8]
  000bd89a: blx r6
  000bd89c: mov r1,r0
  000bd89e: ldr.w r0,[r11,#0x190]
  000bd8a2: movs r2,#0x0
  000bd8a4: blx r4
  000bd8a6: ldr.w r0,[r10,#0x0]
  000bd8aa: ldr r0,[r0,#0x4]
  000bd8ac: ldr.w r0,[r0,#0xec]
  000bd8b0: blx r6
  000bd8b2: mov r1,r0
  000bd8b4: ldr.w r0,[r11,#0x190]
  000bd8b8: movs r2,#0x1
  000bd8ba: blx r4
  000bd8bc: ldr.w r0,[r10,#0x0]
  000bd8c0: ldr r0,[r0,#0x4]
  000bd8c2: ldr.w r0,[r0,#0x148]
  000bd8c6: blx r6
  000bd8c8: mov r1,r0
  000bd8ca: ldr.w r0,[r11,#0x190]
  000bd8ce: movs r2,#0x2
  000bd8d0: blx r4
  000bd8d2: ldr.w r0,[r10,#0x0]
  000bd8d6: ldr r0,[r0,#0x4]
  000bd8d8: ldr.w r0,[r0,#0x124]
  000bd8dc: blx r6
  000bd8de: mov r1,r0
  000bd8e0: ldr.w r0,[r11,#0x190]
  000bd8e4: movs r2,#0x3
  000bd8e6: blx r4
  000bd8e8: ldr.w r0,[r10,#0x0]
  000bd8ec: movs r1,#0x6
  000bd8ee: ldr r0,[r0,#0x4]
  000bd8f0: ldr.w r0,[r0,#0x90]
  000bd8f4: blx 0x000718c0
  000bd8f8: mov r1,r0
  000bd8fa: ldr r0,[0x000bd994]
  000bd8fc: movs r2,#0x0
  000bd8fe: add r0,pc
  000bd900: ldr r0,[r0,#0x0]
  000bd902: ldr r0,[r0,#0x0]
  000bd904: ldr.w r0,[r0,#0x190]
  000bd908: blx r4
  000bd90a: ldr r0,[sp,#0x0]
  000bd90c: ldrb.w r0,[r0,#0x35]
  000bd910: cbz r0,0x000bd91c
  000bd912: ldr.w r0,[r11,#0x38]
  000bd916: ldr r0,[r0,#0x4]
  000bd918: strb.w r9,[r0,#0x19]
  000bd91c: ldr.w r0,[r11,#0x190]
  000bd920: blx 0x00073b04
  000bd924: ldr.w r1,[r11,#0x190]
  000bd928: str.w r0,[r11,#0x64]
  000bd92c: mov r0,r1
  000bd92e: blx 0x000722bc
  000bd932: ldr.w r1,[r11,#0x190]
  000bd936: str.w r0,[r11,#0x5c]
  000bd93a: mov r0,r1
  000bd93c: blx 0x00073b10
  000bd940: str.w r0,[r11,#0x60]
  000bd944: movs r0,#0x64
  000bd946: str.w r0,[r11,#0x68]
  000bd94a: strd r5,r5,[r11,#0x150]
  000bd94e: str.w r5,[r11,#0x158]
  000bd952: pop.w {r3,r8,r9,r10,r11}
  000bd956: pop {r4,r5,r6,r7,pc}
```
