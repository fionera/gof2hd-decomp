# MTitle::OnTouchEnd
elf 0x97a24 body 8
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* MTitle::OnTouchEnd(int, int) */

void MTitle::OnTouchEnd(int param_1,int param_2)

{
  *(undefined4 *)(param_1 + 0x1c) = 5000;
  return;
}

```

## target disasm
```
  000a7a24: movw r1,#0x1388
  000a7a28: str r1,[r0,#0x1c]
  000a7a2a: bx lr
```
