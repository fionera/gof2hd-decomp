# AEGeometry::setScaling
elf 0xa47f8 body 8
Sig: undefined __stdcall setScaling(float param_1)

## decompile
```c

/* AEGeometry::setScaling(float) */

void AEGeometry::setScaling(float param_1)

{
  (*(code *)(DAT_001abd94 + 0x1abd98))();
  return;
}

```

## target disasm
```
  000b47f8: mov r2,r1
  000b47fa: mov r3,r1
  000b47fc: b.w 0x001abd88
```
