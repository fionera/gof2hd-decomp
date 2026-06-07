# TargetFollowCamera::setShipHandling
elf 0x15b574 body 64
Sig: undefined __thiscall setShipHandling(TargetFollowCamera * this, float param_1)

## decompile
```c

/* TargetFollowCamera::setShipHandling(float) */

void __thiscall TargetFollowCamera::setShipHandling(TargetFollowCamera *this,float param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float in_r1;
  float fVar5;
  
  fVar4 = DAT_0016b5c4;
  fVar3 = DAT_0016b5c0;
  fVar2 = DAT_0016b5bc;
  fVar5 = in_r1 * DAT_0016b5b4;
  this[1].field_1C = (int)in_r1;
  fVar1 = fVar5 * DAT_0016b5b8;
  this[1].m_nPosZ = fVar4 + (1.0 - fVar5) * fVar2;
  this[1].field_14 = (int)(fVar3 + fVar1);
  (*(code *)(DAT_001ac5e4 + 0x1ac5e8))(this,0x3f800000);
  return;
}

```

## target disasm
```
  0016b574: vldr.32 s0,[pc,#0x3c]
  0016b578: vmov s2,r1
  0016b57c: vmov.f32 s4,0x3f800000
  0016b580: vldr.32 s6,[pc,#0x38]
  0016b584: vmul.f32 s0,s2,s0
  0016b588: vldr.32 s8,[pc,#0x34]
  0016b58c: vldr.32 s10,[pc,#0x34]
  0016b590: str.w r1,[r0,#0x134]
  0016b594: mov.w r1,#0x3f800000
  0016b598: vsub.f32 s2,s4,s0
  0016b59c: vldr.32 s4,[pc,#0x18]
  0016b5a0: vmla.f32 s8,s0,s4
  0016b5a4: vmla.f32 s10,s2,s6
  0016b5a8: vstr.32 s10,[r0,#0x128]
  0016b5ac: vstr.32 s8,[r0,#0x12c]
  0016b5b0: b.w 0x001ac5d8
```
