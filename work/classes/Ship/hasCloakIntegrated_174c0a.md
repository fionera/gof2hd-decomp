# Ship::hasCloakIntegrated
elf 0x174c0a body 24
Sig: undefined __thiscall hasCloakIntegrated(Ship * this)

## decompile
```c

/* Ship::hasCloakIntegrated() */

bool __thiscall Ship::hasCloakIntegrated(Ship *this)

{
  return *(int *)this == 0x31 || *(int *)this == 0x2c;
}

```
## target disasm
```
  00184c0a: ldr r0,[r0,#0x0]
  00184c0c: sub.w r1,r0,#0x2c
  00184c10: subs r0,#0x31
  00184c12: clz r0,r0
  00184c16: clz r1,r1
  00184c1a: lsrs r0,r0,#0x5
  00184c1c: lsrs r1,r1,#0x5
  00184c1e: orrs r0,r1
  00184c20: bx lr
```
