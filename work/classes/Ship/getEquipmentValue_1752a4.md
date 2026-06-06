# Ship::getEquipmentValue
elf 0x1752a4 body 48
Sig: undefined __thiscall getEquipmentValue(Ship * this)

## decompile
```c

/* Ship::getEquipmentValue() */

int __thiscall Ship::getEquipmentValue(Ship *this)

{
  Item *this_00;
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = 0;
  for (uVar3 = 0; uVar3 < **(uint **)(this + 0x6c); uVar3 = uVar3 + 1) {
    this_00 = *(Item **)((*(uint **)(this + 0x6c))[1] + uVar3 * 4);
    if (this_00 != (Item *)0x0) {
      iVar1 = Item::getTotalPrice(this_00);
      iVar2 = iVar2 + iVar1;
    }
  }
  return iVar2;
}

```
## target disasm
```
  001852a4: push {r4,r5,r6,r7,lr}
  001852a6: add r7,sp,#0xc
  001852a8: push.w r11
  001852ac: mov r5,r0
  001852ae: movs r4,#0x0
  001852b0: movs r6,#0x0
  001852b2: b 0x001852c4
  001852b4: ldr r0,[r0,#0x4]
  001852b6: ldr.w r0,[r0,r6,lsl #0x2]
  001852ba: cbz r0,0x001852c2
  001852bc: blx 0x00077e6c
  001852c0: add r4,r0
  001852c2: adds r6,#0x1
  001852c4: ldr r0,[r5,#0x6c]
  001852c6: ldr r1,[r0,#0x0]
  001852c8: cmp r6,r1
  001852ca: bcc 0x001852b4
  001852cc: mov r0,r4
  001852ce: pop.w r11
  001852d2: pop {r4,r5,r6,r7,pc}
```
