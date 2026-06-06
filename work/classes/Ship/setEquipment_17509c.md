# Ship::setEquipment
elf 0x17509c body 48
Sig: undefined __thiscall setEquipment(Ship * this, Item * param_1)

## decompile
```c

/* Ship::setEquipment(Item*) */

void __thiscall Ship::setEquipment(Ship *this,Item *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = 0;
  do {
    if (**(uint **)(this + 0x6c) <= uVar3) goto LAB_001850c4;
    uVar2 = (*(uint **)(this + 0x6c))[1];
    iVar1 = uVar3 * 4;
    uVar3 = uVar3 + 1;
  } while (*(int *)(uVar2 + iVar1) != 0);
  *(Item **)(uVar2 + uVar3 * 4 + -4) = param_1;
LAB_001850c4:
  (*(code *)(DAT_001abf14 + 0x1abf18))();
  return;
}

```
## target disasm
```
  0018509c: push {r4,r6,r7,lr}
  0018509e: add r7,sp,#0x8
  001850a0: ldr.w lr,[r0,#0x6c]
  001850a4: movs r3,#0x0
  001850a6: ldr.w r12,[lr,#0x0]
  001850aa: cmp r3,r12
  001850ac: bcs 0x001850c4
  001850ae: ldr.w r2,[lr,#0x4]
  001850b2: ldr.w r4,[r2,r3,lsl #0x2]
  001850b6: adds r3,#0x1
  001850b8: cmp r4,#0x0
  001850ba: bne 0x001850aa
  001850bc: add.w r2,r2,r3, lsl #0x2
  001850c0: str.w r1,[r2,#-0x4]
  001850c4: pop.w {r4,r6,r7,lr}
  001850c8: b.w 0x001abf08
```
