# AbyssEngine::operator*
elf 0x6ee56 body 62
Sig: undefined __thiscall operator*(AbyssEngine * this, Quaternion * param_1, float param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::Quaternion const&, float) */

void __thiscall AbyssEngine::operator*(AbyssEngine *this,Quaternion *param_1,float param_2)

{
  float in_r2;
  float in_s1;
  float in_s3;
  
  Quaternion::Quaternion
            ((Quaternion *)this,*(float *)(param_1 + 0xc) * in_r2,in_s1,
             *(float *)(param_1 + 4) * in_r2,in_s3);
  return;
}

```

## target disasm
```
  0007ee56: push {r5,r6,r7,lr}
  0007ee58: add r7,sp,#0x8
  0007ee5a: vldr.32 s0,[r1]
  0007ee5e: vmov s8,r2
  0007ee62: vldr.32 s2,[r1,#0x4]
  0007ee66: vldr.32 s4,[r1,#0x8]
  0007ee6a: vmul.f32 s0,s0,s8
  0007ee6e: vmul.f32 s2,s2,s8
  0007ee72: vldr.32 s6,[r1,#0xc]
  0007ee76: vmul.f32 s4,s4,s8
  0007ee7a: vmov r1,s0
  0007ee7e: vmov r2,s2
  0007ee82: vmov r3,s4
  0007ee86: vmul.f32 s0,s6,s8
  0007ee8a: vstr.32 s0,[sp]
  0007ee8e: blx 0x0006f19c
  0007ee92: pop {r2,r3,r7,pc}
```
