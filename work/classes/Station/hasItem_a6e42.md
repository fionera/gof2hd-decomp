# Station::hasItem
elf 0xa6e42 body 58
Sig: undefined __stdcall hasItem(int param_1)

## decompile
```c

/* Station::hasItem(int) */

undefined4 Station::hasItem(int param_1)

{
  Item *this;
  int iVar1;
  int in_r1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(param_1 + 0x28);
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      this = *(Item **)(puVar2[1] + uVar3 * 4);
      if (this != (Item *)0x0) {
        iVar1 = Item::getIndex(this);
        if (iVar1 == in_r1) {
          return 1;
        }
        puVar2 = *(uint **)(param_1 + 0x28);
      }
    }
  }
  return 0;
}

```

## target disasm
```
  000b6e42: push {r4,r5,r6,r7,lr}
  000b6e44: add r7,sp,#0xc
  000b6e46: push.w r11
  000b6e4a: mov r4,r1
  000b6e4c: ldr r1,[r0,#0x28]
  000b6e4e: mov r5,r0
  000b6e50: cbz r1,0x000b6e70
  000b6e52: movs r6,#0x0
  000b6e54: b 0x000b6e6a
  000b6e56: ldr r0,[r1,#0x4]
  000b6e58: ldr.w r0,[r0,r6,lsl #0x2]
  000b6e5c: cbz r0,0x000b6e68
  000b6e5e: blx 0x000718d8
  000b6e62: cmp r0,r4
  000b6e64: beq 0x000b6e74
  000b6e66: ldr r1,[r5,#0x28]
  000b6e68: adds r6,#0x1
  000b6e6a: ldr r0,[r1,#0x0]
  000b6e6c: cmp r6,r0
  000b6e6e: bcc 0x000b6e56
  000b6e70: movs r0,#0x0
  000b6e72: b 0x000b6e76
  000b6e74: movs r0,#0x1
  000b6e76: pop.w r11
  000b6e7a: pop {r4,r5,r6,r7,pc}
```
