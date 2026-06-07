# AEGeometry::setRotation
elf 0xa4778 body 14
Sig: undefined __thiscall setRotation(AEGeometry * this, Vector * param_1)

## decompile
```c

/* AEGeometry::setRotation(AbyssEngine::AEMath::Vector const&) */

void __thiscall AEGeometry::setRotation(AEGeometry *this,Vector *param_1)

{
  (*(code *)(DAT_001abd84 + 0x1abd88))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  000b4778: ldrd r12,r2,[r1,#0x0]
  000b477c: ldr r3,[r1,#0x8]
  000b477e: mov r1,r12
  000b4780: b.w 0x001abd78
  001abd78: bx pc
```
