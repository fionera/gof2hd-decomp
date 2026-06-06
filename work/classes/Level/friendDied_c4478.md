# Level::friendDied
elf 0xc4478 body 12
Sig: undefined __stdcall friendDied(void)

## decompile
```c

/* Level::friendDied() */

void Level::friendDied(void)

{
  int in_r0;
  
  *(int *)(in_r0 + 0x11c) = *(int *)(in_r0 + 0x11c) + -1;
  return;
}

```
## target disasm
```
  000d4478: ldr.w r1,[r0,#0x11c]
  000d447c: subs r1,#0x1
  000d447e: str.w r1,[r0,#0x11c]
  000d4482: bx lr
```
