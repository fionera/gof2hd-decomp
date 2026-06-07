# AEGeometry::isVisible
elf 0xa4a4a body 6
Sig: undefined __thiscall isVisible(AEGeometry * this)

## decompile
```c

/* AEGeometry::isVisible() */

AEGeometry __thiscall AEGeometry::isVisible(AEGeometry *this)

{
  return this[0x48];
}

```

## target disasm
```
  000b4a4a: ldrb.w r0,[r0,#0x48]
  000b4a4e: bx lr
```
