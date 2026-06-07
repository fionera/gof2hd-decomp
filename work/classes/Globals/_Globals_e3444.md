# Globals::~Globals
elf 0xe3444 body 408
Sig: undefined __thiscall ~Globals(Globals * this)

## decompile
```c

/* Globals::~Globals() */

Globals * __thiscall Globals::~Globals(Globals *this)

{
  int iVar1;
  void *pvVar2;
  GameText *this_00;
  AERandom *this_01;
  Layout *this_02;
  Generator *this_03;
  RecordHandler *this_04;
  int *piVar3;
  FModSound *this_05;
  Galaxy *this_06;
  Achievements *this_07;
  Status *this_08;
  ImageFactory *this_09;
  int *piVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  
  puVar8 = *(undefined4 **)(DAT_000f35e0 + 0xf3454);
  if ((RecordHandler *)*puVar8 != (RecordHandler *)0x0) {
    RecordHandler::saveOptions((RecordHandler *)*puVar8);
  }
  puVar7 = *(undefined4 **)(DAT_000f35e4 + 0xf3466);
  if ((Galaxy *)*puVar7 != (Galaxy *)0x0) {
    pvVar2 = (void *)Galaxy::~Galaxy((Galaxy *)*puVar7);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f35e8;
  *puVar7 = 0;
  puVar6 = *(undefined4 **)(iVar1 + 0xf3482);
  if ((Status *)*puVar6 != (Status *)0x0) {
    pvVar2 = (void *)Status::~Status((Status *)*puVar6);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f35ec;
  *puVar6 = 0;
  puVar5 = *(undefined4 **)(iVar1 + 0xf349c);
  this_00 = (GameText *)*puVar5;
  if (this_00 != (GameText *)0x0) {
    pvVar2 = (void *)GameText::~GameText(this_00);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f35f0;
  *puVar5 = 0;
  puVar5 = *(undefined4 **)(iVar1 + 0xf34b2);
  this_01 = (AERandom *)*puVar5;
  if (this_01 != (AERandom *)0x0) {
    pvVar2 = (void *)AbyssEngine::AERandom::~AERandom(this_01);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f35f4;
  *puVar5 = 0;
  puVar5 = *(undefined4 **)(iVar1 + 0xf34c6);
  this_02 = (Layout *)*puVar5;
  if (this_02 != (Layout *)0x0) {
    pvVar2 = (void *)Layout::~Layout(this_02);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f35f8;
  *puVar5 = 0;
  puVar5 = *(undefined4 **)(iVar1 + 0xf34dc);
  this_03 = (Generator *)*puVar5;
  if (this_03 != (Generator *)0x0) {
    pvVar2 = (void *)Generator::~Generator(this_03);
    operator_delete(pvVar2);
  }
  this_04 = (RecordHandler *)*puVar8;
  *puVar5 = 0;
  if (this_04 != (RecordHandler *)0x0) {
    pvVar2 = (void *)RecordHandler::~RecordHandler(this_04);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f35fc;
  *puVar8 = 0;
  piVar4 = *(int **)(iVar1 + 0xf3504);
  piVar3 = (int *)*piVar4;
  if (piVar3 != (int *)0x0) {
    (**(code **)(*piVar3 + 4))();
  }
  iVar1 = DAT_000f3600;
  *piVar4 = 0;
  puVar8 = *(undefined4 **)(iVar1 + 0xf3516);
  this_05 = (FModSound *)*puVar8;
  if (this_05 != (FModSound *)0x0) {
    pvVar2 = (void *)FModSound::~FModSound(this_05);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f3604;
  *puVar8 = 0;
  puVar8 = *(undefined4 **)(iVar1 + 0xf352c);
  if ((Array *)*puVar8 != (Array *)0x0) {
    ArrayReleaseClasses<Item*>((Array *)*puVar8);
    if ((Array<Item*> *)*puVar8 != (Array<Item*> *)0x0) {
      pvVar2 = (void *)Array<Item*>::~Array((Array<Item*> *)*puVar8);
      operator_delete(pvVar2);
    }
  }
  iVar1 = DAT_000f3608;
  *puVar8 = 0;
  puVar8 = *(undefined4 **)(iVar1 + 0xf3548);
  if ((Array *)*puVar8 != (Array *)0x0) {
    ArrayReleaseClasses<Ship*>((Array *)*puVar8);
    if ((Array<Ship*> *)*puVar8 != (Array<Ship*> *)0x0) {
      pvVar2 = (void *)Array<Ship*>::~Array((Array<Ship*> *)*puVar8);
      operator_delete(pvVar2);
    }
  }
  this_06 = (Galaxy *)*puVar7;
  *puVar8 = 0;
  if (this_06 != (Galaxy *)0x0) {
    pvVar2 = (void *)Galaxy::~Galaxy(this_06);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f360c;
  *puVar7 = 0;
  puVar8 = *(undefined4 **)(iVar1 + 0xf3578);
  this_07 = (Achievements *)*puVar8;
  if (this_07 != (Achievements *)0x0) {
    pvVar2 = (void *)Achievements::~Achievements(this_07);
    operator_delete(pvVar2);
  }
  this_08 = (Status *)*puVar6;
  *puVar8 = 0;
  if (this_08 != (Status *)0x0) {
    pvVar2 = (void *)Status::~Status(this_08);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f3610;
  *puVar6 = 0;
  puVar8 = *(undefined4 **)(iVar1 + 0xf35a0);
  this_09 = (ImageFactory *)*puVar8;
  if (this_09 != (ImageFactory *)0x0) {
    pvVar2 = (void *)ImageFactory::~ImageFactory(this_09);
    operator_delete(pvVar2);
  }
  *puVar8 = 0;
  if (*(Array **)(this + 4) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 4));
    if (*(Array<int> **)(this + 4) != (Array<int> *)0x0) {
      pvVar2 = (void *)Array<int>::~Array(*(Array<int> **)(this + 4));
      operator_delete(pvVar2);
    }
  }
  iVar1 = DAT_000f3614;
  *(undefined4 *)(this + 4) = 0;
  **(undefined4 **)(iVar1 + 0xf35d2) = 0;
  return this;
}

```

