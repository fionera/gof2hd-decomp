# Ship::getSignatureRace
elf 0x174c2c body 4
Sig: undefined __thiscall getSignatureRace(Ship * this)

## decompile
```c

/* Ship::getSignatureRace() */

undefined4 __thiscall Ship::getSignatureRace(Ship *this)

{
  return *(undefined4 *)(this + 0x60);
}

```
## target disasm
```
  00184c2c: ldr r0,[r0,#0x60]
  00184c2e: bx lr
```
