# SpaceLounge::setHangarUpdate
elf 0x16aa74 body 6
Sig: undefined __stdcall setHangarUpdate(bool param_1)

## decompile
```c

/* SpaceLounge::setHangarUpdate(bool) */

void SpaceLounge::setHangarUpdate(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x35) = in_r1;
  return;
}

```

## target disasm
```
  0017aa74: strb.w r1,[r0,#0x35]
  0017aa78: bx lr
```
