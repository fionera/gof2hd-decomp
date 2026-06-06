# Ship::getFreeSpace
elf 0x174c7c body 12
Sig: undefined __stdcall getFreeSpace(void)

## decompile
```c

/* Ship::getFreeSpace() */

int Ship::getFreeSpace(void)

{
  int in_r0;
  
  return (*(int *)(in_r0 + 0x28) + *(int *)(in_r0 + 0xc)) - *(int *)(in_r0 + 0x10);
}

```
## target disasm
```
  00184c7c: ldrd r1,r2,[r0,#0xc]
  00184c80: ldr r0,[r0,#0x28]
  00184c82: add r0,r1
  00184c84: subs r0,r0,r2
  00184c86: bx lr
```
