# Ship::getCurrentWeaponSlot
elf 0x174a16 body 4
Sig: undefined __thiscall getCurrentWeaponSlot(Ship * this)

## decompile
```c

/* Ship::getCurrentWeaponSlot() */

undefined4 __thiscall Ship::getCurrentWeaponSlot(Ship *this)

{
  return *(undefined4 *)(this + 0x74);
}

```
## target disasm
```
  00184a16: ldr r0,[r0,#0x74]
  00184a18: bx lr
```
