# Ship::getFirePower
elf 0x174c30 body 4
Sig: undefined __thiscall getFirePower(Ship * this)

## decompile
```c

/* Ship::getFirePower() */

undefined4 __thiscall Ship::getFirePower(Ship *this)

{
  return *(undefined4 *)(this + 0x48);
}

```
## target disasm
```
  00184c30: ldr r0,[r0,#0x48]
  00184c32: bx lr
```
