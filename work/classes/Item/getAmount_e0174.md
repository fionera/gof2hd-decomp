# Item::getAmount
elf 0xe0174 body 4
Sig: undefined __stdcall getAmount(void)

## decompile
```c

/* Item::getAmount() */

undefined4 Item::getAmount(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x34);
}

```
## target disasm
```
  000f0174: ldr r0,[r0,#0x34]
  000f0176: bx lr
```
