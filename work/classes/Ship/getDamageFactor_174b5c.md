# Ship::getDamageFactor
elf 0x174b5c body 4
Sig: undefined __thiscall getDamageFactor(Ship * this)

## decompile
```c

/* Ship::getDamageFactor() */

undefined4 __thiscall Ship::getDamageFactor(Ship *this)

{
  return *(undefined4 *)(this + 0x58);
}

```
## target disasm
```
  00184b5c: ldr r0,[r0,#0x58]
  00184b5e: bx lr
```
