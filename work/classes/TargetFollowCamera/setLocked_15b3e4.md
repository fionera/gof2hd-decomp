# TargetFollowCamera::setLocked
elf 0x15b3e4 body 122
Sig: undefined __thiscall setLocked(TargetFollowCamera * this, bool param_1)

## decompile
```c

/* TargetFollowCamera::setLocked(bool) */

void __thiscall TargetFollowCamera::setLocked(TargetFollowCamera *this,bool param_1)

{
  undefined4 uVar1;
  int *piVar2;
  Matrix aMStack_64 [12];
  Vector aVStack_58 [60];
  int local_1c;
  
  piVar2 = *(int **)(DAT_0016b460 + 0x16b3f8);
  local_1c = *piVar2;
  *(bool *)&this->field_44 = param_1;
  if (param_1) {
    uVar1 = AEGeometry::getMatrix();
    __aeabi_memcpy(aVStack_58,uVar1,0x3c);
    AbyssEngine::AEMath::MatrixGetUp(aMStack_64);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&this->field_20,(Vector *)aMStack_64);
    AbyssEngine::AEMath::MatrixTransformVector(aMStack_64,aVStack_58);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&this->m_nPosX,(Vector *)aMStack_64);
    update((int)this);
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016b3e4: push {r4,r5,r6,r7,lr}
  0016b3e6: add r7,sp,#0xc
  0016b3e8: push.w r8
  0016b3ec: sub sp,#0x50
  0016b3ee: mov r4,r0
  0016b3f0: ldr r0,[0x0016b460]
  0016b3f2: cmp r1,#0x0
  0016b3f4: add r0,pc
  0016b3f6: ldr r5,[r0,#0x0]
  0016b3f8: ldr r0,[r5,#0x0]
  0016b3fa: str r0,[sp,#0x4c]
  0016b3fc: strb.w r1,[r4,#0x44]
  0016b400: beq 0x0016b44a
  0016b402: ldr r0,[r4,#0x4]
  0016b404: blx 0x000721cc
  0016b408: add.w r8,sp,#0x10
  0016b40c: mov r1,r0
  0016b40e: movs r2,#0x3c
  0016b410: mov r0,r8
  0016b412: blx 0x0006f1e4
  0016b416: add r6,sp,#0x4
  0016b418: mov r1,r8
  0016b41a: mov r0,r6
  0016b41c: blx 0x0006f4d8
  0016b420: add.w r0,r4,#0x20
  0016b424: mov r1,r6
  0016b426: blx 0x0006eb3c
  0016b42a: add r6,sp,#0x4
  0016b42c: add.w r2,r4,#0x38
  0016b430: mov r1,r8
  0016b432: mov r0,r6
  0016b434: blx 0x0006f688
  0016b438: add.w r0,r4,#0x8
  0016b43c: mov r1,r6
  0016b43e: blx 0x0006eb3c
  0016b442: mov r0,r4
  0016b444: movs r1,#0x32
  0016b446: blx 0x000721a8
  0016b44a: ldr r0,[sp,#0x4c]
  0016b44c: ldr r1,[r5,#0x0]
  0016b44e: subs r0,r1,r0
  0016b450: ittt eq
  0016b452: add.eq sp,#0x50
  0016b454: pop.eq.w r8
  0016b458: pop.eq {r4,r5,r6,r7,pc}
  0016b45a: blx 0x0006e824
```
