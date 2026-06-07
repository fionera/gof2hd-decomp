# TargetFollowCamera::setRotationAroundTarget
elf 0x15b464 body 6
Sig: undefined __thiscall setRotationAroundTarget(TargetFollowCamera * this, bool param_1)

## decompile
```c

/* TargetFollowCamera::setRotationAroundTarget(bool) */

void __thiscall TargetFollowCamera::setRotationAroundTarget(TargetFollowCamera *this,bool param_1)

{
  *(bool *)&this->field_4C = param_1;
  return;
}

```

## target disasm
```
  0016b464: strb.w r1,[r0,#0x4c]
  0016b468: bx lr
```
