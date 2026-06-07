# Station::getPirateStationIndex
elf 0xa6fe4 body 36
Sig: undefined __thiscall getPirateStationIndex(Station * this)

## decompile
```c

/* Station::getPirateStationIndex() */

uint __thiscall Station::getPirateStationIndex(Station *this)

{
  uint uVar1;
  
  uVar1 = 0;
  while( true ) {
    if (3 < uVar1) {
      return 0xffffffff;
    }
    if (*(int *)(DAT_000b7008 + 0xb6fee + uVar1 * 4) == *(int *)(this + 0xc)) break;
    uVar1 = uVar1 + 1;
  }
  return uVar1;
}

```

## target disasm
```
  000b6fe4: ldr r2,[0x000b7008]
  000b6fe6: mov r12,r0
  000b6fe8: movs r0,#0x0
  000b6fea: add r2,pc
  000b6fec: b 0x000b6ffe
  000b6fee: ldr.w r3,[r2,r0,lsl #0x2]
  000b6ff2: ldr.w r1,[r12,#0xc]
  000b6ff6: cmp r3,r1
  000b6ff8: it eq
  000b6ffa: bx.eq lr
  000b6ffc: adds r0,#0x1
  000b6ffe: cmp r0,#0x3
  000b7000: bls 0x000b6fee
  000b7002: mov.w r0,#0xffffffff
  000b7006: bx lr
```
