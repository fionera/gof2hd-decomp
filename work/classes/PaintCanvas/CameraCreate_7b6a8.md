# PaintCanvas::CameraCreate
elf 0x7b6a8 body 104
Sig: undefined __thiscall CameraCreate(PaintCanvas * this, uint * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraCreate(unsigned int&) */

void __thiscall AbyssEngine::PaintCanvas::CameraCreate(PaintCanvas *this,uint *param_1)

{
  Camera *this_00;
  undefined4 uVar1;
  undefined4 uVar2;
  uint in_fpscr;
  float fVar3;
  float extraout_s1;
  float fVar4;
  float extraout_s3;
  float extraout_s4;
  
  this_00 = operator_new(0x5c);
  uVar1 = GetWidth();
  uVar2 = GetHeight();
  fVar3 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  Camera::Camera(this_00,fVar3,extraout_s1,fVar4,extraout_s3,extraout_s4);
  ArrayAdd<AbyssEngine::Camera*>(this_00,(Array *)(this + 0x164));
  *param_1 = *(int *)(this + 0x164) - 1;
  return;
}

```

## target disasm
```
  0008b6a8: push {r4,r5,r6,r7,lr}
  0008b6aa: add r7,sp,#0xc
  0008b6ac: push.w r8
  0008b6b0: sub sp,#0x8
  0008b6b2: mov r5,r0
  0008b6b4: movs r0,#0x5c
  0008b6b6: mov r8,r1
  0008b6b8: blx 0x0006eb24
  0008b6bc: mov r6,r0
  0008b6be: mov r0,r5
  0008b6c0: blx 0x0006fafc
  0008b6c4: mov r4,r0
  0008b6c6: mov r0,r5
  0008b6c8: blx 0x0006fb08
  0008b6cc: ldr r1,[0x0008b720]
  0008b6ce: vmov s0,r0
  0008b6d2: vmov s2,r4
  0008b6d6: vcvt.f32.s32 s0,s0
  0008b6da: vcvt.f32.s32 s2,s2
  0008b6de: mov r0,r6
  0008b6e0: mov.w r2,#0x43800000
  0008b6e4: mov.w r3,#0x46800000
  0008b6e8: vstr.32 s2,[sp]
  0008b6ec: vstr.32 s0,[sp,#0x4]
  0008b6f0: blx 0x0006fd6c
  0008b6f4: add.w r1,r5,#0x164
  0008b6f8: mov r0,r6
  0008b6fa: blx 0x0006fd78
  0008b6fe: ldr.w r0,[r5,#0x164]
  0008b702: subs r0,#0x1
  0008b704: str.w r0,[r8,#0x0]
  0008b708: add sp,#0x8
  0008b70a: pop.w r8
  0008b70e: pop {r4,r5,r6,r7,pc}
```
