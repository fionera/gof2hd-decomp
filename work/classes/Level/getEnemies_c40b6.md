# Level::getEnemies
elf 0xc40b6 body 6
Sig: undefined __stdcall getEnemies(void)

## decompile
```c

/* Level::getEnemies() */

undefined4 Level::getEnemies(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xf8);
}

```
## target disasm
```
  000d40b6: ldr.w r0,[r0,#0xf8]
  000d40ba: bx lr
```
