# Ship::getFirstEquipmentOfSort
elf 0x174ce4 body 92
Sig: undefined __stdcall getFirstEquipmentOfSort(int param_1)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Ship::getFirstEquipmentOfSort(int) */

undefined4 Ship::getFirstEquipmentOfSort(int param_1)

{
  Item *this;
  int iVar1;
  int in_r1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(param_1 + 0x6c);
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      this = *(Item **)(puVar2[1] + uVar3 * 4);
      if (this != (Item *)0x0) {
        iVar1 = Item::getSort(this);
        puVar2 = *(uint **)(param_1 + 0x6c);
        if (iVar1 == in_r1) {
          return *(undefined4 *)(puVar2[1] + uVar3 * 4);
        }
      }
    }
    if ((in_r1 == 0x15) && (*(int *)param_1 == 0x31 || *(int *)param_1 == 0x2c)) {
      return *(undefined4 *)(*(int *)(**(int **)(_FUN_00184d40 + 0x184d26) + 4) + 0x17c);
    }
  }
  return 0;
}

```
## target disasm
```
  00184ce4: push {r4,r5,r6,r7,lr}
  00184ce6: add r7,sp,#0xc
  00184ce8: push.w r11
  00184cec: mov r5,r1
  00184cee: ldr r1,[r0,#0x6c]
  00184cf0: mov r4,r0
  00184cf2: cbz r1,0x00184d30
  00184cf4: movs r6,#0x0
  00184cf6: b 0x00184d0c
  00184cf8: ldr r0,[r1,#0x4]
  00184cfa: ldr.w r0,[r0,r6,lsl #0x2]
  00184cfe: cbz r0,0x00184d0a
  00184d00: blx 0x00071908
  00184d04: ldr r1,[r4,#0x6c]
  00184d06: cmp r0,r5
  00184d08: beq 0x00184d34
  00184d0a: adds r6,#0x1
  00184d0c: ldr r0,[r1,#0x0]
  00184d0e: cmp r6,r0
  00184d10: bcc 0x00184cf8
  00184d12: cmp r5,#0x15
  00184d14: bne 0x00184d30
  00184d16: ldr r0,[r4,#0x0]
  00184d18: cmp r0,#0x31
  00184d1a: it ne
  00184d1c: cmp.ne r0,#0x2c
  00184d1e: bne 0x00184d30
  00184d20: ldr r0,[0x00184d40]
  00184d22: add r0,pc
  00184d24: ldr r0,[r0,#0x0]
  00184d26: ldr r0,[r0,#0x0]
  00184d28: ldr r0,[r0,#0x4]
  00184d2a: ldr.w r0,[r0,#0x17c]
  00184d2e: b 0x00184d3a
  00184d30: movs r0,#0x0
  00184d32: b 0x00184d3a
  00184d34: ldr r0,[r1,#0x4]
  00184d36: ldr.w r0,[r0,r6,lsl #0x2]
  00184d3a: pop.w r11
  00184d3e: pop {r4,r5,r6,r7,pc}
```
