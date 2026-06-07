# PaintCanvas::CameraSetPerspective
elf 0x7b99e body 128
Sig: undefined __stdcall CameraSetPerspective(uint param_1, float param_2, float param_3, float param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraSetPerspective(unsigned int, float, float, float) */

float AbyssEngine::PaintCanvas::CameraSetPerspective
                (uint param_1,float param_2,float param_3,float param_4)

{
  undefined4 uVar1;
  uint in_r1;
  Camera *in_r2;
  uint in_fpscr;
  float fVar2;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  float extraout_s4;
  
  if (in_r1 < *(uint *)(param_1 + 0x164)) {
    uVar1 = GetWidth();
    VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    uVar1 = GetHeight();
    fVar2 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    param_2 = (float)AbyssEngine::CameraSetPerspective
                               (fVar2,extraout_s1,extraout_s2,extraout_s3,extraout_s4,in_r2);
    if (*(uint *)(param_1 + 0x170) == in_r1) {
      fVar2 = (float)(*(code *)(DAT_001ab2e4 + 0x1ab2e8))(param_1);
      return fVar2;
    }
  }
  return param_2;
}

```

## target disasm
```
  0008b99e: push {r4,r5,r6,r7,lr}
  0008b9a0: add r7,sp,#0xc
  0008b9a2: push.w r8
  0008b9a6: vpush {d7,d8,d9}
  0008b9aa: mov r5,r0
  0008b9ac: ldr.w r0,[r0,#0x164]
  0008b9b0: mov r8,r3
  0008b9b2: mov r6,r2
  0008b9b4: mov r4,r1
  0008b9b6: cmp r0,r1
  0008b9b8: bls 0x0008ba14
  0008b9ba: mov r0,r5
  0008b9bc: vldr.32 s16,[r7,#0x8]
  0008b9c0: blx 0x0006fafc
  0008b9c4: vmov s0,r0
  0008b9c8: mov r0,r5
  0008b9ca: vcvt.f32.s32 s18,s0
  0008b9ce: blx 0x0006fb08
  0008b9d2: vmov r2,s16
  0008b9d6: mov r1,r8
  0008b9d8: vmov r3,s18
  0008b9dc: vmov s0,r0
  0008b9e0: vcvt.f32.s32 s0,s0
  0008b9e4: ldr.w r0,[r5,#0x168]
  0008b9e8: ldr.w r0,[r0,r4,lsl #0x2]
  0008b9ec: str r0,[sp,#0x4]
  0008b9ee: mov r0,r6
  0008b9f0: vstr.32 s0,[sp]
  0008b9f4: blx 0x0006fd84
  0008b9f8: ldr.w r0,[r5,#0x170]
  0008b9fc: cmp r0,r4
  0008b9fe: bne 0x0008ba14
  0008ba00: mov r0,r5
  0008ba02: mov r1,r4
  0008ba04: vpop {d7,d8,d9}
  0008ba08: pop.w r8
  0008ba0c: pop.w {r4,r5,r6,r7,lr}
  0008ba10: b.w 0x001ab2d8
  0008ba14: vpop {d7,d8,d9}
  0008ba18: pop.w r8
  0008ba1c: pop {r4,r5,r6,r7,pc}
```
