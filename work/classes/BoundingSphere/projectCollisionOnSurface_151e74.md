# BoundingSphere::projectCollisionOnSurface
elf 0x151e74 body 166
Sig: undefined __thiscall projectCollisionOnSurface(BoundingSphere * this, Vector * param_1)

## decompile
```c

/* BoundingSphere::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) */

void __thiscall BoundingSphere::projectCollisionOnSurface(BoundingSphere *this,Vector *param_1)

{
  int *piVar1;
  float fVar2;
  undefined4 in_s14;
  float fVar3;
  Vector aVStack_60 [12];
  Vector aVStack_54 [12];
  Vector aVStack_48 [8];
  undefined4 uStack_40;
  int local_3c;
  
  piVar1 = *(int **)(DAT_00161f1c + 0x161e90);
  _uStack_40 = CONCAT44(*piVar1,in_s14);
  AbyssEngine::AEMath::operator+(aVStack_54,param_1 + 8);
  AbyssEngine::AEMath::operator-(aVStack_48,aVStack_54);
  fVar2 = (float)AbyssEngine::AEMath::VectorLength(aVStack_48);
  fVar3 = *(float *)(param_1 + 0x38);
  if (fVar3 <= fVar2) {
    *(undefined4 *)this = 0;
    *(undefined4 *)(this + 4) = 0;
    *(undefined4 *)(this + 8) = 0;
  }
  else {
    AbyssEngine::AEMath::operator+(aVStack_54,param_1 + 8);
    AbyssEngine::AEMath::operator*(fVar3 / fVar2,aVStack_60);
    AbyssEngine::AEMath::operator-((Vector *)this,aVStack_54);
  }
  if (*piVar1 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00161e74: push {r4,r5,r6,r7,lr}
  00161e76: add r7,sp,#0xc
  00161e78: push {r7,r8,r9,r10,r11}
  00161e7c: vpush {d2,d3,d4,d5,d6,d7,d8,d9}
  00161e80: mov r8,r0
  00161e82: ldr r0,[0x00161f1c]
  00161e84: add.w r9,r1,#0x8
  00161e88: add.w r10,r1,#0x14
  00161e8c: add r0,pc
  00161e8e: add.w r11,sp,#0x14
  00161e92: mov r4,r2
  00161e94: mov r5,r1
  00161e96: ldr r0,[r0,#0x0]
  00161e98: mov r1,r9
  00161e9a: str r0,[sp,#0x4]
  00161e9c: mov r2,r10
  00161e9e: ldr r0,[r0,#0x0]
  00161ea0: str r0,[sp,#0x2c]
  00161ea2: mov r0,r11
  00161ea4: blx 0x0006f1cc
  00161ea8: add r6,sp,#0x20
  00161eaa: mov r1,r11
  00161eac: mov r2,r4
  00161eae: mov r0,r6
  00161eb0: blx 0x0006ec38
  00161eb4: mov r0,r6
  00161eb6: blx 0x0006ec44
  00161eba: vldr.32 s16,[r5,#0x38]
  00161ebe: vmov s18,r0
  00161ec2: vcmpe.f32 s18,s16
  00161ec6: vmrs apsr,fpscr
  00161eca: bge 0x00161ef6
  00161ecc: add r4,sp,#0x14
  00161ece: mov r1,r9
  00161ed0: mov r2,r10
  00161ed2: mov r0,r4
  00161ed4: blx 0x0006f1cc
  00161ed8: vdiv.f32 s0,s16,s18
  00161edc: add r5,sp,#0x8
  00161ede: add r2,sp,#0x20
  00161ee0: mov r0,r5
  00161ee2: vmov r1,s0
  00161ee6: blx 0x00072658
  00161eea: mov r0,r8
  00161eec: mov r1,r4
  00161eee: mov r2,r5
  00161ef0: blx 0x0006ec38
  00161ef4: b 0x00161f00
  00161ef6: movs r0,#0x0
  00161ef8: strd r0,r0,[r8,#0x0]
  00161efc: str.w r0,[r8,#0x8]
  00161f00: ldr r0,[sp,#0x2c]
  00161f02: ldr r1,[sp,#0x4]
  00161f04: ldr r1,[r1,#0x0]
  00161f06: subs r0,r1,r0
  00161f08: itttt eq
  00161f0a: vpop.eq {d2,d3,d4,d5,d6,d7,d8,d9}
  00161f0e: add.eq sp,#0x4
  00161f10: pop.eq.w {r8,r9,r10,r11}
  00161f14: pop.eq {r4,r5,r6,r7,pc}
  00161f16: blx 0x0006e824
```
