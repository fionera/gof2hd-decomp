# TargetFollowCamera::setActive
elf 0x15a99a body 6
Sig: undefined __thiscall setActive(TargetFollowCamera * this, bool param_1)

## decompile
```c

/* TargetFollowCamera::setActive(bool) */

void __thiscall TargetFollowCamera::setActive(TargetFollowCamera *this,bool param_1)

{
  *(bool *)((int)&this->field_44 + 2) = param_1;
  return;
}

```

## target disasm
```
  0016a99a: strb.w r1,[r0,#0x46]
  0016a99e: bx lr
```
