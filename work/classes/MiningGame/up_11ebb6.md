# MiningGame::up
elf 0x11ebb6 body 18
Sig: undefined __thiscall up(MiningGame * this, float param_1)

## decompile
```c

/* MiningGame::up(float) */

void __thiscall MiningGame::up(MiningGame *this,float param_1)

{
  float in_r1;
  
  *(float *)(this + 4) = in_r1 * 3.0;
  return;
}

```

## target disasm
```
  0012ebb6: vmov.f32 s0,0x40400000
  0012ebba: vmov s2,r1
  0012ebbe: vmul.f32 s0,s2,s0
  0012ebc2: vstr.32 s0,[r0,#0x4]
  0012ebc6: bx lr
```
