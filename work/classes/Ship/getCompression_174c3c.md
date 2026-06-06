# Ship::getCompression
elf 0x174c3c body 4
Sig: undefined __thiscall getCompression(Ship * this)

## decompile
```c

/* Ship::getCompression() */

undefined4 __thiscall Ship::getCompression(Ship *this)

{
  return *(undefined4 *)(this + 0x2c);
}

```
## target disasm
```
  00184c3c: ldr r0,[r0,#0x2c]
  00184c3e: bx lr
```
