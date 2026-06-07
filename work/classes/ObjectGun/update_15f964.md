# ObjectGun::update
elf 0x15f964 body 992
Sig: undefined __thiscall update(ObjectGun * this, int param_1)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* ObjectGun::update(int) */

void __thiscall ObjectGun::update(ObjectGun *this,int param_1)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  AEGeometry *this_00;
  int *piVar4;
  Matrix *pMVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  float *pfVar8;
  code *pcVar9;
  code *pcVar10;
  int iVar11;
  Vector *pVVar12;
  uint *puVar13;
  float fVar14;
  float fVar15;
  Matrix aMStack_d8 [12];
  float local_cc;
  undefined4 local_c8;
  undefined4 local_c4;
  Matrix aMStack_c0 [64];
  undefined8 local_80;
  float local_78;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  Vector aVStack_34 [12];
  int local_28;
  
  piVar1 = *(int **)(DAT_0016fcb0 + 0x16f97a);
  puVar13 = *(uint **)(DAT_0016fcb4 + 0x16f97c);
  local_28 = *piVar1;
  uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar13);
  AbyssEngine::Transform::Update((ulonglong)uVar2,SUB41(param_1,0));
  *(int *)(this + 0x34) = param_1;
  Gun::update(*(int *)(this + 8));
  if (this[0x1c] == (ObjectGun)0x0) {
    iVar3 = Gun::isPlayerGun();
    if ((((iVar3 == 0) && (*(Player **)(*(int *)(this + 8) + 4) != (Player *)0x0)) &&
        (iVar3 = Player::getKIPlayer(*(Player **)(*(int *)(this + 8) + 4)), iVar3 != 0)) &&
       (iVar3 = Player::getKIPlayer(*(Player **)(*(int *)(this + 8) + 4)),
       *(char *)(iVar3 + 0x3f) != '\0')) {
      this[0x1c] = (ObjectGun)0x1;
      this_00 = operator_new(0xc0);
      AEGeometry::AEGeometry
                (this_00,*(ushort *)
                          (DAT_0016fcb8 + 0x16f9fc + *(int *)(*(int *)(this + 8) + 0x58) * 4),
                 (PaintCanvas *)*puVar13,false);
      *(AEGeometry **)(this + 0x18) = this_00;
    }
    if (this[0x1c] == (ObjectGun)0x0) goto LAB_0016fc8c;
  }
  if (*(char *)(*(int *)(this + 8) + 0xa9) == '\0') {
    pcVar9 = *(code **)(_LAB_0016fda0 + 0x16fa48);
    uVar6 = (*pcVar9)(*puVar13,*(undefined4 *)(*(int *)(this + 0x18) + 0xc));
    pcVar10 = *(code **)(_LAB_0016fda4 + 0x16fa58);
    (*pcVar10)(uVar6,0,0);
    uVar6 = (*pcVar9)(*puVar13,*(undefined4 *)(*(int *)(this + 0x18) + 0xc));
    (*pcVar10)(uVar6,3,0);
    uVar6 = (*pcVar9)(*puVar13,*(undefined4 *)(*(int *)(this + 0x18) + 0xc));
    (*pcVar10)(uVar6,1,0);
    goto LAB_0016fc8c;
  }
  piVar4 = (int *)Level::getPlayer();
  iVar3 = Gun::isPlayerGun();
  if (iVar3 == 0) {
    Player::getPosition();
  }
  else {
    PlayerEgo::getPosition();
  }
  iVar3 = Gun::isPlayerGun();
  iVar11 = *(int *)(this + 8);
  if (iVar3 == 0) {
    piVar4 = (int *)(iVar11 + 4);
  }
  __aeabi_memcpy((Vector *)&local_70,*piVar4 + 4,0x3c);
  local_80 = *(undefined8 *)(iVar11 + 0x7c);
  local_78 = *(float *)(iVar11 + 0x84) + _LAB_0016fd60;
  AbyssEngine::AEMath::MatrixRotateVector(aMStack_c0,(Vector *)&local_70);
  AbyssEngine::AEMath::Vector::operator+=(aVStack_34,(Vector *)aMStack_c0);
  AEGeometry::setPosition(*(AEGeometry **)(this + 0x18),aVStack_34);
  uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar13);
  AbyssEngine::Transform::Update((ulonglong)uVar2,SUB41(param_1,0));
  if (*(int *)(*(int *)(this + 8) + 0x5c) != 8) {
    uVar2 = *puVar13;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar6 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar2);
    __aeabi_memcpy(aMStack_c0,uVar6,0x3c);
    pVVar12 = *(Vector **)(this + 0x18);
    AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_cc);
    AbyssEngine::AEMath::MatrixGetUp(aMStack_d8);
    AEGeometry::setDirection(pVVar12,(Vector *)&local_cc);
    goto LAB_0016fc8c;
  }
  pMVar5 = (Matrix *)AEGeometry::getMatrix();
  AEGeometry::getMatrix();
  AbyssEngine::AEMath::operator*(aMStack_c0,pMVar5);
  iVar3 = *(int *)(this + 8);
  iVar11 = *(int *)(iVar3 + 0x58);
  local_c4 = _LAB_0016fd64;
  if (iVar11 != 0x2f) {
    uVar6 = _LAB_0016fd68;
    if (iVar11 == 0xb5) {
LAB_0016fbe0:
      local_c4 = uVar6;
    }
    else {
      local_c4 = _LAB_0016fd70;
      if (((iVar11 != 0x31) && (local_c4 = _LAB_0016fd6c, iVar11 != 0xb4)) &&
         (local_c4 = _LAB_0016fd74, iVar11 != 0x30)) {
        puVar7 = (undefined4 *)&LAB_0016fd78;
        if (iVar11 == 0xb6) {
          puVar7 = (undefined4 *)&LAB_0016fd7c;
        }
        uVar6 = _LAB_0016fd80;
        if ((iVar11 == 0xe0) || (uVar6 = _LAB_0016fd84, local_c4 = *puVar7, iVar11 == 0xb6))
        goto LAB_0016fbe0;
      }
    }
  }
  local_c8 = 0;
  local_cc = 0.0;
  if (*(char *)(iVar3 + 0xa4) != '\0') {
    if (iVar11 == 0xb5) {
      pfVar8 = (float *)&LAB_0016fd90;
      if (*(char *)(iVar3 + 0xa6) == '\0') {
        pfVar8 = (float *)&LAB_0016fd94;
      }
      local_cc = *pfVar8;
    }
    else {
      fVar14 = _LAB_0016fd88;
      fVar15 = _LAB_0016fd8c;
      if (iVar11 == 0x30) {
        fVar14 = 20.0;
        fVar15 = -20.0;
      }
      local_cc = fVar14 - *(float *)(iVar3 + 0x7c);
      if (*(char *)(iVar3 + 0xa6) != '\0') {
        local_cc = *(float *)(iVar3 + 0x7c) + fVar15;
      }
    }
  }
  if (*(char *)(iVar3 + 0xa5) != '\0') {
    if (iVar11 == 0xe0) {
      local_c8 = 0xc1500000;
      if (*(char *)(iVar3 + 0xa7) == '\0') {
        local_c8 = 0x41500000;
      }
    }
    else if (iVar11 == 0xb5) {
      puVar7 = (undefined4 *)&LAB_0016fd98;
      if (*(char *)(iVar3 + 0xa7) == '\0') {
        puVar7 = (undefined4 *)&LAB_0016fd9c;
      }
      local_c8 = *puVar7;
    }
  }
  AbyssEngine::AEMath::MatrixRotateVector(aMStack_d8,(Vector *)aMStack_c0);
  AEGeometry::setMatrix(*(Matrix **)(this + 0x18));
  AEGeometry::translate(*(Vector **)(this + 0x18));
