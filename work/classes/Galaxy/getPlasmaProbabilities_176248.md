# Galaxy::getPlasmaProbabilities
elf 0x176248 body 378
Sig: undefined __thiscall getPlasmaProbabilities(Galaxy * this, Station * param_1)

## decompile
```c

/* Galaxy::getPlasmaProbabilities(Station*) */

void * __thiscall Galaxy::getPlasmaProbabilities(Galaxy *this,Station *param_1)

{
  bool bVar1;
  int iVar2;
  AEArray *pAVar3;
  int iVar4;
  void *pvVar5;
  void *pvVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  void *pvVar11;
  uint uVar12;
  bool bVar13;
  undefined4 uVar14;
  int iVar15;
  undefined4 uVar16;
  int iVar17;
  
  iVar2 = Status::inAlienOrbit();
  if (iVar2 == 0) {
    pAVar3 = this->systems;
  }
  else {
    pAVar3 = (AEArray *)0x0;
  }
  iVar4 = **(int **)(DAT_001863c8 + 0x186276);
  pvVar5 = operator_new__(0x10);
  pvVar6 = operator_new__(0x10);
  iVar15 = 0;
  for (iVar17 = 0xc9; iVar17 != 0xcd; iVar17 = iVar17 + 1) {
    *(int *)((int)pvVar6 + iVar15 * 4) = iVar17;
    if (iVar2 == 0) {
      iVar8 = Station::getSystem(param_1);
      iVar8 = SolarSystem::getX(*(SolarSystem **)((int)pAVar3->data + iVar8 * 4));
      iVar7 = Station::getSystem(param_1);
      iVar7 = SolarSystem::getY(*(SolarSystem **)((int)pAVar3->data + iVar7 * 4));
      iVar9 = Item::getMinPriceSystem(*(Item **)(*(int *)(iVar4 + 4) + iVar17 * 4));
      iVar9 = SolarSystem::getX(*(SolarSystem **)((int)pAVar3->data + iVar9 * 4));
      iVar10 = Item::getMinPriceSystem(*(Item **)(*(int *)(iVar4 + 4) + iVar17 * 4));
      SolarSystem::getY(*(SolarSystem **)((int)pAVar3->data + iVar10 * 4));
      iVar7 = invDistancePercent((int)this,iVar8,iVar7,iVar9);
      iVar8 = iVar15 + 1;
      if (iVar7 < 0x32) {
        iVar7 = 0;
      }
    }
    else {
      iVar7 = 0;
      iVar8 = iVar15;
    }
    *(int *)((int)pvVar5 + iVar15 * 4) = iVar7;
    iVar15 = iVar8;
  }
  bVar13 = true;
  iVar2 = 1;
  do {
    for (; iVar2 != 4; iVar2 = iVar2 + 1) {
      iVar15 = iVar2 + -1;
      iVar17 = *(int *)((int)pvVar5 + iVar2 * 4);
      iVar4 = *(int *)((int)pvVar5 + iVar15 * 4);
      if (iVar4 < iVar17) {
        *(int *)((int)pvVar5 + iVar15 * 4) = iVar17;
        uVar14 = *(undefined4 *)((int)pvVar6 + iVar15 * 4);
        uVar16 = *(undefined4 *)((int)pvVar6 + iVar2 * 4);
        *(int *)((int)pvVar5 + iVar2 * 4) = iVar4;
        *(undefined4 *)((int)pvVar6 + iVar15 * 4) = uVar16;
        *(undefined4 *)((int)pvVar6 + iVar2 * 4) = uVar14;
        bVar13 = false;
      }
    }
    bVar1 = !bVar13;
    bVar13 = true;
    iVar2 = 1;
  } while (bVar1);
  iVar2 = 0;
  for (iVar15 = 0; iVar15 != 4; iVar15 = iVar15 + 1) {
    iVar4 = *(int *)((int)pvVar5 + iVar15 * 4);
    if (0 < iVar4) {
      *(int *)((int)pvVar5 + iVar15 * 4) = iVar4 + iVar2;
    }
    iVar2 = iVar2 + -2;
  }
  pvVar11 = operator_new__(0x20);
  for (uVar12 = 0; uVar12 < 8; uVar12 = uVar12 + 2) {
    *(undefined4 *)((int)pvVar11 + uVar12 * 4) = *(undefined4 *)((int)pvVar6 + uVar12 * 2);
    *(undefined4 *)((int)pvVar11 + uVar12 * 4 + 4) = *(undefined4 *)((int)pvVar5 + uVar12 * 2);
  }
  operator_delete__(pvVar5);
  operator_delete__(pvVar6);
  return pvVar11;
}

```

