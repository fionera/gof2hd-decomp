# PlayerEgo::shouldSwitchToFreeLookCam
elf 0xa1788 body 22
Sig: undefined __stdcall shouldSwitchToFreeLookCam(void)

## decompile
```c

/* PlayerEgo::shouldSwitchToFreeLookCam() */

undefined4 PlayerEgo::shouldSwitchToFreeLookCam(void)

{
  int in_r0;
  
  if (*(char *)(in_r0 + 0xb1) == '\0') {
    return 0;
  }
  *(undefined1 *)(in_r0 + 0xb1) = 0;
  return 1;
}

```

## target disasm
```
  000b1788: ldrb.w r2,[r0,#0xb1]
  000b178c: mov r1,r0
  000b178e: movs r0,#0x0
  000b1790: cmp r2,#0x0
  000b1792: it eq
  000b1794: bx.eq lr
  000b1796: strb.w r0,[r1,#0xb1]
  000b179a: movs r0,#0x1
  000b179c: bx lr
```