LAB_0016fc8c:
  iVar3 = *(int *)(this + 8);
  this[0x1d] = *(ObjectGun *)(iVar3 + 0xa9);
  if (*(int *)(iVar3 + 0x5c) == 0x19) {
    iVar11 = 0;
    for (uVar2 = 0; uVar2 < *(uint *)(iVar3 + 8); uVar2 = uVar2 + 1) {
      if (*(char *)(*(int *)(iVar3 + 0x40) + uVar2) != '\0') {
        if (*(char *)(*(int *)(this + 0x30) + uVar2) != '\0') {
          local_70 = 0;
          uStack_6c = 0;
          local_68 = 0;
          Explosion::start(*(Vector **)(*(int *)(*(int *)(this + 0x2c) + 4) + uVar2 * 4),
                           (Vector *)(*(int *)(iVar3 + 0x30) + iVar11));
          *(undefined1 *)(*(int *)(this + 0x30) + uVar2) = 0;
        }
        Explosion::update(*(Explosion **)(*(int *)(*(int *)(this + 0x2c) + 4) + uVar2 * 4),param_1,
                          (TargetFollowCamera *)0x0);
        iVar3 = Explosion::isPlaying();
        if (iVar3 == 0) {
          iVar3 = *(int *)(this + 8);
          *(undefined1 *)(*(int *)(iVar3 + 0x40) + uVar2) = 0;
          *(undefined1 *)(iVar3 + 0x88) = 0;
          *(undefined1 *)(*(int *)(this + 0x30) + uVar2) = 1;
          Explosion::reset(*(Explosion **)(*(int *)(*(int *)(this + 0x2c) + 4) + uVar2 * 4));
        }
      }
      iVar11 = iVar11 + 0xc;
      iVar3 = *(int *)(this + 8);
    }
  }
  if (*piVar1 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0016f964: push {r4,r5,r6,r7,lr}
  0016f966: add r7,sp,#0xc
  0016f968: push {r8,r9,r10,r11}
  0016f96c: sub sp,#0xc4
  0016f96e: mov r11,r0
  0016f970: ldr r0,[0x0016fcb0]
  0016f972: mov r9,r1
  0016f974: ldr r1,[0x0016fcb4]
  0016f976: add r0,pc
  0016f978: add r1,pc
  0016f97a: ldr r0,[r0,#0x0]
  0016f97c: ldr.w r8,[r1,#0x0]
  0016f980: str r0,[sp,#0xc]
  0016f982: ldr r1,[r0,#0x0]
  0016f984: ldr.w r0,[r8,#0x0]
  0016f988: str r1,[sp,#0xc0]
  0016f98a: ldr.w r1,[r11,#0x10]
  0016f98e: blx 0x00072088
  0016f992: asr.w r4,r9, asr #0x1f
  0016f996: movs r1,#0x0
  0016f998: mov r2,r9
  0016f99a: mov r3,r4
  0016f99c: str r1,[sp,#0x0]
  0016f99e: blx 0x0006f7cc
  0016f9a2: ldr.w r0,[r11,#0x8]
  0016f9a6: mov r1,r9
  0016f9a8: str.w r9,[r11,#0x34]
  0016f9ac: blx 0x0007780c
  0016f9b0: ldrb.w r0,[r11,#0x1c]
  0016f9b4: cbnz r0,0x0016fa14
  0016f9b6: ldr.w r0,[r11,#0x8]
  0016f9ba: blx 0x0007798c
  0016f9be: cbnz r0,0x0016fa0a
  0016f9c0: ldr.w r0,[r11,#0x8]
  0016f9c4: ldr r0,[r0,#0x4]
  0016f9c6: cbz r0,0x0016fa0a
  0016f9c8: blx 0x00075850
  0016f9cc: cbz r0,0x0016fa0a
  0016f9ce: ldr.w r0,[r11,#0x8]
  0016f9d2: ldr r0,[r0,#0x4]
  0016f9d4: blx 0x00075850
  0016f9d8: ldrb.w r0,[r0,#0x3f]
  0016f9dc: cbz r0,0x0016fa0a
  0016f9de: movs r0,#0x1
  0016f9e0: strb.w r0,[r11,#0x1c]
  0016f9e4: movs r0,#0xc0
  0016f9e6: blx 0x0006eb24
  0016f9ea: mov r6,r0
  0016f9ec: ldr.w r0,[r11,#0x8]
  0016f9f0: ldr r1,[0x0016fcb8]
  0016f9f2: ldr.w r2,[r8,#0x0]
  0016f9f6: ldr r0,[r0,#0x58]
  0016f9f8: add r1,pc
  0016f9fa: ldrh.w r1,[r1,r0,lsl #0x2]
  0016f9fe: mov r0,r6
  0016fa00: movs r3,#0x0
  0016fa02: blx 0x0007207c
  0016fa06: str.w r6,[r11,#0x18]
  0016fa0a: ldrb.w r0,[r11,#0x1c]
  0016fa0e: cmp r0,#0x0
  0016fa10: beq.w 0x0016fc8c
  0016fa14: ldr.w r0,[r11,#0x8]
  0016fa18: ldrb.w r0,[r0,#0xa9]
  0016fa1c: cbz r0,0x0016fa3e
  0016fa1e: ldr.w r0,[r11,#0xc]
  0016fa22: str r4,[sp,#0x8]
  0016fa24: blx 0x00072034
  0016fa28: mov r4,r0
  0016fa2a: ldr.w r0,[r11,#0x8]
  0016fa2e: blx 0x0007798c
  0016fa32: cbz r0,0x0016fa82
  0016fa34: add r0,sp,#0xb4
  0016fa36: mov r1,r4
  0016fa38: blx 0x0007264c
  0016fa3c: b 0x0016fa8e
  0016fa3e: ldr.w r0,[r11,#0x18]
  0016fa42: ldr r2,[0x0016fda0]
  0016fa44: add r2,pc
  0016fa46: ldr r1,[r0,#0xc]
  0016fa48: ldr.w r0,[r8,#0x0]
  0016fa4c: ldr r4,[r2,#0x0]
  0016fa4e: blx r4
  0016fa50: ldr r1,[0x0016fda4]
  0016fa52: movs r2,#0x0
  0016fa54: add r1,pc
  0016fa56: ldr r5,[r1,#0x0]
  0016fa58: movs r1,#0x0
  0016fa5a: blx r5
  0016fa5c: ldr.w r1,[r11,#0x18]
  0016fa60: ldr.w r0,[r8,#0x0]
  0016fa64: ldr r1,[r1,#0xc]
  0016fa66: blx r4
  0016fa68: movs r1,#0x3
  0016fa6a: movs r2,#0x0
  0016fa6c: blx r5
  0016fa6e: ldr.w r1,[r11,#0x18]
  0016fa72: ldr.w r0,[r8,#0x0]
  0016fa76: ldr r1,[r1,#0xc]
  0016fa78: blx r4
  0016fa7a: movs r1,#0x1
  0016fa7c: movs r2,#0x0
  0016fa7e: blx r5
  0016fa80: b 0x0016fc8c
  0016fa82: ldr.w r0,[r11,#0x8]
  0016fa86: ldr r1,[r0,#0x4]
  0016fa88: add r0,sp,#0xb4
  0016fa8a: blx 0x00072964
  0016fa8e: ldr.w r0,[r11,#0x8]
  0016fa92: blx 0x0007798c
  0016fa96: ldr.w r5,[r11,#0x8]
  0016fa9a: cmp r0,#0x0
  0016fa9c: add.w r10,sp,#0x78
  0016faa0: mov.w r2,#0x3c
  0016faa4: ite eq
  0016faa6: add.eq r0,r5,#0x4
  0016faa8: mov.ne r0,r4
  0016faaa: ldr r0,[r0,#0x0]
  0016faac: adds r1,r0,#0x4
  0016faae: mov r0,r10
  0016fab0: blx 0x0006f1e4
  0016fab4: ldr.w r0,[r5,#0x84]
  0016fab8: add r2,sp,#0x68
  0016faba: str r0,[sp,#0x70]
  0016fabc: mov r1,r10
  0016fabe: vldr.32 s0,[pc,#0x2a0]
  0016fac2: vldr.32 s2,[sp,#0x70]
  0016fac6: vldr.64 d16,[r5,#0x7c]
  0016faca: add r5,sp,#0x28
  0016facc: vadd.f32 s0,s2,s0
  0016fad0: mov r0,r5
  0016fad2: vstr.64 d16,[sp,#0x68]
  0016fad6: vstr.32 s0,[sp,#0x70]
  0016fada: blx 0x0006f694
  0016fade: add r6,sp,#0xb4
  0016fae0: mov r1,r5
  0016fae2: mov r0,r6
  0016fae4: blx 0x00073534
  0016fae8: ldr.w r0,[r11,#0x18]
  0016faec: mov r1,r6
  0016faee: blx 0x00072148
  0016faf2: ldr.w r1,[r11,#0x18]
  0016faf6: ldr.w r0,[r8,#0x0]
  0016fafa: ldr r1,[r1,#0xc]
  0016fafc: blx 0x00072088
  0016fb00: ldr r3,[sp,#0x8]
  0016fb02: movs r1,#0x0
  0016fb04: mov r2,r9
  0016fb06: str r1,[sp,#0x0]
  0016fb08: blx 0x0006f7cc
  0016fb0c: ldr.w r0,[r11,#0x8]
  0016fb10: ldr r0,[r0,#0x5c]
  0016fb12: cmp r0,#0x8
  0016fb14: bne 0x0016fb4e
  0016fb16: ldr r0,[r4,#0x2c]
  0016fb18: blx 0x000721cc
  0016fb1c: mov r6,r0
  0016fb1e: ldr r0,[r4,#0x28]
  0016fb20: blx 0x000721cc
  0016fb24: mov r2,r0
  0016fb26: add r0,sp,#0x28
  0016fb28: mov r1,r6
  0016fb2a: blx 0x0006f82c
  0016fb2e: ldr.w r0,[r11,#0x8]
  0016fb32: ldr r1,[r0,#0x58]
  0016fb34: cmp r1,#0x2f
  0016fb36: beq 0x0016fb94
  0016fb38: cmp r1,#0xb5
  0016fb3a: beq 0x0016fb9a
  0016fb3c: cmp r1,#0x31
  0016fb3e: beq 0x0016fba4
  0016fb40: cmp r1,#0xb4
  0016fb42: beq 0x0016fbaa
  0016fb44: cmp r1,#0x30
  0016fb46: bne 0x0016fbba
  0016fb48: vldr.32 s0,[pc,#0x228]
  0016fb4c: b 0x0016fbae
  0016fb4e: ldr.w r6,[r8,#0x0]
  0016fb52: mov r0,r6
  0016fb54: blx 0x000717f4
  0016fb58: mov r1,r0
  0016fb5a: mov r0,r6
  0016fb5c: blx 0x0006ff1c
  0016fb60: add.w r8,sp,#0x28
  0016fb64: mov r1,r0
  0016fb66: movs r2,#0x3c
  0016fb68: mov r0,r8
  0016fb6a: blx 0x0006f1e4
  0016fb6e: add r4,sp,#0x1c
  0016fb70: add r1,sp,#0x78
  0016fb72: ldr.w r6,[r11,#0x18]
  0016fb76: mov r0,r4
  0016fb78: blx 0x0006f4c0
  0016fb7c: add.w r10,sp,#0x10
  0016fb80: mov r1,r8
  0016fb82: mov r0,r10
  0016fb84: blx 0x0006f4d8
  0016fb88: mov r0,r6
  0016fb8a: mov r1,r4
  0016fb8c: mov r2,r10
  0016fb8e: blx 0x000726ac
  0016fb92: b 0x0016fc8c
  0016fb94: vldr.32 s0,[pc,#0x1cc]
  0016fb98: b 0x0016fbae
  0016fb9a: movs r3,#0x0
  0016fb9c: ldr r2,[0x0016fd68]
  0016fb9e: strd r3,r3,[sp,#0x1c]
  0016fba2: b 0x0016fbe0
  0016fba4: vldr.32 s0,[pc,#0x1c8]
  0016fba8: b 0x0016fbae
  0016fbaa: vldr.32 s0,[pc,#0x1c0]
  0016fbae: movs r2,#0x0
  0016fbb0: strd r2,r2,[sp,#0x1c]
  0016fbb4: vstr.32 s0,[sp,#0x24]
  0016fbb8: b 0x0016fbe2
  0016fbba: adr r2,[0x16fd78]
  0016fbbc: cmp r1,#0xb6
  0016fbbe: it eq
  0016fbc0: add.eq r2,#0x4
  0016fbc2: cmp r1,#0xe0
  0016fbc4: vldr.32 s0,[r2]
  0016fbc8: mov.w r2,#0x0
  0016fbcc: strd r2,r2,[sp,#0x1c]
  0016fbd0: vstr.32 s0,[sp,#0x24]
  0016fbd4: beq 0x0016fbde
  0016fbd6: cmp r1,#0xb6
  0016fbd8: bne 0x0016fbe2
  0016fbda: ldr r2,[0x0016fd84]
  0016fbdc: b 0x0016fbe0
  0016fbde: ldr r2,[0x0016fd80]
  0016fbe0: str r2,[sp,#0x24]
  0016fbe2: ldrb.w r2,[r0,#0xa4]
  0016fbe6: cbz r2,0x0016fc30
  0016fbe8: cmp r1,#0xb5
  0016fbea: bne 0x0016fbfe
  0016fbec: ldrb.w r2,[r0,#0xa6]
  0016fbf0: adr r3,[0x16fd90]
  0016fbf2: cmp r2,#0x0
  0016fbf4: it eq
  0016fbf6: add.eq r3,#0x4
  0016fbf8: vldr.32 s0,[r3]
  0016fbfc: b 0x0016fc2c
  0016fbfe: ldrb.w r2,[r0,#0xa6]
  0016fc02: cmp r1,#0x30
  0016fc04: vldr.32 s2,[r0,#0x7c]
  0016fc08: bne 0x0016fc14
  0016fc0a: vmov.f32 s0,0x41a00000
  0016fc0e: vmov.f32 s4,0xc1a00000
  0016fc12: b 0x0016fc1c
  0016fc14: vldr.32 s0,[pc,#0x170]
  0016fc18: vldr.32 s4,[pc,#0x170]
  0016fc1c: vsub.f32 s0,s0,s2
  0016fc20: vadd.f32 s2,s2,s4
  0016fc24: cmp r2,#0x0
  0016fc26: it ne
  0016fc28: vmov.ne.f32 s0,s2
  0016fc2c: vstr.32 s0,[sp,#0x1c]
  0016fc30: ldrb.w r2,[r0,#0xa5]
  0016fc34: cbz r2,0x0016fc68
  0016fc36: cmp r1,#0xe0
  0016fc38: beq 0x0016fc50
  0016fc3a: cmp r1,#0xb5
  0016fc3c: bne 0x0016fc68
  0016fc3e: ldrb.w r0,[r0,#0xa7]
  0016fc42: adr r1,[0x16fd98]
  0016fc44: cmp r0,#0x0
  0016fc46: it eq
  0016fc48: add.eq r1,#0x4
  0016fc4a: vldr.32 s0,[r1]
  0016fc4e: b 0x0016fc64
  0016fc50: vmov.f32 s2,0x41500000
  0016fc54: ldrb.w r0,[r0,#0xa7]
  0016fc58: vmov.f32 s0,0xc1500000
  0016fc5c: cmp r0,#0x0
  0016fc5e: it eq
  0016fc60: vmov.eq.f32 s0,s2
  0016fc64: vstr.32 s0,[sp,#0x20]
  0016fc68: add.w r8,sp,#0x10
  0016fc6c: add r6,sp,#0x28
  0016fc6e: add r2,sp,#0x1c
  0016fc70: mov r0,r8
  0016fc72: mov r1,r6
  0016fc74: blx 0x0006f694
  0016fc78: ldr.w r0,[r11,#0x18]
  0016fc7c: mov r1,r6
  0016fc7e: blx 0x000721e4
  0016fc82: ldr.w r0,[r11,#0x18]
  0016fc86: mov r1,r8
  0016fc88: blx 0x00072460
  0016fc8c: ldr.w r0,[r11,#0x8]
  0016fc90: ldrb.w r1,[r0,#0xa9]
  0016fc94: strb.w r1,[r11,#0x1d]
  0016fc98: ldr r1,[r0,#0x5c]
  0016fc9a: cmp r1,#0x19
  0016fc9c: bne 0x0016fd3c
  0016fc9e: add.w r10,sp,#0x78
  0016fca2: movs r4,#0x0
  0016fca4: mov.w r8,#0x1
  0016fca8: movs r6,#0x0
  0016fcaa: movs r5,#0x0
  0016fcac: b 0x0016fd36
  0016fcbc: ldr r1,[r0,#0x40]
  0016fcbe: ldrb r1,[r1,r5]
  0016fcc0: cbz r1,0x0016fd2e
  0016fcc2: ldr.w r1,[r11,#0x30]
  0016fcc6: ldrb r1,[r1,r5]
  0016fcc8: cbz r1,0x0016fcea
  0016fcca: ldr.w r1,[r11,#0x2c]
  0016fcce: ldr r2,[r0,#0x30]
  0016fcd0: ldr r1,[r1,#0x4]
  0016fcd2: ldr.w r0,[r1,r5,lsl #0x2]
  0016fcd6: adds r1,r2,r6
  0016fcd8: mov r2,r10
  0016fcda: strd r4,r4,[sp,#0x78]
  0016fcde: str r4,[sp,#0x80]
  0016fce0: blx 0x00072808
  0016fce4: ldr.w r0,[r11,#0x30]
  0016fce8: strb r4,[r0,r5]
  0016fcea: ldr.w r0,[r11,#0x2c]
  0016fcee: mov r1,r9
  0016fcf0: movs r2,#0x0
  0016fcf2: ldr r0,[r0,#0x4]
  0016fcf4: ldr.w r0,[r0,r5,lsl #0x2]
  0016fcf8: blx 0x00075934
  0016fcfc: ldr.w r0,[r11,#0x2c]
  0016fd00: ldr r0,[r0,#0x4]
  0016fd02: ldr.w r0,[r0,r5,lsl #0x2]
  0016fd06: blx 0x000757fc
  0016fd0a: cbnz r0,0x0016fd2e
  0016fd0c: ldr.w r0,[r11,#0x8]
  0016fd10: ldr r1,[r0,#0x40]
  0016fd12: strb r4,[r1,r5]
  0016fd14: strb.w r4,[r0,#0x88]
  0016fd18: ldr.w r0,[r11,#0x30]
  0016fd1c: strb.w r8,[r0,r5]
  0016fd20: ldr.w r0,[r11,#0x2c]
  0016fd24: ldr r0,[r0,#0x4]
  0016fd26: ldr.w r0,[r0,r5,lsl #0x2]
  0016fd2a: blx 0x00072c88
  0016fd2e: adds r6,#0xc
  0016fd30: ldr.w r0,[r11,#0x8]
  0016fd34: adds r5,#0x1
  0016fd36: ldr r1,[r0,#0x8]
  0016fd38: cmp r5,r1
  0016fd3a: bcc 0x0016fcbc
  0016fd3c: ldr r0,[sp,#0xc0]
  0016fd3e: ldr r1,[sp,#0xc]
  0016fd40: ldr r1,[r1,#0x0]
  0016fd42: subs r0,r1,r0
  0016fd44: ittt eq
  0016fd46: add.eq sp,#0xc4
  0016fd48: pop.eq.w {r8,r9,r10,r11}
  0016fd4c: pop.eq {r4,r5,r6,r7,pc}
  0016fd4e: blx 0x0006e824
```
