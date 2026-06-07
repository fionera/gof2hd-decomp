# PaintCanvas::CameraSetPerspective
elf 0x7ba1e body 120
Sig: undefined __thiscall CameraSetPerspective(PaintCanvas * this, uint param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraSetPerspective(unsigned int, float, float) */

float __thiscall
AbyssEngine::PaintCanvas::CameraSetPerspective
          (PaintCanvas *this,uint param_1,float param_2,float param_3)

{
  undefined4 uVar1;
  Camera *in_r2;
  uint in_fpscr;
  float fVar2;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  
  if (param_1 < *(uint *)(this + 0x164)) {
    uVar1 = GetWidth();
    VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    uVar1 = GetHeight();
    fVar2 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    param_2 = (float)AbyssEngine::CameraSetPerspective
                               (fVar2,extraout_s1,extraout_s2,extraout_s3,in_r2);
    if (*(uint *)(this + 0x170) == param_1) {
      fVar2 = (float)(*(code *)(DAT_001ab2e4 + 0x1ab2e8))(this,param_1);
      return fVar2;
    }
  }
  return param_2;
}

```

## target disasm
```
  0008ba1e: push {r4,r5,r6,r7,lr}
  0008ba20: add r7,sp,#0xc
  0008ba22: push.w r8
  0008ba26: vpush {d7,d8}
  0008ba2a: mov r5,r0
  0008ba2c: ldr.w r0,[r0,#0x164]
  0008ba30: mov r8,r3
  0008ba32: mov r6,r2
  0008ba34: mov r4,r1
  0008ba36: cmp r0,r1
  0008ba38: bls 0x0008ba8c
  0008ba3a: mov r0,r5
  0008ba3c: blx 0x0006fafc
  0008ba40: vmov s0,r0
  0008ba44: mov r0,r5
  0008ba46: vcvt.f32.s32 s16,s0
  0008ba4a: blx 0x0006fb08
  0008ba4e: vmov s0,r0
  0008ba52: mov r1,r8
  0008ba54: vmov r2,s16
  0008ba58: vcvt.f32.s32 s0,s0
  0008ba5c: ldr.w r0,[r5,#0x168]
  0008ba60: ldr.w r0,[r0,r4,lsl #0x2]
  0008ba64: str r0,[sp,#0x0]
  0008ba66: mov r0,r6
  0008ba68: vmov r3,s0
  0008ba6c: blx 0x0006fda8
  0008ba70: ldr.w r0,[r5,#0x170]
  0008ba74: cmp r0,r4
  0008ba76: bne 0x0008ba8c
  0008ba78: mov r0,r5
  0008ba7a: mov r1,r4
  0008ba7c: vpop {d7,d8}
  0008ba80: pop.w r8
  0008ba84: pop.w {r4,r5,r6,r7,lr}
  0008ba88: b.w 0x001ab2d8
  0008ba8c: vpop {d7,d8}
  0008ba90: pop.w r8
  0008ba94: pop {r4,r5,r6,r7,pc}
```
