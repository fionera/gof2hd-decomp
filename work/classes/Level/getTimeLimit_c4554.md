# Level::getTimeLimit
elf 0xc4554 body 6
Sig: undefined __thiscall getTimeLimit(Level * this)

## decompile
```c

/* Level::getTimeLimit() */

undefined4 __thiscall Level::getTimeLimit(Level *this)

{
  return *(undefined4 *)(this + 0x130);
}

```
## target disasm
```
  000d4554: ldr.w r0,[r0,#0x130]
  000d4558: bx lr
```
