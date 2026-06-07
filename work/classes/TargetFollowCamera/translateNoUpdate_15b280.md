# TargetFollowCamera::translateNoUpdate
elf 0x15b280 body 86
Sig: undefined __thiscall translateNoUpdate(TargetFollowCamera * this, float param_1, float param_2, float param_3)

## decompile
```c

/* TargetFollowCamera::translateNoUpdate(float, float, float) */

void __thiscall
TargetFollowCamera::translateNoUpdate
          (TargetFollowCamera *this,float param_1,float param_2,float param_3)

{
  float in_r1;
  float in_r2;
  float in_r3;
  
  this->m_nPosX = this->m_nPosX + in_r1;
  this->m_nPosY = this->m_nPosY + in_r2;
  this->m_nPosZ = this->m_nPosZ + in_r3;
  this->field_14 = (int)((float)this->field_14 + in_r1);
  this->field_18 = (int)((float)this->field_18 + in_r2);
  this->field_1C = (int)((float)this->field_1C + in_r3);
  return;
}

```

## target disasm
```
  0016b280: vmov s0,r2
  0016b284: vldr.32 s4,[r0,#0xc]
  0016b288: vldr.32 s2,[r0,#0x8]
  0016b28c: vmov s14,r1
  0016b290: vldr.32 s8,[r0,#0x14]
  0016b294: vmov s1,r3
  0016b298: vldr.32 s6,[r0,#0x10]
  0016b29c: vadd.f32 s2,s2,s14
  0016b2a0: vldr.32 s10,[r0,#0x18]
  0016b2a4: vadd.f32 s4,s4,s0
  0016b2a8: vldr.32 s12,[r0,#0x1c]
  0016b2ac: vadd.f32 s6,s6,s1
  0016b2b0: vadd.f32 s0,s10,s0
  0016b2b4: vadd.f32 s8,s8,s14
  0016b2b8: vadd.f32 s10,s12,s1
  0016b2bc: vstr.32 s2,[r0,#0x8]
  0016b2c0: vstr.32 s4,[r0,#0xc]
  0016b2c4: vstr.32 s6,[r0,#0x10]
  0016b2c8: vstr.32 s8,[r0,#0x14]
  0016b2cc: vstr.32 s0,[r0,#0x18]
  0016b2d0: vstr.32 s10,[r0,#0x1c]
  0016b2d4: bx lr
```
