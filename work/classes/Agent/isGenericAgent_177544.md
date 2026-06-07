# Agent::isGenericAgent
elf 0x177544 body 12
Sig: undefined __thiscall isGenericAgent(Agent * this)

## decompile
```c

/* Agent::isGenericAgent() */

bool __thiscall Agent::isGenericAgent(Agent *this)

{
  return *(int *)(this + 0x58) == 1;
}

```

## target disasm
```
  00187544: ldr r0,[r0,#0x58]
  00187546: subs r0,#0x1
  00187548: clz r0,r0
  0018754c: lsrs r0,r0,#0x5
  0018754e: bx lr
```
