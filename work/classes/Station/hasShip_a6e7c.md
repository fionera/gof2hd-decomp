# Station::hasShip
elf 0xa6e7c body 58
Sig: undefined __stdcall hasShip(int param_1)

## decompile
```c

/* Station::hasShip(int) */

undefined4 Station::hasShip(int param_1)

{
  int iVar1;
  int in_r1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(param_1 + 0x2c);
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      if (*(int *)(puVar2[1] + uVar3 * 4) != 0) {
        iVar1 = Ship::getIndex();
        if (iVar1 == in_r1) {
          return 1;
        }
        puVar2 = *(uint **)(param_1 + 0x2c);
      }
    }
  }
  return 0;
}

```

## target disasm
```
  000b6e7c: push {r4,r5,r6,r7,lr}
  000b6e7e: add r7,sp,#0xc
  000b6e80: push.w r11
  000b6e84: mov r4,r1
  000b6e86: ldr r1,[r0,#0x2c]
  000b6e88: mov r5,r0
  000b6e8a: cbz r1,0x000b6eaa
  000b6e8c: movs r6,#0x0
  000b6e8e: b 0x000b6ea4
  000b6e90: ldr r0,[r1,#0x4]
  000b6e92: ldr.w r0,[r0,r6,lsl #0x2]
  000b6e96: cbz r0,0x000b6ea2
  000b6e98: blx 0x000719c8
  000b6e9c: cmp r0,r4
  000b6e9e: beq 0x000b6eae
  000b6ea0: ldr r1,[r5,#0x2c]
  000b6ea2: adds r6,#0x1
  000b6ea4: ldr r0,[r1,#0x0]
  000b6ea6: cmp r6,r0
  000b6ea8: bcc 0x000b6e90
  000b6eaa: movs r0,#0x0
  000b6eac: b 0x000b6eb0
  000b6eae: movs r0,#0x1
  000b6eb0: pop.w r11
  000b6eb4: pop {r4,r5,r6,r7,pc}
```
