# TargetFollowCamera::setTargetOffset
elf 0x15a9a8 body 8
Sig: undefined __stdcall setTargetOffset(Vector * param_1)

## decompile
```c

/* TargetFollowCamera::setTargetOffset(AbyssEngine::AEMath::Vector const&) */

void TargetFollowCamera::setTargetOffset(Vector *param_1)

{
  (*(code *)(DAT_001ac5b4 + 0x1ac5b8))(param_1 + 0x2c);
  return;
}

```

## target disasm
```
  0016a9a8: adds r0,#0x2c
  0016a9aa: b.w 0x001ac5a8
  001ac5a8: bx pc
```
