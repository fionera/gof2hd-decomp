# Status::getMission
elf 0xa967c body 6
Sig: undefined __stdcall getMission(void)

## decompile
```c

/* Status::getMission() */

undefined4 Status::getMission(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x194);
}

```
## target disasm
```
  000b967c: ldr.w r0,[r0,#0x194]
  000b9680: bx lr
```
