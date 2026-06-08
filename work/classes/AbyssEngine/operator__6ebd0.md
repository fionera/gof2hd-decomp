# AbyssEngine::operator-
elf 0x6ebd0 body 74
Sig: undefined __thiscall operator-(AbyssEngine * this, Quaternion * param_1, Quaternion * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion
   const&) */

void __thiscall AbyssEngine::operator-(AbyssEngine *this,Quaternion *param_1,Quaternion *param_2)

{
  float in_s1;
  float in_s3;
  
  Quaternion::Quaternion
            ((Quaternion *)this,*(float *)(param_1 + 0xc) - *(float *)(param_2 + 0xc),in_s1,
             *(float *)(param_1 + 4) - *(float *)(param_2 + 4),in_s3);
  return;
}

```

## target disasm
```
  0007ebd0: push {r5,r6,r7,lr}
  0007ebd2: add r7,sp,#0x8
  0007ebd4: vldr.32 s0,[r2]
  0007ebd8: vldr.32 s8,[r1]
  0007ebdc: vldr.32 s2,[r2,#0x4]
  0007ebe0: vldr.32 s10,[r1,#0x4]
  0007ebe4: vsub.f32 s0,s8,s0
  0007ebe8: vldr.32 s4,[r2,#0x8]
  0007ebec: vldr.32 s12,[r1,#0x8]
  0007ebf0: vsub.f32 s2,s10,s2
  0007ebf4: vldr.32 s6,[r2,#0xc]
  0007ebf8: vsub.f32 s4,s12,s4
  0007ebfc: vldr.32 s14,[r1,#0xc]
  0007ec00: vmov r1,s0
  0007ec04: vsub.f32 s0,s14,s6
  0007ec08: vmov r2,s2
  0007ec0c: vmov r3,s4
  0007ec10: vstr.32 s0,[sp]
  0007ec14: blx 0x0006f19c
  0007ec18: pop {r2,r3,r7,pc}
```
