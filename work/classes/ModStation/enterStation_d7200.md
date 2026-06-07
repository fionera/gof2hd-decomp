# ModStation::enterStation
elf 0xd7200 body 158
Sig: undefined __stdcall enterStation(void)

## decompile
```c

/* ModStation::enterStation() */

void ModStation::enterStation(void)

{
  int iVar1;
  Item *this;
  Item *this_00;
  undefined4 uVar2;
  Station *pSVar3;
  code *pcVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_000e72a0 + 0xe720e);
  pSVar3 = (Station *)*piVar5;
  Status::getStation();
  Status::departStation(pSVar3);
  Status::getStation();
  Station::visit();
  Achievements::applyNewMedals((Achievements *)**(undefined4 **)(DAT_000e72a4 + 0xe722e));
  pcVar4 = *(code **)(DAT_000e72a8 + 0xe723c);
  iVar1 = (*pcVar4)(*piVar5);
  this = (Item *)Ship::getFirstEquipmentOfSort(iVar1);
  iVar1 = (*pcVar4)(*piVar5);
  this_00 = (Item *)Ship::getFirstEquipmentOfSort(iVar1);
  (*pcVar4)(*piVar5);
  uVar2 = Ship::getIndex();
  iVar1 = *piVar5;
  *(undefined4 *)(iVar1 + 0x150) = uVar2;
  if (this == (Item *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = Item::getIndex(this);
    iVar1 = *piVar5;
  }
  *(undefined4 *)(iVar1 + 0x154) = uVar2;
  if (this_00 == (Item *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = Item::getIndex(this_00);
    iVar1 = *piVar5;
  }
  *(undefined4 *)(iVar1 + 0x124) = 0;
  *(undefined4 *)(iVar1 + 0x158) = uVar2;
  *(undefined4 *)(iVar1 + 0x11c) = 0;
  return;
}

```

## target disasm
```
  000e7200: push {r4,r5,r6,r7,lr}
  000e7202: add r7,sp,#0xc
  000e7204: push.w r8
  000e7208: ldr r0,[0x000e72a0]
  000e720a: add r0,pc
  000e720c: ldr r6,[r0,#0x0]
  000e720e: ldr r4,[r6,#0x0]
  000e7210: mov r0,r4
  000e7212: blx 0x00071c14
  000e7216: mov r1,r0
  000e7218: mov r0,r4
  000e721a: blx 0x000745fc
  000e721e: ldr r0,[r6,#0x0]
  000e7220: blx 0x00071c14
  000e7224: blx 0x0007504c
  000e7228: ldr r0,[0x000e72a4]
  000e722a: add r0,pc
  000e722c: ldr r0,[r0,#0x0]
  000e722e: ldr r0,[r0,#0x0]
  000e7230: blx 0x00075058
  000e7234: ldr r1,[0x000e72a8]
  000e7236: ldr r0,[r6,#0x0]
  000e7238: add r1,pc
  000e723a: ldr r4,[r1,#0x0]
  000e723c: blx r4
  000e723e: movs r1,#0xa
  000e7240: blx 0x0007228c
  000e7244: mov r5,r0
  000e7246: ldr r0,[r6,#0x0]
  000e7248: blx r4
  000e724a: movs r1,#0x9
  000e724c: blx 0x0007228c
  000e7250: mov r8,r0
  000e7252: ldr r0,[r6,#0x0]
  000e7254: blx r4
  000e7256: blx 0x000719c8
  000e725a: ldr r1,[r6,#0x0]
  000e725c: cmp r5,#0x0
  000e725e: str.w r0,[r1,#0x150]
  000e7262: beq 0x000e726e
  000e7264: mov r0,r5
  000e7266: blx 0x000718d8
  000e726a: ldr r1,[r6,#0x0]
  000e726c: b 0x000e7272
  000e726e: mov.w r0,#0xffffffff
  000e7272: cmp.w r8,#0x0
  000e7276: str.w r0,[r1,#0x154]
  000e727a: beq 0x000e7286
  000e727c: mov r0,r8
  000e727e: blx 0x000718d8
  000e7282: ldr r1,[r6,#0x0]
  000e7284: b 0x000e728a
  000e7286: mov.w r0,#0xffffffff
  000e728a: movs r2,#0x0
  000e728c: str.w r2,[r1,#0x124]
  000e7290: str.w r0,[r1,#0x158]
  000e7294: str.w r2,[r1,#0x11c]
  000e7298: pop.w r8
  000e729c: pop {r4,r5,r6,r7,pc}
```
