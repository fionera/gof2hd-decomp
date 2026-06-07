# StarMap::setJumpMapMode
elf 0xc804c body 10
Sig: undefined __stdcall setJumpMapMode(bool param_1, bool param_2)

## decompile
```c

/* StarMap::setJumpMapMode(bool, bool) */

void StarMap::setJumpMapMode(bool param_1,bool param_2)

{
  undefined1 in_r2;
  
  *(undefined1 *)(param_1 + 0xab) = in_r2;
  *(bool *)(param_1 + 0xaa) = param_2;
  return;
}

```

## target disasm
```
  000d804c: strb.w r2,[r0,#0xab]
  000d8050: strb.w r1,[r0,#0xaa]
  000d8054: bx lr
```
