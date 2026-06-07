# BeamGun::update
elf 0x1778a4 body 622
Sig: undefined __thiscall update(BeamGun * this, int param_1)

## decompile
```c

/* BeamGun::update(int) */

void __thiscall BeamGun::update(BeamGun *this,int param_1)

{
  undefined4 uVar1;
  int iVar2;
  Matrix *pMVar3;
  int *piVar4;
  uint *puVar5;
  Vector *pVVar6;
  code *pcVar7;
  code *pcVar8;
  int *piVar9;
  uint in_fpscr;
  float fVar10;
  float extraout_s1;
  float extraout_s2;
  longlong lVar11;
  Vector aVStack_88 [12];
  undefined4 local_7c;
  undefined4 uStack_78;
  undefined4 local_74;
  undefined8 local_40;
  float local_38;
  Vector aVStack_34 [12];
  int local_28;
  
  piVar9 = *(int **)(DAT_00187b0c + 0x1878b8);
  local_28 = *piVar9;
  Gun::update(*(int *)(this + 8));
  if (*(char *)(*(int *)(this + 8) + 0x4c) == '\0') {
    if (*piVar9 - local_28 == 0) {
      (*(code *)(DAT_001abde4 + 0x1abde8))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(this + 0x18),*piVar9 - local_28);
  }
  if (*(char *)(*(int *)(this + 8) + 0x4d) != '\0') {
    puVar5 = *(uint **)(DAT_00187b10 + 0x1878e0);
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar5);
    AbyssEngine::Transform::SetAnimationState(uVar1,3,0);
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar5);
    AbyssEngine::Transform::SetAnimationState(uVar1,1,0);
    *(undefined1 *)(*(int *)(this + 8) + 0x4d) = 0;
  }
  puVar5 = *(uint **)(DAT_00187b14 + 0x187912);
  lVar11 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar5);
  AbyssEngine::Transform::Update(lVar11,SUB41(param_1,0));
  fVar10 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 8) + 0x8c),
                                      (byte)(in_fpscr >> 0x16) & 3);
  AEGeometry::setScaling(fVar10,extraout_s1,extraout_s2);
  local_7c = 0;
  uStack_78 = 0x3f800000;
  local_74 = 0;
  AEGeometry::setDirection(*(Vector **)(this + 0x18),(Vector *)(*(int *)(this + 8) + 0x90));
  Level::getPlayer();
  PlayerEgo::getPosition();
  local_7c = 0;
  uStack_78 = 0;
  local_74 = 0;
  iVar2 = AbyssEngine::AEMath::operator!=((Vector *)(*(int *)(this + 8) + 0x7c),(Vector *)&local_7c)
  ;
  if (iVar2 != 0) {
    Level::getPlayer();
    pMVar3 = (Matrix *)AEGeometry::getMatrix();
    Level::getPlayer();
    AEGeometry::getMatrix();
    AbyssEngine::AEMath::operator*((Matrix *)&local_7c,pMVar3);
    AbyssEngine::AEMath::operator+(aVStack_88,(Vector *)(*(int *)(this + 8) + 0x7c));
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_40,(Vector *)&local_7c);
    AbyssEngine::AEMath::Vector::operator+=(aVStack_34,(Vector *)&local_40);
  }
  AEGeometry::setPosition(*(AEGeometry **)(this + 0x18),aVStack_34);
  AEGeometry::setVisible(SUB41(*(undefined4 *)(this + 0x18),0));
  if (this[0x20] != (BeamGun)0x0) {
    if (*(char *)(*(int *)(this + 8) + 0xa9) == '\0') {
      pcVar7 = *(code **)(DAT_00187b18 + 0x187ab4);
      uVar1 = (*pcVar7)(*puVar5,*(undefined4 *)(*(int *)(this + 0x1c) + 0xc));
      pcVar8 = *(code **)(DAT_00187b1c + 0x187ac4);
      (*pcVar8)(uVar1,0,0);
      uVar1 = (*pcVar7)(*puVar5,*(undefined4 *)(*(int *)(this + 0x1c) + 0xc));
      (*pcVar8)(uVar1,3,0);
      uVar1 = (*pcVar7)(*puVar5,*(undefined4 *)(*(int *)(this + 0x1c) + 0xc));
      (*pcVar8)(uVar1,1,0);
    }
    else {
      piVar4 = (int *)Level::getPlayer();
      PlayerEgo::getPosition();
      local_40 = *(undefined8 *)(*(int *)(this + 8) + 0x7c);
      local_38 = *(float *)(*(int *)(this + 8) + 0x84) + DAT_00187b08;
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)aVStack_88,(Vector *)(*piVar4 + 4));
      AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_7c,aVStack_88);
      AEGeometry::setPosition(*(AEGeometry **)(this + 0x1c),(Vector *)&local_7c);
      lVar11 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar5);
      AbyssEngine::Transform::Update(lVar11,SUB41(param_1,0));
      pVVar6 = *(Vector **)(this + 0x1c);
      AbyssEngine::AEMath::MatrixGetDir((Matrix *)aVStack_88);
      AEGeometry::setDirection(pVVar6,aVStack_88);
    }
  }
  this[0x21] = *(BeamGun *)(*(int *)(this + 8) + 0xa9);
  if (*piVar9 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001878a4: push {r4,r5,r6,r7,lr}
  001878a6: add r7,sp,#0xc
  001878a8: push {r8,r9,r10,r11}
  001878ac: sub sp,#0x7c
  001878ae: mov r4,r0
  001878b0: ldr r0,[0x00187b0c]
  001878b2: mov r10,r1
  001878b4: add r0,pc
  001878b6: ldr.w r11,[r0,#0x0]
  001878ba: ldr.w r0,[r11,#0x0]
  001878be: str r0,[sp,#0x78]
  001878c0: ldr r0,[r4,#0x8]
  001878c2: blx 0x0007780c
  001878c6: ldr r0,[r4,#0x8]
  001878c8: ldrb.w r1,[r0,#0x4c]
  001878cc: cmp r1,#0x0
  001878ce: beq.w 0x00187a8a
  001878d2: ldrb.w r0,[r0,#0x4d]
  001878d6: cbz r0,0x0018790a
  001878d8: ldr r0,[0x00187b10]
  001878da: ldr r1,[r4,#0x18]
  001878dc: add r0,pc
  001878de: ldr r5,[r0,#0x0]
  001878e0: ldr r1,[r1,#0xc]
  001878e2: ldr r0,[r5,#0x0]
  001878e4: blx 0x00072088
  001878e8: movs r1,#0x3
  001878ea: movs r2,#0x0
  001878ec: movs r6,#0x0
  001878ee: blx 0x0006fd18
  001878f2: ldr r1,[r4,#0x18]
  001878f4: ldr r0,[r5,#0x0]
  001878f6: ldr r1,[r1,#0xc]
  001878f8: blx 0x00072088
  001878fc: movs r1,#0x1
  001878fe: movs r2,#0x0
  00187900: blx 0x0006fd18
  00187904: ldr r0,[r4,#0x8]
  00187906: strb.w r6,[r0,#0x4d]
  0018790a: ldr r0,[0x00187b14]
  0018790c: ldr r1,[r4,#0x18]
  0018790e: add r0,pc
  00187910: ldr.w r8,[r0,#0x0]
  00187914: ldr r1,[r1,#0xc]
  00187916: ldr.w r0,[r8,#0x0]
  0018791a: blx 0x00072088
  0018791e: movs r5,#0x0
  00187920: asr.w r3,r10, asr #0x1f
  00187924: mov r2,r10
  00187926: str r5,[sp,#0x0]
  00187928: str r3,[sp,#0x8]
  0018792a: blx 0x0006f7cc
  0018792e: ldr r1,[r4,#0x8]
  00187930: mov.w r2,#0x3f800000
  00187934: ldr r0,[r4,#0x18]
  00187936: mov.w r6,#0x3f800000
  0018793a: vldr.32 s0,[r1,#0x8c]
  0018793e: mov.w r1,#0x3f800000
  00187942: vcvt.f32.s32 s0,s0
  00187946: vmov r3,s0
  0018794a: blx 0x000727b4
  0018794e: ldr r1,[r4,#0x8]
  00187950: add r2,sp,#0x24
  00187952: ldr r0,[r4,#0x18]
  00187954: adds r1,#0x90
  00187956: strd r5,r6,[sp,#0x24]
  0018795a: str r5,[sp,#0x2c]
  0018795c: blx 0x000726ac
  00187960: ldr r0,[r4,#0xc]
  00187962: blx 0x00072034
  00187966: mov r1,r0
  00187968: add r0,sp,#0x6c
  0018796a: blx 0x0007264c
  0018796e: ldr r0,[r4,#0x8]
  00187970: add r1,sp,#0x24
  00187972: strd r5,r5,[sp,#0x24]
  00187976: adds r0,#0x7c
  00187978: str r5,[sp,#0x2c]
  0018797a: blx 0x00072a90
  0018797e: cbz r0,0x001879d4
  00187980: ldr r0,[r4,#0xc]
  00187982: blx 0x00072034
  00187986: ldr r0,[r0,#0x8]
  00187988: blx 0x000721cc
  0018798c: mov r6,r0
  0018798e: ldr r0,[r4,#0xc]
  00187990: blx 0x00072034
  00187994: ldr r0,[r0,#0x4]
  00187996: blx 0x000721cc
  0018799a: add.w r9,sp,#0x24
  0018799e: mov r2,r0
  001879a0: mov r1,r6
  001879a2: mov r0,r9
  001879a4: blx 0x0006f82c
  001879a8: ldr r0,[r4,#0x8]
  001879aa: add r6,sp,#0x18
  001879ac: ldr r1,[0x00187b08]
  001879ae: add r2,sp,#0xc
  001879b0: strd r5,r5,[sp,#0xc]
  001879b4: str r1,[sp,#0x14]
  001879b6: add.w r1,r0,#0x7c
  001879ba: mov r0,r6
  001879bc: blx 0x0006f1cc
  001879c0: add r5,sp,#0x60
  001879c2: mov r1,r9
  001879c4: mov r2,r6
  001879c6: mov r0,r5
  001879c8: blx 0x0006f694
  001879cc: add r0,sp,#0x6c
  001879ce: mov r1,r5
  001879d0: blx 0x00073534
  001879d4: ldr r0,[r4,#0x18]
  001879d6: add r1,sp,#0x6c
  001879d8: blx 0x00072148
  001879dc: ldr r0,[r4,#0x18]
  001879de: movs r1,#0x1
  001879e0: blx 0x00072d24
  001879e4: ldrb.w r0,[r4,#0x20]
  001879e8: cmp r0,#0x0
  001879ea: beq 0x00187ae8
  001879ec: ldr r0,[r4,#0x8]
  001879ee: ldrb.w r0,[r0,#0xa9]
  001879f2: cmp r0,#0x0
  001879f4: beq 0x00187aac
  001879f6: ldr r0,[r4,#0xc]
  001879f8: blx 0x00072034
  001879fc: add.w r9,sp,#0x24
  00187a00: mov r6,r0
  00187a02: mov r1,r6
  00187a04: mov r0,r9
  00187a06: blx 0x0007264c
  00187a0a: ldr r0,[r4,#0x8]
  00187a0c: add r5,sp,#0x18
  00187a0e: vldr.32 s0,[pc,#0xf8]
  00187a12: add r2,sp,#0x60
  00187a14: ldr.w r1,[r0,#0x84]
  00187a18: str r1,[sp,#0x68]
  00187a1a: vldr.32 s2,[sp,#0x68]
  00187a1e: vldr.64 d16,[r0,#0x7c]
  00187a22: vadd.f32 s0,s2,s0
  00187a26: vstr.64 d16,[sp,#0x60]
  00187a2a: vstr.32 s0,[sp,#0x68]
  00187a2e: ldr r0,[r6,#0x0]
  00187a30: adds r1,r0,#0x4
  00187a32: mov r0,r5
  00187a34: blx 0x0006f694
  00187a38: mov r0,r9
  00187a3a: mov r1,r5
  00187a3c: blx 0x00073534
  00187a40: ldr r0,[r4,#0x1c]
  00187a42: mov r1,r9
  00187a44: blx 0x00072148
  00187a48: ldr r1,[r4,#0x1c]
  00187a4a: ldr.w r0,[r8,#0x0]
  00187a4e: ldr r1,[r1,#0xc]
  00187a50: blx 0x00072088
  00187a54: mov.w r8,#0x0
  00187a58: mov r2,r10
  00187a5a: str.w r8,[sp,#0x0]
  00187a5e: ldr r3,[sp,#0x8]
  00187a60: blx 0x0006f7cc
  00187a64: ldr r0,[r6,#0x0]
  00187a66: add r6,sp,#0x18
  00187a68: ldr r5,[r4,#0x1c]
  00187a6a: adds r1,r0,#0x4
  00187a6c: mov r0,r6
  00187a6e: blx 0x0006f4c0
  00187a72: mov.w r0,#0x3f800000
  00187a76: add r2,sp,#0xc
  00187a78: strd r8,r0,[sp,#0xc]
  00187a7c: mov r0,r5
  00187a7e: mov r1,r6
  00187a80: str.w r8,[sp,#0x14]
  00187a84: blx 0x000726ac
  00187a88: b 0x00187ae8
  00187a8a: ldr r0,[r4,#0x18]
  00187a8c: ldr r1,[sp,#0x78]
  00187a8e: ldr.w r2,[r11,#0x0]
  00187a92: subs r1,r2,r1
  00187a94: itttt eq
  00187a96: mov.eq r1,#0x0
  00187a98: add.eq sp,#0x7c
  00187a9a: pop.eq.w {r8,r9,r10,r11}
  00187a9e: pop.eq.w {r4,r5,r6,r7,lr}
  00187aa2: it eq
  00187aa4: b.eq.w 0x001abdd8
  00187aa8: blx 0x0006e824
  00187aac: ldr r0,[r4,#0x1c]
  00187aae: ldr r2,[0x00187b18]
  00187ab0: add r2,pc
  00187ab2: ldr r1,[r0,#0xc]
  00187ab4: ldr.w r0,[r8,#0x0]
  00187ab8: ldr r5,[r2,#0x0]
  00187aba: blx r5
  00187abc: ldr r1,[0x00187b1c]
  00187abe: movs r2,#0x0
  00187ac0: add r1,pc
  00187ac2: ldr r6,[r1,#0x0]
  00187ac4: movs r1,#0x0
  00187ac6: blx r6
  00187ac8: ldr r1,[r4,#0x1c]
  00187aca: ldr.w r0,[r8,#0x0]
  00187ace: ldr r1,[r1,#0xc]
  00187ad0: blx r5
  00187ad2: movs r1,#0x3
  00187ad4: movs r2,#0x0
  00187ad6: blx r6
  00187ad8: ldr r1,[r4,#0x1c]
  00187ada: ldr.w r0,[r8,#0x0]
  00187ade: ldr r1,[r1,#0xc]
  00187ae0: blx r5
  00187ae2: movs r1,#0x1
  00187ae4: movs r2,#0x0
  00187ae6: blx r6
  00187ae8: ldr r0,[r4,#0x8]
  00187aea: ldrb.w r0,[r0,#0xa9]
  00187aee: strb.w r0,[r4,#0x21]
  00187af2: ldr r0,[sp,#0x78]
  00187af4: ldr.w r1,[r11,#0x0]
  00187af8: subs r0,r1,r0
  00187afa: ittt eq
  00187afc: add.eq sp,#0x7c
  00187afe: pop.eq.w {r8,r9,r10,r11}
  00187b02: pop.eq {r4,r5,r6,r7,pc}
  00187b04: blx 0x0006e824
  001abdd8: bx pc
  001abddc: ldr r12,[0x1abde4]
  001abde0: add pc,r12,pc
```
