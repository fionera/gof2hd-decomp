# BombGun::BombGun
elf 0x1475cc body 522
Sig: undefined __thiscall BombGun(BombGun * this, Gun * param_1, uint param_2, int param_3, int param_4, bool param_5, Level * param_6)

## decompile
```c

/* BombGun::BombGun(Gun*, unsigned int, int, int, bool, Level*) */

void __thiscall
BombGun::BombGun(BombGun *this,Gun *param_1,uint param_2,int param_3,int param_4,bool param_5,
                Level *param_6)

{
  int *piVar1;
  Explosion *this_00;
  undefined4 uVar2;
  AEGeometry *pAVar3;
  void *pvVar4;
  ushort uVar5;
  int iVar6;
  uint uVar7;
  uint *puVar8;
  int *piVar9;
  bool bVar10;
  float fVar11;
  undefined3 in_stack_00000005;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  
  piVar9 = *(int **)(DAT_00157810 + 0x1575e2);
  local_28 = *piVar9;
  RocketGun::RocketGun((RocketGun *)this,param_3,param_1,param_2,0,0,param_4,false,param_6);
  piVar1 = (int *)(DAT_00157814 + 0x157616);
  *(undefined4 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0x114) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x118) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x11c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(int *)this = *piVar1 + 8;
  *(undefined4 *)(this + 0xf8) = 0;
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x120) = 0;
  *(undefined4 *)(this + 0x124) = 0;
  this_00 = operator_new(0x68);
  if (param_4 == 6) {
    iVar6 = 7;
  }
  else if (param_4 == 0x2a) {
    iVar6 = 0xb;
  }
  else {
    iVar6 = 0;
    if (*(int *)(param_1 + 0x58) == 0xe8) {
      iVar6 = 0xd;
    }
  }
  Explosion::Explosion(this_00,iVar6);
  *(Explosion **)(this + 0xf0) = this_00;
  fVar11 = (float)Explosion::setWeaponIndex(this_00,*(int *)(param_1 + 0x58));
  this[0x24] = (BombGun)param_5;
  this[0x104] = (BombGun)0x1;
  *(int *)(this + 0x128) = param_4;
  *(undefined4 *)(this + 0xf4) = 0xffffffff;
  if (param_4 == 0x2a) {
    Explosion::setScaling(fVar11);
  }
  if (_param_5 == 0) {
    iVar6 = *(int *)(param_1 + 0x58);
    bVar10 = iVar6 != 0xe8;
    if (bVar10) {
      iVar6 = *(int *)(param_1 + 0x5c);
    }
    if (!bVar10 || iVar6 == 0x22) goto LAB_00157708;
    pAVar3 = operator_new(0xc0);
    uVar5 = 0x395d;
    puVar8 = *(uint **)(DAT_0015781c + 0x157778);
    if (param_2 == 0x395a) {
      uVar5 = 0x395b;
    }
    if (param_2 == 0x3958) {
      uVar5 = 0x3959;
    }
    AEGeometry::AEGeometry(pAVar3,uVar5,(PaintCanvas *)*puVar8,false);
    uVar7 = *(uint *)(pAVar3 + 0xc);
    *(uint *)(this + 0xf4) = uVar7;
    AbyssEngine::PaintCanvas::TransformAddChild((PaintCanvas *)*puVar8,*(uint *)(this + 0x10),uVar7)
    ;
    if (param_2 == 0x395c) {
      uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
      AbyssEngine::Transform::SetAnimationState(uVar2,2,0);
    }
    else {
      uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
      AbyssEngine::Transform::SetAnimationState(uVar2,1,0);
    }
  }
  else {
    puVar8 = *(uint **)(DAT_00157818 + 0x157696);
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar8,(uint *)(this + 0x14));
    AbyssEngine::PaintCanvas::TransformAddMesh
              (*puVar8,(ushort)*(undefined4 *)(this + 0x14),SUB21(*(undefined2 *)(this + 0x28),0));
    uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
    AbyssEngine::Transform::SetAnimationState(uVar2,1,0);
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,0x37d6,(PaintCanvas *)*puVar8,false);
    AbyssEngine::PaintCanvas::TransformAddChild
              ((PaintCanvas *)*puVar8,*(uint *)(this + 0x14),*(uint *)(pAVar3 + 0xc));
    AbyssEngine::PaintCanvas::TransformRemoveMesh(*puVar8,(ushort)*(undefined4 *)(this + 0x10));
    AbyssEngine::PaintCanvas::TransformAddChild
              ((PaintCanvas *)*puVar8,*(uint *)(this + 0x10),*(uint *)(this + 0x14));
  }
  pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar3);
  operator_delete(pvVar4);
LAB_00157708:
  local_30 = DAT_00157808;
  local_34 = 0;
  local_2c = DAT_00157804;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x110),(Vector *)&local_34);
  local_34 = 0;
  local_30 = 0;
  local_2c = DAT_0015780c;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x11c),(Vector *)&local_34);
  *(undefined4 *)(this + 0xec) = 0;
  pAVar3 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar3,(PaintCanvas *)**(undefined4 **)(DAT_00157820 + 0x157742));
  *(AEGeometry **)(this + 0xe8) = pAVar3;
  iVar6 = *piVar9 - local_28;
  if (iVar6 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar6);
}

```

