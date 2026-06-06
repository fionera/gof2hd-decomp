# TargetFollowCamera::setTargetOffset

ELF vaddr 0x15a9a8, body 8 bytes
Signature: undefined __stdcall setTargetOffset(Vector * param_1)

## Ghidra decompile
```c

/* TargetFollowCamera::setTargetOffset(AbyssEngine::AEMath::Vector const&) */

void TargetFollowCamera::setTargetOffset(Vector *param_1)

{
  (*(code *)(DAT_001ac5b4 + 0x1ac5b8))(param_1 + 0x2c);
  return;
}

```

## Target disassembly (must match)
```
  0016a9a8: adds r0,#0x2c
  0016a9aa: b.w 0x001ac5a8
  001ac5a8: bx pc
```

Verify: tools/try.sh work/src/setTargetOffset.cpp setTargetOffset 15a9a8 32
