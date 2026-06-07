# MGame::OnRelease
elf 0x17c6bc body 518
Sig: undefined __thiscall OnRelease(MGame * this)

## decompile
```c

/* MGame::OnRelease() */

void __thiscall MGame::OnRelease(MGame *this)

{
  Engine *this_00;
  int iVar1;
  void *pvVar2;
  AEGeometry *this_01;
  SingleLevel *this_02;
  TargetFollowCamera *this_03;
  PlayerEgo *this_04;
  uint *puVar3;
  int *piVar4;
  
  this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
  AbyssEngine::Engine::SetPostEffect(this_00,DAT_0018c8b8,false);
  piVar4 = *(int **)(DAT_0018c8bc + 0x18c6da);
  iVar1 = *piVar4;
  if (iVar1 != 0) {
    FModSound::setDownPitch(SUB41(iVar1,0));
    FModSound::disableReverb();
    FModSound::stopAllSoundFXEvents();
  }
  if ((Level *)this->field_78 != (Level *)0x0) {
    pvVar2 = (void *)Level::~Level((Level *)this->field_78);
    operator_delete(pvVar2);
  }
  this->field_48 = 0;
  this->field_4C = 0;
  this->field_14 = 0;
  this->field_78 = (int *)0x0;
  *(undefined1 *)&this->field_54 = 0;
  this->field_70 = (int *)0x0;
  this->field_40 = 0;
  *(undefined2 *)&this->field_60 = 0;
  this->m_pPlayerEgo = (PlayerEgo *)0x0;
  this->field_5C = 0;
  this_01 = (AEGeometry *)this[1].field_4C;
  this->field_30 = 0;
  this->field_34 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  this->field_38 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  this->field_3C = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this->field_20 = 0;
  this->field_24 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  this->field_28 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  this->field_2C = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  if (this_01 != (AEGeometry *)0x0) {
    pvVar2 = (void *)AEGeometry::~AEGeometry(this_01);
    operator_delete(pvVar2);
  }
  this_02 = this->m_pLevel;
  this[1].field_4C = 0;
  if (this_02 != (SingleLevel *)0x0) {
    pvVar2 = (void *)Hud::~Hud((Hud *)this_02);
    operator_delete(pvVar2);
  }
  this->m_pLevel = (SingleLevel *)0x0;
  if ((LevelScript *)this->field_7C != (LevelScript *)0x0) {
    pvVar2 = (void *)LevelScript::~LevelScript((LevelScript *)this->field_7C);
    operator_delete(pvVar2);
  }
  this->field_7C = (int *)0x0;
  if ((Radar *)this->field_80 != (Radar *)0x0) {
    pvVar2 = (void *)Radar::~Radar((Radar *)this->field_80);
    operator_delete(pvVar2);
  }
  this->field_80 = (int *)0x0;
  if ((Radio *)this->field_84 != (Radio *)0x0) {
    pvVar2 = (void *)Radio::~Radio((Radio *)this->field_84);
    operator_delete(pvVar2);
  }
  iVar1 = DAT_0018c8c0;
  this->field_84 = (int *)0x0;
  puVar3 = *(uint **)(iVar1 + 0x18c792);
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar3);
  if (*(int *)(iVar1 + 0x10) != 0) {
    iVar1 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar3);
    if (*(StarMap **)(iVar1 + 0x10) != (StarMap *)0x0) {
      pvVar2 = (void *)StarMap::~StarMap(*(StarMap **)(iVar1 + 0x10));
      operator_delete(pvVar2);
    }
  }
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar3);
  *(undefined4 *)(iVar1 + 0x10) = 0;
  if (this->m_pDialogueWindow != (DialogueWindow *)0x0) {
    pvVar2 = (void *)MenuTouchWindow::~MenuTouchWindow((MenuTouchWindow *)this->m_pDialogueWindow);
    operator_delete(pvVar2);
  }
  this->m_pDialogueWindow = (DialogueWindow *)0x0;
  if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
    pvVar2 = (void *)DialogueWindow::~DialogueWindow((DialogueWindow *)this->field_8C);
    operator_delete(pvVar2);
  }
  this->field_8C = 0;
  if ((StarMap *)this->field_90 != (StarMap *)0x0) {
    pvVar2 = (void *)StarMap::~StarMap((StarMap *)this->field_90);
    operator_delete(pvVar2);
  }
  this->field_90 = (int *)0x0;
  if ((ChoiceWindow *)this->field_94 != (ChoiceWindow *)0x0) {
    pvVar2 = (void *)ChoiceWindow::~ChoiceWindow((ChoiceWindow *)this->field_94);
    operator_delete(pvVar2);
  }
  this_03 = (TargetFollowCamera *)this[1].field_2C;
  this[1].field_10 = 0;
  this[1].field_14 = 0;
  this->field_94 = 0;
  *(undefined2 *)((int)&this[1].field_0C + 1) = 0;
  this[1].field_4C = 0;
  *(undefined4 *)&this->field_0xc5 = 0;
  *(undefined4 *)((int)&this[1].field_0 + 3) = 0;
  if (this_03 != (TargetFollowCamera *)0x0) {
    pvVar2 = (void *)TargetFollowCamera::~TargetFollowCamera(this_03);
    operator_delete(pvVar2);
  }
  this_04 = this[2].m_pPlayerEgo;
  this[1].field_2C = 0;
  if (this_04 != (PlayerEgo *)0x0) {
    pvVar2 = (void *)GameRecord::~GameRecord((GameRecord *)this_04);
    operator_delete(pvVar2);
  }
  this[2].m_pPlayerEgo = (PlayerEgo *)0x0;
  AbyssEngine::PaintCanvas::ReleaseAllResources();
  iVar1 = **(int **)(DAT_0018c8c8 + 0x18c856);
  GameText::getLanguage();
  Globals::loadFont(iVar1);
  if (**(int **)(DAT_0018c8cc + 0x18c86a) != 0) {
    Layout::reload();
    ImageFactory::reload();
    Layout::initTip();
  }
  ArrayReleaseClasses<AbyssEngine::String*>((Array *)this[2].field_5C);
  if ((Array<AbyssEngine::String*> *)this[2].field_5C != (Array<AbyssEngine::String*> *)0x0) {
    pvVar2 = (void *)Array<AbyssEngine::String*>::~Array
                               ((Array<AbyssEngine::String*> *)this[2].field_5C);
    operator_delete(pvVar2);
  }
  this[2].field_5C = 0;
  if (*piVar4 == 0) {
    return;
  }
  (*(code *)(DAT_001ac174 + 0x1ac178))();
  return;
}

```

