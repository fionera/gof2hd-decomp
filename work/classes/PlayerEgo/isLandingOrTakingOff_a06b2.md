# PlayerEgo::isLandingOrTakingOff
elf 0xa06b2 body 28
Sig: undefined __stdcall isLandingOrTakingOff(void)

## decompile
```c

/* PlayerEgo::isLandingOrTakingOff() */

bool PlayerEgo::isLandingOrTakingOff(void)

{
  int in_r0;
  
  if (*(char *)(in_r0 + 0x356) != '\0') {
    return (*(uint *)(in_r0 + 0x1c4) | 1) == 3;
  }
  return false;
}

```

## target disasm
```
  000b06b2: ldrb.w r1,[r0,#0x356]
  000b06b6: cbz r1,0x000b06ca
  000b06b8: ldr.w r0,[r0,#0x1c4]
  000b06bc: orr r0,r0,#0x1
  000b06c0: subs r0,#0x3
  000b06c2: clz r0,r0
  000b06c6: lsrs r0,r0,#0x5
  000b06c8: bx lr
  000b06ca: movs r0,#0x0
  000b06cc: bx lr
```
