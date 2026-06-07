# Agent::getImageParts
elf 0x177556 body 6
Sig: undefined __thiscall getImageParts(Agent * this)

## decompile
```c

/* Agent::getImageParts() */

undefined4 __thiscall Agent::getImageParts(Agent *this)

{
  return *(undefined4 *)(this + 0x88);
}

```

## target disasm
```
  00187556: ldr.w r0,[r0,#0x88]
  0018755a: bx lr
```
