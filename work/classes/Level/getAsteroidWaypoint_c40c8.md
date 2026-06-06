# Level::getAsteroidWaypoint
elf 0xc40c8 body 6
Sig: undefined __stdcall getAsteroidWaypoint(void)

## decompile
```c

/* Level::getAsteroidWaypoint() */

undefined4 Level::getAsteroidWaypoint(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xd8);
}

```
## target disasm
```
  000d40c8: ldr.w r0,[r0,#0xd8]
  000d40cc: bx lr
```
