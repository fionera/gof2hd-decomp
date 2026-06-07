# Generator::generateStationIndex
elf 0x9672c body 332
Sig: undefined __thiscall generateStationIndex(Generator * this, Array * param_1, int param_2)

## decompile
```c

/* Generator::generateStationIndex(Array<SolarSystem*>*, int) */

int __thiscall Generator::generateStationIndex(Generator *this,Array *param_1,int param_2)

{
  int iVar1;
  SolarSystem *pSVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  byte bVar8;
  int *piVar9;
  undefined4 *puVar10;
  
  iVar7 = 0;
  piVar9 = *(int **)(DAT_000a6878 + 0xa6740);
  iVar6 = DAT_000a6880 + 0xa674c;
  puVar10 = *(undefined4 **)(DAT_000a687c + 0xa674a);
  bVar8 = 0;
  do {
    if (bVar8 != 0) {
      return iVar7;
    }
    iVar1 = AbyssEngine::AERandom::nextInt(*piVar9);
    iVar7 = param_2;
    if (0x13 < iVar1) {
      iVar7 = AbyssEngine::AERandom::nextInt(*piVar9);
      if (iVar7 < 0x28) {
        Status::getSystem();
        iVar7 = SolarSystem::getIndex();
        iVar7 = SolarSystem::getStations(*(SolarSystem **)(*(int *)(param_1 + 4) + iVar7 * 4));
        iVar1 = AbyssEngine::AERandom::nextInt(*piVar9);
        iVar7 = *(int *)(*(int *)(iVar7 + 4) + iVar1 * 4);
      }
      else {
        iVar7 = AbyssEngine::AERandom::nextInt(*piVar9);
      }
    }
    Status::getSystem();
    iVar1 = SolarSystem::getIndex();
    if (iVar1 == 0xf) {
      pSVar2 = (SolarSystem *)Status::getSystem();
      iVar7 = SolarSystem::getStations(pSVar2);
      iVar1 = *piVar9;
      pSVar2 = (SolarSystem *)Status::getSystem();
      SolarSystem::getStations(pSVar2);
      iVar1 = AbyssEngine::AERandom::nextInt(iVar1);
      iVar7 = *(int *)(*(int *)(iVar7 + 4) + iVar1 * 4);
    }
    uVar3 = 0;
    do {
      if (0x33 < uVar3) {
        bVar8 = 1;
        goto LAB_000a67fc;
      }
      iVar1 = uVar3 * 4;
      uVar3 = uVar3 + 1;
    } while (iVar7 != *(int *)(iVar6 + iVar1));
    bVar8 = 0;
LAB_000a67fc:
    for (uVar3 = 0; uVar3 < *(uint *)param_1; uVar3 = uVar3 + 1) {
      iVar1 = SolarSystem::stationIsInSystem
                        (*(SolarSystem **)(*(int *)(param_1 + 4) + uVar3 * 4),iVar7);
      if (iVar1 != 0) goto LAB_000a6822;
    }
    uVar3 = 0;
LAB_000a6822:
    if (iVar7 - 0x6dU < 5) {
      bVar8 = 0;
    }
    puVar4 = (uint *)Status::getSystemVisibilities((Status *)*puVar10);
    if ((puVar4 != (uint *)0x0) && (uVar3 < *puVar4)) {
      bVar8 = *(byte *)(puVar4[1] + uVar3);
    }
    iVar1 = SolarSystem::getRoutes();
    if (iVar1 == 0) {
      Status::getSystem();
      uVar5 = SolarSystem::getIndex();
      if (uVar3 != uVar5) {
        bVar8 = 0;
      }
    }
    bVar8 = bVar8 & 1;
  } while( true );
}

```

