# SpaceLounge::~SpaceLounge
elf 0x16a89a body 262
Sig: undefined __thiscall ~SpaceLounge(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::~SpaceLounge() */

undefined8 __thiscall SpaceLounge::~SpaceLounge(SpaceLounge *this)

{
  void *pvVar1;
  Array *pAVar2;
  Array<ImagePart*> *this_00;
  undefined4 *puVar3;
  undefined4 extraout_r1;
  undefined8 *extraout_r2;
  undefined8 *extraout_r2_00;
  undefined8 *extraout_r2_01;
  undefined8 *extraout_r2_02;
  undefined8 *extraout_r2_03;
  undefined8 *puVar4;
  uint uVar5;
  int iVar6;
  
  if (*(ChoiceWindow **)(this + 8) != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(this + 8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 8) = 0;
  if (*(CutScene **)(this + 0x44) != (CutScene *)0x0) {
    pvVar1 = (void *)CutScene::~CutScene(*(CutScene **)(this + 0x44));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x44) = 0;
  operator_delete__(*(void **)(this + 0x58));
  *(undefined4 *)(this + 0x58) = 0;
  puVar4 = extraout_r2;
  if (*(Array **)(this + 0x28) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0x28));
    puVar4 = extraout_r2_00;
    if (*(Array<AbyssEngine::String*> **)(this + 0x28) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(this + 0x28));
      operator_delete(pvVar1);
      puVar4 = extraout_r2_01;
    }
  }
  *(undefined4 *)(this + 0x28) = 0;
  if (*(Array **)(this + 0x5c) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0x5c));
    puVar4 = extraout_r2_02;
    if (*(Array<TouchButton*> **)(this + 0x5c) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0x5c));
      operator_delete(pvVar1);
      puVar4 = extraout_r2_03;
    }
  }
  *(undefined4 *)(this + 0x5c) = 0;
  if (*(int *)(this + 0x3c) != 0) {
    return *puVar4;
  }
  pAVar2 = *(Array **)(this + 0x38);
  *(undefined4 *)(this + 0x3c) = 0;
  if (pAVar2 != (Array *)0x0) {
    iVar6 = 0;
    for (uVar5 = 0; uVar5 < *(uint *)pAVar2; uVar5 = uVar5 + 1) {
      ArrayReleaseClasses<ImagePart*>(*(Array **)(*(int *)(pAVar2 + 4) + iVar6));
      this_00 = *(Array<ImagePart*> **)(*(int *)(*(int *)(this + 0x38) + 4) + iVar6);
      if (this_00 == (Array<ImagePart*> *)0x0) {
        puVar3 = (undefined4 *)(*(int *)(*(int *)(this + 0x38) + 4) + uVar5 * 4);
      }
      else {
        pvVar1 = (void *)Array<ImagePart*>::~Array(this_00);
        operator_delete(pvVar1);
        puVar3 = (undefined4 *)(*(int *)(*(int *)(this + 0x38) + 4) + iVar6);
      }
      *puVar3 = 0;
      iVar6 = iVar6 + 4;
      pAVar2 = *(Array **)(this + 0x38);
    }
    ArrayReleaseClasses<Array<ImagePart*>*>(pAVar2);
    if (*(Array<Array<ImagePart*>*> **)(this + 0x38) != (Array<Array<ImagePart*>*> *)0x0) {
      pvVar1 = (void *)Array<Array<ImagePart*>*>::~Array
                                 (*(Array<Array<ImagePart*>*> **)(this + 0x38));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x38) = 0;
  if (*(Array **)(this + 0x40) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(*(Array **)(this + 0x40));
    if (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x40) !=
        (Array<AbyssEngine::AEMath::Vector*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::AEMath::Vector*>::~Array
                                 (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x40));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x40) = 0;
  if (*(EaseInOutMatrix **)(this + 0x48) != (EaseInOutMatrix *)0x0) {
    pvVar1 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix
                               (*(EaseInOutMatrix **)(this + 0x48));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x48) = 0;
  if (*(void **)(this + 0xc0) != (void *)0x0) {
    operator_delete(*(void **)(this + 0xc0));
  }
  *(undefined4 *)(this + 0xc0) = 0;
  AbyssEngine::String::~String((String *)(this + 0xa4));
  return CONCAT44(extraout_r1,this);
}

