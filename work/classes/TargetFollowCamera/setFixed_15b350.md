# TargetFollowCamera::setFixed
elf 0x15b350 body 6
Sig: undefined __thiscall setFixed(TargetFollowCamera * this, bool param_1)

## decompile
```c

/* TargetFollowCamera::setFixed(bool) */

void __thiscall TargetFollowCamera::setFixed(TargetFollowCamera *this,bool param_1)

{
  *(bool *)&this[1].field_20 = param_1;
  return;
}

```

## target disasm
```
  0016b350: strb.w r1,[r0,#0x138]
  0016b354: bx lr
```
