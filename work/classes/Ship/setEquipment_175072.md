# Ship::setEquipment
elf 0x175072 body 42
Sig: undefined __thiscall setEquipment(Ship * this, Array * param_1)

## decompile
```c

/* Ship::setEquipment(Array<Item*>*) */

void __thiscall Ship::setEquipment(Ship *this,Array *param_1)

{
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < *(uint *)param_1; uVar1 = uVar1 + 1) {
    setEquipment(this,*(Item **)(*(int *)(param_1 + 4) + uVar1 * 4));
  }
  return;
}

```
## target disasm
```
  00185072: push {r4,r5,r6,r7,lr}
  00185074: add r7,sp,#0xc
  00185076: push.w r11
  0018507a: mov r4,r1
  0018507c: mov r5,r0
  0018507e: movs r6,#0x0
  00185080: b 0x00185090
  00185082: ldr r0,[r4,#0x4]
  00185084: ldr.w r1,[r0,r6,lsl #0x2]
  00185088: mov r0,r5
  0018508a: blx 0x00077e90
  0018508e: adds r6,#0x1
  00185090: ldr r0,[r4,#0x0]
  00185092: cmp r6,r0
  00185094: bcc 0x00185082
  00185096: pop.w r11
  0018509a: pop {r4,r5,r6,r7,pc}
```
