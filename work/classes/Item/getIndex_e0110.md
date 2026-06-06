# Item::getIndex
elf 0xe0110 body 4
Sig: undefined __thiscall getIndex(Item * this)

## decompile
```c

/* Item::getIndex() */

undefined4 __thiscall Item::getIndex(Item *this)

{
  return *(undefined4 *)this;
}

```
## target disasm
```
  000f0110: ldr r0,[r0,#0x0]
  000f0112: bx lr
```
