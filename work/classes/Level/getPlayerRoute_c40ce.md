# Level::getPlayerRoute
elf 0xc40ce body 6
Sig: undefined __stdcall getPlayerRoute(void)

## decompile
```c

/* Level::getPlayerRoute() */

undefined4 Level::getPlayerRoute(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x108);
}

```
## target disasm
```
  000d40ce: ldr.w r0,[r0,#0x108]
  000d40d2: bx lr
```
