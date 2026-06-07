# TargetFollowCamera::hideShipForFirstPersonCam
elf 0x15b32c body 6
Sig: undefined __stdcall hideShipForFirstPersonCam(void)

## decompile
```c

/* TargetFollowCamera::hideShipForFirstPersonCam() */

undefined1 TargetFollowCamera::hideShipForFirstPersonCam(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x100);
}

```

## target disasm
```
  0016b32c: ldrb.w r0,[r0,#0x100]
  0016b330: bx lr
```
