# Level::getPlayer
elf 0xc40b0 body 6
Sig: undefined __stdcall getPlayer(void)

## decompile
```c

/* Level::getPlayer() */

undefined4 Level::getPlayer(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xf0);
}

```
## target disasm
```
  000d40b0: ldr.w r0,[r0,#0xf0]
  000d40b4: bx lr
```
