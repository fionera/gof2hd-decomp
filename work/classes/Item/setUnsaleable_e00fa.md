# Item::setUnsaleable
elf 0xe00fa body 6
Sig: undefined __stdcall setUnsaleable(bool param_1)

## decompile
```c

/* Item::setUnsaleable(bool) */

void Item::setUnsaleable(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x44) = in_r1;
  return;
}

```
## target disasm
```
  000f00fa: strb.w r1,[r0,#0x44]
  000f00fe: bx lr
```
