# BoundingSphere::getCollisionNormal
elf 0x151e10 body 90
Sig: undefined __thiscall getCollisionNormal(BoundingSphere * this, Vector * param_1)

## decompile
```c

/* BoundingSphere::getCollisionNormal(AbyssEngine::AEMath::Vector const&) */

void __thiscall BoundingSphere::getCollisionNormal(BoundingSphere *this,Vector *param_1)

{
  int *piVar1;
  Vector local_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00161e6c + 0x161e28);
  local_1c = *piVar1;
  AbyssEngine::AEMath::operator+(local_28,param_1 + 8);
  AbyssEngine::AEMath::operator-((Vector *)this,local_28);
  AbyssEngine::AEMath::VectorNormalize((AEMath *)local_28,(Vector *)this);
  AbyssEngine::AEMath::Vector::operator=((Vector *)this,local_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00161e10: push {r4,r5,r6,r7,lr}
  00161e12: add r7,sp,#0xc
  00161e14: push.w r8
  00161e18: sub sp,#0x10
  00161e1a: mov r5,r0
  00161e1c: ldr r0,[0x00161e6c]
  00161e1e: add.w r3,r1,#0x8
  00161e22: mov r4,r2
  00161e24: add r0,pc
  00161e26: add.w r2,r1,#0x14
  00161e2a: mov r8,sp
  00161e2c: mov r1,r3
  00161e2e: ldr r6,[r0,#0x0]
  00161e30: ldr r0,[r6,#0x0]
  00161e32: str r0,[sp,#0xc]
  00161e34: mov r0,r8
  00161e36: blx 0x0006f1cc
  00161e3a: mov r0,r5
  00161e3c: mov r1,r8
  00161e3e: mov r2,r4
  00161e40: blx 0x0006ec38
  00161e44: mov r4,sp
  00161e46: mov r1,r5
  00161e48: mov r0,r4
  00161e4a: blx 0x0006ec80
  00161e4e: mov r0,r5
  00161e50: mov r1,r4
  00161e52: blx 0x0006eb3c
  00161e56: ldr r0,[sp,#0xc]
  00161e58: ldr r1,[r6,#0x0]
  00161e5a: subs r0,r1,r0
  00161e5c: ittt eq
  00161e5e: add.eq sp,#0x10
  00161e60: pop.eq.w r8
  00161e64: pop.eq {r4,r5,r6,r7,pc}
  00161e66: blx 0x0006e824
```
