# Ship::getNumAddedDeviceSlots
elf 0x175694 body 4
Sig: undefined __thiscall getNumAddedDeviceSlots(Ship * this)

## decompile
```c

/* Ship::getNumAddedDeviceSlots() */

undefined4 __thiscall Ship::getNumAddedDeviceSlots(Ship *this)

{
  return *(undefined4 *)(this + 0x7c);
}

```
## target disasm
```
  00185694: ldr r0,[r0,#0x7c]
  00185696: bx lr
```
