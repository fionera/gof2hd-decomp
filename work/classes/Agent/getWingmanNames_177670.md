# Agent::getWingmanNames
elf 0x177670 body 6
Sig: undefined __thiscall getWingmanNames(Agent * this)

## decompile
```c

/* Agent::getWingmanNames() */

undefined4 __thiscall Agent::getWingmanNames(Agent *this)

{
  return *(undefined4 *)(this + 0x90);
}

```

## target disasm
```
  00187670: ldr.w r0,[r0,#0x90]
  00187674: bx lr
```
