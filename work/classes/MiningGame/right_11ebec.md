# MiningGame::right
elf 0x11ebec body 18
Sig: undefined __thiscall right(MiningGame * this, float param_1)

## decompile
```c

/* MiningGame::right(float) */

void __thiscall MiningGame::right(MiningGame *this,float param_1)

{
  float in_r1;
  
  *(float *)this = in_r1 * 3.0;
  return;
}

```

## target disasm
```
  0012ebec: vmov.f32 s0,0x40400000
  0012ebf0: vmov s2,r1
  0012ebf4: vmul.f32 s0,s2,s0
  0012ebf8: vstr.32 s0,[r0]
  0012ebfc: bx lr
```
