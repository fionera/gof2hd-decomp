# PlayerStaticFar::getProjectionVector
elf 0x11c510 body 30
Sig: undefined __thiscall getProjectionVector(PlayerStaticFar * this, Vector * param_1)

## decompile
```c

/* PlayerStaticFar::getProjectionVector(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerStaticFar::getProjectionVector(PlayerStaticFar *this,Vector *param_1)

{
  if (*(int *)(param_1 + 0x130) != 0) {
    BoundingVolume::getProjectionVector((Vector *)this);
    return;
  }
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  return;
}

```

## target disasm
```
  0012c510: ldr.w r1,[r1,#0x130]
  0012c514: cbz r1,0x0012c524
  0012c516: push {r7,lr}
  0012c518: mov r7,sp
  0012c51a: ldr r1,[r1,#0x4]
  0012c51c: ldr r1,[r1,#0x0]
  0012c51e: blx 0x00075fe8
  0012c522: pop {r7,pc}
  0012c524: movs r1,#0x0
  0012c526: strd r1,r1,[r0,#0x0]
  0012c52a: str r1,[r0,#0x8]
  0012c52c: bx lr
```
