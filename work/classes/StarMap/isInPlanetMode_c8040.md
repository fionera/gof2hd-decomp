# StarMap::isInPlanetMode
elf 0xc8040 body 12
Sig: undefined __thiscall isInPlanetMode(StarMap * this)

## decompile
```c

/* StarMap::isInPlanetMode() */

bool __thiscall StarMap::isInPlanetMode(StarMap *this)

{
  return *(int *)(this + 4) == 3;
}

```

## target disasm
```
  000d8040: ldr r0,[r0,#0x4]
  000d8042: subs r0,#0x3
  000d8044: clz r0,r0
  000d8048: lsrs r0,r0,#0x5
  000d804a: bx lr
```
