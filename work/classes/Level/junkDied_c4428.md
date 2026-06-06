# Level::junkDied
elf 0xc4428 body 30
Sig: undefined __stdcall junkDied(void)

## decompile
```c

/* Level::junkDied() */

void Level::junkDied(void)

{
  int in_r0;
  
  *(int *)(**(int **)(DAT_000d4448 + 0xd442e) + 0xb0) =
       *(int *)(**(int **)(DAT_000d4448 + 0xd442e) + 0xb0) + 1;
  *(int *)(in_r0 + 0x118) = *(int *)(in_r0 + 0x118) + -1;
  return;
}

```
## target disasm
```
  000d4428: ldr r1,[0x000d4448]
  000d442a: add r1,pc
  000d442c: ldr r1,[r1,#0x0]
  000d442e: ldr r1,[r1,#0x0]
  000d4430: ldr.w r2,[r1,#0xb0]
  000d4434: adds r2,#0x1
  000d4436: str.w r2,[r1,#0xb0]
  000d443a: ldr.w r1,[r0,#0x118]
  000d443e: subs r1,#0x1
  000d4440: str.w r1,[r0,#0x118]
  000d4444: bx lr
```
