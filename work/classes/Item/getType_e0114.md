# Item::getType
elf 0xe0114 body 4
Sig: undefined __thiscall getType(Item * this)

## decompile
```c

/* Item::getType() */

undefined4 __thiscall Item::getType(Item *this)

{
  return *(undefined4 *)(this + 4);
}

```
## target disasm
```
  000f0114: ldr r0,[r0,#0x4]
  000f0116: bx lr
```
