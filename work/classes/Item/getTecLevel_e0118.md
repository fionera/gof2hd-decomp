# Item::getTecLevel
elf 0xe0118 body 4
Sig: undefined __thiscall getTecLevel(Item * this)

## decompile
```c

/* Item::getTecLevel() */

undefined4 __thiscall Item::getTecLevel(Item *this)

{
  return *(undefined4 *)(this + 0xc);
}

```
## target disasm
```
  000f0118: ldr r0,[r0,#0xc]
  000f011a: bx lr
```
