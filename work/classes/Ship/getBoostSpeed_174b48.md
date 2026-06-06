# Ship::getBoostSpeed
elf 0x174b48 body 4
Sig: undefined __thiscall getBoostSpeed(Ship * this)

## decompile
```c

/* Ship::getBoostSpeed() */

undefined4 __thiscall Ship::getBoostSpeed(Ship *this)

{
  return *(undefined4 *)(this + 0x34);
}

```
## target disasm
```
  00184b48: ldr r0,[r0,#0x34]
  00184b4a: bx lr
```
