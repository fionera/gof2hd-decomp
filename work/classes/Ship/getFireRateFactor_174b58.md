# Ship::getFireRateFactor
elf 0x174b58 body 4
Sig: undefined __stdcall getFireRateFactor(void)

## decompile
```c

/* Ship::getFireRateFactor() */

undefined4 Ship::getFireRateFactor(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x54);
}

```
## target disasm
```
  00184b58: ldr r0,[r0,#0x54]
  00184b5a: bx lr
```
