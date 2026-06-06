# Ship::hasEquipment
elf 0x174d44 body 76
Sig: undefined __stdcall hasEquipment(int param_1, int param_2)

## decompile
```c

/* Ship::hasEquipment(int, int) */

undefined4 Ship::hasEquipment(int param_1,int param_2)

{
  uint *puVar1;
  Item *this;
  int iVar2;
  int in_r2;
  uint uVar3;
  
  puVar1 = *(uint **)(param_1 + 0x6c);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      this = *(Item **)(puVar1[1] + uVar3 * 4);
      if (((this != (Item *)0x0) && (iVar2 = Item::getIndex(this), iVar2 == param_2)) &&
         (iVar2 = Item::getAmount(), in_r2 <= iVar2)) {
        return 1;
      }
      puVar1 = *(uint **)(param_1 + 0x6c);
    }
  }
  return 0;
}

```
## target disasm
```
  00184d44: push {r4,r5,r6,r7,lr}
  00184d46: add r7,sp,#0xc
  00184d48: push.w r8
  00184d4c: mov r6,r0
  00184d4e: ldr r0,[r0,#0x6c]
  00184d50: mov r8,r2
  00184d52: mov r5,r1
  00184d54: cbz r0,0x00184d84
  00184d56: movs r4,#0x0
  00184d58: b 0x00184d7e
  00184d5a: ldr r0,[r0,#0x4]
  00184d5c: ldr.w r0,[r0,r4,lsl #0x2]
  00184d60: cbz r0,0x00184d7a
  00184d62: blx 0x000718d8
  00184d66: cmp r0,r5
  00184d68: bne 0x00184d7a
  00184d6a: ldr r0,[r6,#0x6c]
  00184d6c: ldr r0,[r0,#0x4]
  00184d6e: ldr.w r0,[r0,r4,lsl #0x2]
  00184d72: blx 0x0007183c
  00184d76: cmp r0,r8
  00184d78: bge 0x00184d8c
  00184d7a: ldr r0,[r6,#0x6c]
  00184d7c: adds r4,#0x1
  00184d7e: ldr r1,[r0,#0x0]
  00184d80: cmp r4,r1
  00184d82: bcc 0x00184d5a
  00184d84: movs r0,#0x0
  00184d86: pop.w r8
  00184d8a: pop {r4,r5,r6,r7,pc}
  00184d8c: movs r0,#0x1
  00184d8e: b 0x00184d86
```
