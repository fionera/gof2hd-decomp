# Agent::hasAcceptedOffer
elf 0x1776f2 body 6
Sig: undefined __thiscall hasAcceptedOffer(Agent * this)

## decompile
```c

/* Agent::hasAcceptedOffer() */

Agent __thiscall Agent::hasAcceptedOffer(Agent *this)

{
  return this[0x84];
}

```

## target disasm
```
  001876f2: ldrb.w r0,[r0,#0x84]
  001876f6: bx lr
```
