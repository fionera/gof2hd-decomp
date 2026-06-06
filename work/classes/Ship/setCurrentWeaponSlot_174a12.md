# Ship::setCurrentWeaponSlot
elf 0x174a12 body 4
Sig: undefined __thiscall setCurrentWeaponSlot(Ship * this, int param_1)

## decompile
```c

/* Ship::setCurrentWeaponSlot(int) */

void __thiscall Ship::setCurrentWeaponSlot(Ship *this,int param_1)

{
  *(int *)(this + 0x74) = param_1;
  return;
}

```
## target disasm
```
  00184a12: str r1,[r0,#0x74]
  00184a14: bx lr
```