```

## target disasm
```
  0017a89a: push {r4,r5,r6,r7,lr}
  0017a89c: add r7,sp,#0xc
  0017a89e: push.w r8
  0017a8a2: mov r4,r0
  0017a8a4: ldr r0,[r0,#0x8]
  0017a8a6: cbz r0,0x0017a8b0
  0017a8a8: blx 0x00074d88
  0017a8ac: blx 0x0006eb48
  0017a8b0: ldr r0,[r4,#0x44]
  0017a8b2: movs r5,#0x0
  0017a8b4: str r5,[r4,#0x8]
  0017a8b6: cbz r0,0x0017a8c0
  0017a8b8: blx 0x00075370
  0017a8bc: blx 0x0006eb48
  0017a8c0: ldr r0,[r4,#0x58]
  0017a8c2: str r5,[r4,#0x44]
  0017a8c4: blx 0x0006ebfc
  0017a8c8: ldr r0,[r4,#0x28]
  0017a8ca: str r5,[r4,#0x58]
  0017a8cc: cbz r0,0x0017a8de
  0017a8ce: blx 0x0006facc
  0017a8d2: ldr r0,[r4,#0x28]
  0017a8d4: cbz r0,0x0017a8de
  0017a8d6: blx 0x0006f64c
  0017a8da: blx 0x0006eb48
  0017a8de: ldr r0,[r4,#0x5c]
  0017a8e0: movs r5,#0x0
  0017a8e2: str r5,[r4,#0x28]
  0017a8e4: cbz r0,0x0017a8f6
  0017a8e6: blx 0x00075328
  0017a8ea: ldr r0,[r4,#0x5c]
  0017a8ec: cbz r0,0x0017a8f6
  0017a8ee: blx 0x00075334
  0017a8f2: blx 0x0006eb48
  0017a8f6: ldr r0,[r4,#0x3c]
  0017a8f8: str r5,[r4,#0x5c]
  0017a8fa: cbz r0,0x0017a90c
  0017a8fc: ldmia r2!,{r0,r1,r3,r4,r5,r6,r7,r9,r10,r12,sp,lr,pc}^
  0017a90c: ldr r0,[r4,#0x38]
  0017a90e: mov.w r8,#0x0
  0017a912: str.w r8,[r4,#0x3c]
  0017a916: cbz r0,0x0017a962
  0017a918: movs r6,#0x0
  0017a91a: movs r5,#0x0
  0017a91c: b 0x0017a94c
  0017a91e: ldr r0,[r0,#0x4]
  0017a920: ldr r0,[r0,r6]
  0017a922: blx 0x00075ae4
  0017a926: ldr r0,[r4,#0x38]
  0017a928: ldr r1,[r0,#0x4]
  0017a92a: ldr r0,[r1,r6]
  0017a92c: cbz r0,0x0017a93e
  0017a92e: blx 0x00075af0
  0017a932: blx 0x0006eb48
  0017a936: ldr r0,[r4,#0x38]
  0017a938: ldr r0,[r0,#0x4]
  0017a93a: add r0,r6
  0017a93c: b 0x0017a942
  0017a93e: add.w r0,r1,r5, lsl #0x2
  0017a942: str.w r8,[r0,#0x0]
  0017a946: adds r6,#0x4
  0017a948: ldr r0,[r4,#0x38]
  0017a94a: adds r5,#0x1
  0017a94c: ldr r1,[r0,#0x0]
  0017a94e: cmp r5,r1
  0017a950: bcc 0x0017a91e
  0017a952: blx 0x00077d34
  0017a956: ldr r0,[r4,#0x38]
  0017a958: cbz r0,0x0017a962
  0017a95a: blx 0x00077d40
  0017a95e: blx 0x0006eb48
  0017a962: ldr r0,[r4,#0x40]
  0017a964: str.w r8,[r4,#0x38]
  0017a968: cbz r0,0x0017a97a
  0017a96a: blx 0x0007216c
  0017a96e: ldr r0,[r4,#0x40]
  0017a970: cbz r0,0x0017a97a
  0017a972: blx 0x00072178
  0017a976: blx 0x0006eb48
  0017a97a: ldr r0,[r4,#0x48]
  0017a97c: movs r5,#0x0
  0017a97e: str r5,[r4,#0x40]
  0017a980: cbz r0,0x0017a98a
  0017a982: blx 0x00072340
  0017a986: blx 0x0006eb48
  0017a98a: ldr.w r0,[r4,#0xc0]
  0017a98e: str r5,[r4,#0x48]
  0017a990: cbz r0,0x0017a996
  0017a992: blx 0x0006eb48
  0017a996: movs r0,#0x0
  0017a998: str.w r0,[r4,#0xc0]
  0017a99c: add.w r0,r4,#0xa4
  0017a9a0: blx 0x0006ee30
  0017a9a4: mov r0,r4
  0017a9a6: pop.w r8
  0017a9aa: pop {r4,r5,r6,r7,pc}
```
