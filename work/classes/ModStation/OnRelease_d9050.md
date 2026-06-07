# ModStation::OnRelease
elf 0xd9050 body 470
Sig: undefined __thiscall OnRelease(ModStation * this)

## decompile
```c

/* ModStation::OnRelease() */

void __thiscall ModStation::OnRelease(ModStation *this)

{
  void *pvVar1;
  int *piVar2;
  Radio *this_00;
  ScrollTouchBox *this_01;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000e9228 + 0xe9060);
  if (*piVar4 != 0) {
    FModSound::disableReverb();
    FModSound::stopAllSoundFXEvents();
  }
  AbyssEngine::PaintCanvas::FogEnable(**(undefined4 **)(DAT_000e922c + 0xe9080),0,1);
  if ((Array *)this->field_8C != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>((Array *)this->field_8C);
    if ((Array<TouchButton*> *)this->field_8C != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array((Array<TouchButton*> *)this->field_8C);
      operator_delete(pvVar1);
    }
  }
  this->field_8C = (int *)0x0;
  if ((HangarWindow *)this->field_78 != (HangarWindow *)0x0) {
    pvVar1 = (void *)HangarWindow::~HangarWindow((HangarWindow *)this->field_78);
    operator_delete(pvVar1);
  }
  this->field_78 = 0;
  if ((StarMap *)this->field_10 != (StarMap *)0x0) {
    pvVar1 = (void *)StarMap::~StarMap((StarMap *)this->field_10);
    operator_delete(pvVar1);
  }
  this->field_10 = 0;
  if ((SpaceLounge *)this->field_74 != (SpaceLounge *)0x0) {
    pvVar1 = (void *)SpaceLounge::~SpaceLounge((SpaceLounge *)this->field_74);
    operator_delete(pvVar1);
  }
  this->field_74 = 0;
  if (this->m_pDialogueWindow != (DialogueWindow *)0x0) {
    pvVar1 = (void *)MissionsWindow::~MissionsWindow((MissionsWindow *)this->m_pDialogueWindow);
    operator_delete(pvVar1);
  }
  this->m_pDialogueWindow = (DialogueWindow *)0x0;
  if ((DialogueWindow *)this->field_84 != (DialogueWindow *)0x0) {
    pvVar1 = (void *)DialogueWindow::~DialogueWindow((DialogueWindow *)this->field_84);
    operator_delete(pvVar1);
  }
  this->field_84 = 0;
  if ((ChoiceWindow *)this->field_88 != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow((ChoiceWindow *)this->field_88);
    operator_delete(pvVar1);
  }
  this->field_88 = (int *)0x0;
  if ((StatusWindow *)this->field_7C != (StatusWindow *)0x0) {
    pvVar1 = (void *)StatusWindow::~StatusWindow((StatusWindow *)this->field_7C);
    operator_delete(pvVar1);
  }
  this->field_7C = 0;
  if ((CutScene *)this->field_14 != (CutScene *)0x0) {
    pvVar1 = (void *)CutScene::~CutScene((CutScene *)this->field_14);
    operator_delete(pvVar1);
  }
  this->field_14 = (int *)0x0;
  if ((MenuTouchWindow *)this->field_50 != (MenuTouchWindow *)0x0) {
    pvVar1 = (void *)MenuTouchWindow::~MenuTouchWindow((MenuTouchWindow *)this->field_50);
    operator_delete(pvVar1);
  }
  this->field_50 = 0;
  if ((Radio *)this->field_54 != (Radio *)0x0) {
    pvVar1 = (void *)Radio::~Radio((Radio *)this->field_54);
    operator_delete(pvVar1);
  }
  this->field_54 = 0;
  if ((Array<RadioMessage*> *)this->field_58 != (Array<RadioMessage*> *)0x0) {
    pvVar1 = (void *)Array<RadioMessage*>::~Array((Array<RadioMessage*> *)this->field_58);
    operator_delete(pvVar1);
  }
  this->field_58 = 0;
  if ((NewsTicker *)this->field_1C != (NewsTicker *)0x0) {
    pvVar1 = (void *)NewsTicker::~NewsTicker((NewsTicker *)this->field_1C);
    operator_delete(pvVar1);
  }
  this->field_1C = (int *)0x0;
  if ((ChoiceWindow *)this->field_70 != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow((ChoiceWindow *)this->field_70);
    operator_delete(pvVar1);
  }
  this->field_70 = 0;
  AbyssEngine::PaintCanvas::ReleaseAllResources();
  iVar3 = **(int **)(DAT_000e9230 + 0xe917e);
  GameText::getLanguage();
  Globals::loadFont(iVar3);
  if (**(int **)(DAT_000e9234 + 0xe9192) != 0) {
    Layout::reload();
    ImageFactory::reload();
    Layout::initTip();
  }
  if ((EaseInOutMatrix *)this->field_20 != (EaseInOutMatrix *)0x0) {
    pvVar1 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix
                               ((EaseInOutMatrix *)this->field_20);
    operator_delete(pvVar1);
  }
  piVar2 = this[2].field_1C;
  this->field_20 = 0;
  if (piVar2 != (int *)0x0) {
    operator_delete(piVar2);
  }
  this[2].field_1C = (int *)0x0;
  if ((void *)this[2].field_20 != (void *)0x0) {
    operator_delete((void *)this[2].field_20);
  }
  this[2].field_20 = 0;
  if ((void *)this[2].field_24 != (void *)0x0) {
    operator_delete((void *)this[2].field_24);
  }
  this_00 = (Radio *)this->field_54;
  this[2].field_24 = 0;
  if (this_00 != (Radio *)0x0) {
    pvVar1 = (void *)Radio::~Radio(this_00);
    operator_delete(pvVar1);
  }
  this_01 = (ScrollTouchBox *)this[1].field_8;
  this->field_54 = 0;
  if (this_01 != (ScrollTouchBox *)0x0) {
    pvVar1 = (void *)ScrollTouchBox::~ScrollTouchBox(this_01);
    operator_delete(pvVar1);
  }
  this[1].field_8 = (int *)0x0;
  *(undefined2 *)&this[1].field_20 = 0;
  if (*piVar4 == 0) {
    return;
  }
  (*(code *)(DAT_001ac174 + 0x1ac178))();
  return;
}

```

