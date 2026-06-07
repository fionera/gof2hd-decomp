# PaintCanvas::~PaintCanvas
elf 0x77a70 body 394
Sig: undefined __thiscall ~PaintCanvas(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::~PaintCanvas() */

PaintCanvas * __thiscall AbyssEngine::PaintCanvas::~PaintCanvas(PaintCanvas *this)

{
  int iVar1;
  void *pvVar2;
  undefined4 *puVar3;
  code *pcVar4;
  uint uVar5;
  int iVar6;
  
  ReleaseAllResources();
  iVar6 = 0;
  uVar5 = 0;
  do {
    if (*(uint *)(this + 0x134) <= uVar5) {
      MeshRelease(*(Engine **)(this + 0x34),(Mesh **)(this + 8));
      MeshRelease(*(Engine **)(this + 0x34),(Mesh **)(this + 0x1c8));
      iVar6 = 0;
      for (uVar5 = 0; uVar5 < *(uint *)(this + 0x10); uVar5 = uVar5 + 1) {
        pvVar2 = *(void **)(*(int *)(this + 0x14) + iVar6);
        if (pvVar2 == (void *)0x0) {
          puVar3 = (undefined4 *)(*(int *)(this + 0x14) + uVar5 * 4);
        }
        else {
          String::~String((String *)((int)pvVar2 + 4));
          operator_delete(pvVar2);
          puVar3 = (undefined4 *)(*(int *)(this + 0x14) + iVar6);
        }
        *puVar3 = 0;
        iVar6 = iVar6 + 4;
      }
      pcVar4 = *(code **)(DAT_00087c80 + 0x87b9a);
      (*pcVar4)(this + 0x1bc);
      Array<unsigned_int>::~Array((Array<unsigned_int> *)(this + 0x1b0));
      (*pcVar4)(this + 0x1a4);
      (*pcVar4)(this + 0x198);
      Array<AbyssEngine::Mesh*>::~Array((Array<AbyssEngine::Mesh*> *)(this + 0x18c));
      Array<AbyssEngine::SpriteSystem*>::~Array((Array<AbyssEngine::SpriteSystem*> *)(this + 0x180))
      ;
      Array<AbyssEngine::Material*>::~Array((Array<AbyssEngine::Material*> *)(this + 0x174));
      Array<AbyssEngine::Camera*>::~Array((Array<AbyssEngine::Camera*> *)(this + 0x164));
      Array<AbyssEngine::Transform*>::~Array((Array<AbyssEngine::Transform*> *)(this + 0x158));
      Array<AbyssEngine::Image2D*>::~Array((Array<AbyssEngine::Image2D*> *)(this + 0x14c));
      Array<AbyssEngine::ImageFont*>::~Array((Array<AbyssEngine::ImageFont*> *)(this + 0x140));
      Array<AbyssEngine::Resource*>::~Array((Array<AbyssEngine::Resource*> *)(this + 0x134));
      Array<AbyssEngine::Mesh*>::~Array((Array<AbyssEngine::Mesh*> *)(this + 0x24));
      Array<AbyssEngine::AELoadedTexture*>::~Array
                ((Array<AbyssEngine::AELoadedTexture*> *)(this + 0x10));
      return this;
    }
    iVar1 = *(int *)(*(int *)(this + 0x138) + iVar6);
    if (iVar1 != 0) {
      switch(*(undefined4 *)(iVar1 + 4)) {
      case 1:
      case 3:
      case 6:
        pvVar2 = *(void **)(iVar1 + 0xc);
LAB_00087aca:
        operator_delete(pvVar2);
        break;
      case 2:
        if (*(ResourceTexture **)(iVar1 + 0xc) != (ResourceTexture *)0x0) {
          pvVar2 = (void *)ResourceTexture::~ResourceTexture(*(ResourceTexture **)(iVar1 + 0xc));
          goto LAB_00087aca;
        }
        break;
      case 4:
        if (*(ResourceMesh **)(iVar1 + 0xc) != (ResourceMesh *)0x0) {
          pvVar2 = (void *)ResourceMesh::~ResourceMesh(*(ResourceMesh **)(iVar1 + 0xc));
          goto LAB_00087aca;
        }
        break;
      case 5:
        if (*(ResourceTransform **)(iVar1 + 0xc) != (ResourceTransform *)0x0) {
          pvVar2 = (void *)ResourceTransform::~ResourceTransform
                                     (*(ResourceTransform **)(iVar1 + 0xc));
          goto LAB_00087aca;
        }
      }
      pvVar2 = *(void **)(*(int *)(this + 0x138) + iVar6);
      if (pvVar2 == (void *)0x0) {
        puVar3 = (undefined4 *)(*(int *)(this + 0x138) + uVar5 * 4);
      }
      else {
        operator_delete(pvVar2);
        puVar3 = (undefined4 *)(*(int *)(this + 0x138) + iVar6);
      }
      *puVar3 = 0;
      ArrayRelease<AbyssEngine::Mesh*>((Array *)(this + 0x18c));
      pcVar4 = *(code **)(DAT_00087c7c + 0x87af6);
      (*pcVar4)(this + 0x198);
      (*pcVar4)(this + 0x1a4);
      ArrayRelease<unsigned_int>((Array *)(this + 0x1b0));
      (*pcVar4)(this + 0x1bc);
    }
    iVar6 = iVar6 + 4;
    uVar5 = uVar5 + 1;
  } while( true );
}

```

## target disasm
```
  00087a70: push {r4,r5,r6,r7,lr}
  00087a72: add r7,sp,#0xc
  00087a74: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00087a78: mov r11,r0
  00087a7a: blx 0x0006f964
  00087a7e: add.w r0,r11,#0x1bc
  00087a82: str r0,[sp,#0xc]
  00087a84: add.w r0,r11,#0x1b0
  00087a88: add.w r10,r11,#0x18c
  00087a8c: add.w r9,r11,#0x134
  00087a90: str r0,[sp,#0x10]
  00087a92: add.w r0,r11,#0x1a4
  00087a96: mov.w r8,#0x0
  00087a9a: movs r6,#0x0
  00087a9c: movs r5,#0x0
  00087a9e: str r0,[sp,#0x14]
  00087aa0: add.w r0,r11,#0x198
  00087aa4: str r0,[sp,#0x18]
  00087aa6: ldr.w r0,[r9,#0x0]
  00087aaa: cmp r5,r0
  00087aac: bcs 0x00087b32
  00087aae: ldr.w r0,[r11,#0x138]
  00087ab2: ldr r0,[r0,r6]
  00087ab4: cbz r0,0x00087b08
  00087ab6: ldr r1,[r0,#0x4]
  00087ab8: subs r1,#0x1
  00087aba: cmp r1,#0x5
  00087abc: bhi 0x00087ace
  00087abe: tbb [pc,r1]
  00087ac8: ldr r0,[r0,#0xc]
  00087aca: blx 0x0006eb48
  00087ace: ldr.w r1,[r11,#0x138]
  00087ad2: ldr r0,[r1,r6]
  00087ad4: cbz r0,0x00087ae2
  00087ad6: blx 0x0006eb48
  00087ada: ldr.w r0,[r11,#0x138]
  00087ade: add r0,r6
  00087ae0: b 0x00087ae6
  00087ae2: add.w r0,r1,r5, lsl #0x2
  00087ae6: str.w r8,[r0,#0x0]
  00087aea: mov r0,r10
  00087aec: blx 0x0006f970
  00087af0: ldr r0,[0x00087c7c]
  00087af2: add r0,pc
  00087af4: ldr r4,[r0,#0x0]
  00087af6: ldr r0,[sp,#0x18]
  00087af8: blx r4
  00087afa: ldr r0,[sp,#0x14]
  00087afc: blx r4
  00087afe: ldr r0,[sp,#0x10]
  00087b00: blx 0x0006f97c
  00087b04: ldr r0,[sp,#0xc]
  00087b06: blx r4
  00087b08: adds r6,#0x4
  00087b0a: adds r5,#0x1
  00087b0c: b 0x00087aa6
  00087b0e: ldr r0,[r0,#0xc]
  00087b10: cmp r0,#0x0
  00087b12: beq 0x00087ace
  00087b14: blx 0x0006f988
  00087b18: b 0x00087aca
  00087b1a: ldr r0,[r0,#0xc]
  00087b1c: cmp r0,#0x0
  00087b1e: beq 0x00087ace
  00087b20: blx 0x0006f994
  00087b24: b 0x00087aca
  00087b26: ldr r0,[r0,#0xc]
  00087b28: cmp r0,#0x0
  00087b2a: beq 0x00087ace
  00087b2c: blx 0x0006f9a0
  00087b30: b 0x00087aca
  00087b32: strd r10,r9,[sp,#0x4]
  00087b36: ldr.w r0,[r11,#0x34]
  00087b3a: add.w r1,r11,#0x8
  00087b3e: blx 0x0006ebd8
  00087b42: ldr.w r0,[r11,#0x34]
  00087b46: add.w r1,r11,#0x1c8
  00087b4a: blx 0x0006ebd8
  00087b4e: add.w r8,r11,#0x10
  00087b52: movs r5,#0x0
  00087b54: movs r6,#0x0
  00087b56: mov.w r10,#0x0
  00087b5a: b 0x00087b8c
  00087b5c: ldr.w r0,[r11,#0x14]
  00087b60: ldr.w r9,[r0,r6]
  00087b64: cmp.w r9,#0x0
  00087b68: beq 0x00087b80
  00087b6a: add.w r0,r9,#0x4
  00087b6e: blx 0x0006ee30
  00087b72: mov r0,r9
  00087b74: blx 0x0006eb48
  00087b78: ldr.w r0,[r11,#0x14]
  00087b7c: add r0,r6
  00087b7e: b 0x00087b84
  00087b80: add.w r0,r0,r10, lsl #0x2
  00087b84: str r5,[r0,#0x0]
  00087b86: adds r6,#0x4
  00087b88: add.w r10,r10,#0x1
  00087b8c: ldr.w r0,[r8,#0x0]
  00087b90: cmp r10,r0
  00087b92: bcc 0x00087b5c
  00087b94: ldr r0,[0x00087c80]
  00087b96: add r0,pc
  00087b98: ldr r4,[r0,#0x0]
  00087b9a: ldr r0,[sp,#0xc]
  00087b9c: blx r4
  00087b9e: ldr r0,[sp,#0x10]
  00087ba0: blx 0x0006f904
  00087ba4: ldr r0,[sp,#0x14]
  00087ba6: blx r4
  00087ba8: ldr r0,[sp,#0x18]
  00087baa: blx r4
  00087bac: ldr r0,[sp,#0x4]
  00087bae: blx 0x0006f760
  00087bb2: add.w r0,r11,#0x180
  00087bb6: blx 0x0006f910
  00087bba: add.w r0,r11,#0x174
  00087bbe: blx 0x0006f91c
  00087bc2: add.w r0,r11,#0x164
  00087bc6: blx 0x0006f928
  00087bca: add.w r0,r11,#0x158
  00087bce: blx 0x0006f754
  00087bd2: add.w r0,r11,#0x14c
  00087bd6: blx 0x0006f934
  00087bda: add.w r0,r11,#0x140
  00087bde: blx 0x0006f940
  00087be2: ldr r0,[sp,#0x8]
  00087be4: blx 0x0006f94c
  00087be8: add.w r0,r11,#0x24
  00087bec: blx 0x0006f760
  00087bf0: mov r0,r8
  00087bf2: blx 0x0006f958
  00087bf6: mov r0,r11
  00087bf8: add sp,#0x1c
  00087bfa: pop.w {r8,r9,r10,r11}
  00087bfe: pop {r4,r5,r6,r7,pc}
```
