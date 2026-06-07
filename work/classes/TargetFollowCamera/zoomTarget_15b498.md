# TargetFollowCamera::zoomTarget
elf 0x15b498 body 6
Sig: undefined __stdcall zoomTarget(float param_1)

## decompile
```c

/* TargetFollowCamera::zoomTarget(float) */

void TargetFollowCamera::zoomTarget(float param_1)

{
  int in_r0;
  undefined4 in_r1;
  
  *(undefined4 *)(in_r0 + 0xb0) = in_r1;
  return;
}

```

## target disasm
```
  0016b498: str.w r1,[r0,#0xb0]
  0016b49c: bx lr
```