## target disasm
```
  000f3444: push {r4,r5,r6,r7,lr}
  000f3446: add r7,sp,#0xc
  000f3448: push {r7,r8,r9,r10,r11}
  000f344c: mov r8,r0
  000f344e: ldr r0,[0x000f35e0]
  000f3450: add r0,pc
  000f3452: ldr.w r11,[r0,#0x0]
  000f3456: ldr.w r0,[r11,#0x0]
  000f345a: cbz r0,0x000f3460
  000f345c: blx 0x00071788
  000f3460: ldr r0,[0x000f35e4]
  000f3462: add r0,pc
  000f3464: ldr.w r10,[r0,#0x0]
  000f3468: ldr.w r0,[r10,#0x0]
  000f346c: cbz r0,0x000f3476
  000f346e: blx 0x00075ca0
  000f3472: blx 0x0006eb48
  000f3476: ldr r0,[0x000f35e8]
  000f3478: movs r6,#0x0
  000f347a: str.w r6,[r10,#0x0]
  000f347e: add r0,pc
  000f3480: ldr.w r9,[r0,#0x0]
  000f3484: ldr.w r0,[r9,#0x0]
  000f3488: cbz r0,0x000f3492
  000f348a: blx 0x00075cac
  000f348e: blx 0x0006eb48
  000f3492: ldr r0,[0x000f35ec]
  000f3494: str.w r6,[r9,#0x0]
  000f3498: add r0,pc
  000f349a: ldr r5,[r0,#0x0]
  000f349c: ldr r0,[r5,#0x0]
  000f349e: cbz r0,0x000f34a8
  000f34a0: blx 0x00075cb8
  000f34a4: blx 0x0006eb48
  000f34a8: ldr r0,[0x000f35f0]
  000f34aa: movs r4,#0x0
  000f34ac: str r4,[r5,#0x0]
  000f34ae: add r0,pc
  000f34b0: ldr r6,[r0,#0x0]
  000f34b2: ldr r0,[r6,#0x0]
  000f34b4: cbz r0,0x000f34be
  000f34b6: blx 0x00075cc4
  000f34ba: blx 0x0006eb48
  000f34be: ldr r0,[0x000f35f4]
  000f34c0: str r4,[r6,#0x0]
  000f34c2: add r0,pc
  000f34c4: ldr r5,[r0,#0x0]
  000f34c6: ldr r0,[r5,#0x0]
  000f34c8: cbz r0,0x000f34d2
  000f34ca: blx 0x00075cd0
  000f34ce: blx 0x0006eb48
  000f34d2: ldr r0,[0x000f35f8]
  000f34d4: movs r6,#0x0
  000f34d6: str r6,[r5,#0x0]
  000f34d8: add r0,pc
  000f34da: ldr r4,[r0,#0x0]
  000f34dc: ldr r0,[r4,#0x0]
  000f34de: cbz r0,0x000f34e8
  000f34e0: blx 0x000740d4
  000f34e4: blx 0x0006eb48
  000f34e8: ldr.w r0,[r11,#0x0]
  000f34ec: str r6,[r4,#0x0]
  000f34ee: cbz r0,0x000f34f8
  000f34f0: blx 0x00071794
  000f34f4: blx 0x0006eb48
  000f34f8: ldr r0,[0x000f35fc]
  000f34fa: movs r5,#0x0
  000f34fc: str.w r5,[r11,#0x0]
  000f3500: add r0,pc
  000f3502: ldr r4,[r0,#0x0]
  000f3504: ldr r0,[r4,#0x0]
  000f3506: cbz r0,0x000f350e
  000f3508: ldr r1,[r0,#0x0]
  000f350a: ldr r1,[r1,#0x4]
  000f350c: blx r1
  000f350e: ldr r0,[0x000f3600]
  000f3510: str r5,[r4,#0x0]
  000f3512: add r0,pc
  000f3514: ldr r6,[r0,#0x0]
  000f3516: ldr r0,[r6,#0x0]
  000f3518: cbz r0,0x000f3522
  000f351a: blx 0x00075cdc
  000f351e: blx 0x0006eb48
  000f3522: ldr r0,[0x000f3604]
  000f3524: movs r5,#0x0
  000f3526: str r5,[r6,#0x0]
  000f3528: add r0,pc
  000f352a: ldr r4,[r0,#0x0]
  000f352c: ldr r0,[r4,#0x0]
  000f352e: cbz r0,0x000f3540
  000f3530: blx 0x00073420
  000f3534: ldr r0,[r4,#0x0]
  000f3536: cbz r0,0x000f3540
  000f3538: blx 0x0007342c
  000f353c: blx 0x0006eb48
  000f3540: ldr r0,[0x000f3608]
  000f3542: str r5,[r4,#0x0]
  000f3544: add r0,pc
  000f3546: ldr r6,[r0,#0x0]
  000f3548: ldr r0,[r6,#0x0]
  000f354a: cbz r0,0x000f355c
  000f354c: blx 0x00073408
  000f3550: ldr r0,[r6,#0x0]
  000f3552: cbz r0,0x000f355c
  000f3554: blx 0x00073414
  000f3558: blx 0x0006eb48
  000f355c: ldr.w r0,[r10,#0x0]
  000f3560: movs r4,#0x0
  000f3562: str r4,[r6,#0x0]
  000f3564: cbz r0,0x000f356e
  000f3566: blx 0x00075ca0
  000f356a: blx 0x0006eb48
  000f356e: ldr r0,[0x000f360c]
  000f3570: str.w r4,[r10,#0x0]
  000f3574: add r0,pc
  000f3576: ldr r5,[r0,#0x0]
  000f3578: ldr r0,[r5,#0x0]
  000f357a: cbz r0,0x000f3584
  000f357c: blx 0x00075ce8
  000f3580: blx 0x0006eb48
  000f3584: ldr.w r0,[r9,#0x0]
  000f3588: movs r4,#0x0
  000f358a: str r4,[r5,#0x0]
  000f358c: cbz r0,0x000f3596
  000f358e: blx 0x00075cac
  000f3592: blx 0x0006eb48
  000f3596: ldr r0,[0x000f3610]
  000f3598: str.w r4,[r9,#0x0]
  000f359c: add r0,pc
  000f359e: ldr r5,[r0,#0x0]
  000f35a0: ldr r0,[r5,#0x0]
  000f35a2: cbz r0,0x000f35ac
  000f35a4: blx 0x00075cf4
  000f35a8: blx 0x0006eb48
  000f35ac: movs r4,#0x0
  000f35ae: str r4,[r5,#0x0]
  000f35b0: ldr.w r0,[r8,#0x4]
  000f35b4: cbz r0,0x000f35c8
  000f35b6: blx 0x0007369c
  000f35ba: ldr.w r0,[r8,#0x4]
  000f35be: cbz r0,0x000f35c8
  000f35c0: blx 0x00070204
  000f35c4: blx 0x0006eb48
  000f35c8: ldr r0,[0x000f3614]
  000f35ca: str.w r4,[r8,#0x4]
  000f35ce: add r0,pc
  000f35d0: ldr r0,[r0,#0x0]
  000f35d2: str r4,[r0,#0x0]
  000f35d4: mov r0,r8
  000f35d6: pop.w {r3,r8,r9,r10,r11}
  000f35da: pop {r4,r5,r6,r7,pc}
```
