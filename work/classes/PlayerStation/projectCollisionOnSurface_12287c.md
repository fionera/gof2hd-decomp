# PlayerStation::projectCollisionOnSurface
elf 0x12287c body 18
Sig: undefined __stdcall projectCollisionOnSurface(Vector * param_1)

## decompile
```c

/* PlayerStation::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) */

void PlayerStation::projectCollisionOnSurface(Vector *param_1)

{
  int in_r1;
  Vector *in_r2;
  
  BoundingVolume::staticProjectCollisionOnSurface
            ((BoundingVolume *)param_1,in_r2,*(Array **)(in_r1 + 0x130));
  return;
}

```

## target disasm
```
  0013287c: push {r7,lr}
  0013287e: mov r7,sp
  00132880: mov r3,r2
  00132882: ldr.w r2,[r1,#0x130]
  00132886: mov r1,r3
  00132888: blx 0x00075ff4
  0013288c: pop {r7,pc}
```
