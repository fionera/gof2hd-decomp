# Ship::getPrice
elf 0x174a0e body 4
Sig: undefined __thiscall getPrice(Ship * this)

## decompile
```c

/* Ship::getPrice() */

undefined4 __thiscall Ship::getPrice(Ship *this)

{
  return *(undefined4 *)(this + 0x14);
}

```
## target disasm
```
  00184a0e: ldr r0,[r0,#0x14]
  00184a10: bx lr
```
