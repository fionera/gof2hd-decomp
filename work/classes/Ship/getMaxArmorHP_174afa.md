# Ship::getMaxArmorHP
elf 0x174afa body 4
Sig: undefined __thiscall getMaxArmorHP(Ship * this)

## decompile
```c

/* Ship::getMaxArmorHP() */

undefined4 __thiscall Ship::getMaxArmorHP(Ship *this)

{
  return *(undefined4 *)(this + 0x20);
}

```
## target disasm
```
  00184afa: ldr r0,[r0,#0x20]
  00184afc: bx lr
```
