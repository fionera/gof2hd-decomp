# Ship::setRace
elf 0x174a02 body 4
Sig: undefined __stdcall setRace(int param_1)

## decompile
```c

/* Ship::setRace(int) */

void Ship::setRace(int param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 100) = in_r1;
  return;
}

```
## target disasm
```
  00184a02: str r1,[r0,#0x64]
  00184a04: bx lr
```
