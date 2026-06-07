# TargetFollowCamera::calculateCoefficents
elf 0x15b4a4 body 102
Sig: undefined __thiscall calculateCoefficents(TargetFollowCamera * this, float param_1)

## decompile
```c

/* TargetFollowCamera::calculateCoefficents(float) */

void __thiscall TargetFollowCamera::calculateCoefficents(TargetFollowCamera *this,float param_1)

{
  float in_r1;
  float fVar1;
  
  fVar1 = this[1].m_nPosZ * in_r1;
  aproximateCooefficientsForAproximationOfDampingFunktion
            ((TargetFollowCamera *)&this->field_80,fVar1,(double *)fVar1,(double *)&this->field_60,
             (double *)&this->field_68,(double *)&this->field_70,(double *)&this->field_78);
  fVar1 = (float)this[1].field_14 * in_r1;
  aproximateCooefficientsForAproximationOfDampingFunktion
            ((TargetFollowCamera *)&this->field_A8,fVar1,(double *)fVar1,(double *)&this->field_88,
             (double *)&this->field_90,(double *)&this->field_98,(double *)&this->field_A0);
  return;
}

```

## target disasm
```
  0016b4a4: push {r4,r6,r7,lr}
  0016b4a6: add r7,sp,#0x8
  0016b4a8: vpush {d6,d7,d8}
  0016b4ac: vmov s16,r1
  0016b4b0: vldr.32 s0,[r0,#0x128]
  0016b4b4: mov r4,r0
  0016b4b6: add.w r2,r4,#0x78
  0016b4ba: vmul.f32 s0,s0,s16
  0016b4be: add.w r3,r4,#0x70
  0016b4c2: strd r3,r2,[sp,#0x0]
  0016b4c6: adds r0,#0x80
  0016b4c8: add.w r2,r4,#0x60
  0016b4cc: add.w r3,r4,#0x68
  0016b4d0: str r0,[sp,#0x8]
  0016b4d2: vmov r1,s0
  0016b4d6: blx 0x00077788
  0016b4da: vldr.32 s0,[r4,#0x12c]
  0016b4de: add.w r2,r4,#0xa0
  0016b4e2: add.w r3,r4,#0x98
  0016b4e6: add.w r0,r4,#0xa8
  0016b4ea: vmul.f32 s0,s0,s16
  0016b4ee: strd r3,r2,[sp,#0x0]
  0016b4f2: add.w r2,r4,#0x88
  0016b4f6: add.w r3,r4,#0x90
  0016b4fa: str r0,[sp,#0x8]
  0016b4fc: vmov r1,s0
  0016b500: blx 0x00077788
  0016b504: vpop {d6,d7,d8}
  0016b508: pop {r4,r6,r7,pc}
```
