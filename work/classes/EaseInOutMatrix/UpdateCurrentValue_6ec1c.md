# EaseInOutMatrix::UpdateCurrentValue
elf 0x6ec1c body 204
Sig: undefined __thiscall UpdateCurrentValue(EaseInOutMatrix * this)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::UpdateCurrentValue() */

void __thiscall AbyssEngine::EaseInOutMatrix::UpdateCurrentValue(EaseInOutMatrix *this)

{
  float fVar1;
  int *piVar2;
  AbyssEngine aAStack_7c [60];
  Vector aVStack_40 [12];
  AbyssEngine aAStack_34 [16];
  int local_24;
  
  piVar2 = *(int **)(DAT_0007ed04 + 0x7ec32);
  local_24 = *piVar2;
  if (*(float *)(this + 0x74) == 1.25) {
    AEMath::Matrix::operator=((Matrix *)(this + 0x78),(Matrix *)(this + 0xb4));
  }
  else {
    fVar1 = (float)AEMath::Sinf(*(float *)(this + 0x74) * DAT_0007ed00);
    operator*(aAStack_34,(Quaternion *)(this + 0x58),fVar1 * 0.5 + 0.5);
    operator+(aAStack_7c,(Quaternion *)aAStack_34,(Quaternion *)(this + 0x3c));
    Quaternion::Convert((Quaternion *)aAStack_7c,(Matrix *)(this + 0x78));
    Quaternion::~Quaternion((Quaternion *)aAStack_7c);
    fVar1 = (float)Quaternion::~Quaternion((Quaternion *)aAStack_34);
    AEMath::operator*(aVStack_40,fVar1);
    AEMath::operator+((Vector *)aAStack_34,aVStack_40);
    AEMath::MatrixSetTranslation((Matrix *)aAStack_7c,(Vector *)(this + 0x78));
  }
  if (*piVar2 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0007ec1c: push {r4,r5,r6,r7,lr}
  0007ec1e: add r7,sp,#0xc
  0007ec20: push {r8,r9,r11}
  0007ec24: sub sp,#0x60
  0007ec26: mov r4,r0
  0007ec28: ldr r0,[0x0007ed04]
  0007ec2a: vmov.f32 s2,0x3fa00000
  0007ec2e: add r0,pc
  0007ec30: ldr r5,[r0,#0x0]
  0007ec32: ldr r0,[r5,#0x0]
  0007ec34: str r0,[sp,#0x5c]
  0007ec36: vldr.32 s0,[r4,#0x74]
  0007ec3a: vcmpe.f32 s0,s2
  0007ec3e: vmrs apsr,fpscr
  0007ec42: bne 0x0007ec52
  0007ec44: add.w r0,r4,#0x78
  0007ec48: add.w r1,r4,#0xb4
  0007ec4c: blx 0x0006f148
  0007ec50: b 0x0007ecd4
  0007ec52: vldr.32 s2,[pc,#0xac]
  0007ec56: vmul.f32 s0,s0,s2
  0007ec5a: vmov r0,s0
  0007ec5e: blx 0x0006f1a8
  0007ec62: vmov s0,r0
  0007ec66: add r6,sp,#0x4c
  0007ec68: vmov.f64 d17,0x3fe0000000000000
  0007ec6c: add.w r1,r4,#0x58
  0007ec70: mov r0,r6
  0007ec72: vcvt.f64.f32 d16,s0
  0007ec76: vmla.f64 d17,d16,d17
  0007ec7a: vcvt.f32.f64 s0,d17
  0007ec7e: vmov r8,s0
  0007ec82: mov r2,r8
  0007ec84: blx 0x0006f1b4
  0007ec88: add.w r2,r4,#0x3c
  0007ec8c: add r0,sp,#0x4
  0007ec8e: mov r1,r6
  0007ec90: blx 0x0006f1c0
  0007ec94: add.w r6,r4,#0x78
  0007ec98: add r0,sp,#0x4
  0007ec9a: mov r1,r6
  0007ec9c: blx 0x0006f160
  0007eca0: add r0,sp,#0x4
  0007eca2: blx 0x0006f13c
  0007eca6: add r0,sp,#0x4c
  0007eca8: blx 0x0006f13c
  0007ecac: add.w r9,sp,#0x40
  0007ecb0: add.w r1,r4,#0x68
  0007ecb4: mov r2,r8
  0007ecb6: mov r0,r9
  0007ecb8: blx 0x0006ec74
  0007ecbc: add.w r2,r4,#0x4c
  0007ecc0: add r4,sp,#0x4c
  0007ecc2: mov r1,r9
  0007ecc4: mov r0,r4
  0007ecc6: blx 0x0006f1cc
  0007ecca: add r0,sp,#0x4
  0007eccc: mov r1,r6
  0007ecce: mov r2,r4
  0007ecd0: blx 0x0006f1d8
  0007ecd4: ldr r0,[sp,#0x5c]
  0007ecd6: ldr r1,[r5,#0x0]
  0007ecd8: subs r0,r1,r0
  0007ecda: ittt eq
  0007ecdc: add.eq sp,#0x60
  0007ecde: pop.eq.w {r8,r9,r11}
  0007ece2: pop.eq {r4,r5,r6,r7,pc}
  0007ece4: blx 0x0006e824
```
