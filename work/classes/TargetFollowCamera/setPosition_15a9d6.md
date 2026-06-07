# TargetFollowCamera::setPosition
elf 0x15a9d6 body 14
Sig: undefined __thiscall setPosition(TargetFollowCamera * this, Vector * param_1)

## decompile
```c

/* TargetFollowCamera::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall TargetFollowCamera::setPosition(TargetFollowCamera *this,Vector *param_1)

{
  float fVar1;
  float fVar2;
  
  fVar2 = *(float *)(param_1 + 4);
  fVar1 = *(float *)(param_1 + 8);
  this->m_nPosX = *(float *)param_1;
  this->m_nPosY = fVar2;
  this->m_nPosZ = fVar1;
  return;
}

```

## target disasm
```
  0016a9d6: ldrd r2,r3,[r1,#0x0]
  0016a9da: ldr r1,[r1,#0x8]
  0016a9dc: strd r2,r3,[r0,#0x8]
  0016a9e0: str r1,[r0,#0x10]
  0016a9e2: bx lr
```
