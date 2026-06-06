# Ship::getBaseHP
elf 0x174ab4 body 4
Sig: undefined __thiscall getBaseHP(Ship * this)

## decompile
```c

/* Ship::getBaseHP() */

undefined4 __thiscall Ship::getBaseHP(Ship *this)

{
  return *(undefined4 *)(this + 4);
}

```
## target disasm
```
  00184ab4: ldr r0,[r0,#0x4]
  00184ab6: bx lr
```
