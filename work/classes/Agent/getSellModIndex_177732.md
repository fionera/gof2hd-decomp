# Agent::getSellModIndex
elf 0x177732 body 6
Sig: undefined __thiscall getSellModIndex(Agent * this)

## decompile
```c

/* Agent::getSellModIndex() */

undefined4 __thiscall Agent::getSellModIndex(Agent *this)

{
  return *(undefined4 *)(this + 0x94);
}

```

## target disasm
```
  00187732: ldr.w r0,[r0,#0x94]
  00187736: bx lr
```