## target disasm
```
  0018c6bc: push {r4,r5,r6,r7,lr}
  0018c6be: add r7,sp,#0xc
  0018c6c0: push.w r8
  0018c6c4: mov r4,r0
  0018c6c6: ldr r0,[r0,#0x8]
  0018c6c8: blx 0x0007258c
  0018c6cc: ldr r1,[0x0018c8b8]
  0018c6ce: movs r2,#0x0
  0018c6d0: blx 0x000707f8
  0018c6d4: ldr r0,[0x0018c8bc]
  0018c6d6: add r0,pc
  0018c6d8: ldr.w r8,[r0,#0x0]
  0018c6dc: ldr.w r0,[r8,#0x0]
  0018c6e0: cbz r0,0x0018c6f8
  0018c6e2: movs r1,#0x0
  0018c6e4: blx 0x00074f74
  0018c6e8: ldr.w r0,[r8,#0x0]
  0018c6ec: blx 0x00075310
  0018c6f0: ldr.w r0,[r8,#0x0]
  0018c6f4: blx 0x0007531c
  0018c6f8: ldr r0,[r4,#0x78]
  0018c6fa: cbz r0,0x0018c704
  0018c6fc: blx 0x00071fd4
  0018c700: blx 0x0006eb48
  0018c704: movs r5,#0x0
  0018c706: vmov.i32 q8,#0x0
  0018c70a: strd r5,r5,[r4,#0x48]
  0018c70e: add.w r1,r4,#0x30
  0018c712: str r5,[r4,#0x14]
  0018c714: str r5,[r4,#0x78]
  0018c716: strb.w r5,[r4,#0x54]
  0018c71a: str r5,[r4,#0x70]
  0018c71c: str r5,[r4,#0x40]
  0018c71e: strh.w r5,[r4,#0x60]
  0018c722: strd r5,r5,[r4,#0x58]
  0018c726: ldr.w r0,[r4,#0x114]
  0018c72a: vst1.64 {d16,d17},[r1]
  0018c72e: add.w r1,r4,#0x20
  0018c732: cmp r0,#0x0
  0018c734: vst1.64 {d16,d17},[r1]
  0018c738: beq 0x0018c742
  0018c73a: blx 0x00071fc8
  0018c73e: blx 0x0006eb48
  0018c742: ldr r0,[r4,#0x74]
  0018c744: str.w r5,[r4,#0x114]
  0018c748: cbz r0,0x0018c752
  0018c74a: blx 0x0007846c
  0018c74e: blx 0x0006eb48
  0018c752: ldr r0,[r4,#0x7c]
  0018c754: movs r5,#0x0
  0018c756: str r5,[r4,#0x74]
  0018c758: cbz r0,0x0018c762
  0018c75a: blx 0x00078478
  0018c75e: blx 0x0006eb48
  0018c762: ldr.w r0,[r4,#0x80]
  0018c766: str r5,[r4,#0x7c]
  0018c768: cbz r0,0x0018c772
  0018c76a: blx 0x00078484
  0018c76e: blx 0x0006eb48
  0018c772: ldr.w r0,[r4,#0x84]
  0018c776: movs r6,#0x0
  0018c778: str.w r6,[r4,#0x80]
  0018c77c: cbz r0,0x0018c786
  0018c77e: blx 0x00075388
  0018c782: blx 0x0006eb48
  0018c786: ldr r0,[0x0018c8c0]
  0018c788: movs r1,#0x5
  0018c78a: str.w r6,[r4,#0x84]
  0018c78e: add r0,pc
  0018c790: ldr r5,[r0,#0x0]
  0018c792: ldr r0,[r5,#0x0]
  0018c794: blx 0x00075bd4
  0018c798: ldr r0,[r0,#0x10]
  0018c79a: cbz r0,0x0018c7b0
  0018c79c: ldr r0,[r5,#0x0]
  0018c79e: movs r1,#0x5
  0018c7a0: blx 0x00075bd4
  0018c7a4: ldr r0,[r0,#0x10]
  0018c7a6: cbz r0,0x0018c7b0
  0018c7a8: blx 0x00075340
  0018c7ac: blx 0x0006eb48
  0018c7b0: ldr r0,[r5,#0x0]
  0018c7b2: movs r1,#0x5
  0018c7b4: blx 0x00075bd4
  0018c7b8: movs r5,#0x0
  0018c7ba: str r5,[r0,#0x10]
  0018c7bc: ldr.w r0,[r4,#0x88]
  0018c7c0: cbz r0,0x0018c7ca
  0018c7c2: blx 0x0007537c
  0018c7c6: blx 0x0006eb48
  0018c7ca: ldr.w r0,[r4,#0x8c]
  0018c7ce: str.w r5,[r4,#0x88]
  0018c7d2: cbz r0,0x0018c7dc
  0018c7d4: blx 0x00075010
  0018c7d8: blx 0x0006eb48
  0018c7dc: ldr.w r0,[r4,#0x90]
  0018c7e0: movs r5,#0x0
  0018c7e2: str.w r5,[r4,#0x8c]
  0018c7e6: cbz r0,0x0018c7f0
  0018c7e8: blx 0x00075340
  0018c7ec: blx 0x0006eb48
  0018c7f0: ldr.w r0,[r4,#0x94]
  0018c7f4: str.w r5,[r4,#0x90]
  0018c7f8: cbz r0,0x0018c802
  0018c7fa: blx 0x00074d88
  0018c7fe: blx 0x0006eb48
  0018c802: ldr.w r0,[r4,#0xf4]
  0018c806: movs r5,#0x0
  0018c808: strd r5,r5,[r4,#0xd8]
  0018c80c: cmp r0,#0x0
  0018c80e: str.w r5,[r4,#0x94]
  0018c812: strh.w r5,[r4,#0xd5]
  0018c816: str.w r5,[r4,#0x114]
  0018c81a: str.w r5,[r4,#0xc5]
  0018c81e: str.w r5,[r4,#0xcb]
  0018c822: beq 0x0018c82c
  0018c824: blx 0x00072064
  0018c828: blx 0x0006eb48
  0018c82c: ldr.w r0,[r4,#0x1e8]
  0018c830: str.w r5,[r4,#0xf4]
  0018c834: cbz r0,0x0018c83e
  0018c836: blx 0x000765ac
  0018c83a: blx 0x0006eb48
  0018c83e: ldr r0,[0x0018c8c4]
  0018c840: movs r1,#0x0
  0018c842: str.w r1,[r4,#0x1e8]
  0018c846: add r0,pc
  0018c848: ldr r0,[r0,#0x0]
  0018c84a: ldr r0,[r0,#0x0]
  0018c84c: blx 0x0006f964
  0018c850: ldr r0,[0x0018c8c8]
  0018c852: add r0,pc
  0018c854: ldr r0,[r0,#0x0]
  0018c856: ldr r5,[r0,#0x0]
  0018c858: blx 0x0006f544
  0018c85c: mov r1,r0
  0018c85e: mov r0,r5
  0018c860: blx 0x00071d04
  0018c864: ldr r0,[0x0018c8cc]
  0018c866: add r0,pc
  0018c868: ldr r5,[r0,#0x0]
  0018c86a: ldr r0,[r5,#0x0]
  0018c86c: cbz r0,0x0018c884
  0018c86e: blx 0x00071d10
  0018c872: ldr r0,[0x0018c8d0]
  0018c874: add r0,pc
  0018c876: ldr r0,[r0,#0x0]
  0018c878: ldr r0,[r0,#0x0]
  0018c87a: blx 0x00071d1c
  0018c87e: ldr r0,[r5,#0x0]
  0018c880: blx 0x00071d28
  0018c884: ldr.w r0,[r4,#0x1ec]
  0018c888: blx 0x0006facc
  0018c88c: ldr.w r0,[r4,#0x1ec]
  0018c890: cbz r0,0x0018c89a
  0018c892: blx 0x0006f64c
  0018c896: blx 0x0006eb48
  0018c89a: movs r0,#0x0
  0018c89c: str.w r0,[r4,#0x1ec]
  0018c8a0: ldr.w r0,[r8,#0x0]
  0018c8a4: cbz r0,0x0018c8b2
  0018c8a6: pop.w r8
  0018c8aa: pop.w {r4,r5,r6,r7,lr}
  0018c8ae: b.w 0x001ac168
  0018c8b2: pop.w r8
  0018c8b6: pop {r4,r5,r6,r7,pc}
  001ac168: bx pc
  001ac16c: ldr r12,[0x1ac174]
  001ac170: add pc,r12,pc
```
