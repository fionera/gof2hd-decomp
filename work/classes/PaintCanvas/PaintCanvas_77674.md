# PaintCanvas::PaintCanvas
elf 0x77674 body 370
Sig: undefined __thiscall PaintCanvas(PaintCanvas * this, Engine * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::PaintCanvas(AbyssEngine::Engine*) */

PaintCanvas * __thiscall AbyssEngine::PaintCanvas::PaintCanvas(PaintCanvas *this,Engine *param_1)

{
  undefined1 auVar1 [16];
  ushort uVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  Array<AbyssEngine::AELoadedTexture*>::Array((Array<AbyssEngine::AELoadedTexture*> *)(this + 0x10))
  ;
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 0x24));
  AEMath::Matrix::Matrix((Matrix *)(this + 0xf8));
  Array<AbyssEngine::Resource*>::Array((Array<AbyssEngine::Resource*> *)(this + 0x134));
  Array<AbyssEngine::ImageFont*>::Array((Array<AbyssEngine::ImageFont*> *)(this + 0x140));
  Array<AbyssEngine::Image2D*>::Array((Array<AbyssEngine::Image2D*> *)(this + 0x14c));
  Array<AbyssEngine::Transform*>::Array((Array<AbyssEngine::Transform*> *)(this + 0x158));
  Array<AbyssEngine::Camera*>::Array((Array<AbyssEngine::Camera*> *)(this + 0x164));
  Array<AbyssEngine::Material*>::Array((Array<AbyssEngine::Material*> *)(this + 0x174));
  Array<AbyssEngine::SpriteSystem*>::Array((Array<AbyssEngine::SpriteSystem*> *)(this + 0x180));
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 0x18c));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x198));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x1a4));
  Array<unsigned_int>::Array((Array<unsigned_int> *)(this + 0x1b0));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x1bc));
  this[0x1f1] = (PaintCanvas)0x0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 500) = 1;
  *(undefined4 *)(this + 4) = 0;
  *this = (PaintCanvas)0x0;
  *(Engine **)(this + 0x34) = param_1;
  *(undefined4 *)(this + 0x170) = 0xffffffff;
  AbyssEngine::MeshCreate(param_1,4,2,0x11,this + 0x1c8);
  uVar4 = DAT_00087878;
  auVar1._8_8_ = SUB128(SUB1612((undefined1  [16])0x0,4),4);
  auVar1._0_8_ = 0x3f8000003f800000;
  auVar1 = auVar1 << 0x40 | auVar1;
  puVar5 = *(undefined4 **)(*(int *)(this + 0x1c8) + 0x2c);
  *puVar5 = 0x20000;
  puVar5[1] = 1;
  puVar5[2] = uVar4;
  *(undefined4 *)(this + 0x1fc) = *(undefined4 *)auVar1;
  *(undefined4 *)(this + 0x200) = *(undefined4 *)(auVar1 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x204) = *(undefined4 *)(auVar1 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x208) = *(undefined4 *)(auVar1 + (undefined1  [16])0xc);
  this[0x1c] = (PaintCanvas)0x1;
  param_1[0xfc] = (Engine)0x1;
  AbyssEngine::MeshCreate(param_1,400,200,0x1b,this + 8);
  iVar9 = 0;
  iVar6 = *(int *)(*(int *)(this + 8) + 0x2c);
  for (iVar7 = 0; iVar7 != 0x4b0; iVar7 = iVar7 + 0xc) {
    iVar8 = iVar6 + iVar7;
    uVar3 = (undefined2)(iVar9 << 2);
    *(undefined2 *)(iVar6 + iVar7) = uVar3;
    uVar2 = (ushort)(iVar9 << 2) | 2;
    *(ushort *)(iVar8 + 2) = uVar2;
    *(ushort *)(iVar8 + 4) = (ushort)(iVar9 << 2) | 1;
    *(undefined2 *)(iVar8 + 6) = uVar3;
    *(ushort *)(iVar8 + 8) = (ushort)(iVar9 << 2) | 3;
    iVar9 = iVar9 + 1;
    *(ushort *)(iVar8 + 10) = uVar2;
  }
  this[0x1f8] = (PaintCanvas)0x1;
  *(undefined4 *)(this + 0x1cc) = 0;
  return this;
}

