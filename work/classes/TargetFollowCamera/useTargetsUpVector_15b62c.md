# TargetFollowCamera::useTargetsUpVector
elf 0x15b62c body 6
Sig: undefined __stdcall useTargetsUpVector(bool param_1)

## decompile
```c

/* TargetFollowCamera::useTargetsUpVector(bool) */

void TargetFollowCamera::useTargetsUpVector(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x10c) = in_r1;
  return;
}

```

## target disasm
```
  0016b62c: strb.w r1,[r0,#0x10c]
  0016b630: bx lr
```
