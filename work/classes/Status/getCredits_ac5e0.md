# Status::getCredits
elf 0xac5e0 body 6
Sig: undefined __stdcall getCredits(void)

## decompile
```c

/* Status::getCredits() */

undefined4 Status::getCredits(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x1b0);
}

```
## target disasm
```
  000bc5e0: ldr.w r0,[r0,#0x1b0]
  000bc5e4: bx lr
```