## target disasm
```
  001575cc: push {r4,r5,r6,r7,lr}
  001575ce: add r7,sp,#0xc
  001575d0: push {r8,r9,r10,r11}
  001575d4: sub sp,#0x24
  001575d6: mov r4,r0
  001575d8: ldr r0,[0x00157810]
  001575da: mov r5,r1
  001575dc: mov r9,r2
  001575de: add r0,pc
  001575e0: ldr r1,[r7,#0x10]
  001575e2: ldr.w r11,[r7,#0x8]
  001575e6: movs r6,#0x0
  001575e8: ldr.w r10,[r0,#0x0]
  001575ec: mov r2,r5
  001575ee: ldr.w r0,[r10,#0x0]
  001575f2: str r0,[sp,#0x20]
  001575f4: mov r0,r4
  001575f6: strd r6,r6,[sp,#0x0]
  001575fa: strd r11,r6,[sp,#0x8]
  001575fe: str r1,[sp,#0x10]
  00157600: mov r1,r3
  00157602: mov r3,r9
  00157604: blx 0x00076dbc
  00157608: ldr r0,[0x00157814]
  0015760a: vmov.i32 q8,#0x0
  0015760e: add.w r8,r4,#0x110
  00157612: add r0,pc
  00157614: vst1.32 {d16,d17},[r8]
  00157618: ldr r0,[r0,#0x0]
  0015761a: adds r0,#0x8
  0015761c: str r0,[r4,#0x0]
  0015761e: strd r6,r6,[r4,#0xf8]
  00157622: str.w r6,[r4,#0x100]
  00157626: strd r6,r6,[r4,#0x120]
  0015762a: movs r0,#0x68
  0015762c: blx 0x0006eb24
  00157630: mov r6,r0
  00157632: cmp.w r11,#0x6
  00157636: beq 0x00157642
  00157638: cmp.w r11,#0x2a
  0015763c: bne 0x00157646
  0015763e: movs r1,#0xb
  00157640: b 0x00157650
  00157642: movs r1,#0x7
  00157644: b 0x00157650
  00157646: ldr r0,[r5,#0x58]
  00157648: movs r1,#0x0
  0015764a: cmp r0,#0xe8
  0015764c: it eq
  0015764e: mov.eq r1,#0xd
  00157650: mov r0,r6
  00157652: blx 0x000727f0
  00157656: str.w r6,[r4,#0xf0]
  0015765a: ldr r1,[r5,#0x58]
  0015765c: mov r0,r6
  0015765e: blx 0x00076dc8
  00157662: ldr r6,[r7,#0xc]
  00157664: movs r0,#0x1
  00157666: strb.w r6,[r4,#0x24]
  0015766a: cmp.w r11,#0x2a
  0015766e: strb.w r0,[r4,#0x104]
  00157672: mov.w r0,#0xffffffff
  00157676: str.w r11,[r4,#0x128]
  0015767a: str.w r0,[r4,#0xf4]
  0015767e: bne 0x0015768a
  00157680: ldr.w r0,[r4,#0xf0]
  00157684: ldr r1,[0x00157800]
  00157686: blx 0x000727fc
  0015768a: cbz r6,0x001576fc
  0015768c: ldr r0,[0x00157818]
  0015768e: add.w r9,r4,#0x14
  00157692: add r0,pc
  00157694: ldr r5,[r0,#0x0]
  00157696: ldr r0,[r5,#0x0]
  00157698: mov r1,r9
  0015769a: blx 0x000720ac
  0015769e: ldr r1,[r4,#0x14]
  001576a0: ldr r0,[r5,#0x0]
  001576a2: ldrh r2,[r4,#0x28]
  001576a4: movs r3,#0x0
  001576a6: blx 0x0007303c
  001576aa: ldr.w r1,[r9,#0x0]
  001576ae: ldr r0,[r5,#0x0]
  001576b0: blx 0x00072088
  001576b4: movs r1,#0x1
  001576b6: movs r2,#0x0
  001576b8: blx 0x0006fd18
  001576bc: movs r0,#0xc0
  001576be: blx 0x0006eb24
  001576c2: mov r6,r0
  001576c4: ldr r2,[r5,#0x0]
  001576c6: movw r1,#0x37d6
  001576ca: movs r3,#0x0
  001576cc: blx 0x0007207c
  001576d0: ldr.w r1,[r9,#0x0]
  001576d4: ldr r0,[r5,#0x0]
  001576d6: ldr r2,[r6,#0xc]
  001576d8: blx 0x000720d0
  001576dc: ldr r1,[r4,#0x10]
  001576de: ldr r0,[r5,#0x0]
  001576e0: ldrh r2,[r4,#0x28]
  001576e2: blx 0x00076dd4
  001576e6: ldrd r1,r2,[r4,#0x10]
  001576ea: ldr r0,[r5,#0x0]
  001576ec: blx 0x000720d0
  001576f0: mov r0,r6
  001576f2: blx 0x00071fc8
  001576f6: blx 0x0006eb48
  001576fa: b 0x00157708
  001576fc: ldr r0,[r5,#0x58]
  001576fe: cmp r0,#0xe8
  00157700: itt ne
  00157702: ldr.ne r0,[r5,#0x5c]
  00157704: cmp.ne r0,#0x22
  00157706: bne 0x00157766
  00157708: ldr r1,[0x00157808]
  0015770a: movs r5,#0x0
  0015770c: ldr r0,[0x00157804]
  0015770e: str r1,[sp,#0x18]
  00157710: str r5,[sp,#0x14]
  00157712: str r0,[sp,#0x1c]
  00157714: add r1,sp,#0x14
  00157716: mov r0,r8
  00157718: blx 0x0006eb3c
  0015771c: ldr r1,[0x0015780c]
  0015771e: add.w r0,r4,#0x11c
  00157722: strd r5,r5,[sp,#0x14]
  00157726: str r1,[sp,#0x1c]
  00157728: add r1,sp,#0x14
  0015772a: blx 0x0006eb3c
  0015772e: movs r0,#0x0
  00157730: str.w r0,[r4,#0xec]
  00157734: movs r0,#0xc0
  00157736: blx 0x0006eb24
  0015773a: mov r5,r0
  0015773c: ldr r0,[0x00157820]
  0015773e: add r0,pc
  00157740: ldr r0,[r0,#0x0]
  00157742: ldr r1,[r0,#0x0]
  00157744: mov r0,r5
  00157746: blx 0x000720f4
  0015774a: str.w r5,[r4,#0xe8]
  0015774e: ldr r0,[sp,#0x20]
  00157750: ldr.w r1,[r10,#0x0]
  00157754: subs r0,r1,r0
  00157756: itttt eq
  00157758: mov.eq r0,r4
  0015775a: add.eq sp,#0x24
  0015775c: pop.eq.w {r8,r9,r10,r11}
  00157760: pop.eq {r4,r5,r6,r7,pc}
  00157762: blx 0x0006e824
  00157766: movs r0,#0xc0
  00157768: blx 0x0006eb24
  0015776c: mov r5,r0
  0015776e: ldr r0,[0x0015781c]
  00157770: movw r1,#0x395d
  00157774: add r0,pc
  00157776: ldr r6,[r0,#0x0]
  00157778: movw r0,#0x395a
  0015777c: cmp r9,r0
  0015777e: it eq
  00157780: movw.eq r1,#0x395b
  00157784: movw r0,#0x3958
  00157788: cmp r9,r0
  0015778a: ldr r2,[r6,#0x0]
  0015778c: it eq
  0015778e: movw.eq r1,#0x3959
  00157792: mov r0,r5
  00157794: movs r3,#0x0
  00157796: blx 0x0007207c
  0015779a: ldr r2,[r5,#0xc]
  0015779c: ldr r1,[r4,#0x10]
  0015779e: str.w r2,[r4,#0xf4]
  001577a2: ldr r0,[r6,#0x0]
  001577a4: blx 0x000720d0
  001577a8: ldr r0,[r6,#0x0]
  001577aa: movw r1,#0x395c
  001577ae: cmp r9,r1
  001577b0: bne 0x001577c4
  001577b2: ldr.w r1,[r4,#0xf4]
  001577b6: blx 0x00072088
  001577ba: movs r1,#0x2
  001577bc: movs r2,#0x0
  001577be: blx 0x0006fd18
  001577c2: b 0x001577d2
  001577c4: ldr r1,[r4,#0x10]
  001577c6: blx 0x00072088
  001577ca: movs r1,#0x1
  001577cc: movs r2,#0x0
  001577ce: blx 0x0006fd18
  001577d2: mov r0,r5
  001577d4: b 0x001576f2
```
