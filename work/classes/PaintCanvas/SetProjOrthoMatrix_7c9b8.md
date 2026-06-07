# PaintCanvas::SetProjOrthoMatrix
elf 0x7c9b8 body 182
Sig: undefined __thiscall SetProjOrthoMatrix(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetProjOrthoMatrix() */

void __thiscall AbyssEngine::PaintCanvas::SetProjOrthoMatrix(PaintCanvas *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint in_fpscr;
  uint uVar4;
  float in_s1;
  double dVar5;
  
  uVar4 = in_fpscr & 0xfffffff | (uint)(**(float **)(DAT_0008ca78 + 0x8c9cc) == -1.0) << 0x1e;
  if ((byte)(uVar4 >> 0x1e) == 0) {
    SetProjectionMatrix3d(this,**(float **)(DAT_0008ca78 + 0x8c9cc),in_s1,-1.0);
  }
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = uVar2;
  *(undefined4 *)(this + 0xb0) = uVar3;
  *(undefined4 *)(this + 0xb4) = uVar1;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = uVar2;
  *(undefined4 *)(this + 0xa0) = uVar3;
  *(undefined4 *)(this + 0xa4) = uVar1;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = uVar2;
  *(undefined4 *)(this + 0x90) = uVar3;
  *(undefined4 *)(this + 0x94) = uVar1;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = uVar2;
  *(undefined4 *)(this + 0x80) = uVar3;
  *(undefined4 *)(this + 0x84) = uVar1;
  uVar2 = Engine::GetDisplayWidth();
  dVar5 = (double)VectorSignedToFloat(uVar2,(byte)(uVar4 >> 0x16) & 3);
  *(float *)(this + 0x78) = (float)(2.0 / dVar5);
  uVar3 = Engine::GetDisplayHeight();
  uVar2 = DAT_0008ca70;
  dVar5 = (double)VectorSignedToFloat(uVar3,(byte)(uVar4 >> 0x16) & 3);
  *(undefined4 *)(this + 0xa0) = DAT_0008ca74;
  *(undefined4 *)(this + 0xb4) = 0x3f800000;
  *(undefined4 *)(this + 0xa8) = uVar2;
  *(undefined4 *)(this + 0xac) = 0x3f800000;
  *(float *)(this + 0x8c) = -(float)(2.0 / dVar5);
  return;
}

```

## target disasm
```
  0008c9b8: push {r4,r6,r7,lr}
  0008c9ba: add r7,sp,#0x8
  0008c9bc: vpush {d8}
  0008c9c0: mov r4,r0
  0008c9c2: ldr r0,[0x0008ca78]
  0008c9c4: vmov.f32 s2,0xbf800000
  0008c9c8: add r0,pc
  0008c9ca: ldr r0,[r0,#0x0]
  0008c9cc: vldr.32 s0,[r0]
  0008c9d0: vcmpe.f32 s0,s2
  0008c9d4: vmrs apsr,fpscr
  0008c9d8: beq 0x0008c9f4
  0008c9da: vmov r1,s0
  0008c9de: ldr r0,[0x0008ca7c]
  0008c9e0: ldr r2,[0x0008ca80]
  0008c9e2: add r0,pc
  0008c9e4: add r2,pc
  0008c9e6: ldr r0,[r0,#0x0]
  0008c9e8: ldr r2,[r2,#0x0]
  0008c9ea: ldr r3,[r0,#0x0]
  0008c9ec: mov r0,r4
  0008c9ee: ldr r2,[r2,#0x0]
  0008c9f0: blx 0x0006fd90
  0008c9f4: vmov.i32 q8,#0x0
  0008c9f8: add.w r1,r4,#0xa8
  0008c9fc: ldr r0,[r4,#0x34]
  0008c9fe: vst1.32 {d16,d17},[r1]
  0008ca02: add.w r1,r4,#0x98
  0008ca06: vst1.32 {d16,d17},[r1]
  0008ca0a: add.w r1,r4,#0x88
  0008ca0e: vst1.32 {d16,d17},[r1]
  0008ca12: add.w r1,r4,#0x78
  0008ca16: vst1.32 {d16,d17},[r1]
  0008ca1a: blx 0x0006e8cc
  0008ca1e: vmov s0,r0
  0008ca22: vmov.f64 d8,0x4000000000000000
  0008ca26: vcvt.f64.s32 d16,s0
  0008ca2a: ldr r0,[r4,#0x34]
  0008ca2c: vdiv.f64 d16,d8,d16
  0008ca30: vcvt.f32.f64 s0,d16
  0008ca34: vstr.32 s0,[r4,#0x78]
  0008ca38: blx 0x0006e8d8
  0008ca3c: vmov s0,r0
  0008ca40: ldr r1,[0x0008ca74]
  0008ca42: ldr r0,[0x0008ca70]
  0008ca44: vcvt.f64.s32 d16,s0
  0008ca48: str.w r1,[r4,#0xa0]
  0008ca4c: mov.w r1,#0x3f800000
  0008ca50: str.w r1,[r4,#0xb4]
  0008ca54: strd r0,r1,[r4,#0xa8]
  0008ca58: vdiv.f64 d16,d8,d16
  0008ca5c: vcvt.f32.f64 s0,d16
  0008ca60: vneg.f32 s0,s0
  0008ca64: vstr.32 s0,[r4,#0x8c]
  0008ca68: vpop {d8}
  0008ca6c: pop {r4,r6,r7,pc}
```
