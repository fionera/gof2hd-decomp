# Item::getTotalPrice
elf 0xe0124 body 8
Sig: undefined __thiscall getTotalPrice(Item * this)

## decompile
```c

/* Item::getTotalPrice() */

int __thiscall Item::getTotalPrice(Item *this)

{
  return *(int *)(this + 0x18) * *(int *)(this + 0x34);
}

```
## target disasm
```
  000f0124: ldr r1,[r0,#0x18]
  000f0126: ldr r0,[r0,#0x34]
  000f0128: muls r0,r1
  000f012a: bx lr
```
