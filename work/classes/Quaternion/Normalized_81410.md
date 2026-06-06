# AbyssEngine::Quaternion::Normalized
elf 0x81410 body 76
Sig: undefined __stdcall Normalized(void)

## decompile
```c

/* AbyssEngine::Quaternion::Normalized() */

void AbyssEngine::Quaternion::Normalized(void)

{
  undefined8 *in_r0;
  float fVar1;
  float *in_r1;
  undefined8 uVar2;
  
  fVar1 = (float)Length();
  *in_r1 = *in_r1 / fVar1;
  in_r1[1] = in_r1[1] / fVar1;
  in_r1[2] = in_r1[2] / fVar1;
  in_r1[3] = in_r1[3] / fVar1;
  uVar2 = *(undefined8 *)(in_r1 + 2);
  *in_r0 = *(undefined8 *)in_r1;
  in_r0[1] = uVar2;
  return;
}

```
## target disasm
```
  00091410: push {r4,r5,r7,lr}
  00091412: add r7,sp,#0x8
  00091414: mov r5,r0
  00091416: mov r0,r1
  00091418: mov r4,r1
  0009141a: blx 0x000701ec
  0009141e: vldr.32 s0,[r4]
  00091422: vmov s8,r0
  00091426: vldr.32 s2,[r4,#0x4]
  0009142a: vldr.32 s4,[r4,#0x8]
  0009142e: vldr.32 s6,[r4,#0xc]
  00091432: vdiv.f32 s0,s0,s8
  00091436: vdiv.f32 s2,s2,s8
  0009143a: vdiv.f32 s4,s4,s8
  0009143e: vdiv.f32 s6,s6,s8
  00091442: vstr.32 s0,[r4]
  00091446: vstr.32 s2,[r4,#0x4]
  0009144a: vstr.32 s4,[r4,#0x8]
  0009144e: vstr.32 s6,[r4,#0xc]
  00091452: vld1.32 {d16,d17},[r4]
  00091456: vst1.32 {d16,d17},[r5]
  0009145a: pop {r4,r5,r7,pc}
```
