# TargetFollowCamera::isInFastForwardMode
elf 0x15b5c8 body 6
Sig: undefined __thiscall isInFastForwardMode(TargetFollowCamera * this)

## decompile
```c

/* TargetFollowCamera::isInFastForwardMode() */

undefined1 __thiscall TargetFollowCamera::isInFastForwardMode(TargetFollowCamera *this)

{
  return *(undefined1 *)((int)&this->field_4C + 1);
}

```

## target disasm
```
  0016b5c8: ldrb.w r0,[r0,#0x4d]
  0016b5cc: bx lr
```
