# SolarSystem::setCoords
elf 0x15589c body 6
Sig: undefined __thiscall setCoords(SolarSystem * this, int param_1, int param_2)

## decompile
```c

/* SolarSystem::setCoords(int, int) */

void __thiscall SolarSystem::setCoords(SolarSystem *this,int param_1,int param_2)

{
  *(int *)(this + 0x24) = param_1;
  *(int *)(this + 0x28) = param_2;
  return;
}

```

## target disasm
```
  0016589c: strd r1,r2,[r0,#0x24]
  001658a0: bx lr
```
