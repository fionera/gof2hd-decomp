# Ship::getShieldRegen
elf 0x174b3c body 4
Sig: undefined __thiscall getShieldRegen(Ship * this)

## decompile
```c

/* Ship::getShieldRegen() */

undefined4 __thiscall Ship::getShieldRegen(Ship *this)

{
  return *(undefined4 *)(this + 0x24);
}

```
## target disasm
```
  00184b3c: ldr r0,[r0,#0x24]
  00184b3e: bx lr
```
