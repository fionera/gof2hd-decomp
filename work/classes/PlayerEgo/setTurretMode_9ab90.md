# PlayerEgo::setTurretMode
elf 0x9ab90 body 506
Sig: undefined __stdcall setTurretMode(bool param_1)

## decompile
```c

/* PlayerEgo::setTurretMode(bool) */

void PlayerEgo::setTurretMode(bool param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  AEGeometry *pAVar5;
  Transform *this;
  int in_r1;
  int *piVar6;
  Matrix *pMVar7;
  uint uVar8;
  uint *puVar9;
  float extraout_s0;
  float fVar10;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  
  piVar1 = (int *)(uint)param_1;
  piVar6 = *(int **)(DAT_000aadb8 + 0xaaba2);
  iVar2 = *piVar6;
  if ((((char)piVar1[0x5c] == '\0') || (piVar1[0x79] != 0)) || ((char)piVar1[0x60] != '\0')) {
    if (piVar1[0x65] != 0) {
      AbyssEngine::PaintCanvas::CameraSetCurrent
                ((PaintCanvas *)**(undefined4 **)(DAT_000aadbc + 0xaabc8),*(uint *)piVar1[0x22]);
      LevelScript::resetCamera((Level *)piVar1[4]);
    }
LAB_000aabd8:
    uVar3 = 0;
  }
  else {
    *(bool *)(piVar1 + 0x68) = SUB41(in_r1,0);
    if (in_r1 == 0) {
      stopShooting((int)piVar1);
      AbyssEngine::PaintCanvas::CameraSetCurrent
                ((PaintCanvas *)**(undefined4 **)(DAT_000aadcc + 0xaad38),*(uint *)piVar1[0x22]);
      LevelScript::resetCamera((Level *)piVar1[4]);
    }
    else {
      if (piVar1[0x65] != 0) goto LAB_000aabd8;
      if (piVar1[0x5e] == 0) {
        puVar9 = *(uint **)(DAT_000aadc0 + 0xaac12);
        AbyssEngine::PaintCanvas::CameraCreate((PaintCanvas *)*puVar9,(uint *)(piVar1 + 0x5d));
        uVar8 = *puVar9;
        iVar4 = Status::inAlienOrbit();
        fVar10 = DAT_000aad9c;
        if (iVar4 != 0) {
          fVar10 = DAT_000aada0;
        }
        AbyssEngine::PaintCanvas::CameraSetPerspective(uVar8,fVar10,extraout_s1,DAT_000aada0);
        pAVar5 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar5,(PaintCanvas *)*puVar9);
        piVar1[0x5e] = (int)pAVar5;
        AEGeometry::setRotationOrder(pAVar5,2);
        AEGeometry::translate((Vector *)piVar1[0x5e]);
        pAVar5 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar5,(PaintCanvas *)*puVar9);
        piVar1[0x67] = (int)pAVar5;
        AEGeometry::translate(extraout_s0,extraout_s1_00,extraout_s2);
        AEGeometry::addChild(piVar1[0x5e]);
        pAVar5 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar5,(PaintCanvas *)*puVar9);
        piVar1[0x5f] = (int)pAVar5;
        AEGeometry::addChild((uint)pAVar5);
        iVar4 = Status::getShip();
        iVar4 = Ship::getFirstEquipmentOfSort(iVar4);
        if (iVar4 != 0) {
          AEGeometry::rotate((Vector *)piVar1[0x5e]);
        }
      }
      AEGeometry::setPosition((AEGeometry *)piVar1[0x5f],(Vector *)(piVar1 + 0x52));
      pMVar7 = (Matrix *)piVar1[0x5f];
      AEGeometry::getMatrix();
      AEGeometry::setMatrix(pMVar7);
      AbyssEngine::PaintCanvas::CameraSetCurrent
                ((PaintCanvas *)**(undefined4 **)(DAT_000aadc8 + 0xaad16),piVar1[0x5d]);
      Player::stopShooting(*piVar1);
    }
    if (piVar1[0xc] != 0) {
      this = (Transform *)
             AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000aadd0 + 0xaad52));
      fVar10 = (float)AbyssEngine::Transform::SetVisible(this,SUB41(in_r1,0));
      if (in_r1 == 0) {
        FModSound::stop(**(int **)(DAT_000aadd4 + 0xaad68));
      }
      else {
        FModSound::play(**(int **)(DAT_000aadd4 + 0xaad68),(Vector *)0x8cf,(Vector *)0x0,fVar10);
      }
    }
    uVar3 = 1;
  }
  if (*piVar6 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

```

