# Player::gunAvailable
elf 0xa36ac body 34
Sig: undefined __stdcall gunAvailable(int param_1)

## decompile
```c

/* Player::gunAvailable(int) */

bool Player::gunAvailable(int param_1)

{
  int *piVar1;
  uint in_r1;
  
  if (((in_r1 < 4) &&
      (piVar1 = *(int **)(*(int *)(*(int *)param_1 + 4) + in_r1 * 4), piVar1 != (int *)0x0)) &&
     (*piVar1 != 0)) {
    return *(int *)piVar1[1] != 0;
  }
  return false;
}

```
## target disasm
```
  000b36ac: cmp r1,#0x3
  000b36ae: bhi 0x000b36ca
  000b36b0: ldr r0,[r0,#0x0]
  000b36b2: ldr r0,[r0,#0x4]
  000b36b4: ldr.w r0,[r0,r1,lsl #0x2]
  000b36b8: cbz r0,0x000b36ca
  000b36ba: ldr r1,[r0,#0x0]
  000b36bc: cbz r1,0x000b36ca
  000b36be: ldr r0,[r0,#0x4]
  000b36c0: ldr r0,[r0,#0x0]
  000b36c2: cmp r0,#0x0
  000b36c4: it ne
  000b36c6: mov.ne r0,#0x1
  000b36c8: bx lr
  000b36ca: movs r0,#0x0
  000b36cc: bx lr
```
