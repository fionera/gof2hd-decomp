# Camera::Camera
elf 0x7b724 body 56
Sig: undefined __thiscall Camera(Camera * this, float param_1, float param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* AbyssEngine::Camera::Camera(float, float, float, float, float) */

Camera * __thiscall
AbyssEngine::Camera::Camera
          (Camera *this,float param_1,float param_2,float param_3,float param_4,float param_5)

{
  Camera *in_r1;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  float extraout_s4;
  float in_stack_00000004;
  
  AEMath::Matrix::Matrix((Matrix *)(this + 0xc));
  CameraSetPerspective(in_stack_00000004,extraout_s1,extraout_s2,extraout_s3,extraout_s4,in_r1);
  return this;
}

```

## target disasm
```
  0008b724: push {r4,r5,r6,r7,lr}
  0008b726: add r7,sp,#0xc
  0008b728: push.w r8
  0008b72c: sub sp,#0x8
  0008b72e: mov r4,r0
  0008b730: adds r0,#0xc
  0008b732: mov r8,r3
  0008b734: mov r5,r2
  0008b736: mov r6,r1
  0008b738: blx 0x0006f118
  0008b73c: ldr r3,[r7,#0x8]
  0008b73e: mov r0,r6
  0008b740: vldr.32 s0,[r7,#0xc]
  0008b744: mov r1,r5
  0008b746: mov r2,r8
  0008b748: str r4,[sp,#0x4]
  0008b74a: vstr.32 s0,[sp]
  0008b74e: blx 0x0006fd84
  0008b752: mov r0,r4
  0008b754: add sp,#0x8
  0008b756: pop.w r8
  0008b75a: pop {r4,r5,r6,r7,pc}
```
