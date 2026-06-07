# Galaxy::getAsteroidProbabilities
elf 0x176098 body 422
Sig: undefined __thiscall getAsteroidProbabilities(Galaxy * this, Station * param_1)

## decompile
```c

/* Galaxy::getAsteroidProbabilities(Station*) */

void * __thiscall Galaxy::getAsteroidProbabilities(Galaxy *this,Station *param_1)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  AEArray *pAVar4;
  int iVar5;
  void *pvVar6;
  void *pvVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  void *pvVar12;
  bool bVar13;
  int iVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  uint uVar17;
  int iVar18;
  
  iVar2 = Status::inAlienOrbit();
  iVar3 = Status::inSupernovaOrbit();
  if (iVar2 == 0) {
    pAVar4 = this->systems;
  }
  else {
    pAVar4 = (AEArray *)0x0;
  }
  iVar5 = **(int **)(DAT_00186244 + 0x1860d0);
  pvVar6 = operator_new__(0x2c);
  pvVar7 = operator_new__(0x2c);
  iVar14 = 0;
  for (iVar18 = 0x9a; iVar18 != 0xa4; iVar18 = iVar18 + 1) {
    *(int *)((int)pvVar7 + iVar14 * 4) = iVar18;
    if (iVar2 == 0) {
      iVar9 = Station::getSystem(param_1);
      iVar9 = SolarSystem::getX(*(SolarSystem **)((int)pAVar4->data + iVar9 * 4));
      iVar8 = Station::getSystem(param_1);
      iVar8 = SolarSystem::getY(*(SolarSystem **)((int)pAVar4->data + iVar8 * 4));
      iVar10 = Item::getMinPriceSystem(*(Item **)(*(int *)(iVar5 + 4) + iVar18 * 4));
      iVar10 = SolarSystem::getX(*(SolarSystem **)((int)pAVar4->data + iVar10 * 4));
      iVar11 = Item::getMinPriceSystem(*(Item **)(*(int *)(iVar5 + 4) + iVar18 * 4));
      SolarSystem::getY(*(SolarSystem **)((int)pAVar4->data + iVar11 * 4));
      iVar8 = invDistancePercent((int)this,iVar9,iVar8,iVar10);
      iVar9 = iVar14 + 1;
      if (iVar8 < 0x32) {
        iVar8 = 0;
      }
    }
    else {
      iVar8 = 0;
      iVar9 = iVar14;
    }
    *(int *)((int)pvVar6 + iVar14 * 4) = iVar8;
    iVar14 = iVar9;
  }
  *(undefined4 *)((int)pvVar7 + 0x28) = 0xa4;
  uVar15 = 0;
  if (iVar2 != 0) {
    uVar15 = 100;
  }
  *(undefined4 *)((int)pvVar6 + 0x28) = uVar15;
  bVar13 = true;
  iVar2 = 1;
  do {
    for (; iVar2 != 0xb; iVar2 = iVar2 + 1) {
      iVar14 = iVar2 + -1;
      iVar18 = *(int *)((int)pvVar6 + iVar2 * 4);
      iVar5 = *(int *)((int)pvVar6 + iVar14 * 4);
      if (iVar5 < iVar18) {
        *(int *)((int)pvVar6 + iVar14 * 4) = iVar18;
        uVar15 = *(undefined4 *)((int)pvVar7 + iVar14 * 4);
        uVar16 = *(undefined4 *)((int)pvVar7 + iVar2 * 4);
        *(int *)((int)pvVar6 + iVar2 * 4) = iVar5;
        *(undefined4 *)((int)pvVar7 + iVar14 * 4) = uVar16;
        *(undefined4 *)((int)pvVar7 + iVar2 * 4) = uVar15;
        bVar13 = false;
      }
    }
    bVar1 = !bVar13;
    bVar13 = true;
    iVar2 = 1;
  } while (bVar1);
  iVar2 = 0;
  for (iVar14 = 0; iVar14 != 0xb; iVar14 = iVar14 + 1) {
    iVar5 = *(int *)((int)pvVar6 + iVar14 * 4);
    if (0 < iVar5) {
      *(int *)((int)pvVar6 + iVar14 * 4) = iVar5 + iVar2;
    }
    iVar2 = iVar2 + -2;
  }
  pvVar12 = operator_new__(0x58);
  for (uVar17 = 0; uVar17 < 0x16; uVar17 = uVar17 + 2) {
    *(undefined4 *)((int)pvVar12 + uVar17 * 4) = *(undefined4 *)((int)pvVar7 + uVar17 * 2);
    *(undefined4 *)((int)pvVar12 + uVar17 * 4 + 4) = *(undefined4 *)((int)pvVar6 + uVar17 * 2);
    if ((iVar3 != 0) && (iVar2 = Status::getCurrentCampaignMission(), 0x59 < iVar2)) {
      *(undefined4 *)((int)pvVar12 + uVar17 * 4) = 0xd9;
    }
  }
  operator_delete__(pvVar6);
  operator_delete__(pvVar7);
  return pvVar12;
}

```

