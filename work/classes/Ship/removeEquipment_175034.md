# Ship::removeEquipment
elf 0x175034 body 62
Sig: undefined __thiscall removeEquipment(Ship * this, Item * param_1)

## decompile
```c

/* Ship::removeEquipment(Item*) */

void __thiscall Ship::removeEquipment(Ship *this,Item *param_1)

{
  Item *this_00;
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(this + 0x6c);
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      this_00 = *(Item **)(puVar2[1] + uVar3 * 4);
      if (this_00 != (Item *)0x0) {
        iVar1 = Item::equals(this_00,param_1);
        puVar2 = *(uint **)(this + 0x6c);
        if (iVar1 != 0) {
          *(undefined4 *)(puVar2[1] + uVar3 * 4) = 0;
          return;
        }
      }
    }
  }
  return;
}

```
## target disasm
```
  00185034: push {r4,r5,r6,r7,lr}
  00185036: add r7,sp,#0xc
  00185038: push.w r11
  0018503c: mov r4,r1
  0018503e: ldr r1,[r0,#0x6c]
  00185040: mov r5,r0
  00185042: cbz r1,0x0018506c
  00185044: movs r6,#0x0
  00185046: b 0x0018505c
  00185048: ldr r0,[r1,#0x4]
  0018504a: ldr.w r0,[r0,r6,lsl #0x2]
  0018504e: cbz r0,0x0018505a
  00185050: mov r1,r4
  00185052: blx 0x000734e0
  00185056: ldr r1,[r5,#0x6c]
  00185058: cbnz r0,0x00185064
  0018505a: adds r6,#0x1
  0018505c: ldr r0,[r1,#0x0]
  0018505e: cmp r6,r0
  00185060: bcc 0x00185048
  00185062: b 0x0018506c
  00185064: ldr r0,[r1,#0x4]
  00185066: movs r1,#0x0
  00185068: str.w r1,[r0,r6,lsl #0x2]
  0018506c: pop.w r11
  00185070: pop {r4,r5,r6,r7,pc}
```
