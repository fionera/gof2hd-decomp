# Ship::hasJumpDrive
elf 0x174ba6 body 38
Sig: undefined __stdcall hasJumpDrive(void)

## decompile
```c

/* Ship::hasJumpDrive() */

uint Ship::hasJumpDrive(void)

{
  int *in_r0;
  
  if ((char)in_r0[0x14] != '\0') {
    return 1;
  }
  if (*in_r0 - 0x25U < 4) {
    return 0xbU >> (*in_r0 - 0x25U & 0xf) & 1;
  }
  return 0;
}

```
## target disasm
```
  00184ba6: ldrb.w r1,[r0,#0x50]
  00184baa: cbz r1,0x00184bb0
  00184bac: movs r0,#0x1
  00184bae: bx lr
  00184bb0: ldr r0,[r0,#0x0]
  00184bb2: subs r0,#0x25
  00184bb4: cmp r0,#0x3
  00184bb6: itt hi
  00184bb8: mov.hi r0,#0x0
  00184bba: bx.hi lr
  00184bbc: and r0,r0,#0xf
  00184bc0: movs r1,#0xb
  00184bc2: lsr.w r0,r1,r0
  00184bc6: and r0,r0,#0x1
  00184bca: bx lr
```
