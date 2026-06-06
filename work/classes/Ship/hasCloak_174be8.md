# Ship::hasCloak
elf 0x174be8 body 34
Sig: undefined __stdcall hasCloak(void)

## decompile
```c

/* Ship::hasCloak() */

bool Ship::hasCloak(void)

{
  int *in_r0;
  
  if (*(char *)((int)in_r0 + 0x51) != '\0') {
    return true;
  }
  return *in_r0 == 0x31 || *in_r0 == 0x2c;
}

```
## target disasm
```
  00184be8: ldrb.w r1,[r0,#0x51]
  00184bec: cbz r1,0x00184bf2
  00184bee: movs r0,#0x1
  00184bf0: bx lr
  00184bf2: ldr r0,[r0,#0x0]
  00184bf4: sub.w r1,r0,#0x2c
  00184bf8: subs r0,#0x31
  00184bfa: clz r0,r0
  00184bfe: clz r1,r1
  00184c02: lsrs r0,r0,#0x5
  00184c04: lsrs r1,r1,#0x5
  00184c06: orrs r0,r1
  00184c08: bx lr
```
