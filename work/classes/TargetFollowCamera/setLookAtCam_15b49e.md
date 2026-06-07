# TargetFollowCamera::setLookAtCam
elf 0x15b49e body 6
Sig: undefined __stdcall setLookAtCam(bool param_1)

## decompile
```c

/* TargetFollowCamera::setLookAtCam(bool) */

void TargetFollowCamera::setLookAtCam(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x45) = in_r1;
  return;
}

```

## target disasm
```
  0016b49e: strb.w r1,[r0,#0x45]
  0016b4a2: bx lr
```
