# Agent::isKnown
elf 0x17756c body 12
Sig: undefined __thiscall isKnown(Agent * this)

## decompile
```c

/* Agent::isKnown() */

bool __thiscall Agent::isKnown(Agent *this)

{
  return 0 < *(int *)(this + 0x54);
}

```

## target disasm
```
  0018756c: ldr r1,[r0,#0x54]
  0018756e: movs r0,#0x0
  00187570: cmp r1,#0x0
  00187572: it gt
  00187574: mov.gt r0,#0x1
  00187576: bx lr
```
