# Ship::getBoostTime
elf 0x174b4c body 4
Sig: undefined __thiscall getBoostTime(Ship * this)

## decompile
```c

/* Ship::getBoostTime() */

undefined4 __thiscall Ship::getBoostTime(Ship *this)

{
  return *(undefined4 *)(this + 0x3c);
}

```
## target disasm
```
  00184b4c: ldr r0,[r0,#0x3c]
  00184b4e: bx lr
```
