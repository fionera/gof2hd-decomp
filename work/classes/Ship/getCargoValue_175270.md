# Ship::getCargoValue
elf 0x175270 body 52
Sig: undefined __thiscall getCargoValue(Ship * this)

## decompile
```c

/* Ship::getCargoValue() */

int __thiscall Ship::getCargoValue(Ship *this)

{
  Item *this_00;
  int iVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  
  puVar2 = *(uint **)(this + 0x70);
  iVar3 = 0;
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      this_00 = *(Item **)(puVar2[1] + uVar4 * 4);
      if (this_00 != (Item *)0x0) {
        iVar1 = Item::getTotalPrice(this_00);
        puVar2 = *(uint **)(this + 0x70);
        iVar3 = iVar3 + iVar1;
      }
    }
  }
  return iVar3;
}

```
## target disasm
```
  00185270: push {r4,r5,r6,r7,lr}
  00185272: add r7,sp,#0xc
  00185274: push.w r11
  00185278: ldr r1,[r0,#0x70]
  0018527a: mov r5,r0
  0018527c: movs r4,#0x0
  0018527e: cbz r1,0x0018529c
  00185280: movs r6,#0x0
  00185282: b 0x00185296
  00185284: ldr r0,[r1,#0x4]
  00185286: ldr.w r0,[r0,r6,lsl #0x2]
  0018528a: cbz r0,0x00185294
  0018528c: blx 0x00077e6c
  00185290: ldr r1,[r5,#0x70]
  00185292: add r4,r0
  00185294: adds r6,#0x1
  00185296: ldr r0,[r1,#0x0]
  00185298: cmp r6,r0
  0018529a: bcc 0x00185284
  0018529c: mov r0,r4
  0018529e: pop.w r11
  001852a2: pop {r4,r5,r6,r7,pc}
```
