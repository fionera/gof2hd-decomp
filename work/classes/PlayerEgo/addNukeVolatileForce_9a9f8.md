# PlayerEgo::addNukeVolatileForce
elf 0x9a9f8 body 24
Sig: undefined __stdcall addNukeVolatileForce(float param_1)

## decompile
```c

/* PlayerEgo::addNukeVolatileForce(float) */

void PlayerEgo::addNukeVolatileForce(float param_1)

{
  int *in_r0;
  float in_r1;
  
  *(float *)(*in_r0 + 0x60) = *(float *)(*in_r0 + 0x60) + in_r1 * 3.0;
  return;
}

```

## target disasm
```
  000aa9f8: vmov.f32 s0,0x40400000
  000aa9fc: ldr r0,[r0,#0x0]
  000aa9fe: vmov s2,r1
  000aaa02: vldr.32 s4,[r0,#0x60]
  000aaa06: vmla.f32 s4,s2,s0
  000aaa0a: vstr.32 s4,[r0,#0x60]
  000aaa0e: bx lr
```