## target disasm
```
  00186098: push {r4,r5,r6,r7,lr}
  0018609a: add r7,sp,#0xc
  0018609c: push {r8,r9,r10,r11}
  001860a0: sub sp,#0x2c
  001860a2: str r1,[sp,#0x20]
  001860a4: mov r5,r0
  001860a6: ldr r0,[0x00186240]
  001860a8: add r0,pc
  001860aa: ldr r4,[r0,#0x0]
  001860ac: ldr r0,[r4,#0x0]
  001860ae: blx 0x000723d0
  001860b2: mov r6,r0
  001860b4: ldr r0,[r4,#0x0]
  001860b6: blx 0x000722f8
  001860ba: strd r0,r4,[sp,#0x8]
  001860be: cmp r6,#0x0
  001860c0: str r5,[sp,#0x1c]
  001860c2: ite eq
  001860c4: ldr.eq r0,[r5,#0x4]
  001860c6: mov.ne r0,#0x0
  001860c8: str r0,[sp,#0x14]
  001860ca: ldr r0,[0x00186244]
  001860cc: add r0,pc
  001860ce: ldr r0,[r0,#0x0]
  001860d0: ldr r0,[r0,#0x0]
  001860d2: str r0,[sp,#0x10]
  001860d4: movs r0,#0x2c
  001860d6: blx 0x0006ec08
  001860da: mov r5,r0
  001860dc: movs r0,#0x2c
  001860de: blx 0x0006ec08
  001860e2: mov r9,r0
  001860e4: mov.w r11,#0x9a
  001860e8: mov.w r10,#0x0
  001860ec: str r6,[sp,#0x18]
  001860ee: b 0x00186176
  001860f0: cmp r6,#0x0
  001860f2: str.w r11,[r9,r10,lsl #0x2]
  001860f6: beq 0x001860fe
  001860f8: movs r0,#0x0
  001860fa: mov r1,r10
  001860fc: b 0x0018616c
  001860fe: ldr r6,[sp,#0x20]
  00186100: mov r0,r6
  00186102: blx 0x0007189c
  00186106: ldr r4,[sp,#0x14]
  00186108: ldr r1,[r4,#0x4]
  0018610a: ldr.w r0,[r1,r0,lsl #0x2]
  0018610e: blx 0x00071968
  00186112: str r0,[sp,#0x28]
  00186114: mov r0,r6
  00186116: blx 0x0007189c
  0018611a: ldr r1,[r4,#0x4]
  0018611c: ldr.w r0,[r1,r0,lsl #0x2]
  00186120: blx 0x00071974
  00186124: ldr r6,[sp,#0x10]
  00186126: str r0,[sp,#0x24]
  00186128: ldr r0,[r6,#0x4]
  0018612a: ldr.w r0,[r0,r11,lsl #0x2]
  0018612e: blx 0x0007195c
  00186132: ldr r1,[r4,#0x4]
  00186134: ldr.w r0,[r1,r0,lsl #0x2]
  00186138: blx 0x00071968
  0018613c: mov r8,r0
  0018613e: ldr r0,[r6,#0x4]
  00186140: ldr.w r0,[r0,r11,lsl #0x2]
  00186144: blx 0x0007195c
  00186148: ldr r1,[r4,#0x4]
  0018614a: ldr.w r0,[r1,r0,lsl #0x2]
  0018614e: blx 0x00071974
  00186152: str r0,[sp,#0x0]
  00186154: mov r3,r8
  00186156: ldrd r2,r1,[sp,#0x24]
  0018615a: ldrd r6,r0,[sp,#0x18]
  0018615e: blx 0x00071980
  00186162: add.w r1,r10,#0x1
  00186166: cmp r0,#0x32
  00186168: it lt
  0018616a: mov.lt r0,#0x0
  0018616c: str.w r0,[r5,r10,lsl #0x2]
  00186170: add.w r11,r11,#0x1
  00186174: mov r10,r1
  00186176: cmp.w r11,#0xa4
  0018617a: bne 0x001860f0
  0018617c: movs r0,#0xa4
  0018617e: cmp r6,#0x0
  00186180: str.w r0,[r9,#0x28]
  00186184: it ne
  00186186: mov.ne r6,#0x64
  00186188: str r6,[r5,#0x28]
  0018618a: movs r1,#0x1
  0018618c: ldrd r11,r10,[sp,#0x8]
  00186190: movs r0,#0x1
  00186192: cmp r0,#0xb
  00186194: beq 0x001861c2
  00186196: subs r2,r0,#0x1
  00186198: ldr.w r6,[r5,r0,lsl #0x2]
  0018619c: ldr.w r3,[r5,r2,lsl #0x2]
  001861a0: cmp r3,r6
  001861a2: bge 0x001861be
  001861a4: str.w r6,[r5,r2,lsl #0x2]
  001861a8: ldr.w r1,[r9,r2,lsl #0x2]
  001861ac: ldr.w r6,[r9,r0,lsl #0x2]
  001861b0: str.w r3,[r5,r0,lsl #0x2]
  001861b4: str.w r6,[r9,r2,lsl #0x2]
  001861b8: str.w r1,[r9,r0,lsl #0x2]
  001861bc: movs r1,#0x0
  001861be: adds r0,#0x1
  001861c0: b 0x00186192
  001861c2: lsls r0,r1,#0x1f
  001861c4: mov.w r1,#0x1
  001861c8: mov.w r0,#0x1
  001861cc: beq 0x00186192
  001861ce: movs r0,#0x0
  001861d0: movs r1,#0x0
  001861d2: b 0x001861e6
  001861d4: ldr.w r2,[r5,r1,lsl #0x2]
  001861d8: cmp r2,#0x1
  001861da: itt ge
  001861dc: add.ge r2,r0
  001861de: str.ge.w r2,[r5,r1,lsl #0x2]
  001861e2: subs r0,#0x2
  001861e4: adds r1,#0x1
  001861e6: cmp r1,#0xb
  001861e8: bne 0x001861d4
  001861ea: movs r0,#0x58
  001861ec: blx 0x0006ec08
  001861f0: mov r4,r0
  001861f2: movs r6,#0x0
  001861f4: mov.w r8,#0xd9
  001861f8: b 0x00186224
  001861fa: ldr.w r0,[r9,r6,lsl #0x1]
  001861fe: add.w r1,r4,r6, lsl #0x2
  00186202: cmp.w r11,#0x0
  00186206: str.w r0,[r4,r6,lsl #0x2]
  0018620a: ldr.w r0,[r5,r6,lsl #0x1]
  0018620e: str r0,[r1,#0x4]
  00186210: beq 0x00186222
  00186212: ldr.w r0,[r10,#0x0]
  00186216: blx 0x00071770
  0018621a: cmp r0,#0x5a
  0018621c: it ge
  0018621e: str.ge.w r8,[r4,r6,lsl #0x2]
  00186222: adds r6,#0x2
  00186224: cmp r6,#0x15
  00186226: bls 0x001861fa
  00186228: mov r0,r5
  0018622a: blx 0x0006ebfc
  0018622e: mov r0,r9
  00186230: blx 0x0006ebfc
  00186234: mov r0,r4
  00186236: add sp,#0x2c
  00186238: pop.w {r8,r9,r10,r11}
  0018623c: pop {r4,r5,r6,r7,pc}
```
