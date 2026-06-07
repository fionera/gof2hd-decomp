# PlayerEgo::throttleChanged
elf 0xa20f4 body 58
Sig: undefined __stdcall throttleChanged(void)

## decompile
```c

/* PlayerEgo::throttleChanged() */

void PlayerEgo::throttleChanged(void)

{
  int in_r0;
  int iVar1;
  
  if (*(char *)(in_r0 + 0x370) == '\0') {
    *(undefined1 *)(in_r0 + 0x370) = 1;
    iVar1 = 0;
  }
  else {
    iVar1 = *(int *)(in_r0 + 0x374);
    if (iVar1 - 0x1f5U < 999) {
      iVar1 = 500;
    }
    else {
      if (iVar1 < 0x5dc) {
        return;
      }
      iVar1 = 2000 - iVar1;
    }
  }
  *(int *)(in_r0 + 0x374) = iVar1;
  return;
}

```

## target disasm
```
  000b20f4: ldrb.w r1,[r0,#0x370]
  000b20f8: cbz r1,0x000b2110
  000b20fa: ldr.w r1,[r0,#0x374]
  000b20fe: movw r3,#0x3e6
  000b2102: subw r2,r1,#0x1f5
  000b2106: cmp r2,r3
  000b2108: bhi 0x000b211a
  000b210a: mov.w r1,#0x1f4
  000b210e: b 0x000b2128
  000b2110: movs r1,#0x1
  000b2112: strb.w r1,[r0,#0x370]
  000b2116: movs r1,#0x0
  000b2118: b 0x000b2128
  000b211a: movw r2,#0x5dc
  000b211e: cmp r1,r2
  000b2120: it lt
  000b2122: bx.lt lr
  000b2124: rsb.w r1,r1,#0x7d0
  000b2128: str.w r1,[r0,#0x374]
  000b212c: bx lr
```