## target disasm
```
  000e9050: push {r4,r5,r6,r7,lr}
  000e9052: add r7,sp,#0xc
  000e9054: push.w r8
  000e9058: mov r4,r0
  000e905a: ldr r0,[0x000e9228]
  000e905c: add r0,pc
  000e905e: ldr.w r8,[r0,#0x0]
  000e9062: ldr.w r0,[r8,#0x0]
  000e9066: cbz r0,0x000e9074
  000e9068: blx 0x00075310
  000e906c: ldr.w r0,[r8,#0x0]
  000e9070: blx 0x0007531c
  000e9074: ldr r0,[0x000e922c]
  000e9076: movs r1,#0x0
  000e9078: movs r2,#0x1
  000e907a: movs r6,#0x0
  000e907c: add r0,pc
  000e907e: ldr r5,[r0,#0x0]
  000e9080: ldr r0,[r5,#0x0]
  000e9082: blx 0x00071fe0
  000e9086: ldr.w r0,[r4,#0x8c]
  000e908a: cbz r0,0x000e909e
  000e908c: blx 0x00075328
  000e9090: ldr.w r0,[r4,#0x8c]
  000e9094: cbz r0,0x000e909e
  000e9096: blx 0x00075334
  000e909a: blx 0x0006eb48
  000e909e: ldr r0,[r4,#0x78]
  000e90a0: str.w r6,[r4,#0x8c]
  000e90a4: cbz r0,0x000e90ae
  000e90a6: blx 0x00075214
  000e90aa: blx 0x0006eb48
  000e90ae: ldr r0,[r4,#0x10]
  000e90b0: movs r6,#0x0
  000e90b2: str r6,[r4,#0x78]
  000e90b4: cbz r0,0x000e90be
  000e90b6: blx 0x00075340
  000e90ba: blx 0x0006eb48
  000e90be: ldr r0,[r4,#0x74]
  000e90c0: str r6,[r4,#0x10]
  000e90c2: cbz r0,0x000e90cc
  000e90c4: blx 0x0007534c
  000e90c8: blx 0x0006eb48
  000e90cc: ldr.w r0,[r4,#0x80]
  000e90d0: movs r6,#0x0
  000e90d2: str r6,[r4,#0x74]
  000e90d4: cbz r0,0x000e90de
  000e90d6: blx 0x00075358
  000e90da: blx 0x0006eb48
  000e90de: ldr.w r0,[r4,#0x84]
  000e90e2: str.w r6,[r4,#0x80]
  000e90e6: cbz r0,0x000e90f0
  000e90e8: blx 0x00075010
  000e90ec: blx 0x0006eb48
  000e90f0: ldr.w r0,[r4,#0x88]
  000e90f4: movs r6,#0x0
  000e90f6: str.w r6,[r4,#0x84]
  000e90fa: cbz r0,0x000e9104
  000e90fc: blx 0x00074d88
  000e9100: blx 0x0006eb48
  000e9104: ldr r0,[r4,#0x7c]
  000e9106: str.w r6,[r4,#0x88]
  000e910a: cbz r0,0x000e9114
  000e910c: blx 0x00075364
  000e9110: blx 0x0006eb48
  000e9114: ldr r0,[r4,#0x14]
  000e9116: movs r6,#0x0
  000e9118: str r6,[r4,#0x7c]
  000e911a: cbz r0,0x000e9124
  000e911c: blx 0x00075370
  000e9120: blx 0x0006eb48
  000e9124: ldr r0,[r4,#0x50]
  000e9126: str r6,[r4,#0x14]
  000e9128: cbz r0,0x000e9132
  000e912a: blx 0x0007537c
  000e912e: blx 0x0006eb48
  000e9132: ldr r0,[r4,#0x54]
  000e9134: movs r6,#0x0
  000e9136: str r6,[r4,#0x50]
  000e9138: cbz r0,0x000e9142
  000e913a: blx 0x00075388
  000e913e: blx 0x0006eb48
  000e9142: ldr r0,[r4,#0x58]
  000e9144: str r6,[r4,#0x54]
  000e9146: cbz r0,0x000e9150
  000e9148: blx 0x00073bac
  000e914c: blx 0x0006eb48
  000e9150: ldr r0,[r4,#0x1c]
  000e9152: movs r6,#0x0
  000e9154: str r6,[r4,#0x58]
  000e9156: cbz r0,0x000e9160
  000e9158: blx 0x00075394
  000e915c: blx 0x0006eb48
  000e9160: ldr r0,[r4,#0x70]
  000e9162: str r6,[r4,#0x1c]
  000e9164: cbz r0,0x000e916e
  000e9166: blx 0x00074d88
  000e916a: blx 0x0006eb48
  000e916e: movs r0,#0x0
  000e9170: str r0,[r4,#0x70]
  000e9172: ldr r0,[r5,#0x0]
  000e9174: blx 0x0006f964
  000e9178: ldr r0,[0x000e9230]
  000e917a: add r0,pc
  000e917c: ldr r0,[r0,#0x0]
  000e917e: ldr r5,[r0,#0x0]
  000e9180: blx 0x0006f544
  000e9184: mov r1,r0
  000e9186: mov r0,r5
  000e9188: blx 0x00071d04
  000e918c: ldr r0,[0x000e9234]
  000e918e: add r0,pc
  000e9190: ldr r5,[r0,#0x0]
  000e9192: ldr r0,[r5,#0x0]
  000e9194: cbz r0,0x000e91ac
  000e9196: blx 0x00071d10
  000e919a: ldr r0,[0x000e9238]
  000e919c: add r0,pc
  000e919e: ldr r0,[r0,#0x0]
  000e91a0: ldr r0,[r0,#0x0]
  000e91a2: blx 0x00071d1c
  000e91a6: ldr r0,[r5,#0x0]
  000e91a8: blx 0x00071d28
  000e91ac: ldr r0,[r4,#0x20]
  000e91ae: cbz r0,0x000e91b8
  000e91b0: blx 0x00072340
  000e91b4: blx 0x0006eb48
  000e91b8: ldr.w r0,[r4,#0x13c]
  000e91bc: movs r5,#0x0
  000e91be: str r5,[r4,#0x20]
  000e91c0: cbz r0,0x000e91c6
  000e91c2: blx 0x0006eb48
  000e91c6: ldr.w r0,[r4,#0x140]
  000e91ca: str.w r5,[r4,#0x13c]
  000e91ce: cbz r0,0x000e91d4
  000e91d0: blx 0x0006eb48
  000e91d4: ldr.w r0,[r4,#0x144]
  000e91d8: movs r5,#0x0
  000e91da: str.w r5,[r4,#0x140]
  000e91de: cbz r0,0x000e91e4
  000e91e0: blx 0x0006eb48
  000e91e4: ldr r0,[r4,#0x54]
  000e91e6: str.w r5,[r4,#0x144]
  000e91ea: cbz r0,0x000e91f4
  000e91ec: blx 0x00075388
  000e91f0: blx 0x0006eb48
  000e91f4: ldr.w r0,[r4,#0x98]
  000e91f8: movs r5,#0x0
  000e91fa: str r5,[r4,#0x54]
  000e91fc: cbz r0,0x000e9206
  000e91fe: blx 0x000753a0
  000e9202: blx 0x0006eb48
  000e9206: str.w r5,[r4,#0x98]
  000e920a: strh.w r5,[r4,#0xb0]
  000e920e: ldr.w r0,[r8,#0x0]
  000e9212: cbz r0,0x000e9220
  000e9214: pop.w r8
  000e9218: pop.w {r4,r5,r6,r7,lr}
  000e921c: b.w 0x001ac168
  000e9220: pop.w r8
  000e9224: pop {r4,r5,r6,r7,pc}
```
