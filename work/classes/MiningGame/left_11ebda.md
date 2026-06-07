# MiningGame::left
elf 0x11ebda body 18
Sig: undefined __thiscall left(MiningGame * this, float param_1)

## decompile
```c

/* MiningGame::left(float) */

void __thiscall MiningGame::left(MiningGame *this,float param_1)

{
  float in_r1;
  
  *(float *)this = in_r1 * 3.0;
  return;
}

```

## target disasm
```
  0012ebda: vmov.f32 s0,0x40400000
  0012ebde: vmov s2,r1
  0012ebe2: vmul.f32 s0,s2,s0
  0012ebe6: vstr.32 s0,[r0]
  0012ebea: bx lr
```
