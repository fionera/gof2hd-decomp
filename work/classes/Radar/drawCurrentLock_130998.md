# Radar::drawCurrentLock
elf 0x130998 body 1418
Sig: undefined __stdcall drawCurrentLock(Hud * param_1)

## decompile
```c

/* Radar::drawCurrentLock(Hud*) */

void Radar::drawCurrentLock(Hud *param_1)

{
  undefined1 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int extraout_r1;
  int extraout_r1_00;
  int iVar5;
  PlayerAsteroid *pPVar6;
  int *piVar7;
  code *pcVar8;
  int iVar9;
  String *pSVar10;
  Sprite *pSVar11;
  uint *puVar12;
  uint uVar13;
  PlayerAsteroid *pPVar14;
  String *this;
  int *piVar15;
  PlayerAsteroid *this_00;
  bool bVar16;
  undefined8 uVar17;
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar15 = *(int **)(DAT_00140d30 + 0x1409aa);
  local_28 = *piVar15;
  if (param_1[0x48] == (Hud)0x0) goto LAB_00140f18;
  puVar1 = *(undefined1 **)(DAT_00140d34 + 0x1409c4);
  *puVar1 = 1;
  if (*(int *)(param_1 + 0x14) == 0) {
    this_00 = *(PlayerAsteroid **)(param_1 + 0xc);
    if (this_00 == (PlayerAsteroid *)0x0) {
      this_00 = *(PlayerAsteroid **)(param_1 + 0x38);
      bVar16 = this_00 == (PlayerAsteroid *)0x0;
      if (bVar16) {
        this_00 = *(PlayerAsteroid **)(param_1 + 4);
      }
      if ((bVar16 && this_00 == (PlayerAsteroid *)0x0) &&
         (this_00 = *(PlayerAsteroid **)(param_1 + 0x24), this_00 == (PlayerAsteroid *)0x0)) {
        *puVar1 = 0;
        goto LAB_00140f18;
      }
    }
    puVar12 = *(uint **)(DAT_00140d4c + 0x140a7a);
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar12,*(int *)(param_1 + 0xcc),*(int *)(param_1 + 0x210));
    pPVar14 = *(PlayerAsteroid **)(param_1 + 0xc);
    pPVar6 = *(PlayerAsteroid **)(param_1 + 0x24);
    if (pPVar6 == (PlayerAsteroid *)**(int **)(*(int *)(param_1 + 0x138) + 4)) {
      puVar3 = *(undefined4 **)(*(int *)(param_1 + 0x188) + 4);
    }
    else if (pPVar6 == (PlayerAsteroid *)(*(int **)(*(int *)(param_1 + 0x138) + 4))[3]) {
      puVar3 = (undefined4 *)(*(int *)(*(int *)(param_1 + 0x188) + 4) + 0xc);
    }
    else {
      puVar3 = (undefined4 *)(*(int *)(*(int *)(param_1 + 0x188) + 4) + 4);
    }
    AbyssEngine::String::String(aSStack_34,(String *)*puVar3,false);
    if (this_00 == *(PlayerAsteroid **)(param_1 + 0x38)) {
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_00140d70 + 0x140bf4));
      AbyssEngine::String::String(aSStack_40,pSVar10,false);
      puVar3 = *(undefined4 **)(DAT_00140d78 + 0x140c12);
      iVar5 = **(int **)(DAT_00140d74 + 0x140c10);
      iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar12,(String *)*puVar3);
      AbyssEngine::PaintCanvas::DrawString
                (*puVar12,(String *)*puVar3,(int)aSStack_40,(iVar5 >> 1) - (iVar2 >> 1),
                 (bool)((char)*(undefined4 *)(**(int **)(DAT_00140d7c + 0x140c2a) + 0xc4) +
                       (char)*(undefined4 *)(param_1 + 0x214)));
    }
    else if (this_00 == pPVar14) {
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_00140fe4 + 0x140c56));
      AbyssEngine::String::String(aSStack_40,pSVar10,false);
      puVar3 = *(undefined4 **)(DAT_00140fec + 0x140c76);
      iVar2 = **(int **)(DAT_00140fe8 + 0x140c72);
      iVar5 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar12,(String *)*puVar3);
      pSVar11 = *(Sprite **)(param_1 + 0xf8);
      iVar9 = PlayerAsteroid::getQualityFrameIndex(this_00);
      Sprite::setFrame(pSVar11,iVar9);
      pSVar11 = *(Sprite **)(param_1 + 0xf8);
      piVar7 = *(int **)(DAT_00140ff0 + 0x140ca6);
      iVar4 = *(int *)(*piVar7 + 0x2c);
      iVar9 = Sprite::getFrameWidth(pSVar11);
      iVar2 = (iVar2 >> 1) - (iVar5 >> 1);
      uVar17 = Sprite::setPosition(pSVar11,(iVar2 - iVar4) - iVar9,
                                   *(int *)(param_1 + 0x214) + *(int *)(*piVar7 + 0xc0));
      Sprite::draw((float)uVar17,(float)((ulonglong)uVar17 >> 0x20));
      AbyssEngine::PaintCanvas::DrawString
                (*puVar12,(String *)*puVar3,(int)aSStack_40,iVar2,
                 (bool)((char)*(undefined4 *)(*piVar7 + 0xc4) +
                       (char)*(undefined4 *)(param_1 + 0x214)));
    }
    else {
      AbyssEngine::String::String(aSStack_40);
      this = (String *)(this_00 + 0x18);
      if (*(int *)(this_00 + 0x20) == 0) {
LAB_00140d0a:
        if (this_00 == pPVar6) {
          this = aSStack_34;
        }
        else if (*(int *)(this_00 + 0x20) == 0) {
          this = (String *)GameText::getText(**(int **)(DAT_00140ff4 + 0x140d20));
        }
        AbyssEngine::String::operator=(aSStack_40,this);
        if (this_00 == pPVar6) {
          iVar2 = Status::inAlienOrbit();
          if (iVar2 == 0) {
            pSVar11 = *(Sprite **)(param_1 + 0xf0);
            Status::getSystem();
            iVar2 = SolarSystem::getRace();
            Sprite::setFrame(pSVar11,iVar2);
          }
          else {
            iVar2 = Status::dlc1Won();
            if (iVar2 == 0) {
              Sprite::setFrame(*(Sprite **)(param_1 + 0xf0),9);
            }
            else {
              Sprite::setFrame(*(Sprite **)(param_1 + 0xf0),8);
            }
          }
          iVar2 = **(int **)(DAT_00141010 + 0x140e78);
          iVar5 = AbyssEngine::PaintCanvas::GetTextWidth
                            (*puVar12,(String *)**(undefined4 **)(DAT_00141014 + 0x140e7a));
          pSVar11 = *(Sprite **)(param_1 + 0xf0);
          piVar7 = *(int **)(DAT_00141018 + 0x140e96);
          iVar9 = *(int *)(*piVar7 + 0x2c);
          iVar4 = Sprite::getFrameWidth(pSVar11);
          iVar2 = (iVar2 >> 1) - (iVar5 >> 1);
          uVar17 = Sprite::setPosition(pSVar11,(iVar2 - iVar9) - iVar4,
                                       *(int *)(param_1 + 0x214) + *(int *)(*piVar7 + 200));
          Sprite::draw((float)uVar17,(float)((ulonglong)uVar17 >> 0x20));
        }
        else {
          AbyssEngine::String::String(aSStack_70,(char *)(DAT_00140ff8 + 0x140d96),false);
          iVar2 = Player::getDamageRate(*(Player **)(this_00 + 4));
          AbyssEngine::String::String(aSStack_7c,iVar2);
          AbyssEngine::operator+(aSStack_64,aSStack_70);
          AbyssEngine::String::String(aSStack_88,(char *)(DAT_00140ffc + 0x140dbc),false);
          AbyssEngine::operator+(aSStack_58,aSStack_64);
          AbyssEngine::String::String(aSStack_4c,aSStack_58,false);
          AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
          pcVar8 = *(code **)(DAT_00141000 + 0x140de4);
          (*pcVar8)(aSStack_4c);
          (*pcVar8)(aSStack_58);
          (*pcVar8)(aSStack_88);
          (*pcVar8)(aSStack_64);
          (*pcVar8)(aSStack_7c);
          (*pcVar8)(aSStack_70);
          iVar5 = **(int **)(DAT_00141008 + 0x140e06);
          iVar2 = AbyssEngine::PaintCanvas::GetTextWidth
                            (*puVar12,(String *)**(undefined4 **)(DAT_00141004 + 0x140e04));
          iVar2 = (iVar5 >> 1) - (iVar2 >> 1);
        }
        AbyssEngine::PaintCanvas::DrawString
                  (*puVar12,(String *)**(undefined4 **)(DAT_00141020 + 0x140ee2),(int)aSStack_40,
                   iVar2,(bool)((char)*(undefined4 *)(**(int **)(DAT_0014101c + 0x140edc) + 0xc4) +
                               (char)*(undefined4 *)(param_1 + 0x214)));
        iVar5 = extraout_r1_00;
      }
      else {
        piVar7 = *(int **)(DAT_00140d50 + 0x140ae6);
        pSVar10 = (String *)GameText::getText(*piVar7);
        iVar2 = AbyssEngine::String::Compare(this,pSVar10);
        if (iVar2 != 0) {
          pSVar10 = (String *)GameText::getText(*piVar7);
          iVar2 = AbyssEngine::String::Compare(this,pSVar10);
          if ((iVar2 != 0) && (this_00[0x42] == (PlayerAsteroid)0x0)) goto LAB_00140d0a;
        }
        AbyssEngine::PaintCanvas::SetColor(*puVar12);
        AbyssEngine::String::operator=(aSStack_40,this);
        if (this_00[0x42] != (PlayerAsteroid)0x0) {
          AbyssEngine::String::String(aSStack_70,(char *)(DAT_00140d58 + 0x140b34),false);
          iVar2 = Player::getDamageRate(*(Player **)(this_00 + 4));
          AbyssEngine::String::String(aSStack_7c,iVar2);
          AbyssEngine::operator+(aSStack_64,aSStack_70);
          AbyssEngine::String::String(aSStack_88,(char *)(DAT_00140d5c + 0x140b5a),false);
          AbyssEngine::operator+(aSStack_58,aSStack_64);
          AbyssEngine::String::String(aSStack_4c,aSStack_58,false);
          AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
          pcVar8 = *(code **)(DAT_00140d60 + 0x140b82);
          (*pcVar8)(aSStack_4c);
          (*pcVar8)(aSStack_58);
          (*pcVar8)(aSStack_88);
          (*pcVar8)(aSStack_64);
          (*pcVar8)(aSStack_7c);
          (*pcVar8)(aSStack_70);
        }
        puVar3 = *(undefined4 **)(DAT_00140d68 + 0x140ba4);
        iVar5 = **(int **)(DAT_00140d64 + 0x140ba2);
        iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar12,(String *)*puVar3);
        iVar2 = (iVar5 >> 1) - (iVar2 >> 1);
        AbyssEngine::PaintCanvas::DrawString
                  (*puVar12,(String *)*puVar3,(int)aSStack_40,iVar2,
                   (bool)((char)*(undefined4 *)(**(int **)(DAT_00140d6c + 0x140bc2) + 0xc4) +
                         (char)*(undefined4 *)(param_1 + 0x214)));
        AbyssEngine::PaintCanvas::SetColor(*puVar12);
        iVar5 = extraout_r1;
      }
      if (this_00 != pPVar6) {
        iVar5 = *(int *)(this_00 + 0x28);
      }
      if (this_00 != pPVar6 && iVar5 != 10) {
        Sprite::setFrame(*(Sprite **)(param_1 + 0xf0),iVar5);
        pSVar11 = *(Sprite **)(param_1 + 0xf0);
        piVar7 = *(int **)(DAT_00141024 + 0x140f40);
        iVar9 = *(int *)(*piVar7 + 0x2c);
        iVar5 = Sprite::getFrameWidth(pSVar11);
        uVar17 = Sprite::setPosition(pSVar11,(iVar2 - iVar9) - iVar5,
                                     *(int *)(param_1 + 0x214) + *(int *)(*piVar7 + 200));
        Sprite::draw((float)uVar17,(float)((ulonglong)uVar17 >> 0x20));
      }
    }
    AbyssEngine::String::~String(aSStack_40);
  }
  else {
    puVar12 = *(uint **)(DAT_00140d38 + 0x1409d8);
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar12,*(int *)(param_1 + 0xcc),*(int *)(param_1 + 0x210));
    iVar2 = Status::getPlanetNames((Status *)**(undefined4 **)(DAT_00140d3c + 0x1409e8));
    AbyssEngine::String::String
              (aSStack_34,*(String **)(*(int *)(iVar2 + 4) + *(int *)(param_1 + 0x40) * 4),false);
    uVar13 = *puVar12;
    iVar5 = **(int **)(DAT_00140d40 + 0x140a10);
    pSVar10 = (String *)**(undefined4 **)(DAT_00140d44 + 0x140a14);
    iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(uVar13,pSVar10);
    AbyssEngine::PaintCanvas::DrawString
              (uVar13,pSVar10,(int)aSStack_34,(iVar5 >> 1) - iVar2 / 2,
               (bool)((char)*(undefined4 *)(**(int **)(DAT_00140d48 + 0x140a2e) + 0xbc) +
                     (char)*(undefined4 *)(param_1 + 0x214)));
  }
  AbyssEngine::String::~String(aSStack_34);
LAB_00140f18:
  if (*piVar15 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00140998: push {r4,r5,r6,r7,lr}
  0014099a: add r7,sp,#0xc
  0014099c: push {r8,r9,r10,r11}
  001409a0: sub sp,#0x7c
  001409a2: mov r9,r0
  001409a4: ldr r0,[0x00140d30]
  001409a6: add r0,pc
  001409a8: ldr.w r10,[r0,#0x0]
  001409ac: ldr.w r0,[r10,#0x0]
  001409b0: str r0,[sp,#0x78]
  001409b2: ldrb.w r0,[r9,#0x48]
  001409b6: cmp r0,#0x0
  001409b8: beq.w 0x00140f18
  001409bc: ldr r0,[0x00140d34]
  001409be: movs r1,#0x1
  001409c0: add r0,pc
  001409c2: ldr r0,[r0,#0x0]
  001409c4: strb r1,[r0,#0x0]
  001409c6: ldr.w r1,[r9,#0x14]
  001409ca: cmp r1,#0x0
  001409cc: beq 0x00140a50
  001409ce: ldr r0,[0x00140d38]
  001409d0: ldr.w r1,[r9,#0xcc]
  001409d4: add r0,pc
  001409d6: ldrd r2,r3,[r9,#0x210]
  001409da: ldr r6,[r0,#0x0]
  001409dc: ldr r0,[r6,#0x0]
  001409de: blx 0x00072dc0
  001409e2: ldr r0,[0x00140d3c]
  001409e4: add r0,pc
  001409e6: ldr r0,[r0,#0x0]
  001409e8: ldr r0,[r0,#0x0]
  001409ea: blx 0x000767ec
  001409ee: ldr.w r1,[r9,#0x40]
  001409f2: add.w r11,sp,#0x6c
  001409f6: ldr r0,[r0,#0x4]
  001409f8: movs r2,#0x0
  001409fa: mov.w r8,#0x0
  001409fe: ldr.w r1,[r0,r1,lsl #0x2]
  00140a02: mov r0,r11
  00140a04: blx 0x0006f028
  00140a08: ldr r0,[0x00140d40]
  00140a0a: ldr r1,[0x00140d44]
  00140a0c: add r0,pc
  00140a0e: ldr r6,[r6,#0x0]
  00140a10: add r1,pc
  00140a12: ldr r0,[r0,#0x0]
  00140a14: ldr r1,[r1,#0x0]
  00140a16: ldr r4,[r0,#0x0]
  00140a18: ldr r5,[r1,#0x0]
  00140a1a: mov r0,r6
  00140a1c: mov r1,r5
  00140a1e: mov r2,r11
  00140a20: blx 0x0006faa8
  00140a24: ldr r1,[0x00140d48]
  00140a26: ldr.w r2,[r9,#0x214]
  00140a2a: add r1,pc
  00140a2c: ldr r1,[r1,#0x0]
  00140a2e: ldr r1,[r1,#0x0]
  00140a30: ldr.w r1,[r1,#0xbc]
  00140a34: add.w r0,r0,r0, lsr #0x1f
  00140a38: add r1,r2
  00140a3a: strd r1,r8,[sp,#0x0]
  00140a3e: asrs r1,r4,#0x1
  00140a40: sub.w r3,r1,r0, asr #0x1
  00140a44: add r2,sp,#0x6c
  00140a46: mov r0,r6
  00140a48: mov r1,r5
  00140a4a: blx 0x0006fe14
  00140a4e: b 0x00140f12
  00140a50: ldr.w r11,[r9,#0xc]
  00140a54: cmp.w r11,#0x0
  00140a58: bne 0x00140a70
  00140a5a: ldr.w r11,[r9,#0x38]
  00140a5e: cmp.w r11,#0x0
  00140a62: itt eq
  00140a64: ldr.eq.w r11,[r9,#0x4]
  00140a68: cmp.eq.w r11,#0x0
  00140a6c: beq.w 0x00140e40
  00140a70: ldr r0,[0x00140d4c]
  00140a72: ldr.w r1,[r9,#0xcc]
  00140a76: add r0,pc
  00140a78: ldrd r2,r3,[r9,#0x210]
  00140a7c: ldr r5,[r0,#0x0]
  00140a7e: ldr r0,[r5,#0x0]
  00140a80: blx 0x00072dc0
  00140a84: ldr.w r0,[r9,#0x138]
  00140a88: ldr.w r6,[r9,#0xc]
  00140a8c: ldr.w r4,[r9,#0x24]
  00140a90: ldr r0,[r0,#0x4]
  00140a92: ldr r1,[r0,#0x0]
  00140a94: cmp r4,r1
  00140a96: beq 0x00140aaa
  00140a98: ldr.w r1,[r9,#0x188]
  00140a9c: ldr r2,[r0,#0xc]
  00140a9e: ldr r0,[r1,#0x4]
  00140aa0: cmp r4,r2
  00140aa2: ite ne
  00140aa4: add.ne r0,#0x4
  00140aa6: add.eq r0,#0xc
  00140aa8: b 0x00140ab0
  00140aaa: ldr.w r0,[r9,#0x188]
  00140aae: ldr r0,[r0,#0x4]
  00140ab0: ldr r1,[r0,#0x0]
  00140ab2: add r0,sp,#0x6c
  00140ab4: movs r2,#0x0
  00140ab6: blx 0x0006f028
  00140aba: ldr.w r0,[r9,#0x38]
  00140abe: cmp r11,r0
  00140ac0: beq.w 0x00140bec
  00140ac4: cmp r11,r6
  00140ac6: beq.w 0x00140c4c
  00140aca: add r0,sp,#0x60
  00140acc: str r4,[sp,#0x14]
  00140ace: blx 0x0006efbc
  00140ad2: ldr.w r0,[r11,#0x20]
  00140ad6: add.w r6,r11,#0x18
  00140ada: cmp r0,#0x0
  00140adc: beq.w 0x00140d0a
  00140ae0: ldr r0,[0x00140d50]
  00140ae2: add r0,pc
  00140ae4: ldr r4,[r0,#0x0]
  00140ae6: ldr r0,[r4,#0x0]
  00140ae8: movw r1,#0x64b
  00140aec: blx 0x00072f70
  00140af0: mov r1,r0
  00140af2: mov r0,r6
  00140af4: blx 0x0006f664
  00140af8: cbz r0,0x00140b18
  00140afa: ldr r0,[r4,#0x0]
  00140afc: movw r1,#0x67f
  00140b00: blx 0x00072f70
  00140b04: mov r1,r0
  00140b06: mov r0,r6
  00140b08: blx 0x0006f664
  00140b0c: cbz r0,0x00140b18
  00140b0e: ldrb.w r0,[r11,#0x42]
  00140b12: cmp r0,#0x0
  00140b14: beq.w 0x00140d0a
  00140b18: ldr r0,[r5,#0x0]
  00140b1a: ldr r1,[0x00140d54]
  00140b1c: blx 0x0006fac0
  00140b20: add r0,sp,#0x60
  00140b22: mov r1,r6
  00140b24: blx 0x0006f2b0
  00140b28: ldrb.w r0,[r11,#0x42]
  00140b2c: cbz r0,0x00140b9a
  00140b2e: ldr r1,[0x00140d58]
  00140b30: add r1,pc
  00140b32: add r0,sp,#0x30
  00140b34: movs r2,#0x0
  00140b36: blx 0x0006ee18
  00140b3a: ldr.w r0,[r11,#0x4]
  00140b3e: blx 0x00072550
  00140b42: mov r1,r0
  00140b44: add r0,sp,#0x24
  00140b46: blx 0x0006f658
  00140b4a: add r0,sp,#0x3c
  00140b4c: add r1,sp,#0x30
  00140b4e: add r2,sp,#0x24
  00140b50: blx 0x0006ef98
  00140b54: ldr r1,[0x00140d5c]
  00140b56: add r1,pc
  00140b58: add r0,sp,#0x18
  00140b5a: movs r2,#0x0
  00140b5c: blx 0x0006ee18
  00140b60: add r0,sp,#0x48
  00140b62: add r1,sp,#0x3c
  00140b64: add r2,sp,#0x18
  00140b66: blx 0x0006ef98
  00140b6a: add r0,sp,#0x54
  00140b6c: add r1,sp,#0x48
  00140b6e: movs r2,#0x0
  00140b70: blx 0x0006f028
  00140b74: add r0,sp,#0x60
  00140b76: add r1,sp,#0x54
  00140b78: blx 0x0006ef5c
  00140b7c: ldr r0,[0x00140d60]
  00140b7e: add r0,pc
  00140b80: ldr r4,[r0,#0x0]
  00140b82: add r0,sp,#0x54
  00140b84: blx r4
  00140b86: add r0,sp,#0x48
  00140b88: blx r4
  00140b8a: add r0,sp,#0x18
  00140b8c: blx r4
  00140b8e: add r0,sp,#0x3c
  00140b90: blx r4
  00140b92: add r0,sp,#0x24
  00140b94: blx r4
  00140b96: add r0,sp,#0x30
  00140b98: blx r4
  00140b9a: ldr r0,[0x00140d64]
  00140b9c: ldr r1,[0x00140d68]
  00140b9e: add r0,pc
  00140ba0: add r1,pc
  00140ba2: ldr r0,[r0,#0x0]
  00140ba4: ldr r4,[r1,#0x0]
  00140ba6: ldr r6,[r0,#0x0]
  00140ba8: ldr r1,[r4,#0x0]
  00140baa: ldr r0,[r5,#0x0]
  00140bac: add r2,sp,#0x60
  00140bae: blx 0x0006faa8
  00140bb2: ldr r1,[0x00140d6c]
  00140bb4: asrs r6,r6,#0x1
  00140bb6: sub.w r8,r6,r0, asr #0x1
  00140bba: ldr.w r12,[r9,#0x214]
  00140bbe: add r1,pc
  00140bc0: ldr r3,[r5,#0x0]
  00140bc2: ldr r1,[r1,#0x0]
  00140bc4: ldr r2,[r1,#0x0]
  00140bc6: ldr r1,[r4,#0x0]
  00140bc8: mov r4,r5
  00140bca: ldr.w r0,[r2,#0xc4]
  00140bce: movs r6,#0x0
  00140bd0: add r0,r12
  00140bd2: add r2,sp,#0x60
  00140bd4: strd r0,r6,[sp,#0x0]
  00140bd8: mov r0,r3
  00140bda: mov r3,r8
  00140bdc: blx 0x0006fe14
  00140be0: ldr r0,[r4,#0x0]
  00140be2: mov.w r1,#0xffffffff
  00140be6: blx 0x0006fac0
  00140bea: b 0x00140efe
  00140bec: ldr r0,[0x00140d70]
  00140bee: mov r6,r5
  00140bf0: add r0,pc
  00140bf2: ldr r0,[r0,#0x0]
  00140bf4: ldr r0,[r0,#0x0]
  00140bf6: movw r1,#0xca4
  00140bfa: blx 0x00072f70
  00140bfe: mov r1,r0
  00140c00: add r0,sp,#0x60
  00140c02: movs r2,#0x0
  00140c04: blx 0x0006f028
  00140c08: ldr r0,[0x00140d74]
  00140c0a: ldr r1,[0x00140d78]
  00140c0c: add r0,pc
  00140c0e: add r1,pc
  00140c10: ldr r0,[r0,#0x0]
  00140c12: ldr r5,[r1,#0x0]
  00140c14: ldr r4,[r0,#0x0]
  00140c16: ldr r1,[r5,#0x0]
  00140c18: ldr r0,[r6,#0x0]
  00140c1a: add r2,sp,#0x60
  00140c1c: blx 0x0006faa8
  00140c20: ldr r1,[0x00140d7c]
  00140c22: ldr.w r3,[r9,#0x214]
  00140c26: add r1,pc
  00140c28: ldr r6,[r6,#0x0]
  00140c2a: ldr r1,[r1,#0x0]
  00140c2c: ldr r2,[r1,#0x0]
  00140c2e: ldr r1,[r5,#0x0]
  00140c30: ldr.w r2,[r2,#0xc4]
  00140c34: movs r5,#0x0
  00140c36: add r2,r3
  00140c38: strd r2,r5,[sp,#0x0]
  00140c3c: asrs r2,r4,#0x1
  00140c3e: sub.w r3,r2,r0, asr #0x1
  00140c42: add r2,sp,#0x60
  00140c44: mov r0,r6
  00140c46: blx 0x0006fe14
  00140c4a: b 0x00140f0c
  00140c4c: ldr r0,[0x00140fe4]
  00140c4e: ldr.w r1,[r11,#0x128]
  00140c52: add r0,pc
  00140c54: ldr r0,[r0,#0x0]
  00140c56: ldr r0,[r0,#0x0]
  00140c58: addw r1,r1,#0x4fa
  00140c5c: blx 0x00072f70
  00140c60: mov r1,r0
  00140c62: add r0,sp,#0x60
  00140c64: movs r2,#0x0
  00140c66: blx 0x0006f028
  00140c6a: ldr r0,[0x00140fe8]
  00140c6c: ldr r1,[0x00140fec]
  00140c6e: add r0,pc
  00140c70: str r5,[sp,#0x10]
  00140c72: add r1,pc
  00140c74: ldr r0,[r0,#0x0]
  00140c76: ldr r1,[r1,#0x0]
  00140c78: str r1,[sp,#0xc]
  00140c7a: ldr r0,[r0,#0x0]
  00140c7c: str r0,[sp,#0x14]
  00140c7e: ldr r1,[r1,#0x0]
  00140c80: ldr r0,[r5,#0x0]
  00140c82: add r2,sp,#0x60
  00140c84: blx 0x0006faa8
  00140c88: ldr.w r6,[r9,#0xf8]
  00140c8c: mov r8,r0
  00140c8e: mov r0,r11
  00140c90: blx 0x00076894
  00140c94: mov r1,r0
  00140c96: mov r0,r6
  00140c98: blx 0x00074d70
  00140c9c: ldr r0,[0x00140ff0]
  00140c9e: ldr.w r6,[r9,#0xf8]
  00140ca2: add r0,pc
  00140ca4: ldr.w r11,[r0,#0x0]
  00140ca8: ldr.w r0,[r11,#0x0]
  00140cac: ldr r4,[r0,#0x2c]
  00140cae: mov r0,r6
  00140cb0: blx 0x000768ac
  00140cb4: ldr r1,[sp,#0x14]
  00140cb6: ldr.w r2,[r9,#0x214]
  00140cba: asrs r1,r1,#0x1
  00140cbc: sub.w r5,r1,r8, asr #0x1
  00140cc0: ldr.w r1,[r11,#0x0]
  00140cc4: ldr.w r3,[r1,#0xc0]
  00140cc8: subs r1,r5,r4
  00140cca: subs r1,r1,r0
  00140ccc: add r2,r3
  00140cce: mov r0,r6
  00140cd0: blx 0x00076060
  00140cd4: ldr.w r0,[r9,#0xf8]
  00140cd8: mov.w r1,#0x3f800000
  00140cdc: mov.w r2,#0x3f800000
  00140ce0: blx 0x0007606c
  00140ce4: ldr r0,[sp,#0xc]
  00140ce6: ldr.w r2,[r11,#0x0]
  00140cea: ldr.w r3,[r9,#0x214]
  00140cee: ldr r1,[r0,#0x0]
  00140cf0: ldr r0,[sp,#0x10]
  00140cf2: ldr.w r2,[r2,#0xc4]
  00140cf6: ldr r0,[r0,#0x0]
  00140cf8: add r2,r3
  00140cfa: movs r6,#0x0
  00140cfc: strd r2,r6,[sp,#0x0]
  00140d00: add r2,sp,#0x60
  00140d02: mov r3,r5
  00140d04: blx 0x0006fe14
  00140d08: b 0x00140f0c
  00140d0a: ldr r0,[sp,#0x14]
  00140d0c: cmp r11,r0
  00140d0e: beq 0x00140d80
  00140d10: ldr.w r0,[r11,#0x20]
  00140d14: cbnz r0,0x00140d82
  00140d16: ldr r0,[0x00140ff4]
  00140d18: ldr.w r1,[r11,#0x28]
  00140d1c: add r0,pc
  00140d1e: ldr r0,[r0,#0x0]
  00140d20: ldr r0,[r0,#0x0]
  00140d22: add.w r1,r1,#0x196
  00140d26: blx 0x00072f70
  00140d2a: mov r6,r0
  00140d2c: b 0x00140d82
  00140d80: add r6,sp,#0x6c
  00140d82: add r0,sp,#0x60
  00140d84: mov r1,r6
  00140d86: blx 0x0006f2b0
  00140d8a: ldr r0,[sp,#0x14]
  00140d8c: cmp r11,r0
  00140d8e: beq 0x00140e1c
  00140d90: ldr r1,[0x00140ff8]
  00140d92: add r1,pc
  00140d94: add r0,sp,#0x30
  00140d96: movs r2,#0x0
  00140d98: blx 0x0006ee18
  00140d9c: ldr.w r0,[r11,#0x4]
  00140da0: blx 0x00072550
  00140da4: mov r1,r0
  00140da6: add r0,sp,#0x24
  00140da8: blx 0x0006f658
  00140dac: add r0,sp,#0x3c
  00140dae: add r1,sp,#0x30
  00140db0: add r2,sp,#0x24
  00140db2: blx 0x0006ef98
  00140db6: ldr r1,[0x00140ffc]
  00140db8: add r1,pc
  00140dba: add r0,sp,#0x18
  00140dbc: movs r2,#0x0
  00140dbe: blx 0x0006ee18
  00140dc2: add r0,sp,#0x48
  00140dc4: add r1,sp,#0x3c
  00140dc6: add r2,sp,#0x18
  00140dc8: blx 0x0006ef98
  00140dcc: add r0,sp,#0x54
  00140dce: add r1,sp,#0x48
  00140dd0: movs r2,#0x0
  00140dd2: blx 0x0006f028
  00140dd6: add r0,sp,#0x60
  00140dd8: add r1,sp,#0x54
  00140dda: blx 0x0006ef5c
  00140dde: ldr r0,[0x00141000]
  00140de0: add r0,pc
  00140de2: ldr r4,[r0,#0x0]
  00140de4: add r0,sp,#0x54
  00140de6: blx r4
  00140de8: add r0,sp,#0x48
  00140dea: blx r4
  00140dec: add r0,sp,#0x18
  00140dee: blx r4
  00140df0: add r0,sp,#0x3c
  00140df2: blx r4
  00140df4: add r0,sp,#0x24
  00140df6: blx r4
  00140df8: add r0,sp,#0x30
  00140dfa: blx r4
  00140dfc: ldr r0,[0x00141004]
  00140dfe: ldr r1,[0x00141008]
  00140e00: add r0,pc
  00140e02: add r1,pc
  00140e04: ldr r0,[r0,#0x0]
  00140e06: ldr r1,[r1,#0x0]
  00140e08: ldr r4,[r1,#0x0]
  00140e0a: ldr r1,[r0,#0x0]
  00140e0c: ldr r0,[r5,#0x0]
  00140e0e: add r2,sp,#0x60
  00140e10: blx 0x0006faa8
  00140e14: asrs r1,r4,#0x1
  00140e16: sub.w r8,r1,r0, asr #0x1
  00140e1a: b 0x00140ed4
  00140e1c: ldr r0,[0x0014100c]
  00140e1e: add r0,pc
  00140e20: ldr r4,[r0,#0x0]
  00140e22: ldr r0,[r4,#0x0]
  00140e24: blx 0x000723d0
  00140e28: cbz r0,0x00140e52
  00140e2a: ldr r0,[r4,#0x0]
  00140e2c: blx 0x000719a4
  00140e30: mov r1,r0
  00140e32: ldr.w r0,[r9,#0xf0]
  00140e36: cbz r1,0x00140e6a
  00140e38: movs r1,#0x8
  00140e3a: blx 0x00074d70
  00140e3e: b 0x00140e70
  00140e40: ldr.w r11,[r9,#0x24]
  00140e44: cmp.w r11,#0x0
  00140e48: bne.w 0x00140a70
  00140e4c: movs r1,#0x0
  00140e4e: strb r1,[r0,#0x0]
  00140e50: b 0x00140f18
  00140e52: ldr.w r6,[r9,#0xf0]
  00140e56: ldr r0,[r4,#0x0]
  00140e58: blx 0x00071a10
  00140e5c: blx 0x00071950
  00140e60: mov r1,r0
  00140e62: mov r0,r6
  00140e64: blx 0x00074d70
  00140e68: b 0x00140e70
  00140e6a: movs r1,#0x9
  00140e6c: blx 0x00074d70
  00140e70: ldr r0,[0x00141010]
  00140e72: ldr r1,[0x00141014]
  00140e74: add r0,pc
  00140e76: add r1,pc
  00140e78: ldr r0,[r0,#0x0]
  00140e7a: ldr r1,[r1,#0x0]
  00140e7c: ldr r0,[r0,#0x0]
  00140e7e: str r0,[sp,#0x10]
  00140e80: ldr r1,[r1,#0x0]
  00140e82: ldr r0,[r5,#0x0]
  00140e84: add r2,sp,#0x60
  00140e86: blx 0x0006faa8
  00140e8a: mov r8,r0
  00140e8c: ldr r0,[0x00141018]
  00140e8e: ldr.w r6,[r9,#0xf0]
  00140e92: add r0,pc
  00140e94: ldr r4,[r0,#0x0]
  00140e96: ldr r0,[r4,#0x0]
  00140e98: ldr r0,[r0,#0x2c]
  00140e9a: str r0,[sp,#0xc]
  00140e9c: mov r0,r6
  00140e9e: blx 0x000768ac
  00140ea2: ldr r1,[sp,#0x10]
  00140ea4: ldr.w r2,[r9,#0x214]
  00140ea8: asrs r1,r1,#0x1
  00140eaa: sub.w r8,r1,r8, asr #0x1
  00140eae: ldr r1,[r4,#0x0]
  00140eb0: ldr.w r3,[r1,#0xc8]
  00140eb4: ldr r1,[sp,#0xc]
  00140eb6: add r2,r3
  00140eb8: sub.w r1,r8,r1
  00140ebc: subs r1,r1,r0
  00140ebe: mov r0,r6
  00140ec0: blx 0x00076060
  00140ec4: ldr.w r0,[r9,#0xf0]
  00140ec8: mov.w r1,#0x3f800000
  00140ecc: mov.w r2,#0x3f800000
  00140ed0: blx 0x0007606c
  00140ed4: ldr r0,[0x0014101c]
  00140ed6: ldr r1,[0x00141020]
  00140ed8: add r0,pc
  00140eda: ldr.w r3,[r9,#0x214]
  00140ede: add r1,pc
  00140ee0: ldr r0,[r0,#0x0]
  00140ee2: ldr r1,[r1,#0x0]
  00140ee4: ldr r2,[r0,#0x0]
  00140ee6: ldr r1,[r1,#0x0]
  00140ee8: ldr r0,[r5,#0x0]
  00140eea: ldr.w r2,[r2,#0xc4]
  00140eee: add r2,r3
  00140ef0: movs r6,#0x0
  00140ef2: strd r2,r6,[sp,#0x0]
  00140ef6: add r2,sp,#0x60
  00140ef8: mov r3,r8
  00140efa: blx 0x0006fe14
  00140efe: ldr r0,[sp,#0x14]
  00140f00: cmp r11,r0
  00140f02: itt ne
  00140f04: ldr.ne.w r1,[r11,#0x28]
  00140f08: cmp.ne r1,#0xa
  00140f0a: bne 0x00140f2e
  00140f0c: add r0,sp,#0x60
  00140f0e: blx 0x0006ee30
  00140f12: add r0,sp,#0x6c
  00140f14: blx 0x0006ee30
  00140f18: ldr r0,[sp,#0x78]
  00140f1a: ldr.w r1,[r10,#0x0]
  00140f1e: subs r0,r1,r0
  00140f20: ittt eq
  00140f22: add.eq sp,#0x7c
  00140f24: pop.eq.w {r8,r9,r10,r11}
  00140f28: pop.eq {r4,r5,r6,r7,pc}
  00140f2a: blx 0x0006e824
  00140f2e: ldr.w r0,[r9,#0xf0]
  00140f32: blx 0x00074d70
  00140f36: ldr r0,[0x00141024]
  00140f38: ldr.w r5,[r9,#0xf0]
  00140f3c: add r0,pc
  00140f3e: ldr r6,[r0,#0x0]
  00140f40: ldr r0,[r6,#0x0]
  00140f42: ldr r4,[r0,#0x2c]
  00140f44: mov r0,r5
  00140f46: blx 0x000768ac
  00140f4a: ldr r1,[r6,#0x0]
  00140f4c: ldr.w r2,[r9,#0x214]
  00140f50: ldr.w r3,[r1,#0xc8]
  00140f54: sub.w r1,r8,r4
  00140f58: subs r1,r1,r0
  00140f5a: add r2,r3
  00140f5c: mov r0,r5
  00140f5e: blx 0x00076060
  00140f62: ldr.w r0,[r9,#0xf0]
  00140f66: mov.w r1,#0x3f800000
  00140f6a: mov.w r2,#0x3f800000
  00140f6e: blx 0x0007606c
  00140f72: b 0x00140f0c
```
