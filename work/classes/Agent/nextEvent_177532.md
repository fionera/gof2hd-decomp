# Agent::nextEvent
elf 0x177532 body 8
Sig: undefined __thiscall nextEvent(Agent * this)

## decompile
```c

/* Agent::nextEvent() */

void __thiscall Agent::nextEvent(Agent *this)

{
  *(int *)(this + 0x54) = *(int *)(this + 0x54) + 1;
  return;
}

```

## target disasm
```
  00187532: ldr r1,[r0,#0x54]
  00187534: adds r1,#0x1
  00187536: str r1,[r0,#0x54]
  00187538: bx lr
```
