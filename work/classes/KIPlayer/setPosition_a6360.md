# KIPlayer::setPosition
elf 0xa6360 body 50
Sig: undefined __stdcall setPosition(Vector * param_1)

## decompile
```c

/* KIPlayer::setPosition(AbyssEngine::AEMath::Vector const&) */

void KIPlayer::setPosition(Vector *param_1)

{
  Matrix *pMVar1;
  Vector *in_r1;
  
  if (*(AEGeometry **)(param_1 + 8) != (AEGeometry *)0x0) {
    AEGeometry::setPosition(*(AEGeometry **)(param_1 + 8),in_r1);
    pMVar1 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(param_1 + 4) + 4),pMVar1);
    if (*(int *)(param_1 + 0xc) != 0) {
      pMVar1 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::Matrix::operator*=((Matrix *)(*(int *)(param_1 + 4) + 4),pMVar1);
    }
  }
  return;
}

```

## target disasm
```
  000b6360: push {r4,r6,r7,lr}
  000b6362: add r7,sp,#0x8
  000b6364: mov r4,r0
  000b6366: ldr r0,[r0,#0x8]
  000b6368: cbz r0,0x000b6390
  000b636a: blx 0x00072148
  000b636e: ldr r0,[r4,#0x8]
  000b6370: blx 0x000721cc
  000b6374: mov r1,r0
  000b6376: ldr r0,[r4,#0x4]
  000b6378: adds r0,#0x4
  000b637a: blx 0x0006f148
  000b637e: ldr r0,[r4,#0xc]
  000b6380: cbz r0,0x000b6390
  000b6382: blx 0x000721cc
  000b6386: mov r1,r0
  000b6388: ldr r0,[r4,#0x4]
  000b638a: adds r0,#0x4
  000b638c: blx 0x0006f4a8
  000b6390: pop {r4,r6,r7,pc}
```
