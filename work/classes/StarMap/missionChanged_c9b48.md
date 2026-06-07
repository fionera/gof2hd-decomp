# StarMap::missionChanged
elf 0xc9b48 body 6
Sig: undefined __thiscall missionChanged(StarMap * this)

## decompile
```c

/* StarMap::missionChanged() */

StarMap __thiscall StarMap::missionChanged(StarMap *this)

{
  return this[0xdc];
}

```

## target disasm
```
  000d9b48: ldrb.w r0,[r0,#0xdc]
  000d9b4c: bx lr
```
