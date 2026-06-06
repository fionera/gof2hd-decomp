# Status::setKills
elf 0xac5f2 body 6
Sig: undefined __stdcall setKills(int param_1)

## decompile
```c

/* Status::setKills(int) */

void Status::setKills(int param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0x1c0) = in_r1;
  return;
}

```
## target disasm
```
  000bc5f2: str.w r1,[r0,#0x1c0]
  000bc5f6: bx lr
```
