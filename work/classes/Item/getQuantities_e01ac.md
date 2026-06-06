# Item::getQuantities
elf 0xe01ac body 4
Sig: undefined __stdcall getQuantities(void)

## decompile
```c

/* Item::getQuantities() */

undefined4 Item::getQuantities(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x2c);
}

```
## target disasm
```
  000f01ac: ldr r0,[r0,#0x2c]
  000f01ae: bx lr
```
