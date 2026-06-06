# Ship::getBoostDelay
elf 0x174b50 body 4
Sig: undefined __thiscall getBoostDelay(Ship * this)

## decompile
```c

/* Ship::getBoostDelay() */

undefined4 __thiscall Ship::getBoostDelay(Ship *this)

{
  return *(undefined4 *)(this + 0x38);
}

```
## target disasm
```
  00184b50: ldr r0,[r0,#0x38]
  00184b52: bx lr
```
