# SolarSystem::setVisible
elf 0x1558dc body 6
Sig: undefined __thiscall setVisible(SolarSystem * this, bool param_1)

## decompile
```c

/* SolarSystem::setVisible(bool) */

void __thiscall SolarSystem::setVisible(SolarSystem *this,bool param_1)

{
  this[0x44] = (SolarSystem)param_1;
  return;
}

```

## target disasm
```
  001658dc: strb.w r1,[r0,#0x44]
  001658e0: bx lr
```
