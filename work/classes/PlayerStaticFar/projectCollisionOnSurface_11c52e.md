# PlayerStaticFar::projectCollisionOnSurface
elf 0x11c52e body 30
Sig: undefined __stdcall projectCollisionOnSurface(Vector * param_1)

## decompile
```c

/* PlayerStaticFar::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) */

void PlayerStaticFar::projectCollisionOnSurface(Vector *param_1)

{
  int in_r1;
  Vector *in_r2;
  
  if (*(Array **)(in_r1 + 0x130) != (Array *)0x0) {
    BoundingVolume::staticProjectCollisionOnSurface
              ((BoundingVolume *)param_1,in_r2,*(Array **)(in_r1 + 0x130));
    return;
  }
  *(undefined4 *)param_1 = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}

```

## target disasm
```
  0012c52e: ldr.w r3,[r1,#0x130]
  0012c532: cbz r3,0x0012c542
  0012c534: push {r7,lr}
  0012c536: mov r7,sp
  0012c538: mov r1,r2
  0012c53a: mov r2,r3
  0012c53c: blx 0x00075ff4
  0012c540: pop {r7,pc}
  0012c542: movs r1,#0x0
  0012c544: strd r1,r1,[r0,#0x0]
  0012c548: str r1,[r0,#0x8]
  0012c54a: bx lr
```
