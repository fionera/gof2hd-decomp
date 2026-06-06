# Ship::getBaseLoad
elf 0x174c34 body 4
Sig: undefined __thiscall getBaseLoad(Ship * this)

## decompile
```c

/* Ship::getBaseLoad() */

undefined4 __thiscall Ship::getBaseLoad(Ship *this)

{
  return *(undefined4 *)(this + 0xc);
}

```
## target disasm
```
  00184c34: ldr r0,[r0,#0xc]
  00184c36: bx lr
```
