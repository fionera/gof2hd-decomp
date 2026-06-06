# Ship::setPrice
elf 0x1755b8 body 4
Sig: undefined __stdcall setPrice(int param_1)

## decompile
```c

/* Ship::setPrice(int) */

void Ship::setPrice(int param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0x14) = in_r1;
  return;
}

```
## target disasm
```
  001855b8: str r1,[r0,#0x14]
  001855ba: bx lr
```
