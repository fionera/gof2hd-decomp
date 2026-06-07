# Station::stationHasPirateBase
elf 0xa6fa4 body 56
Sig: undefined __stdcall stationHasPirateBase(void)

## decompile
```c

/* Station::stationHasPirateBase() */

undefined4 Station::stationHasPirateBase(void)

{
  int in_r0;
  uint uVar1;
  
  uVar1 = 0;
  while( true ) {
    if (3 < uVar1) {
      return 0;
    }
    if ((*(int *)(DAT_000b6fe0 + 0xb6fba + uVar1 * 4) == *(int *)(in_r0 + 0xc)) &&
       (*(char *)(*(int *)(*(int *)(**(int **)(DAT_000b6fdc + 0xb6fb0) + 0x4c) + 4) + uVar1) == '\0'
       )) break;
    uVar1 = uVar1 + 1;
  }
  return 1;
}

```

## target disasm
```
  000b6fa4: push {r4,r6,r7,lr}
  000b6fa6: add r7,sp,#0x8
  000b6fa8: ldr r1,[0x000b6fdc]
  000b6faa: movs r2,#0x0
  000b6fac: add r1,pc
  000b6fae: ldr r1,[r1,#0x0]
  000b6fb0: ldr.w r12,[r1,#0x0]
  000b6fb4: ldr r4,[0x000b6fe0]
  000b6fb6: add r4,pc
  000b6fb8: b 0x000b6fd0
  000b6fba: ldr.w r1,[r4,r2,lsl #0x2]
  000b6fbe: ldr r3,[r0,#0xc]
  000b6fc0: cmp r1,r3
  000b6fc2: bne 0x000b6fce
  000b6fc4: ldr.w r1,[r12,#0x4c]
  000b6fc8: ldr r1,[r1,#0x4]
  000b6fca: ldrb r1,[r1,r2]
  000b6fcc: cbz r1,0x000b6fd8
  000b6fce: adds r2,#0x1
  000b6fd0: cmp r2,#0x3
  000b6fd2: bls 0x000b6fba
  000b6fd4: movs r0,#0x0
  000b6fd6: pop {r4,r6,r7,pc}
  000b6fd8: movs r0,#0x1
  000b6fda: pop {r4,r6,r7,pc}
```
