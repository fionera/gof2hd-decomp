# Agent::setOfferAccepted
elf 0x1776f8 body 6
Sig: undefined __thiscall setOfferAccepted(Agent * this, bool param_1)

## decompile
```c

/* Agent::setOfferAccepted(bool) */

void __thiscall Agent::setOfferAccepted(Agent *this,bool param_1)

{
  this[0x84] = (Agent)param_1;
  return;
}

```

## target disasm
```
  001876f8: strb.w r1,[r0,#0x84]
  001876fc: bx lr
```
