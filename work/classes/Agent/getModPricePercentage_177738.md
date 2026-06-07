# Agent::getModPricePercentage
elf 0x177738 body 22
Sig: undefined __thiscall getModPricePercentage(Agent * this)

## decompile
```c

/* Agent::getModPricePercentage() */

undefined4 __thiscall Agent::getModPricePercentage(Agent *this)

{
  if (*(uint *)(this + 0x94) < 4) {
    return *(undefined4 *)(DAT_00187750 + 0x18774a + *(uint *)(this + 0x94) * 4);
  }
  return 0x28;
}

```

## target disasm
```
  00187738: ldr.w r0,[r0,#0x94]
  0018773c: cmp r0,#0x3
  0018773e: itt hi
  00187740: mov.hi r0,#0x28
  00187742: bx.hi lr
  00187744: ldr r1,[0x00187750]
  00187746: add r1,pc
  00187748: ldr.w r0,[r1,r0,lsl #0x2]
  0018774c: bx lr
```
