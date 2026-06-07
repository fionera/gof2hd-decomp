# Achievements::incCatches
elf 0x156efe body 8
Sig: undefined __thiscall incCatches(Achievements * this)

## decompile
```c

/* Achievements::incCatches() */

void __thiscall Achievements::incCatches(Achievements *this)

{
  *(int *)(this + 0xc) = *(int *)(this + 0xc) + 1;
  return;
}

```

## target disasm
```
  00166efe: ldr r1,[r0,#0xc]
  00166f00: adds r1,#0x1
  00166f02: str r1,[r0,#0xc]
  00166f04: bx lr
```