## target disasm
```
  000aab90: push {r4,r5,r6,r7,lr}
  000aab92: add r7,sp,#0xc
  000aab94: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000aab98: mov r5,r0
  000aab9a: ldr r0,[0x000aadb8]
  000aab9c: mov r4,r1
  000aab9e: add r0,pc
  000aaba0: ldr r6,[r0,#0x0]
  000aaba2: ldr r0,[r6,#0x0]
  000aaba4: str r0,[sp,#0x10]
  000aaba6: ldrb.w r0,[r5,#0x170]
  000aabaa: cbz r0,0x000aabb8
  000aabac: ldr.w r0,[r5,#0x1e4]
  000aabb0: cbnz r0,0x000aabb8
  000aabb2: ldrb.w r0,[r5,#0x180]
  000aabb6: cbz r0,0x000aabee
  000aabb8: ldr.w r0,[r5,#0x194]
  000aabbc: cbz r0,0x000aabd8
  000aabbe: ldr r0,[0x000aadbc]
  000aabc0: ldr.w r1,[r5,#0x88]
  000aabc4: add r0,pc
  000aabc6: ldr r0,[r0,#0x0]
  000aabc8: ldr r1,[r1,#0x0]
  000aabca: ldr r0,[r0,#0x0]
  000aabcc: blx 0x0006fd9c
  000aabd0: ldrd r1,r0,[r5,#0xc]
  000aabd4: blx 0x0007246c
  000aabd8: movs r0,#0x0
  000aabda: ldr r1,[sp,#0x10]
  000aabdc: ldr r2,[r6,#0x0]
  000aabde: subs r1,r2,r1
  000aabe0: ittt eq
  000aabe2: add.eq sp,#0x14
  000aabe4: pop.eq.w {r8,r9,r10,r11}
  000aabe8: pop.eq {r4,r5,r6,r7,pc}
  000aabea: blx 0x0006e824
  000aabee: cmp r4,#0x0
  000aabf0: strb.w r4,[r5,#0x1a0]
  000aabf4: beq.w 0x000aad26
  000aabf8: ldr.w r0,[r5,#0x194]
  000aabfc: cmp r0,#0x0
  000aabfe: bne 0x000aabd8
  000aac00: ldr.w r0,[r5,#0x178]
  000aac04: cmp r0,#0x0
  000aac06: bne 0x000aacee
  000aac08: ldr r0,[0x000aadc0]
  000aac0a: add.w r1,r5,#0x174
  000aac0e: add r0,pc
  000aac10: ldr.w r11,[r0,#0x0]
  000aac14: ldr.w r0,[r11,#0x0]
  000aac18: blx 0x00072058
  000aac1c: ldr r0,[0x000aadc4]
  000aac1e: ldr.w r8,[r5,#0x174]
  000aac22: add r0,pc
  000aac24: ldr.w r9,[r11,#0x0]
  000aac28: ldr.w r10,[r0,#0x0]
  000aac2c: ldr.w r0,[r10,#0x0]
  000aac30: blx 0x000723d0
  000aac34: vldr.32 s0,[pc,#0x164]
  000aac38: cmp r0,#0x0
  000aac3a: vldr.32 s2,[pc,#0x164]
  000aac3e: mov r0,r9
  000aac40: ldr r2,[0x000aada4]
  000aac42: mov r1,r8
  000aac44: ldr r3,[0x000aada8]
  000aac46: it ne
  000aac48: vmov.ne.f32 s0,s2
  000aac4c: vstr.32 s0,[sp]
  000aac50: blx 0x00072004
  000aac54: movs r0,#0xc0
  000aac56: blx 0x0006eb24
  000aac5a: ldr.w r1,[r11,#0x0]
  000aac5e: mov r8,r0
  000aac60: blx 0x000720f4
  000aac64: mov r0,r8
  000aac66: movs r1,#0x2
  000aac68: str.w r8,[r5,#0x178]
  000aac6c: blx 0x00072100
  000aac70: ldr.w r0,[r5,#0x178]
  000aac74: add.w r1,r5,#0x224
  000aac78: blx 0x00072460
  000aac7c: movs r0,#0xc0
  000aac7e: blx 0x0006eb24
  000aac82: ldr.w r1,[r11,#0x0]
  000aac86: mov r8,r0
  000aac88: blx 0x000720f4
  000aac8c: ldr r2,[0x000aadac]
  000aac8e: mov r0,r8
  000aac90: ldr r3,[0x000aadb0]
  000aac92: movs r1,#0x0
  000aac94: str.w r8,[r5,#0x19c]
  000aac98: blx 0x00072154
  000aac9c: ldr.w r1,[r5,#0x19c]
  000aaca0: ldr.w r0,[r5,#0x178]
  000aaca4: ldr r1,[r1,#0xc]
  000aaca6: blx 0x000720e8
  000aacaa: movs r0,#0xc0
  000aacac: blx 0x0006eb24
  000aacb0: ldr.w r1,[r11,#0x0]
  000aacb4: mov r8,r0
  000aacb6: blx 0x000720f4
  000aacba: ldr.w r0,[r5,#0x178]
  000aacbe: str.w r8,[r5,#0x17c]
  000aacc2: ldr r1,[r0,#0xc]
  000aacc4: mov r0,r8
  000aacc6: blx 0x000720e8
  000aacca: ldr.w r0,[r10,#0x0]
  000aacce: blx 0x00071a58
  000aacd2: movs r1,#0x23
  000aacd4: blx 0x0007228c
  000aacd8: cbz r0,0x000aacee
  000aacda: ldr r1,[0x000aadb4]
  000aacdc: ldr.w r0,[r5,#0x178]
  000aace0: str r1,[sp,#0x8]
  000aace2: movs r1,#0x0
  000aace4: str r1,[sp,#0x4]
  000aace6: str r1,[sp,#0xc]
  000aace8: add r1,sp,#0x4
  000aacea: blx 0x00072478
  000aacee: ldr.w r0,[r5,#0x17c]
  000aacf2: add.w r1,r5,#0x148
  000aacf6: blx 0x00072148
  000aacfa: ldr r0,[r5,#0x8]
  000aacfc: ldr.w r8,[r5,#0x17c]
  000aad00: blx 0x000721cc
  000aad04: mov r1,r0
  000aad06: mov r0,r8
  000aad08: blx 0x000721e4
  000aad0c: ldr r0,[0x000aadc8]
  000aad0e: ldr.w r1,[r5,#0x174]
  000aad12: add r0,pc
  000aad14: ldr r0,[r0,#0x0]
  000aad16: ldr r0,[r0,#0x0]
  000aad18: blx 0x0006fd9c
  000aad1c: ldr r0,[r5,#0x0]
  000aad1e: movs r1,#0x0
  000aad20: blx 0x00072484
  000aad24: b 0x000aad48
  000aad26: mov r0,r5
  000aad28: movs r1,#0x2
  000aad2a: blx 0x00072490
  000aad2e: ldr r0,[0x000aadcc]
  000aad30: ldr.w r1,[r5,#0x88]
  000aad34: add r0,pc
  000aad36: ldr r0,[r0,#0x0]
  000aad38: ldr r1,[r1,#0x0]
  000aad3a: ldr r0,[r0,#0x0]
  000aad3c: blx 0x0006fd9c
  000aad40: ldrd r1,r0,[r5,#0xc]
  000aad44: blx 0x0007246c
  000aad48: ldr r0,[r5,#0x30]
  000aad4a: cbz r0,0x000aad86
  000aad4c: ldr r1,[0x000aadd0]
  000aad4e: add r1,pc
  000aad50: ldr r2,[r1,#0x0]
  000aad52: ldr r1,[r0,#0xc]
  000aad54: ldr r0,[r2,#0x0]
  000aad56: blx 0x00072088
  000aad5a: mov r1,r4
  000aad5c: blx 0x0007249c
  000aad60: ldr r0,[0x000aadd4]
  000aad62: cmp r4,#0x0
  000aad64: add r0,pc
  000aad66: ldr r0,[r0,#0x0]
  000aad68: ldr r0,[r0,#0x0]
  000aad6a: beq 0x000aad7e
  000aad6c: movs r1,#0x0
  000aad6e: movs r2,#0x0
  000aad70: str r1,[sp,#0x0]
  000aad72: movw r1,#0x8cf
  000aad76: movs r3,#0x0
  000aad78: blx 0x00071548
  000aad7c: b 0x000aad86
  000aad7e: movw r1,#0x8cf
  000aad82: blx 0x000724a8
  000aad86: movs r0,#0x1
  000aad88: b 0x000aabda
```
