# Level::getGasClouds
elf 0xbde9e body 6
Sig: undefined __thiscall getGasClouds(Level * this)

## decompile
```c

/* Level::getGasClouds() */

undefined4 __thiscall Level::getGasClouds(Level *this)

{
  return *(undefined4 *)(this + 0xf4);
}

```
## target disasm
```
  000cde9e: ldr.w r0,[r0,#0xf4]
  000cdea2: bx lr
```
