# Ship::removeCargo
elf 0x174ef4 body 8
Sig: undefined __stdcall removeCargo(int param_1)

## decompile
```c

/* Ship::removeCargo(int) */

void Ship::removeCargo(int param_1)

{
  undefined4 in_r1;
  
  (*(code *)(DAT_001ac734 + 0x1ac738))(param_1,in_r1,DAT_00184efc);
  return;
}

```
## target disasm
```
  00184ef4: ldr r2,[0x00184efc]
  00184ef6: b.w 0x001ac728
  001ac728: bx pc
```