## target disasm
```
  00186248: push {r4,r5,r6,r7,lr}
  0018624a: add r7,sp,#0xc
  0018624c: push {r8,r9,r10,r11}
  00186250: sub sp,#0x24
  00186252: str r1,[sp,#0x18]
  00186254: mov r4,r0
  00186256: ldr r0,[0x001863c4]
  00186258: add r0,pc
  0018625a: ldr r0,[r0,#0x0]
  0018625c: ldr r0,[r0,#0x0]
  0018625e: blx 0x000723d0
  00186262: str r4,[sp,#0x14]
  00186264: mov r6,r0
  00186266: cmp r0,#0x0
  00186268: ite eq
  0018626a: ldr.eq r0,[r4,#0x4]
  0018626c: mov.ne r0,#0x0
  0018626e: str r0,[sp,#0xc]
  00186270: ldr r0,[0x001863c8]
  00186272: add r0,pc
  00186274: ldr r0,[r0,#0x0]
  00186276: ldr r0,[r0,#0x0]
  00186278: str r0,[sp,#0x8]
  0018627a: movs r0,#0x10
  0018627c: blx 0x0006ec08
  00186280: mov r4,r0
  00186282: movs r0,#0x10
  00186284: blx 0x0006ec08
  00186288: mov r5,r0
  0018628a: mov.w r10,#0xc9
  0018628e: mov.w r9,#0x0
  00186292: str r6,[sp,#0x10]
  00186294: b 0x00186326
  00186296: cmp r6,#0x0
  00186298: str.w r10,[r5,r9,lsl #0x2]
  0018629c: beq 0x001862a4
  0018629e: movs r0,#0x0
  001862a0: mov r1,r9
  001862a2: b 0x0018631c
  001862a4: ldr r6,[sp,#0x18]
  001862a6: mov r0,r6
  001862a8: blx 0x0007189c
  001862ac: ldr.w r11,[sp,#0xc]
  001862b0: ldr.w r1,[r11,#0x4]
  001862b4: ldr.w r0,[r1,r0,lsl #0x2]
  001862b8: blx 0x00071968
  001862bc: str r0,[sp,#0x20]
  001862be: mov r0,r6
  001862c0: blx 0x0007189c
  001862c4: ldr.w r1,[r11,#0x4]
  001862c8: ldr.w r0,[r1,r0,lsl #0x2]
  001862cc: blx 0x00071974
  001862d0: ldr r6,[sp,#0x8]
  001862d2: str r0,[sp,#0x1c]
  001862d4: ldr r0,[r6,#0x4]
  001862d6: ldr.w r0,[r0,r10,lsl #0x2]
  001862da: blx 0x0007195c
  001862de: ldr.w r1,[r11,#0x4]
  001862e2: ldr.w r0,[r1,r0,lsl #0x2]
  001862e6: blx 0x00071968
  001862ea: mov r8,r0
  001862ec: ldr r0,[r6,#0x4]
  001862ee: ldr.w r0,[r0,r10,lsl #0x2]
  001862f2: blx 0x0007195c
  001862f6: ldr.w r1,[r11,#0x4]
  001862fa: ldr.w r0,[r1,r0,lsl #0x2]
  001862fe: blx 0x00071974
  00186302: str r0,[sp,#0x0]
  00186304: mov r3,r8
  00186306: ldrd r2,r1,[sp,#0x1c]
  0018630a: ldrd r6,r0,[sp,#0x10]
  0018630e: blx 0x00071980
  00186312: add.w r1,r9,#0x1
  00186316: cmp r0,#0x32
  00186318: it lt
  0018631a: mov.lt r0,#0x0
  0018631c: str.w r0,[r4,r9,lsl #0x2]
  00186320: add.w r10,r10,#0x1
  00186324: mov r9,r1
  00186326: cmp.w r10,#0xcd
  0018632a: bne 0x00186296
  0018632c: movs r1,#0x1
  0018632e: movs r0,#0x1
  00186330: cmp r0,#0x4
  00186332: beq 0x00186360
  00186334: subs r2,r0,#0x1
  00186336: ldr.w r6,[r4,r0,lsl #0x2]
  0018633a: ldr.w r3,[r4,r2,lsl #0x2]
  0018633e: cmp r3,r6
  00186340: bge 0x0018635c
  00186342: str.w r6,[r4,r2,lsl #0x2]
  00186346: ldr.w r1,[r5,r2,lsl #0x2]
  0018634a: ldr.w r6,[r5,r0,lsl #0x2]
  0018634e: str.w r3,[r4,r0,lsl #0x2]
  00186352: str.w r6,[r5,r2,lsl #0x2]
  00186356: str.w r1,[r5,r0,lsl #0x2]
  0018635a: movs r1,#0x0
  0018635c: adds r0,#0x1
  0018635e: b 0x00186330
  00186360: lsls r0,r1,#0x1f
  00186362: mov.w r1,#0x1
  00186366: mov.w r0,#0x1
  0018636a: beq 0x00186330
  0018636c: movs r0,#0x0
  0018636e: movs r1,#0x0
  00186370: b 0x00186384
  00186372: ldr.w r2,[r4,r1,lsl #0x2]
  00186376: cmp r2,#0x1
  00186378: itt ge
  0018637a: add.ge r2,r0
  0018637c: str.ge.w r2,[r4,r1,lsl #0x2]
  00186380: subs r0,#0x2
  00186382: adds r1,#0x1
  00186384: cmp r1,#0x4
  00186386: bne 0x00186372
  00186388: movs r0,#0x20
  0018638a: blx 0x0006ec08
  0018638e: mov r6,r0
  00186390: movs r0,#0x0
  00186392: b 0x001863a8
  00186394: ldr.w r1,[r5,r0,lsl #0x1]
  00186398: add.w r2,r6,r0, lsl #0x2
  0018639c: str.w r1,[r6,r0,lsl #0x2]
  001863a0: ldr.w r1,[r4,r0,lsl #0x1]
  001863a4: adds r0,#0x2
  001863a6: str r1,[r2,#0x4]
  001863a8: cmp r0,#0x7
  001863aa: bls 0x00186394
  001863ac: mov r0,r4
  001863ae: blx 0x0006ebfc
  001863b2: mov r0,r5
  001863b4: blx 0x0006ebfc
  001863b8: mov r0,r6
  001863ba: add sp,#0x24
  001863bc: pop.w {r8,r9,r10,r11}
  001863c0: pop {r4,r5,r6,r7,pc}
```
