# Status::getStanding
elf 0xac720 body 4
Sig: undefined __stdcall getStanding(void)

## decompile
```c

/* Status::getStanding() */

undefined4 Status::getStanding(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x14);
}

```
## target disasm
```
  000bc720: ldr r0,[r0,#0x14]
  000bc722: bx lr
```
