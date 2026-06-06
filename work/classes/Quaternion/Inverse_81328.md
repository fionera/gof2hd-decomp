# AbyssEngine::Quaternion::Inverse
elf 0x81328 body 74
Sig: undefined __stdcall Inverse(void)

## decompile
```c

/* AbyssEngine::Quaternion::Inverse() const */

void AbyssEngine::Quaternion::Inverse(void)

{
  float *in_r0;
  float *in_r1;
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  
  fVar2 = in_r1[1];
  fVar1 = *in_r1;
  fVar3 = in_r1[2];
  fVar4 = in_r1[3];
  fVar5 = 1.0 / (fVar2 * fVar2 + fVar1 * fVar1 + fVar3 * fVar3 + fVar4 * fVar4);
  *in_r0 = -(fVar5 * fVar1);
  in_r0[1] = -(fVar5 * fVar2);
  in_r0[2] = -(fVar5 * fVar3);
  in_r0[3] = fVar4 * fVar5;
  return;
}

```
## target disasm
```
  00091328: vldr.32 s2,[r1,#0x4]
  0009132c: vmov.f32 s10,0x3f800000
  00091330: vldr.32 s0,[r1]
  00091334: vmul.f32 s8,s2,s2
  00091338: vldr.32 s4,[r1,#0x8]
  0009133c: vldr.32 s6,[r1,#0xc]
  00091340: vmla.f32 s8,s0,s0
  00091344: vmla.f32 s8,s4,s4
  00091348: vmla.f32 s8,s6,s6
  0009134c: vdiv.f32 s8,s10,s8
  00091350: vnmul.f32 s0,s8,s0
  00091354: vnmul.f32 s2,s8,s2
  00091358: vnmul.f32 s4,s8,s4
  0009135c: vmul.f32 s6,s6,s8
  00091360: vstr.32 s0,[r0]
  00091364: vstr.32 s2,[r0,#0x4]
  00091368: vstr.32 s4,[r0,#0x8]
  0009136c: vstr.32 s6,[r0,#0xc]
  00091370: bx lr
```
