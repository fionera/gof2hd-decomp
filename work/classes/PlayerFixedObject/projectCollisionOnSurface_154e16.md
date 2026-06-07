# PlayerFixedObject::projectCollisionOnSurface
elf 0x154e16 body 58
Sig: undefined __stdcall projectCollisionOnSurface(Vector * param_1)

## decompile
```c

/* PlayerFixedObject::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) */

void PlayerFixedObject::projectCollisionOnSurface(Vector *param_1)

{
  int in_r1;
  Vector *in_r2;
  
  if ((*(Array **)(in_r1 + 300) != (Array *)0x0) && (*(int *)(in_r1 + 0x88) == 4)) {
    BoundingVolume::staticProjectCollisionOnSurface
              ((BoundingVolume *)param_1,in_r2,*(Array **)(in_r1 + 300));
    return;
  }
  if (*(Array **)(in_r1 + 0x128) != (Array *)0x0) {
    BoundingVolume::staticProjectCollisionOnSurface
              ((BoundingVolume *)param_1,in_r2,*(Array **)(in_r1 + 0x128));
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
  00164e16: push {r7,lr}
  00164e18: mov r7,sp
  00164e1a: ldr.w r12,[r1,#0x12c]
  00164e1e: cmp.w r12,#0x0
  00164e22: beq 0x00164e36
  00164e24: ldr.w r3,[r1,#0x88]
  00164e28: cmp r3,#0x4
  00164e2a: bne 0x00164e36
  00164e2c: mov r1,r2
  00164e2e: mov r2,r12
  00164e30: blx 0x00075ff4
  00164e34: pop {r7,pc}
  00164e36: ldr.w r3,[r1,#0x128]
  00164e3a: cbz r3,0x00164e46
  00164e3c: mov r1,r2
  00164e3e: mov r2,r3
  00164e40: blx 0x00075ff4
  00164e44: pop {r7,pc}
  00164e46: movs r1,#0x0
  00164e48: strd r1,r1,[r0,#0x0]
  00164e4c: str r1,[r0,#0x8]
  00164e4e: pop {r7,pc}
```
