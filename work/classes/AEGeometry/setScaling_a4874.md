# AEGeometry::setScaling
elf 0xa4874 body 12
Sig: undefined __thiscall setScaling(AEGeometry * this, Vector * param_1)

## decompile
```c

/* AEGeometry::setScaling(AbyssEngine::AEMath::Vector const&) */

void __thiscall AEGeometry::setScaling(AEGeometry *this,Vector *param_1)

{
  (*(code *)(DAT_001abd94 + 0x1abd98))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  000b4874: ldrd r12,r2,[r1,#0x0]
  000b4878: ldr r3,[r1,#0x8]
  000b487a: mov r1,r12
  000b487c: b.w 0x001abd88
```
