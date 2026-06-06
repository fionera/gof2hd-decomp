# Level::getStarSystem
elf 0xbde48 body 6
Sig: undefined __stdcall getStarSystem(void)

## decompile
```c

/* Level::getStarSystem() */

undefined4 Level::getStarSystem(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xec);
}

```
## target disasm
```
  000cde48: ldr.w r0,[r0,#0xec]
  000cde4c: bx lr
```
