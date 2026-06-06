# Ship::getMaxShieldHP
elf 0x174b38 body 4
Sig: undefined __stdcall getMaxShieldHP(void)

## decompile
```c

/* Ship::getMaxShieldHP() */

undefined4 Ship::getMaxShieldHP(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x1c);
}

```
## target disasm
```
  00184b38: ldr r0,[r0,#0x1c]
  00184b3a: bx lr
```
