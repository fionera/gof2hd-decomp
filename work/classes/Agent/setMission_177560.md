# Agent::setMission
elf 0x177560 body 6
Sig: undefined __thiscall setMission(Agent * this, Mission * param_1)

## decompile
```c

/* Agent::setMission(Mission*) */

void __thiscall Agent::setMission(Agent *this,Mission *param_1)

{
  *(Mission **)(this + 0x8c) = param_1;
  return;
}

```

## target disasm
```
  00187560: str.w r1,[r0,#0x8c]
  00187564: bx lr
```
