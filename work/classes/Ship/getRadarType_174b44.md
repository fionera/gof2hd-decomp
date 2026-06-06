# Ship::getRadarType
elf 0x174b44 body 4
Sig: undefined __thiscall getRadarType(Ship * this)

## decompile
```c

/* Ship::getRadarType() */

undefined4 __thiscall Ship::getRadarType(Ship *this)

{
  return *(undefined4 *)(this + 0x30);
}

```
## target disasm
```
  00184b44: ldr r0,[r0,#0x30]
  00184b46: bx lr
```
