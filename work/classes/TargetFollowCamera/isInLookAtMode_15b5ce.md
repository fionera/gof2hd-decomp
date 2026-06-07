# TargetFollowCamera::isInLookAtMode
elf 0x15b5ce body 6
Sig: undefined __thiscall isInLookAtMode(TargetFollowCamera * this)

## decompile
```c

/* TargetFollowCamera::isInLookAtMode() */

undefined1 __thiscall TargetFollowCamera::isInLookAtMode(TargetFollowCamera *this)

{
  return *(undefined1 *)((int)&this->field_44 + 1);
}

```

## target disasm
```
  0016b5ce: ldrb.w r0,[r0,#0x45]
  0016b5d2: bx lr
```
