# TargetFollowCamera::setRumblePercentage
elf 0x15b34a body 6
Sig: undefined __stdcall setRumblePercentage(float param_1, int param_2)

## decompile
```c

/* TargetFollowCamera::setRumblePercentage(float, int) */

void TargetFollowCamera::setRumblePercentage(float param_1,int param_2)

{
  undefined4 in_r1;
  undefined4 in_r2;
  
  *(undefined4 *)(param_2 + 0x110) = in_r1;
  *(undefined4 *)(param_2 + 0x114) = in_r2;
  return;
}

```

## target disasm
```
  0016b34a: strd r1,r2,[r0,#0x110]
  0016b34e: bx lr
```
