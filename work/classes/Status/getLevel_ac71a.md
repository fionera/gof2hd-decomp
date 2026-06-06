# Status::getLevel
elf 0xac71a body 6
Sig: undefined __stdcall getLevel(void)

## decompile
```c

/* Status::getLevel() */

undefined4 Status::getLevel(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x1c8);
}

```
## target disasm
```
  000bc71a: ldr.w r0,[r0,#0x1c8]
  000bc71e: bx lr
```