## target disasm
```
  000a672c: push {r4,r5,r6,r7,lr}
  000a672e: add r7,sp,#0xc
  000a6730: push {r7,r8,r9,r10,r11}
  000a6734: str r2,[sp,#0x0]
  000a6736: mov r5,r1
  000a6738: ldr r0,[0x000a6878]
  000a673a: movs r6,#0x0
  000a673c: add r0,pc
  000a673e: ldr.w r10,[r0,#0x0]
  000a6742: ldr r0,[0x000a687c]
  000a6744: ldr r4,[0x000a6880]
  000a6746: add r0,pc
  000a6748: add r4,pc
  000a674a: ldr.w r11,[r0,#0x0]
  000a674e: movs r0,#0x0
  000a6750: b 0x000a686a
  000a6752: ldr.w r0,[r10,#0x0]
  000a6756: movs r1,#0x64
  000a6758: blx 0x00071848
  000a675c: ldr r6,[sp,#0x0]
  000a675e: cmp r0,#0x14
  000a6760: blt 0x000a67a6
  000a6762: ldr.w r0,[r10,#0x0]
  000a6766: movs r1,#0x64
  000a6768: blx 0x00071848
  000a676c: cmp r0,#0x27
  000a676e: bgt 0x000a679a
  000a6770: ldr.w r0,[r11,#0x0]
  000a6774: blx 0x00071a10
  000a6778: blx 0x00071a7c
  000a677c: ldr r1,[r5,#0x4]
  000a677e: ldr.w r0,[r1,r0,lsl #0x2]
  000a6782: blx 0x00071a88
  000a6786: mov r6,r0
  000a6788: ldr r1,[r0,#0x0]
  000a678a: ldr.w r0,[r10,#0x0]
  000a678e: blx 0x00071848
  000a6792: ldr r1,[r6,#0x4]
  000a6794: ldr.w r6,[r1,r0,lsl #0x2]
  000a6798: b 0x000a67a6
  000a679a: ldr.w r0,[r10,#0x0]
  000a679e: movs r1,#0x87
  000a67a0: blx 0x00071848
  000a67a4: mov r6,r0
  000a67a6: ldr.w r0,[r11,#0x0]
  000a67aa: blx 0x00071a10
  000a67ae: blx 0x00071a7c
  000a67b2: cmp r0,#0xf
  000a67b4: bne 0x000a67e2
  000a67b6: ldr.w r0,[r11,#0x0]
  000a67ba: blx 0x00071a10
  000a67be: blx 0x00071a88
  000a67c2: mov r6,r0
  000a67c4: ldr.w r0,[r11,#0x0]
  000a67c8: ldr.w r9,[r10,#0x0]
  000a67cc: blx 0x00071a10
  000a67d0: blx 0x00071a88
  000a67d4: ldr r1,[r0,#0x0]
  000a67d6: mov r0,r9
  000a67d8: blx 0x00071848
  000a67dc: ldr r1,[r6,#0x4]
  000a67de: ldr.w r6,[r1,r0,lsl #0x2]
  000a67e2: movs r0,#0x0
  000a67e4: cmp r0,#0x33
  000a67e6: bhi 0x000a67f8
  000a67e8: ldr.w r1,[r4,r0,lsl #0x2]
  000a67ec: adds r0,#0x1
  000a67ee: cmp r6,r1
  000a67f0: bne 0x000a67e4
  000a67f2: mov.w r9,#0x0
  000a67f6: b 0x000a67fc
  000a67f8: mov.w r9,#0x1
  000a67fc: mov.w r8,#0x0
  000a6800: b 0x000a6806
  000a6802: add.w r8,r8,#0x1
  000a6806: ldr r0,[r5,#0x0]
  000a6808: cmp r8,r0
  000a680a: bcs 0x000a681e
  000a680c: ldr r0,[r5,#0x4]
  000a680e: mov r1,r6
  000a6810: ldr.w r0,[r0,r8,lsl #0x2]
  000a6814: blx 0x00071a94
  000a6818: cmp r0,#0x0
  000a681a: beq 0x000a6802
  000a681c: b 0x000a6822
  000a681e: mov.w r8,#0x0
  000a6822: ldr.w r0,[r11,#0x0]
  000a6826: sub.w r1,r6,#0x6d
  000a682a: cmp r1,#0x5
  000a682c: it cc
  000a682e: mov.cc.w r9,#0x0
  000a6832: blx 0x00071aa0
  000a6836: cbz r0,0x000a6844
  000a6838: ldr r1,[r0,#0x0]
  000a683a: cmp r1,r8
  000a683c: itt hi
  000a683e: ldr.hi r0,[r0,#0x4]
  000a6840: ldrb.hi.w r9,[r0,r8]
  000a6844: ldr r0,[r5,#0x4]
  000a6846: ldr.w r0,[r0,r8,lsl #0x2]
  000a684a: blx 0x00071aac
  000a684e: cbnz r0,0x000a6866
  000a6850: ldr.w r0,[r11,#0x0]
  000a6854: blx 0x00071a10
  000a6858: blx 0x00071a7c
  000a685c: cmp r8,r0
  000a685e: mov.w r0,#0x0
  000a6862: it ne
  000a6864: mov.ne r9,r0
  000a6866: and r0,r9,#0x1
  000a686a: lsls r0,r0,#0x1f
  000a686c: beq.w 0x000a6752
  000a6870: mov r0,r6
  000a6872: pop.w {r3,r8,r9,r10,r11}
  000a6876: pop {r4,r5,r6,r7,pc}
```
