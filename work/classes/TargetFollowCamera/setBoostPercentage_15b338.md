# TargetFollowCamera::setBoostPercentage
elf 0x15b338 body 18
Sig: undefined __thiscall setBoostPercentage(TargetFollowCamera * this, float param_1, int param_2)

## decompile
```c

/* TargetFollowCamera::setBoostPercentage(float, int) */

void __thiscall
TargetFollowCamera::setBoostPercentage(TargetFollowCamera *this,float param_1,int param_2)

{
  int in_r2;
  
  if (7 < in_r2) {
    in_r2 = 8;
  }
  if (in_r2 < 3) {
    in_r2 = 2;
  }
  this->m_nShakeAmount = (float)param_2;
  this->m_nShakeFrequency = in_r2;
  return;
}

```

## target disasm
```
  0016b338: cmp r2,#0x8
  0016b33a: it ge
  0016b33c: mov.ge r2,#0x8
  0016b33e: cmp r2,#0x2
  0016b340: it le
  0016b342: mov.le r2,#0x2
  0016b344: strd r1,r2,[r0,#0x110]
  0016b348: bx lr
```
