# MiningGame::down
elf 0x11ebc8 body 18
Sig: undefined __thiscall down(MiningGame * this, float param_1)

## decompile
```c

/* MiningGame::down(float) */

void __thiscall MiningGame::down(MiningGame *this,float param_1)

{
  float in_r1;
  
  *(float *)(this + 4) = in_r1 * 3.0;
  return;
}

```

## target disasm
```
  0012ebc8: vmov.f32 s0,0x40400000
  0012ebcc: vmov s2,r1
  0012ebd0: vmul.f32 s0,s2,s0
  0012ebd4: vstr.32 s0,[r0,#0x4]
  0012ebd8: bx lr
```
