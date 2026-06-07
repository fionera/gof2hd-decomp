# TargetFollowCamera::setPosition
elf 0x15a9d0 body 6
Sig: undefined __thiscall setPosition(TargetFollowCamera * this, float param_1, float param_2, float param_3)

## decompile
```c

/* TargetFollowCamera::setPosition(float, float, float) */

int * __thiscall
TargetFollowCamera::setPosition(TargetFollowCamera *this,float param_1,float param_2,float param_3)

{
  float in_r1;
  float in_r2;
  float in_r3;
  
  this->m_nPosX = in_r1;
  this->m_nPosY = in_r2;
  this->m_nPosZ = in_r3;
  return &this->field_14;
}

```

## target disasm
```
  0016a9d0: adds r0,#0x8
  0016a9d2: stmia r0!,{r1,r2,r3}
  0016a9d4: bx lr
```
