# AbyssEngine::operator+
elf 0x6ee0c body 74
Sig: undefined __thiscall operator+(AbyssEngine * this, Quaternion * param_1, Quaternion * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion
   const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,Quaternion *param_1,Quaternion *param_2)

{
  float in_s1;
  float in_s3;
  
  Quaternion::Quaternion
            ((Quaternion *)this,*(float *)(param_1 + 0xc) + *(float *)(param_2 + 0xc),in_s1,
             *(float *)(param_1 + 4) + *(float *)(param_2 + 4),in_s3);
  return;
}

```

## target disasm
```
  0007ee0c: push {r5,r6,r7,lr}
  0007ee0e: add r7,sp,#0x8
  0007ee10: vldr.32 s0,[r2]
  0007ee14: vldr.32 s8,[r1]
  0007ee18: vldr.32 s2,[r2,#0x4]
  0007ee1c: vldr.32 s10,[r1,#0x4]
  0007ee20: vadd.f32 s0,s8,s0
  0007ee24: vldr.32 s4,[r2,#0x8]
  0007ee28: vldr.32 s12,[r1,#0x8]
  0007ee2c: vadd.f32 s2,s10,s2
  0007ee30: vldr.32 s6,[r2,#0xc]
  0007ee34: vadd.f32 s4,s12,s4
  0007ee38: vldr.32 s14,[r1,#0xc]
  0007ee3c: vmov r1,s0
  0007ee40: vadd.f32 s0,s14,s6
  0007ee44: vmov r2,s2
  0007ee48: vmov r3,s4
  0007ee4c: vstr.32 s0,[sp]
  0007ee50: blx 0x0006f19c
  0007ee54: pop {r2,r3,r7,pc}
```
