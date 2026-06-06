# Ship::getRace
elf 0x174a06 body 4
Sig: undefined __thiscall getRace(Ship * this)

## decompile
```c

/* Ship::getRace() */

undefined4 __thiscall Ship::getRace(Ship *this)

{
  return *(undefined4 *)(this + 100);
}

```
## target disasm
```
  00184a06: ldr r0,[r0,#0x64]
  00184a08: bx lr
```
