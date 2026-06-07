# Wanted::setTerminated
elf 0x12296c body 6
Sig: undefined __stdcall setTerminated(bool param_1)

## decompile
```c

/* Wanted::setTerminated(bool) */

void Wanted::setTerminated(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x50) = in_r1;
  return;
}

```

## target disasm
```
  0013296c: strb.w r1,[r0,#0x50]
  00132970: bx lr
```
