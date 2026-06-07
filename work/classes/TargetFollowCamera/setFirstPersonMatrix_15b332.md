# TargetFollowCamera::setFirstPersonMatrix
elf 0x15b332 body 6
Sig: undefined __stdcall setFirstPersonMatrix(Matrix * param_1)

## decompile
```c

/* TargetFollowCamera::setFirstPersonMatrix(AbyssEngine::AEMath::Matrix&) */

void TargetFollowCamera::setFirstPersonMatrix(Matrix *param_1)

{
  (*(code *)(DAT_001abdb4 + 0x1abdb8))(param_1 + 0xb4);
  return;
}

```

## target disasm
```
  0016b332: adds r0,#0xb4
  0016b334: b.w 0x001abda8
```
