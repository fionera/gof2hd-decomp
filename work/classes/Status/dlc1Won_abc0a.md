# Status::dlc1Won
elf 0xabc0a body 14
Sig: undefined __stdcall dlc1Won(void)

## decompile
```c

/* Status::dlc1Won() */

bool Status::dlc1Won(void)

{
  int in_r0;
  
  return 0x53 < *(int *)(in_r0 + 0x1e8);
}

```
## target disasm
```
  000bbc0a: ldr.w r1,[r0,#0x1e8]
  000bbc0e: movs r0,#0x0
  000bbc10: cmp r1,#0x53
  000bbc12: it gt
  000bbc14: mov.gt r0,#0x1
  000bbc16: bx lr
```
