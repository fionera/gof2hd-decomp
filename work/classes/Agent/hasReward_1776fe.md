# Agent::hasReward
elf 0x1776fe body 6
Sig: undefined __thiscall hasReward(Agent * this)

## decompile
```c

/* Agent::hasReward() */

Agent __thiscall Agent::hasReward(Agent *this)

{
  return this[0x85];
}

```

## target disasm
```
  001876fe: ldrb.w r0,[r0,#0x85]
  00187702: bx lr
```
