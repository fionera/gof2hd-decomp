# PlayerEgo::setDockingState
elf 0xa179e body 22
Sig: undefined __stdcall setDockingState(int param_1)

## decompile
```c

/* PlayerEgo::setDockingState(int) */

void PlayerEgo::setDockingState(int param_1)

{
  int in_r1;
  int in_r2;
  
  if (in_r1 == 2) {
    in_r2 = *(int *)(param_1 + 0x1c4);
  }
  if (in_r1 == 2 && in_r2 == 1) {
    *(undefined1 *)(param_1 + 0xb2) = 1;
  }
  *(int *)(param_1 + 0x1c4) = in_r1;
  return;
}

```

## target disasm
```
  000b179e: cmp r1,#0x2
  000b17a0: itttt eq
  000b17a2: ldr.eq.w r2,[r0,#0x1c4]
  000b17a6: cmp.eq r2,#0x1
  000b17a8: mov.eq r2,#0x1
  000b17aa: strb.eq.w r2,[r0,#0xb2]
  000b17ae: str.w r1,[r0,#0x1c4]
  000b17b2: bx lr
```
