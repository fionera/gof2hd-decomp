# Status::setPlayingTime
elf 0xac5ec body 6
Sig: undefined __stdcall setPlayingTime(longlong param_1)

## decompile
```c

/* Status::setPlayingTime(long long) */

void Status::setPlayingTime(longlong param_1)

{
  undefined4 in_r2;
  undefined4 in_r3;
  
  *(undefined4 *)((int)param_1 + 0x1b8) = in_r2;
  *(undefined4 *)((int)param_1 + 0x1bc) = in_r3;
  return;
}

```
## target disasm
```
  000bc5ec: strd r2,r3,[r0,#0x1b8]
  000bc5f0: bx lr
```
