# Ship::getMaxPassengers
elf 0x1752d4 body 4
Sig: undefined __stdcall getMaxPassengers(void)

## decompile
```c

/* Ship::getMaxPassengers() */

undefined4 Ship::getMaxPassengers(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x44);
}

```
## target disasm
```
  001852d4: ldr r0,[r0,#0x44]
  001852d6: bx lr
```
