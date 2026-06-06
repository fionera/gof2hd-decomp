# Ship::getCargoPlus
elf 0x174c38 body 4
Sig: undefined __thiscall getCargoPlus(Ship * this)

## decompile
```c

/* Ship::getCargoPlus() */

undefined4 __thiscall Ship::getCargoPlus(Ship *this)

{
  return *(undefined4 *)(this + 0x28);
}

```
## target disasm
```
  00184c38: ldr r0,[r0,#0x28]
  00184c3a: bx lr
```
