# Ship::getValue
elf 0x174b40 body 4
Sig: undefined __thiscall getValue(Ship * this)

## decompile
```c

/* Ship::getValue() */

undefined4 __thiscall Ship::getValue(Ship *this)

{
  return *(undefined4 *)(this + 8);
}

```
## target disasm
```
  00184b40: ldr r0,[r0,#0x8]
  00184b42: bx lr
```
