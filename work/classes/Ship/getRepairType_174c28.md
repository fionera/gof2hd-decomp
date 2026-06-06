# Ship::getRepairType
elf 0x174c28 body 4
Sig: undefined __thiscall getRepairType(Ship * this)

## decompile
```c

/* Ship::getRepairType() */

undefined4 __thiscall Ship::getRepairType(Ship *this)

{
  return *(undefined4 *)(this + 0x4c);
}

```
## target disasm
```
  00184c28: ldr r0,[r0,#0x4c]
  00184c2a: bx lr
```
