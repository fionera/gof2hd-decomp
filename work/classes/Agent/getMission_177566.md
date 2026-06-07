# Agent::getMission
elf 0x177566 body 6
Sig: undefined __thiscall getMission(Agent * this)

## decompile
```c

/* Agent::getMission() */

undefined4 __thiscall Agent::getMission(Agent *this)

{
  return *(undefined4 *)(this + 0x8c);
}

```

## target disasm
```
  00187566: ldr.w r0,[r0,#0x8c]
  0018756a: bx lr
```
