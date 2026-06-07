# Station::getHiddenBlueprintIndex
elf 0xa7050 body 36
Sig: undefined __thiscall getHiddenBlueprintIndex(Station * this)

## decompile
```c

/* Station::getHiddenBlueprintIndex() */

uint __thiscall Station::getHiddenBlueprintIndex(Station *this)

{
  uint uVar1;
  
  uVar1 = 0;
  while( true ) {
    if (4 < uVar1) {
      return 0xffffffff;
    }
    if (*(int *)(DAT_000b7074 + 0xb705a + uVar1 * 4) == *(int *)(this + 0xc)) break;
    uVar1 = uVar1 + 1;
  }
  return uVar1;
}

```

## target disasm
```
  000b7050: ldr r2,[0x000b7074]
  000b7052: mov r12,r0
  000b7054: movs r0,#0x0
  000b7056: add r2,pc
  000b7058: b 0x000b706a
  000b705a: ldr.w r3,[r2,r0,lsl #0x2]
  000b705e: ldr.w r1,[r12,#0xc]
  000b7062: cmp r3,r1
  000b7064: it eq
  000b7066: bx.eq lr
  000b7068: adds r0,#0x1
  000b706a: cmp r0,#0x4
  000b706c: bls 0x000b705a
  000b706e: mov.w r0,#0xffffffff
  000b7072: bx lr
```
