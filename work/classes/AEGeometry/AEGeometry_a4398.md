# AEGeometry::AEGeometry
elf 0xa4398 body 186
Sig: undefined __thiscall AEGeometry(AEGeometry * this, ushort param_1, PaintCanvas * param_2, bool param_3)

## decompile
```c

/* AEGeometry::AEGeometry(unsigned short, AbyssEngine::PaintCanvas*, bool) */

AEGeometry * __thiscall
AEGeometry::AEGeometry(AEGeometry *this,ushort param_1,PaintCanvas *param_2,bool param_3)

{
  AEGeometry *pAVar1;
  undefined8 uVar2;
  Matrix *pMVar3;
  
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x84));
  *(PaintCanvas **)(this + 0x2c) = param_2;
  *(ushort *)(this + 8) = param_1;
  *(undefined4 *)(this + 0xc) = 0;
  pAVar1 = this + 0x18;
  *(uint *)pAVar1 = 0;
  AbyssEngine::PaintCanvas::TransformCreate(param_2,(uint *)pAVar1);
  AbyssEngine::PaintCanvas::MeshCreate(param_2,param_1,(uint *)(this + 0x1c),param_3);
  AbyssEngine::PaintCanvas::TransformAddMeshId(param_2,*(uint *)pAVar1,*(uint *)(this + 0x1c));
  uVar2 = DAT_000b4460;
  *(undefined8 *)(this + 0x30) = DAT_000b4458;
  *(undefined8 *)(this + 0x38) = uVar2;
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
  *(undefined4 *)(this + 0x20) = 0xffffffff;
  *(undefined4 *)(this + 0x24) = 0xffffffff;
  *(undefined4 *)(this + 0x28) = 0;
  *(uint *)(this + 0xc) = *(uint *)pAVar1;
  *(undefined4 *)(this + 0x10) = 0xffffffff;
  *(undefined4 *)(this + 0x14) = 0xffffffff;
  pMVar3 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal((uint)param_2);
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x84),pMVar3);
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  000b4398: push {r4,r5,r6,r7,lr}
  000b439a: add r7,sp,#0xc
  000b439c: push {r7,r8,r9,r10,r11}
  000b43a0: add.w r8,r0,#0x84
  000b43a4: mov.w r11,#0x0
  000b43a8: strd r11,r11,[r0,#0x78]
  000b43ac: mov r4,r0
  000b43ae: str.w r11,[r0,#0x80]
  000b43b2: mov r0,r8
  000b43b4: mov r9,r3
  000b43b6: mov r5,r2
  000b43b8: mov r10,r1
  000b43ba: blx 0x0006f118
  000b43be: mov r6,r4
  000b43c0: str r5,[r4,#0x2c]
  000b43c2: strh.w r10,[r4,#0x8]
  000b43c6: mov r0,r5
  000b43c8: str.w r11,[r4,#0xc]
  000b43cc: str.w r11,[r6,#0x18]!
  000b43d0: mov r1,r6
  000b43d2: blx 0x000720ac
  000b43d6: add.w r2,r4,#0x1c
  000b43da: mov r0,r5
  000b43dc: mov r1,r10
  000b43de: mov r3,r9
  000b43e0: blx 0x0006fcf4
  000b43e4: ldr r1,[r6,#0x0]
  000b43e6: mov r0,r5
  000b43e8: ldr r2,[r4,#0x1c]
  000b43ea: blx 0x00073030
  000b43ee: adr r0,[0xb4458]
  000b43f0: vmov.i32 q8,#0x0
  000b43f4: vld1.64 {d18,d19},[r0]
  000b43f8: add.w r0,r4,#0x30
  000b43fc: vst1.64 {d18,d19},[r0]
  000b4400: add.w r0,r4,#0x54
  000b4404: vst1.32 {d16,d17},[r0]
  000b4408: mov.w r0,#0x3f800000
  000b440c: strd r11,r11,[r4,#0x70]
  000b4410: strd r0,r0,[r4,#0x40]
  000b4414: movw r0,#0x101
  000b4418: strh.w r0,[r4,#0x48]
  000b441c: mov.w r0,#0xffffffff
  000b4420: str.w r11,[r4,#0x4c]
  000b4424: str.w r11,[r4,#0x64]
  000b4428: ldr r1,[r6,#0x0]
  000b442a: strd r0,r0,[r4,#0x20]
  000b442e: str.w r11,[r4,#0x28]
  000b4432: strd r1,r0,[r4,#0xc]
  000b4436: str r0,[r4,#0x14]
  000b4438: mov r0,r5
  000b443a: blx 0x000720c4
  000b443e: mov r1,r0
  000b4440: mov r0,r8
  000b4442: blx 0x0006f148
  000b4446: str.w r11,[r4,#0x4]
  000b444a: mov r0,r4
  000b444c: pop.w {r3,r8,r9,r10,r11}
  000b4450: pop {r4,r5,r6,r7,pc}
```
