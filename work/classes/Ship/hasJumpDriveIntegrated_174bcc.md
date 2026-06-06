# Ship::hasJumpDriveIntegrated
elf 0x174bcc body 28
Sig: undefined __stdcall hasJumpDriveIntegrated(void)

## decompile
```c

/* Ship::hasJumpDriveIntegrated() */

uint Ship::hasJumpDriveIntegrated(void)

{
  int *in_r0;
  
  if (*in_r0 - 0x25U < 4) {
    return 0xbU >> (*in_r0 - 0x25U & 0xf) & 1;
  }
  return 0;
}

```
## target disasm
```
  00184bcc: ldr r0,[r0,#0x0]
  00184bce: subs r0,#0x25
  00184bd0: cmp r0,#0x3
  00184bd2: itt hi
  00184bd4: mov.hi r0,#0x0
  00184bd6: bx.hi lr
  00184bd8: and r0,r0,#0xf
  00184bdc: movs r1,#0xb
  00184bde: lsr.w r0,r1,r0
  00184be2: and r0,r0,#0x1
  00184be6: bx lr
```
