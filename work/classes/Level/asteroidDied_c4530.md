# Level::asteroidDied
elf 0xc4530 body 12
Sig: undefined __stdcall asteroidDied(void)

## decompile
```c

/* Level::asteroidDied() */

void Level::asteroidDied(void)

{
  int in_r0;
  
  *(int *)(in_r0 + 0x128) = *(int *)(in_r0 + 0x128) + -1;
  return;
}

```
## target disasm
```
  000d4530: ldr.w r1,[r0,#0x128]
  000d4534: subs r1,#0x1
  000d4536: str.w r1,[r0,#0x128]
  000d453a: bx lr
```
