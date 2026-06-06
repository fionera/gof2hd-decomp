# Item::getSort
elf 0xe011c body 4
Sig: undefined __thiscall getSort(Item * this)

## decompile
```c

/* Item::getSort() */

undefined4 __thiscall Item::getSort(Item *this)

{
  return *(undefined4 *)(this + 8);
}

```
## target disasm
```
  000f011c: ldr r0,[r0,#0x8]
  000f011e: bx lr
```
