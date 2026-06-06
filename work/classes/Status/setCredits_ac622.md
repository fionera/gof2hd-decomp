# Status::setCredits
elf 0xac622 body 6
Sig: undefined __stdcall setCredits(int param_1)

## decompile
```c

/* Status::setCredits(int) */

void Status::setCredits(int param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0x1b0) = in_r1;
  return;
}

```
## target disasm
```
  000bc622: str.w r1,[r0,#0x1b0]
  000bc626: bx lr
```
