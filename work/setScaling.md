# AEGeometry::setScaling

ELF vaddr 0xa47f8, body 8 bytes
Signature: undefined __stdcall setScaling(float param_1)

## Ghidra decompile
```c

/* AEGeometry::setScaling(float) */

void AEGeometry::setScaling(float param_1)

{
  (*(code *)(DAT_001abd94 + 0x1abd98))();
  return;
}

```

## Target disassembly (must match)
```
  000b47f8: mov r2,r1
  000b47fa: mov r3,r1
  000b47fc: b.w 0x001abd88
```

Verify: tools/try.sh work/src/setScaling.cpp setScaling a47f8 32
