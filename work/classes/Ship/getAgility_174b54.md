# Ship::getAgility
elf 0x174b54 body 4
Sig: undefined __thiscall getAgility(Ship * this)

## decompile
```c

/* Ship::getAgility() */

undefined4 __thiscall Ship::getAgility(Ship *this)

{
  return *(undefined4 *)(this + 0x40);
}

```
## target disasm
```
  00184b54: ldr r0,[r0,#0x40]
  00184b56: bx lr
```
