# PlayerFixedObject::getProjectionVector
elf 0x154dd6 body 64
Sig: undefined __thiscall getProjectionVector(PlayerFixedObject * this, Vector * param_1)

## decompile
```c

/* PlayerFixedObject::getProjectionVector(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerFixedObject::getProjectionVector(PlayerFixedObject *this,Vector *param_1)

{
  if (((*(int *)(param_1 + 300) == 0) || (*(int *)(param_1 + 0x88) != 4)) &&
     (*(int *)(param_1 + 0x128) == 0)) {
    *(undefined4 *)this = 0;
    *(undefined4 *)(this + 4) = 0;
    *(undefined4 *)(this + 8) = 0;
    return;
  }
  BoundingVolume::getProjectionVector((Vector *)this);
  return;
}

```

## target disasm
```
  00164dd6: push {r7,lr}
  00164dd8: mov r7,sp
  00164dda: ldr.w r12,[r1,#0x12c]
  00164dde: cmp.w r12,#0x0
  00164de2: beq 0x00164df6
  00164de4: ldr.w r3,[r1,#0x88]
  00164de8: cmp r3,#0x4
  00164dea: bne 0x00164df6
  00164dec: ldr.w r1,[r1,#0x16c]
  00164df0: ldr.w r3,[r12,#0x4]
  00164df4: b 0x00164e02
  00164df6: ldr.w r3,[r1,#0x128]
  00164dfa: cbz r3,0x00164e0c
  00164dfc: ldr.w r1,[r1,#0x16c]
  00164e00: ldr r3,[r3,#0x4]
  00164e02: ldr.w r1,[r3,r1,lsl #0x2]
  00164e06: blx 0x00075fe8
  00164e0a: pop {r7,pc}
  00164e0c: movs r1,#0x0
  00164e0e: strd r1,r1,[r0,#0x0]
  00164e12: str r1,[r0,#0x8]
  00164e14: pop {r7,pc}
```
