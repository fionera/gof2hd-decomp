# SolarSystem::hasNoOwner
elf 0x1558e2 body 30
Sig: undefined __stdcall hasNoOwner(void)

## decompile
```c

/* SolarSystem::hasNoOwner() */

uint SolarSystem::hasNoOwner(void)

{
  int in_r0;
  uint uVar1;
  
  uVar1 = *(int *)(in_r0 + 0x18) - 0x17;
  if (uVar1 < 0xb) {
    return 0x60bU >> (uVar1 & 0xff) & 1;
  }
  return 0;
}

```

## target disasm
```
  001658e2: ldr r0,[r0,#0x18]
  001658e4: subs r0,#0x17
  001658e6: cmp r0,#0xa
  001658e8: itt hi
  001658ea: mov.hi r0,#0x0
  001658ec: bx.hi lr
  001658ee: bfc r0,#0xb,#0x15
  001658f2: movw r1,#0x60b
  001658f6: lsr.w r0,r1,r0
  001658fa: and r0,r0,#0x1
  001658fe: bx lr
```
