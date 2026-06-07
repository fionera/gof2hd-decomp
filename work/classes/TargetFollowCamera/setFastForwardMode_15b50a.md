# TargetFollowCamera::setFastForwardMode
elf 0x15b50a body 104
Sig: undefined __stdcall setFastForwardMode(bool param_1)

## decompile
```c

/* TargetFollowCamera::setFastForwardMode(bool) */

undefined8 TargetFollowCamera::setFastForwardMode(bool param_1)

{
  char cVar1;
  TargetFollowCamera *this;
  int *in_r1;
  float in_s0;
  float fVar2;
  TargetFollowCamera *pTVar3;
  int *piVar4;
  
  this = (TargetFollowCamera *)(uint)param_1;
  cVar1 = *(char *)((int)&this->field_4C + 1);
  if (in_r1 == (int *)0x0) {
    if (cVar1 == '\0') goto LAB_0016b570;
  }
  else if (cVar1 != '\0') goto LAB_0016b570;
  fVar2 = (float)setShipHandling(this,in_s0);
  fVar2 = (float)aproximateCooefficientsForAproximationOfDampingFunktion
                           ((TargetFollowCamera *)&this->field_80,fVar2,(double *)this[1].m_nPosZ,
                            (double *)&this->field_60,(double *)&this->field_68,
                            (double *)&this->field_70,(double *)&this->field_78);
  piVar4 = &this->field_A0;
  pTVar3 = (TargetFollowCamera *)&this->field_98;
  aproximateCooefficientsForAproximationOfDampingFunktion
            ((TargetFollowCamera *)&this->field_A8,fVar2,(double *)this[1].field_14,
             (double *)&this->field_88,(double *)&this->field_90,(double *)pTVar3,(double *)piVar4);
  *(char *)((int)&this->field_4C + 1) = (char)in_r1;
  this = pTVar3;
  in_r1 = piVar4;
LAB_0016b570:
  return CONCAT44(in_r1,this);
}

```

## target disasm
```
  0016b50a: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0016b50c: add r7,sp,#0x18
  0016b50e: mov r5,r0
  0016b510: ldrb.w r0,[r0,#0x4d]
  0016b514: mov r4,r1
  0016b516: cbz r1,0x0016b51c
  0016b518: cbnz r0,0x0016b570
  0016b51a: b 0x0016b51e
  0016b51c: cbz r0,0x0016b570
  0016b51e: ldr.w r1,[r5,#0x134]
  0016b522: mov r0,r5
  0016b524: blx 0x00072a0c
  0016b528: add.w r2,r5,#0x78
  0016b52c: ldr.w r1,[r5,#0x128]
  0016b530: add.w r3,r5,#0x70
  0016b534: add.w r0,r5,#0x80
  0016b538: strd r3,r2,[sp,#0x0]
  0016b53c: add.w r2,r5,#0x60
  0016b540: add.w r3,r5,#0x68
  0016b544: str r0,[sp,#0x8]
  0016b546: blx 0x00077788
  0016b54a: add.w r2,r5,#0xa0
  0016b54e: ldr.w r1,[r5,#0x12c]
  0016b552: add.w r3,r5,#0x98
  0016b556: add.w r0,r5,#0xa8
  0016b55a: strd r3,r2,[sp,#0x0]
  0016b55e: add.w r2,r5,#0x88
  0016b562: add.w r3,r5,#0x90
  0016b566: str r0,[sp,#0x8]
  0016b568: blx 0x00077788
  0016b56c: strb.w r4,[r5,#0x4d]
  0016b570: pop {r0,r1,r2,r3,r4,r5,r7,pc}
```
