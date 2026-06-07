# MGame::nextCamId
elf 0x17c624 body 144
Sig: undefined __stdcall nextCamId(int param_1)

## decompile
```c

/* MGame::nextCamId(int) */

int MGame::nextCamId(int param_1)

{
  int iVar1;
  int iVar2;
  int in_r1;
  
  iVar1 = in_r1 + 1;
  if (iVar1 == 2) {
    iVar1 = in_r1 + 2;
  }
  if (iVar1 != 1) goto LAB_0018c666;
  iVar1 = Status::getShip();
  iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
  if (iVar1 == 0) {
    iVar1 = Status::getShip();
    iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
    if (iVar1 != 0) goto LAB_0018c658;
  }
  else {
LAB_0018c658:
    iVar1 = PlayerEgo::hasAutoTurret();
    if (iVar1 == 0) {
      iVar1 = 1;
      goto LAB_0018c666;
    }
  }
  iVar1 = 2;
LAB_0018c666:
  if (iVar1 == 2) {
    iVar1 = 3;
  }
  if (3 < iVar1) {
    iVar1 = PlayerEgo::isDockedToDockingPoint();
    if (iVar1 == 0) {
      return 0;
    }
    iVar1 = Status::getShip();
    iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
    if (iVar1 == 0) {
      iVar1 = Status::getShip();
      iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
      if (iVar1 == 0) {
        return 3;
      }
    }
    iVar2 = PlayerEgo::hasAutoTurret();
    iVar1 = 1;
    if (iVar2 != 0) {
      iVar1 = 3;
    }
  }
  return iVar1;
}

```

## target disasm
```
  0018c624: push {r4,r5,r7,lr}
  0018c626: add r7,sp,#0x8
  0018c628: mov r4,r0
  0018c62a: adds r0,r1,#0x1
  0018c62c: cmp r0,#0x2
  0018c62e: it eq
  0018c630: add.eq r0,r1,#0x2
  0018c632: cmp r0,#0x1
  0018c634: bne 0x0018c666
  0018c636: ldr r0,[0x0018c6b4]
  0018c638: add r0,pc
  0018c63a: ldr r5,[r0,#0x0]
  0018c63c: ldr r0,[r5,#0x0]
  0018c63e: blx 0x00071a58
  0018c642: movs r1,#0x8
  0018c644: blx 0x0007228c
  0018c648: cbnz r0,0x0018c658
  0018c64a: ldr r0,[r5,#0x0]
  0018c64c: blx 0x00071a58
  0018c650: movs r1,#0x23
  0018c652: blx 0x0007228c
  0018c656: cbz r0,0x0018c660
  0018c658: ldr r0,[r4,#0x58]
  0018c65a: blx 0x00077b6c
  0018c65e: cbz r0,0x0018c664
  0018c660: movs r0,#0x2
  0018c662: b 0x0018c666
  0018c664: movs r0,#0x1
  0018c666: cmp r0,#0x2
  0018c668: it eq
  0018c66a: add.eq r0,#0x1
  0018c66c: cmp r0,#0x4
  0018c66e: blt 0x0018c6aa
  0018c670: ldr r0,[r4,#0x58]
  0018c672: blx 0x00075928
  0018c676: cbz r0,0x0018c6ac
  0018c678: ldr r0,[0x0018c6b8]
  0018c67a: add r0,pc
  0018c67c: ldr r5,[r0,#0x0]
  0018c67e: ldr r0,[r5,#0x0]
  0018c680: blx 0x00071a58
  0018c684: movs r1,#0x8
  0018c686: blx 0x0007228c
  0018c68a: cbnz r0,0x0018c69a
  0018c68c: ldr r0,[r5,#0x0]
  0018c68e: blx 0x00071a58
  0018c692: movs r1,#0x23
  0018c694: blx 0x0007228c
  0018c698: cbz r0,0x0018c6b0
  0018c69a: ldr r0,[r4,#0x58]
  0018c69c: blx 0x00077b6c
  0018c6a0: mov r1,r0
  0018c6a2: movs r0,#0x1
  0018c6a4: cmp r1,#0x0
  0018c6a6: it ne
  0018c6a8: mov.ne r0,#0x3
  0018c6aa: pop {r4,r5,r7,pc}
  0018c6ac: movs r0,#0x0
  0018c6ae: pop {r4,r5,r7,pc}
  0018c6b0: movs r0,#0x3
  0018c6b2: pop {r4,r5,r7,pc}
```
