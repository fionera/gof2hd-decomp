# AEGeometry::AEGeometry
elf 0xa4468 body 122
Sig: undefined __thiscall AEGeometry(AEGeometry * this, PaintCanvas * param_1)

## decompile
```c

/* AEGeometry::AEGeometry(AbyssEngine::PaintCanvas*) */

AEGeometry * __thiscall AEGeometry::AEGeometry(AEGeometry *this,PaintCanvas *param_1)

{
  undefined8 uVar1;
  
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x84));
  *(undefined2 *)(this + 8) = 0;
  *(PaintCanvas **)(this + 0x2c) = param_1;
  AbyssEngine::PaintCanvas::TransformCreate(param_1,(uint *)(this + 0x18));
  uVar1 = DAT_000b44f0;
  *(undefined8 *)(this + 0x30) = DAT_000b44e8;
  *(undefined8 *)(this + 0x38) = uVar1;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x5c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x60) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x40) = 0x3f800000;
  *(undefined4 *)(this + 0x44) = 0x3f800000;
  *(undefined2 *)(this + 0x48) = 0x101;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0x1c) = 0xffffffff;
  *(undefined4 *)(this + 0x20) = 0xffffffff;
  *(undefined4 *)(this + 0x24) = 0xffffffff;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)(this + 0x18);
  *(undefined4 *)(this + 0x10) = 0xffffffff;
  *(undefined4 *)(this + 0x14) = 0xffffffff;
  return this;
}

```

## target disasm
```
  000b4468: push {r4,r5,r6,r7,lr}
  000b446a: add r7,sp,#0xc
  000b446c: push.w r11
  000b4470: movs r6,#0x0
  000b4472: mov r4,r0
  000b4474: strd r6,r6,[r0,#0x78]
  000b4478: mov r5,r1
  000b447a: str.w r6,[r0,#0x80]
  000b447e: adds r0,#0x84
  000b4480: blx 0x0006f118
  000b4484: add.w r1,r4,#0x18
  000b4488: mov r0,r5
  000b448a: strh r6,[r4,#0x8]
  000b448c: str r5,[r4,#0x2c]
  000b448e: blx 0x000720ac
  000b4492: adr r0,[0xb44e8]
  000b4494: vmov.i32 q8,#0x0
  000b4498: vld1.64 {d18,d19},[r0]
  000b449c: add.w r0,r4,#0x30
  000b44a0: mov.w r1,#0xffffffff
  000b44a4: vst1.64 {d18,d19},[r0]
  000b44a8: add.w r0,r4,#0x54
  000b44ac: vst1.32 {d16,d17},[r0]
  000b44b0: mov.w r0,#0x3f800000
  000b44b4: strd r6,r6,[r4,#0x70]
  000b44b8: strd r0,r0,[r4,#0x40]
  000b44bc: movw r0,#0x101
  000b44c0: strh.w r0,[r4,#0x48]
  000b44c4: str r6,[r4,#0x4c]
  000b44c6: str r6,[r4,#0x64]
  000b44c8: ldr r0,[r4,#0x18]
  000b44ca: strd r1,r1,[r4,#0x1c]
  000b44ce: strd r1,r6,[r4,#0x24]
  000b44d2: str r6,[r4,#0x4]
  000b44d4: strd r0,r1,[r4,#0xc]
  000b44d8: mov r0,r4
  000b44da: str r1,[r4,#0x14]
  000b44dc: pop.w r11
  000b44e0: pop {r4,r5,r6,r7,pc}
```
