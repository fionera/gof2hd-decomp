# Ship::getCurrentLoad
elf 0x174c48 body 4
Sig: undefined __stdcall getCurrentLoad(void)

## decompile
```c

/* Ship::getCurrentLoad() */

undefined4 Ship::getCurrentLoad(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x10);
}

```
## target disasm
```
  00184c48: ldr r0,[r0,#0x10]
  00184c4a: bx lr
```
