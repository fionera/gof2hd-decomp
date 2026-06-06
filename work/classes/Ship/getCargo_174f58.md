# Ship::getCargo
elf 0x174f58 body 62
Sig: undefined __stdcall getCargo(int param_1)

## decompile
```c

/* Ship::getCargo(int) */

undefined4 Ship::getCargo(int param_1)

{
  Item *this;
  int iVar1;
  int in_r1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(param_1 + 0x70);
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      this = *(Item **)(puVar2[1] + uVar3 * 4);
      if (this != (Item *)0x0) {
        iVar1 = Item::getIndex(this);
        puVar2 = *(uint **)(param_1 + 0x70);
        if (iVar1 == in_r1) {
          return *(undefined4 *)(puVar2[1] + uVar3 * 4);
        }
      }
    }
  }
  return 0;
}

```
## target disasm
```
  00184f58: push {r4,r5,r6,r7,lr}
  00184f5a: add r7,sp,#0xc
  00184f5c: push.w r11
  00184f60: mov r4,r1
  00184f62: ldr r1,[r0,#0x70]
  00184f64: mov r5,r0
  00184f66: cbz r1,0x00184f86
  00184f68: movs r6,#0x0
  00184f6a: b 0x00184f80
  00184f6c: ldr r0,[r1,#0x4]
  00184f6e: ldr.w r0,[r0,r6,lsl #0x2]
  00184f72: cbz r0,0x00184f7e
  00184f74: blx 0x000718d8
  00184f78: ldr r1,[r5,#0x70]
  00184f7a: cmp r0,r4
  00184f7c: beq 0x00184f8a
  00184f7e: adds r6,#0x1
  00184f80: ldr r0,[r1,#0x0]
  00184f82: cmp r6,r0
  00184f84: bcc 0x00184f6c
  00184f86: movs r0,#0x0
  00184f88: b 0x00184f90
  00184f8a: ldr r0,[r1,#0x4]
  00184f8c: ldr.w r0,[r0,r6,lsl #0x2]
  00184f90: pop.w r11
  00184f94: pop {r4,r5,r6,r7,pc}
```
