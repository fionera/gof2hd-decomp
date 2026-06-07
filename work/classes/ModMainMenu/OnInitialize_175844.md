# ModMainMenu::OnInitialize
elf 0x175844 body 498
Sig: undefined __stdcall OnInitialize(void)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* ModMainMenu::OnInitialize() */

void ModMainMenu::OnInitialize(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  GameRecord *this;
  void *pvVar3;
  RecordHandler *this_00;
  MenuTouchWindow *this_01;
  undefined4 uVar4;
  CutScene *this_02;
  undefined4 in_r1;
  undefined4 extraout_r1;
  uint *puVar5;
  undefined4 *puVar6;
  int *piVar7;
  code *pcVar8;
  Station *pSVar9;
  int *piVar10;
  
  piVar10 = *(int **)(_LAB_00185a48 + 0x185854);
  iVar1 = *piVar10;
  if (*(int *)(in_r0 + 0x1c) == 0) {
    puVar6 = *(undefined4 **)(_LAB_00185a4c + 0x1858f6);
    Globals::startNewSoundResourceList();
    pcVar8 = *(code **)(_LAB_00185a50 + 0x185904);
    (*pcVar8)(*puVar6,0x7e);
    (*pcVar8)(*puVar6,0x15);
    (*pcVar8)(*puVar6,0x12);
    (*pcVar8)(*puVar6,0x13);
    (*pcVar8)(*puVar6,0x14);
    puVar6 = *(undefined4 **)(_LAB_00185a54 + 0x185926);
    Status::resetGame();
    piVar7 = *(int **)(_LAB_00185a58 + 0x185936);
    AbyssEngine::AERandom::reset();
    pSVar9 = (Station *)*puVar6;
    iVar2 = **(int **)(_LAB_00185a5c + 0x185948);
    AbyssEngine::AERandom::nextInt(*piVar7);
    Galaxy::getStation(iVar2);
    Status::setStation(pSVar9);
    this_02 = operator_new(0xa0);
    CutScene::CutScene(this_02,2);
    *(CutScene **)(in_r0 + 0x1c) = this_02;
    CutScene::initialize();
    iVar2 = Status::inAlienOrbit();
    uVar4 = *(undefined4 *)(in_r0 + 4);
    if (iVar2 == 0) {
      Status::getSystem();
      iVar2 = SolarSystem::getTextureIndex();
      puVar5 = (uint *)(iVar2 + 0x2efeU & 0xffff);
    }
    else {
      puVar5 = (uint *)0x2f08;
    }
    AbyssEngine::PaintCanvas::TextureCreate((ushort)uVar4,puVar5,true);
    AbyssEngine::PaintCanvas::ChangeCubeTexture(*(uint *)(in_r0 + 4));
    uVar4 = 100;
    goto LAB_00185a22;
  }
  iVar2 = *(int *)(in_r0 + 0xc);
  if (iVar2 == 0x1e) {
    *(undefined4 *)(in_r0 + 0xc) = 1;
LAB_001859c6:
    piVar7 = *(int **)(_LAB_00185a74 + 0x1859cc);
    if (*piVar7 != -1) {
      Globals::playMusicAndFadeOutCurrent(**(int **)(_LAB_00185a78 + 0x1859d8));
    }
    *piVar7 = -1;
    *(undefined1 *)(in_r0 + 0x10) = 1;
    *(undefined4 *)(in_r0 + 0xc) = 100;
    uVar4 = 0;
  }
  else {
    if (iVar2 == 0x3c) {
      uVar4 = 0x1e;
    }
    else if (iVar2 == 0x50) {
      AbyssEngine::PaintCanvas::Image2DCreate
                ((PaintCanvas *)**(undefined4 **)(_LAB_00185a6c + 0x185a02),0x1b5a,
                 (uint *)(in_r0 + 0x20));
      iVar2 = _LAB_00185a70;
      *(undefined1 *)(in_r0 + 0x28) = 1;
      *(undefined4 *)(in_r0 + 0x24) = 0;
      **(undefined4 **)(iVar2 + 0x185a16) = 1;
      uVar4 = 0x3c;
    }
    else {
      if (iVar2 != 100) goto LAB_001859c6;
      iVar2 = *(int *)(_LAB_00185a60 + 0x185886);
      if (*(char *)(iVar2 + 0x48) == '\0') {
        piVar7 = *(int **)(_LAB_00185a64 + 0x185894);
        this = (GameRecord *)RecordHandler::recordStoreReadPreview(*piVar7);
        if (this != (GameRecord *)0x0) {
          *(undefined1 *)(in_r0 + 0x29) = 1;
          pvVar3 = (void *)GameRecord::~GameRecord(this);
          operator_delete(pvVar3);
        }
        this_00 = (RecordHandler *)*piVar7;
        *(undefined1 *)(iVar2 + 0x48) = 1;
        RecordHandler::saveOptions(this_00);
        in_r1 = extraout_r1;
      }
      Status::setPlayingTime(CONCAT44(in_r1,**(undefined4 **)(_LAB_00185a68 + 0x1858c0)));
      this_01 = operator_new(0x240);
      MenuTouchWindow::MenuTouchWindow(this_01,0);
      *(MenuTouchWindow **)(in_r0 + 0x18) = this_01;
      if (*(char *)(in_r0 + 0x29) != '\0') {
        MenuTouchWindow::showSupernovaMessage();
        *(undefined1 *)(in_r0 + 0x29) = 0;
      }
      uVar4 = 0x50;
    }
    *(undefined4 *)(in_r0 + 0xc) = uVar4;
  }
LAB_00185a22:
  iVar2 = *piVar10;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4,iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  00185844: push {r4,r5,r6,r7,lr}
  00185846: add r7,sp,#0xc
  00185848: push {r6,r7,r8,r9,r10}
  0018584c: mov r9,r0
  0018584e: ldr r0,[0x00185a48]
  00185850: add r0,pc
  00185852: ldr.w r8,[r0,#0x0]
  00185856: ldr.w r0,[r8,#0x0]
  0018585a: str r0,[sp,#0x4]
  0018585c: ldr.w r0,[r9,#0x1c]
  00185860: cmp r0,#0x0
  00185862: beq 0x001858f0
  00185864: ldr.w r0,[r9,#0xc]
  00185868: cmp r0,#0x1e
  0018586a: beq.w 0x001859c0
  0018586e: cmp r0,#0x3c
  00185870: beq.w 0x001859bc
  00185874: cmp r0,#0x50
  00185876: beq.w 0x001859f4
  0018587a: cmp r0,#0x64
  0018587c: bne.w 0x001859c6
  00185880: ldr r0,[0x00185a60]
  00185882: add r0,pc
  00185884: ldr r4,[r0,#0x0]
  00185886: ldrb.w r0,[r4,#0x48]
  0018588a: cbnz r0,0x001858b6
  0018588c: ldr r0,[0x00185a64]
  0018588e: movs r1,#0x0
  00185890: add r0,pc
  00185892: ldr r5,[r0,#0x0]
  00185894: ldr r0,[r5,#0x0]
  00185896: blx 0x000747f4
  0018589a: cbz r0,0x001858aa
  0018589c: movs r1,#0x1
  0018589e: strb.w r1,[r9,#0x29]
  001858a2: blx 0x000765ac
  001858a6: blx 0x0006eb48
  001858aa: ldr r0,[r5,#0x0]
  001858ac: movs r1,#0x1
  001858ae: strb.w r1,[r4,#0x48]
  001858b2: blx 0x00071788
  001858b6: ldr r0,[0x00185a68]
  001858b8: movs r2,#0x0
  001858ba: movs r3,#0x0
  001858bc: add r0,pc
  001858be: ldr r0,[r0,#0x0]
  001858c0: ldr r0,[r0,#0x0]
  001858c2: blx 0x00077518
  001858c6: mov.w r0,#0x240
  001858ca: blx 0x0006eb24
  001858ce: mov r5,r0
  001858d0: movs r1,#0x0
  001858d2: blx 0x00075268
  001858d6: ldrb.w r0,[r9,#0x29]
  001858da: str.w r5,[r9,#0x18]
  001858de: cbz r0,0x001858ec
  001858e0: mov r0,r5
  001858e2: blx 0x00077ecc
  001858e6: movs r0,#0x0
  001858e8: strb.w r0,[r9,#0x29]
  001858ec: movs r0,#0x50
  001858ee: b 0x00185a1e
  001858f0: ldr r0,[0x00185a4c]
  001858f2: add r0,pc
  001858f4: ldr r4,[r0,#0x0]
  001858f6: ldr r0,[r4,#0x0]
  001858f8: blx 0x00074f08
  001858fc: ldr r0,[0x00185a50]
  001858fe: movs r1,#0x7e
  00185900: add r0,pc
  00185902: ldr r5,[r0,#0x0]
  00185904: ldr r0,[r4,#0x0]
  00185906: blx r5
  00185908: ldr r0,[r4,#0x0]
  0018590a: movs r1,#0x15
  0018590c: blx r5
  0018590e: ldr r0,[r4,#0x0]
  00185910: movs r1,#0x12
  00185912: blx r5
  00185914: ldr r0,[r4,#0x0]
  00185916: movs r1,#0x13
  00185918: blx r5
  0018591a: ldr r0,[r4,#0x0]
  0018591c: movs r1,#0x14
  0018591e: blx r5
  00185920: ldr r0,[0x00185a54]
  00185922: add r0,pc
  00185924: ldr.w r10,[r0,#0x0]
  00185928: ldr.w r0,[r10,#0x0]
  0018592c: blx 0x00074d58
  00185930: ldr r0,[0x00185a58]
  00185932: add r0,pc
  00185934: ldr r4,[r0,#0x0]
  00185936: ldr r0,[r4,#0x0]
  00185938: blx 0x000718cc
  0018593c: ldr r0,[0x00185a5c]
  0018593e: movs r1,#0x64
  00185940: ldr.w r5,[r10,#0x0]
  00185944: add r0,pc
  00185946: ldr r0,[r0,#0x0]
  00185948: ldr r6,[r0,#0x0]
  0018594a: ldr r0,[r4,#0x0]
  0018594c: blx 0x00071848
  00185950: mov r1,r0
  00185952: mov r0,r6
  00185954: blx 0x00071c44
  00185958: mov r1,r0
  0018595a: mov r0,r5
  0018595c: blx 0x0007366c
  00185960: movs r0,#0xa0
  00185962: blx 0x0006eb24
  00185966: mov r5,r0
  00185968: movs r1,#0x2
  0018596a: blx 0x00074ef0
  0018596e: mov r0,r5
  00185970: str.w r5,[r9,#0x1c]
  00185974: blx 0x00074efc
  00185978: ldr.w r0,[r10,#0x0]
  0018597c: blx 0x000723d0
  00185980: ldr.w r5,[r9,#0x4]
  00185984: cbz r0,0x00185990
  00185986: mov r2,sp
  00185988: mov r0,r5
  0018598a: movw r1,#0x2f08
  0018598e: b 0x001859a8
  00185990: ldr.w r0,[r10,#0x0]
  00185994: blx 0x00071a10
  00185998: blx 0x0007390c
  0018599c: movw r1,#0x2efe
  001859a0: add r0,r1
  001859a2: mov r2,sp
  001859a4: uxth r1,r0
  001859a6: mov r0,r5
  001859a8: movs r3,#0x0
  001859aa: blx 0x0006fbb0
  001859ae: ldr r1,[sp,#0x0]
  001859b0: ldr.w r0,[r9,#0x4]
  001859b4: blx 0x00077ed8
  001859b8: movs r0,#0x64
  001859ba: b 0x00185a22
  001859bc: movs r0,#0x1e
  001859be: b 0x00185a1e
  001859c0: movs r0,#0x1
  001859c2: str.w r0,[r9,#0xc]
  001859c6: ldr r0,[0x00185a74]
  001859c8: add r0,pc
  001859ca: ldr r4,[r0,#0x0]
  001859cc: ldr r1,[r4,#0x0]
  001859ce: adds r0,r1,#0x1
  001859d0: beq 0x001859de
  001859d2: ldr r0,[0x00185a78]
  001859d4: add r0,pc
  001859d6: ldr r0,[r0,#0x0]
  001859d8: ldr r0,[r0,#0x0]
  001859da: blx 0x00074f80
  001859de: mov.w r0,#0xffffffff
  001859e2: str r0,[r4,#0x0]
  001859e4: movs r0,#0x1
  001859e6: strb.w r0,[r9,#0x10]
  001859ea: movs r0,#0x64
  001859ec: str.w r0,[r9,#0xc]
  001859f0: movs r0,#0x0
  001859f2: b 0x00185a22
  001859f4: ldr r0,[0x00185a6c]
  001859f6: add.w r2,r9,#0x20
  001859fa: movw r1,#0x1b5a
  001859fe: add r0,pc
  00185a00: ldr r0,[r0,#0x0]
  00185a02: ldr r0,[r0,#0x0]
  00185a04: blx 0x00071cf8
  00185a08: ldr r0,[0x00185a70]
  00185a0a: movs r1,#0x1
  00185a0c: movs r2,#0x0
  00185a0e: strb.w r1,[r9,#0x28]
  00185a12: add r0,pc
  00185a14: str.w r2,[r9,#0x24]
  00185a18: ldr r0,[r0,#0x0]
  00185a1a: str r1,[r0,#0x0]
  00185a1c: movs r0,#0x3c
  00185a1e: str.w r0,[r9,#0xc]
  00185a22: ldr r1,[sp,#0x4]
  00185a24: ldr.w r2,[r8,#0x0]
  00185a28: subs r1,r2,r1
  00185a2a: itt eq
  00185a2c: pop.eq.w {r2,r3,r8,r9,r10}
  00185a30: pop.eq {r4,r5,r6,r7,pc}
  00185a32: blx 0x0006e824
```
