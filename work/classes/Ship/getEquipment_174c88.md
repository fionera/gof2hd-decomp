# Ship::getEquipment
elf 0x174c88 body 4
Sig: undefined __thiscall getEquipment(Ship * this)

## decompile
```c

/* Ship::getEquipment() */

undefined4 __thiscall Ship::getEquipment(Ship *this)

{
  return *(undefined4 *)(this + 0x6c);
}

```
## target disasm
```
  00184c88: ldr r0,[r0,#0x6c]
  00184c8a: bx lr
```
