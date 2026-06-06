# Item::setPrice
elf 0xe013c body 4
Sig: undefined __stdcall setPrice(int param_1)

## decompile
```c

/* Item::setPrice(int) */

void Item::setPrice(int param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0x18) = in_r1;
  return;
}

```
## target disasm
```
  000f013c: str r1,[r0,#0x18]
  000f013e: bx lr
```
