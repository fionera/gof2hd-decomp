# TargetFollowCamera::setRoll
elf 0x15b644 body 6
Sig: undefined __thiscall setRoll(TargetFollowCamera * this, float param_1)

## decompile
```c

/* TargetFollowCamera::setRoll(float) */

void __thiscall TargetFollowCamera::setRoll(TargetFollowCamera *this,float param_1)

{
  int in_r1;
  
  this[1].field_18 = in_r1;
  return;
}

```

## target disasm
```
  0016b644: str.w r1,[r0,#0x130]
  0016b648: bx lr
```
