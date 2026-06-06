# Status::getSystem
elf 0xabd60 body 6
Sig: undefined __stdcall getSystem(void)

## decompile
```c

/* Status::getSystem() */

undefined4 Status::getSystem(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x1a4);
}

```
## target disasm
```
  000bbd60: ldr.w r0,[r0,#0x1a4]
  000bbd64: bx lr
```
