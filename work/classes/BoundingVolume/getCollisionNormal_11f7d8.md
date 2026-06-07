# BoundingVolume::getCollisionNormal
elf 0x11f7d8 body 10
Sig: undefined __stdcall getCollisionNormal(Vector * param_1)

## decompile
```c

/* BoundingVolume::getCollisionNormal(AbyssEngine::AEMath::Vector const&) */

void BoundingVolume::getCollisionNormal(Vector *param_1)

{
  *(undefined4 *)param_1 = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}

```

## target disasm
```
  0012f7d8: movs r1,#0x0
  0012f7da: strd r1,r1,[r0,#0x0]
  0012f7de: str r1,[r0,#0x8]
  0012f7e0: bx lr
```
