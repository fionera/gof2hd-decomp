# Hud::checkIfQuickMenuIsEmpty
elf 0x1613d0 body 106
Sig: undefined __stdcall checkIfQuickMenuIsEmpty(void)

## decompile
```c

/* Hud::checkIfQuickMenuIsEmpty() */

void Hud::checkIfQuickMenuIsEmpty(void)

{
  byte bVar1;
  int in_r0;
  Ship *this;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  
  this = (Ship *)Status::getShip();
  puVar2 = (uint *)Ship::getEquipment(this,1);
  *(uint **)(in_r0 + 0x25c) = puVar2;
  if (puVar2 == (uint *)0x0) {
LAB_00171406:
    Status::getShip();
    iVar3 = Ship::hasJumpDrive();
    if ((iVar3 == 0) && (iVar3 = Status::getWingmen(), iVar3 == 0)) {
      Status::getShip();
      bVar1 = Ship::hasCloak();
      bVar1 = bVar1 ^ 1;
      goto LAB_0017141c;
    }
  }
  else {
    uVar4 = 0;
    do {
      if (*puVar2 <= uVar4) goto LAB_00171406;
      iVar3 = uVar4 * 4;
      uVar4 = uVar4 + 1;
    } while (*(int *)(puVar2[1] + iVar3) == 0);
  }
  bVar1 = 0;
LAB_0017141c:
  *(byte *)(in_r0 + 0x283) = bVar1;
  (*(code *)(DAT_001ac644 + 0x1ac648))();
  return;
}

```

## target disasm
```
  001713d0: push {r4,r5,r7,lr}
  001713d2: add r7,sp,#0x8
  001713d4: mov r4,r0
  001713d6: ldr r0,[0x0017143c]
  001713d8: add r0,pc
  001713da: ldr r5,[r0,#0x0]
  001713dc: ldr r0,[r5,#0x0]
  001713de: blx 0x00071a58
  001713e2: movs r1,#0x1
  001713e4: blx 0x00072118
  001713e8: cmp r0,#0x0
  001713ea: str.w r0,[r4,#0x25c]
  001713ee: beq 0x00171406
  001713f0: ldr r1,[r0,#0x0]
  001713f2: movs r2,#0x0
  001713f4: cmp r2,r1
  001713f6: bcs 0x00171406
  001713f8: ldr r3,[r0,#0x4]
  001713fa: ldr.w r3,[r3,r2,lsl #0x2]
  001713fe: adds r2,#0x1
  00171400: cmp r3,#0x0
  00171402: beq 0x001713f4
  00171404: b 0x0017141a
  00171406: ldr r0,[r5,#0x0]
  00171408: blx 0x00071a58
  0017140c: blx 0x00071a64
  00171410: cbnz r0,0x0017141a
  00171412: ldr r0,[r5,#0x0]
  00171414: blx 0x000741dc
  00171418: cbz r0,0x0017142a
  0017141a: movs r0,#0x0
  0017141c: strb.w r0,[r4,#0x283]
  00171420: mov r0,r4
  00171422: pop.w {r4,r5,r7,lr}
  00171426: b.w 0x001ac638
  0017142a: ldr r0,[r5,#0x0]
  0017142c: blx 0x00071a58
  00171430: blx 0x000779e0
  00171434: eor r0,r0,#0x1
  00171438: b 0x0017141c
```
