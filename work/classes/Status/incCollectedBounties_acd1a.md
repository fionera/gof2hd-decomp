# Status::incCollectedBounties
elf 0xacd1a body 16
Sig: undefined __stdcall incCollectedBounties(int param_1)

## decompile
```c

/* Status::incCollectedBounties(int) */

void Status::incCollectedBounties(int param_1)

{
  int in_r1;
  int iVar1;
  
  iVar1 = in_r1;
  if (in_r1 < 4) {
    param_1 = param_1 + in_r1 * 4;
    iVar1 = *(int *)(param_1 + 4) + 1;
  }
  if (in_r1 < 4) {
    *(int *)(param_1 + 4) = iVar1;
  }
  return;
}

```
## target disasm
```
  000bcd1a: cmp r1,#0x3
  000bcd1c: itttt le
  000bcd1e: add.le.w r0,r0,r1, lsl #0x2
  000bcd22: ldr.le r1,[r0,#0x4]
  000bcd24: add.le r1,#0x1
  000bcd26: str.le r1,[r0,#0x4]
  000bcd28: bx lr
```
