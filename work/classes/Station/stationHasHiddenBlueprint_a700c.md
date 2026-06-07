# Station::stationHasHiddenBlueprint
elf 0xa700c body 58
Sig: undefined __thiscall stationHasHiddenBlueprint(Station * this, bool param_1)

## decompile
```c

/* Station::stationHasHiddenBlueprint(bool) */

undefined4 __thiscall Station::stationHasHiddenBlueprint(Station *this,bool param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  while( true ) {
    if (4 < uVar1) {
      return 0;
    }
    if ((*(int *)(DAT_000b704c + 0xb7022 + uVar1 * 4) == *(int *)(this + 0xc)) &&
       ((param_1 ||
        (*(char *)(*(int *)(*(int *)(**(int **)(DAT_000b7048 + 0xb7016) + 0x58) + 4) + uVar1) ==
         '\0')))) break;
    uVar1 = uVar1 + 1;
  }
  return 1;
}

```

## target disasm
```
  000b700c: push {r4,r5,r7,lr}
  000b700e: add r7,sp,#0x8
  000b7010: ldr r2,[0x000b7048]
  000b7012: add r2,pc
  000b7014: ldr r3,[r2,#0x0]
  000b7016: movs r2,#0x0
  000b7018: ldr.w r12,[r3,#0x0]
  000b701c: ldr r5,[0x000b704c]
  000b701e: add r5,pc
  000b7020: b 0x000b703a
  000b7022: ldr.w r3,[r5,r2,lsl #0x2]
  000b7026: ldr r4,[r0,#0xc]
  000b7028: cmp r3,r4
  000b702a: bne 0x000b7038
  000b702c: cbnz r1,0x000b7042
  000b702e: ldr.w r3,[r12,#0x58]
  000b7032: ldr r3,[r3,#0x4]
  000b7034: ldrb r3,[r3,r2]
  000b7036: cbz r3,0x000b7042
  000b7038: adds r2,#0x1
  000b703a: cmp r2,#0x4
  000b703c: bls 0x000b7022
  000b703e: movs r0,#0x0
  000b7040: pop {r4,r5,r7,pc}
  000b7042: movs r0,#0x1
  000b7044: pop {r4,r5,r7,pc}
```
