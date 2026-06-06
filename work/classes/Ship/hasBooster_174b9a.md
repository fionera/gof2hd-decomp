# Ship::hasBooster
elf 0x174b9a body 12
Sig: undefined __stdcall hasBooster(void)

## decompile
```c

/* Ship::hasBooster() */

bool Ship::hasBooster(void)

{
  int in_r0;
  
  return 0 < *(int *)(in_r0 + 0x34);
}

```
## target disasm
```
  00184b9a: ldr r1,[r0,#0x34]
  00184b9c: movs r0,#0x0
  00184b9e: cmp r1,#0x0
  00184ba0: it gt
  00184ba2: mov.gt r0,#0x1
  00184ba4: bx lr
```
