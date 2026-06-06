# Ship::getMods
elf 0x1756ca body 4
Sig: undefined __thiscall getMods(Ship * this)

## decompile
```c

/* Ship::getMods() */

undefined4 __thiscall Ship::getMods(Ship *this)

{
  return *(undefined4 *)(this + 0x78);
}

```
## target disasm
```
  001856ca: ldr r0,[r0,#0x78]
  001856cc: bx lr
```