```

## target disasm
```
  00087674: push {r4,r5,r6,r7,lr}
  00087676: add r7,sp,#0xc
  00087678: push {r8,r9,r10,r11}
  0008767c: sub sp,#0x2c
  0008767e: add.w r10,r0,#0x10
  00087682: mov r8,r0
  00087684: mov r11,r1
  00087686: mov r0,r10
  00087688: blx 0x0006f88c
  0008768c: add.w r9,r8,#0x24
  00087690: mov r0,r9
  00087692: str.w r10,[sp,#0x28]
  00087696: blx 0x0006f724
  0008769a: add.w r0,r8,#0xf8
  0008769e: str.w r9,[sp,#0x24]
  000876a2: blx 0x0006f118
  000876a6: add.w r6,r8,#0x134
  000876aa: mov r0,r6
  000876ac: blx 0x0006f898
  000876b0: add.w r5,r8,#0x140
  000876b4: mov r0,r5
  000876b6: str r6,[sp,#0x20]
  000876b8: blx 0x0006f8a4
  000876bc: add.w r4,r8,#0x14c
  000876c0: mov r0,r4
  000876c2: str r5,[sp,#0x1c]
  000876c4: blx 0x0006f8b0
  000876c8: add.w r0,r8,#0x158
  000876cc: strd r0,r4,[sp,#0x14]
  000876d0: blx 0x0006f730
  000876d4: add.w r0,r8,#0x164
  000876d8: str r0,[sp,#0x10]
  000876da: blx 0x0006f8bc
  000876de: add.w r0,r8,#0x174
  000876e2: str r0,[sp,#0xc]
  000876e4: blx 0x0006f8c8
  000876e8: add.w r0,r8,#0x180
  000876ec: str r0,[sp,#0x8]
  000876ee: blx 0x0006f8d4
  000876f2: add.w r0,r8,#0x18c
  000876f6: str r0,[sp,#0x4]
  000876f8: blx 0x0006f724
  000876fc: add.w r6,r8,#0x198
  00087700: mov r0,r6
  00087702: blx 0x0006f8e0
  00087706: add.w r4,r8,#0x1a4
  0008770a: mov r0,r4
  0008770c: blx 0x0006f8e0
  00087710: add.w r9,r8,#0x1b0
  00087714: mov r0,r9
  00087716: blx 0x0006f8ec
  0008771a: add.w r10,r8,#0x1bc
  0008771e: mov r0,r10
  00087720: blx 0x0006f8e0
  00087724: movs r0,#0x0
  00087726: movs r5,#0x1
  00087728: strb.w r0,[r8,#0x1f1]
  0008772c: str.w r0,[r8,#0x20]
  00087730: str.w r5,[r8,#0x1f4]
  00087734: str.w r0,[r8,#0x4]
  00087738: strb.w r0,[r8,#0x0]
  0008773c: mov.w r0,#0xffffffff
  00087740: str.w r11,[r8,#0x34]
  00087744: str.w r0,[r8,#0x170]
  00087748: add.w r0,r8,#0x1c8
  0008774c: str r0,[sp,#0x0]
  0008774e: mov r0,r11
  00087750: movs r1,#0x4
  00087752: movs r2,#0x2
  00087754: movs r3,#0x11
  00087756: blx 0x0006f1fc
  0008775a: ldr.w r0,[r8,#0x1c8]
  0008775e: vmov.f32 q8,#0x3f800000
  00087762: ldr r1,[0x00087878]
  00087764: mov.w r2,#0x20000
  00087768: ldr r0,[r0,#0x2c]
  0008776a: str r2,[r0,#0x0]
  0008776c: str r5,[r0,#0x4]
  0008776e: str r1,[r0,#0x8]
  00087770: add.w r0,r8,#0x1fc
  00087774: vst1.32 {d16,d17},[r0]
  00087778: strb.w r5,[r8,#0x1c]
  0008777c: strb.w r5,[r11,#0xfc]
  00087780: add.w r5,r8,#0x8
  00087784: mov r0,r11
  00087786: mov.w r1,#0x190
  0008778a: movs r2,#0xc8
  0008778c: movs r3,#0x1b
  0008778e: str r5,[sp,#0x0]
  00087790: blx 0x0006f1fc
  00087794: ldr r0,[r5,#0x0]
  00087796: movs r1,#0x0
  00087798: mov.w r12,#0x2
  0008779c: mov.w lr,#0x1
  000877a0: mov.w r9,#0x3
  000877a4: movs r5,#0x0
  000877a6: ldr r0,[r0,#0x2c]
  000877a8: b 0x000877ca
  000877aa: adds r2,r0,r1
  000877ac: lsls r4,r5,#0x2
  000877ae: strh r4,[r0,r1]
  000877b0: orr.w r3,r12,r5, lsl #0x2
  000877b4: orr.w r6,lr,r5, lsl #0x2
  000877b8: strh r3,[r2,#0x2]
  000877ba: adds r1,#0xc
  000877bc: strh r6,[r2,#0x4]
  000877be: strh r4,[r2,#0x6]
  000877c0: orr.w r4,r9,r5, lsl #0x2
  000877c4: strh r4,[r2,#0x8]
  000877c6: adds r5,#0x1
  000877c8: strh r3,[r2,#0xa]
  000877ca: cmp.w r1,#0x4b0
  000877ce: bne 0x000877aa
  000877d0: movs r0,#0x1
  000877d2: strb.w r0,[r8,#0x1f8]
  000877d6: movs r0,#0x0
  000877d8: str.w r0,[r8,#0x1cc]
  000877dc: mov r0,r8
  000877de: add sp,#0x2c
  000877e0: pop.w {r8,r9,r10,r11}
  000877e4: pop {r4,r5,r6,r7,pc}
```
