# Status::gameWon
elf 0xa965e body 14
Sig: undefined __stdcall gameWon(void)

## decompile
```c

/* Status::gameWon() */

bool Status::gameWon(void)

{
  int in_r0;
  
  return 0x2c < *(int *)(in_r0 + 0x1e8);
}

```
## target disasm
```
  000b965e: ldr.w r1,[r0,#0x1e8]
  000b9662: movs r0,#0x0
  000b9664: cmp r1,#0x2c
  000b9666: it gt
  000b9668: mov.gt r0,#0x1
  000b966a: bx lr
```
