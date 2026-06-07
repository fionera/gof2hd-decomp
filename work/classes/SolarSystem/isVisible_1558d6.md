# SolarSystem::isVisible
elf 0x1558d6 body 6
Sig: undefined __thiscall isVisible(SolarSystem * this)

## decompile
```c

/* SolarSystem::isVisible() */

SolarSystem __thiscall SolarSystem::isVisible(SolarSystem *this)

{
  return this[0x44];
}

```

## target disasm
```
  001658d6: ldrb.w r0,[r0,#0x44]
  001658da: bx lr
```
