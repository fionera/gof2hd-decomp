# Agent::giveRewardAtNextChat
elf 0x177704 body 6
Sig: undefined __thiscall giveRewardAtNextChat(Agent * this, bool param_1)

## decompile
```c

/* Agent::giveRewardAtNextChat(bool) */

void __thiscall Agent::giveRewardAtNextChat(Agent *this,bool param_1)

{
  this[0x85] = (Agent)param_1;
  return;
}

```

## target disasm
```
  00187704: strb.w r1,[r0,#0x85]
  00187708: bx lr
```
