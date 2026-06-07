# Agent::isStoryAgent
elf 0x17753a body 10
Sig: undefined __thiscall isStoryAgent(Agent * this)

## decompile
```c

/* Agent::isStoryAgent() */

bool __thiscall Agent::isStoryAgent(Agent *this)

{
  return *(int *)(this + 0x58) == 0;
}

```

## target disasm
```
  0018753a: ldr r0,[r0,#0x58]
  0018753c: clz r0,r0
  00187540: lsrs r0,r0,#0x5
  00187542: bx lr
```
