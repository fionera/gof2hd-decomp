# TargetFollowCamera::translate
elf 0x15a9e8 body 94
Sig: undefined __thiscall translate(TargetFollowCamera * this, float param_1, float param_2, float param_3)

## decompile
```c

/* TargetFollowCamera::translate(float, float, float) */

void __thiscall
TargetFollowCamera::translate(TargetFollowCamera *this,float param_1,float param_2,float param_3)

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
  (*(code *)(DAT_001ac5d4 + 0x1ac5d8))(this,1000);
  return;
}

```

## target disasm
```
  0016a9e8: vmov s14,r1
  0016a9ec: vldr.32 s2,[r0,#0x8]
  0016a9f0: vldr.32 s8,[r0,#0x14]
  0016a9f4: vmov s0,r2
  0016a9f8: vldr.32 s4,[r0,#0xc]
  0016a9fc: vmov s1,r3
  0016aa00: vldr.32 s6,[r0,#0x10]
  0016aa04: vadd.f32 s2,s2,s14
  0016aa08: vldr.32 s10,[r0,#0x18]
  0016aa0c: vadd.f32 s4,s4,s0
  0016aa10: vldr.32 s12,[r0,#0x1c]
  0016aa14: vadd.f32 s6,s6,s1
  0016aa18: vadd.f32 s0,s10,s0
  0016aa1c: mov.w r1,#0x3e8
  0016aa20: vadd.f32 s8,s8,s14
  0016aa24: vadd.f32 s10,s12,s1
  0016aa28: vstr.32 s2,[r0,#0x8]
  0016aa2c: vstr.32 s4,[r0,#0xc]
  0016aa30: vstr.32 s6,[r0,#0x10]
  0016aa34: vstr.32 s8,[r0,#0x14]
  0016aa38: vstr.32 s0,[r0,#0x18]
  0016aa3c: vstr.32 s10,[r0,#0x1c]
  0016aa40: b.w 0x001ac5c8
  001ac5c8: bx pc
```
