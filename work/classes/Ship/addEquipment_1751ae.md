# Ship::addEquipment
elf 0x1751ae body 86
Sig: undefined __thiscall addEquipment(Ship * this, Item * param_1)

## decompile
```c

/* Ship::addEquipment(Item*) */

undefined4 __thiscall Ship::addEquipment(Ship *this,Item *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = Item::getType(param_1);
  iVar4 = *(int *)(*(int *)(this + 0x68) + iVar1 * 4);
  if (iVar4 < 1) {
LAB_001851fc:
    uVar2 = 0;
  }
  else {
    iVar3 = 0;
    for (iVar5 = 0; iVar5 < iVar1; iVar5 = iVar5 + 1) {
      iVar3 = iVar3 + *(int *)(*(int *)(this + 0x68) + iVar5 * 4);
    }
    iVar4 = iVar3 + iVar4;
    do {
      if (iVar4 <= iVar3) goto LAB_001851fc;
      iVar1 = iVar3 * 4;
      iVar3 = iVar3 + 1;
    } while (*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + iVar1) != 0);
    *(Item **)(*(int *)(*(int *)(this + 0x6c) + 4) + iVar3 * 4 + -4) = param_1;
    uVar2 = 1;
  }
  return uVar2;
}

```
## target disasm
```
  001851ae: push {r4,r5,r6,r7,lr}
  001851b0: add r7,sp,#0xc
  001851b2: push.w r8
  001851b6: mov r5,r0
  001851b8: mov r0,r1
  001851ba: mov r8,r1
  001851bc: blx 0x000718fc
  001851c0: ldr r3,[r5,#0x68]
  001851c2: ldr.w r2,[r3,r0,lsl #0x2]
  001851c6: cmp r2,#0x1
  001851c8: blt 0x001851fc
  001851ca: movs r1,#0x0
  001851cc: movs r4,#0x0
  001851ce: b 0x001851d8
  001851d0: ldr.w r6,[r3,r4,lsl #0x2]
  001851d4: adds r4,#0x1
  001851d6: add r1,r6
  001851d8: cmp r4,r0
  001851da: blt 0x001851d0
  001851dc: adds r0,r1,r2
  001851de: cmp r1,r0
  001851e0: bge 0x001851fc
  001851e2: ldr r2,[r5,#0x6c]
  001851e4: ldr r2,[r2,#0x4]
  001851e6: ldr.w r3,[r2,r1,lsl #0x2]
  001851ea: adds r1,#0x1
  001851ec: cmp r3,#0x0
  001851ee: bne 0x001851de
  001851f0: add.w r0,r2,r1, lsl #0x2
  001851f4: str.w r8,[r0,#-0x4]
  001851f8: movs r0,#0x1
  001851fa: b 0x001851fe
  001851fc: movs r0,#0x0
  001851fe: pop.w r8
  00185202: pop {r4,r5,r6,r7,pc}
```
