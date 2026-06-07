# AEGeometry::setVisible
elf 0xa4a40 body 10
Sig: undefined __stdcall setVisible(bool param_1)

## decompile
```c

/* AEGeometry::setVisible(bool) */

void AEGeometry::setVisible(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x49) = in_r1;
  *(undefined1 *)(param_1 + 0x48) = in_r1;
  return;
}

```

## target disasm
```
  000b4a40: strb.w r1,[r0,#0x49]
  000b4a44: strb.w r1,[r0,#0x48]
  000b4a48: bx lr
```
