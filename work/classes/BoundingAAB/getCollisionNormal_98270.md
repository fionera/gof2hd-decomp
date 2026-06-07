# BoundingAAB::getCollisionNormal
elf 0x98270 body 10
Sig: undefined __stdcall getCollisionNormal(Vector * param_1)

## decompile
```c

/* BoundingAAB::getCollisionNormal(AbyssEngine::AEMath::Vector const&) */

void BoundingAAB::getCollisionNormal(Vector *param_1)

{
  *(undefined4 *)param_1 = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}

```

## target disasm
```
  000a8270: movs r1,#0x0
  000a8272: strd r1,r1,[r0,#0x0]
  000a8276: str r1,[r0,#0x8]
  000a8278: bx lr
```
