# Item::getIngredients
elf 0xe01a8 body 4
Sig: undefined __stdcall getIngredients(void)

## decompile
```c

/* Item::getIngredients() */

undefined4 Item::getIngredients(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x28);
}

```
## target disasm
```
  000f01a8: ldr r0,[r0,#0x28]
  000f01aa: bx lr
```
