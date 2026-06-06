# Ship::getCargo
elf 0x174c8c body 4
Sig: undefined __stdcall getCargo(void)

## decompile
```c

/* Ship::getCargo() */

undefined4 Ship::getCargo(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x70);
}

```
## target disasm
```
  00184c8c: ldr r0,[r0,#0x70]
  00184c8e: bx lr
```
