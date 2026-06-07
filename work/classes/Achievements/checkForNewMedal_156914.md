# Achievements::checkForNewMedal
elf 0x156914 body 922
Sig: undefined __thiscall checkForNewMedal(Achievements * this, PlayerEgo * param_1)

## decompile
```c

/* Achievements::checkForNewMedal(PlayerEgo*) */

void __thiscall Achievements::checkForNewMedal(Achievements *this,PlayerEgo *param_1)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  uint *puVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  bool bVar12;
  undefined8 uVar13;
  
  initCheckEquipmentAndWeapons(this);
  uVar11 = 0;
  iVar8 = DAT_00166c50 + 0x16692e;
  do {
    if (uVar11 == 0x2d) {
      return;
    }
    iVar10 = 0;
    if (*(int *)(*(int *)this + uVar11 * 4) == 1) {
LAB_00166940:
      *(int *)(*(int *)(this + 4) + uVar11 * 4) = iVar10;
    }
    else {
      for (uVar6 = 0; uVar6 < 3; uVar6 = uVar6 + 1) {
        uVar7 = *(uint *)(iVar8 + uVar11 * 0xc + uVar6 * 4);
        if ((int)uVar7 < 0) goto switchD_00166978_default;
        if (0 < iVar10) break;
        uVar2 = uVar11 & 0x7fffffff;
        switch(uVar2) {
        case 1:
          iVar5 = PlayerEgo::getHullDamageRate(param_1);
          if ((int)uVar7 < iVar5) goto switchD_00166978_default;
          goto LAB_00166cba;
        case 2:
        case 3:
        case 9:
          iVar5 = **(int **)(DAT_00166d10 + 0x1669dc);
          if (uVar2 == 3) {
            piVar3 = (int *)(iVar5 + 0x98);
          }
          else if (uVar2 == 2) {
            piVar3 = (int *)(iVar5 + 0x94);
          }
          else {
            piVar3 = (int *)(iVar5 + 0xac);
          }
          iVar5 = 0;
          for (iVar9 = 0; *(int *)*piVar3 != iVar9; iVar9 = iVar9 + 1) {
            iVar5 = iVar5 + (uint)*(byte *)(((int *)*piVar3)[1] + iVar9);
          }
          goto LAB_00166cdc;
        case 4:
          iVar5 = Status::getKills((Status *)**(undefined4 **)(DAT_00166d1c + 0x166a04));
          goto LAB_00166c6e;
        case 5:
          iVar5 = *(int *)(**(int **)(DAT_00166d20 + 0x166a12) + 0x9c);
          break;
        case 6:
          iVar5 = *(int *)(**(int **)(DAT_00166d24 + 0x166a20) + 0xa0);
          break;
        case 7:
          iVar5 = *(int *)(**(int **)(DAT_00166d28 + 0x166a2e) + 0xa4);
          break;
        case 8:
          iVar5 = *(int *)(**(int **)(DAT_00166d2c + 0x166a3c) + 0xa8);
          break;
        case 10:
          iVar5 = *(int *)(**(int **)(DAT_00166d18 + 0x166a4a) + 0xb0);
          break;
        case 0xb:
          iVar5 = Status::getStationsVisited((Status *)**(undefined4 **)(DAT_00166d30 + 0x166a58));
          goto LAB_00166c6e;
        case 0xc:
          iVar9 = 0;
          for (iVar5 = 0; iVar5 != 0x16; iVar5 = iVar5 + 1) {
            iVar9 = iVar9 + (uint)*(byte *)(*(int *)(*(int *)(**(int **)(DAT_00166d14 + 0x166a6a) +
                                                             0xb4) + 4) + iVar5);
          }
          if ((int)uVar7 <= iVar9) goto LAB_00166cba;
          goto switchD_00166978_default;
        case 0xd:
          iVar5 = 0;
          for (uVar2 = 0; puVar4 = *(uint **)(**(int **)(DAT_00166d34 + 0x166aa2) + 0x18),
              uVar2 < *puVar4; uVar2 = uVar2 + 1) {
            iVar9 = BluePrint::isUnlocked(*(BluePrint **)(puVar4[1] + uVar2 * 4));
            iVar5 = iVar5 + iVar9;
          }
          iVar9 = iVar5 - uVar7;
          goto LAB_00166cde;
        case 0xe:
          iVar9 = 0;
          iVar5 = 0;
          piVar3 = *(int **)(**(int **)(DAT_00166d38 + 0x166aba) + 0x18);
          while (*piVar3 != iVar9) {
            iVar1 = iVar9 * 4;
            iVar9 = iVar9 + 1;
            if (0 < *(int *)(*(int *)(piVar3[1] + iVar1) + 0xc)) {
              iVar5 = iVar5 + 1;
            }
          }
LAB_00166cdc:
          iVar9 = iVar5 - uVar7;
LAB_00166cde:
          if (iVar9 < 0 != SBORROW4(iVar5,uVar7)) goto switchD_00166978_default;
          goto LAB_00166cba;
        case 0xf:
          uVar7 = DAT_00166d0c * uVar7;
          iVar9 = (int)uVar7 >> 0x1f;
          uVar13 = Status::getPlayingTime();
          iVar5 = (int)((ulonglong)uVar13 >> 0x20);
          bVar12 = uVar7 < (uint)uVar13;
          if ((int)((iVar9 - iVar5) - (uint)bVar12) < 0 !=
              (SBORROW4(iVar9,iVar5) != SBORROW4(iVar9 - iVar5,(uint)bVar12))) goto LAB_00166cba;
          goto switchD_00166978_default;
        case 0x10:
          iVar5 = Status::getMissionCount((Status *)**(undefined4 **)(DAT_00166d40 + 0x166afe));
          break;
        case 0x11:
          iVar5 = Status::getJumpgateUsed((Status *)**(undefined4 **)(DAT_00166d44 + 0x166b0c));
          goto LAB_00166c6e;
        case 0x12:
          iVar5 = *(int *)(**(int **)(DAT_00166d48 + 0x166b1a) + 0xb8);
          break;
        case 0x13:
          uVar13 = FUN_001aae54(*(undefined4 *)(**(int **)(DAT_00166d4c + 0x166b2e) + 0xc0),
                                *(undefined4 *)(**(int **)(DAT_00166d4c + 0x166b2e) + 0xc4),60000,0)
          ;
          iVar9 = (int)((ulonglong)uVar13 >> 0x20);
          bVar12 = (uint)uVar13 < uVar7;
          iVar5 = (int)uVar7 >> 0x1f;
          if ((int)((iVar9 - iVar5) - (uint)bVar12) < 0 ==
              (SBORROW4(iVar9,iVar5) != SBORROW4(iVar9 - iVar5,(uint)bVar12))) goto LAB_00166cba;
          goto switchD_00166978_default;
        case 0x14:
          iVar5 = *(int *)(**(int **)(DAT_00166d50 + 0x166b4a) + 200);
          break;
        case 0x15:
          iVar5 = *(int *)(**(int **)(DAT_00166d54 + 0x166b58) + 0xcc);
          break;
        case 0x16:
          if (this[0x18] == (Achievements)0x0) {
            iVar10 = uVar6 + 1;
          }
          goto switchD_00166978_default;
        case 0x17:
          iVar5 = *(int *)(this + 0x14);
          goto LAB_00166c6e;
        case 0x18:
          iVar5 = Status::getCapturedCrates((Status *)**(undefined4 **)(DAT_00166d58 + 0x166b7a));
          goto LAB_00166c6e;
        case 0x19:
          iVar5 = *(int *)(this + 0x1c);
          goto LAB_00166c6e;
        case 0x1a:
          iVar5 = *(int *)(**(int **)(DAT_00166d5c + 0x166b8e) + 0xd0);
          break;
        case 0x1b:
          iVar5 = *(int *)(**(int **)(DAT_00166d60 + 0x166b9c) + 0xd4);
          break;
        case 0x1c:
          Status::getStanding();
          uVar7 = Standing::isEnemyWithAnyone();
          goto LAB_00166cb8;
        case 0x1d:
          iVar5 = *(int *)(**(int **)(DAT_00166d68 + 0x166bbc) + 0xd8);
          break;
        case 0x1e:
          uVar7 = Status::gameWon();
          goto LAB_00166cb8;
        case 0x1f:
          iVar5 = *(int *)(**(int **)(DAT_00166d70 + 0x166bd8) + 0xdc);
          break;
        case 0x20:
          iVar5 = *(int *)(**(int **)(DAT_00166d74 + 0x166be6) + 0xe0);
          break;
        case 0x21:
          iVar5 = *(int *)(**(int **)(DAT_00166d78 + 0x166bf4) + 0xe8);
          break;
        case 0x22:
          iVar5 = *(int *)(**(int **)(DAT_00166d7c + 0x166c02) + 0xec);
          break;
        case 0x23:
          uVar7 = 0;
          do {
            if (0x2b < uVar7) goto LAB_00166cba;
            iVar5 = uVar7 * 4;
            uVar7 = uVar7 + 1;
          } while (*(int *)(*(int *)this + iVar5) != 0);
          goto switchD_00166978_default;
        case 0x24:
          Status::getShip();
          iVar5 = Ship::getMaxLoad();
          break;
        case 0x25:
          puVar4 = (uint *)Station::getShips();
          if ((puVar4 != (uint *)0x0) && (uVar7 <= *puVar4)) goto LAB_00166cba;
          goto switchD_00166978_default;
        case 0x26:
          uVar7 = (uint)*(byte *)(**(int **)(DAT_00166d88 + 0x166c5a) + 0x128);
          goto LAB_00166cb8;
        case 0x27:
          iVar5 = *(int *)(**(int **)(DAT_00166d8c + 0x166c68) + 0x118);
LAB_00166c6e:
          if (iVar5 < (int)uVar7) goto switchD_00166978_default;
          goto LAB_00166cba;
        case 0x28:
          uVar7 = (uint)*(byte *)(**(int **)(DAT_00166d90 + 0x166c7a) + 0x120);
          goto LAB_00166cb8;
        case 0x29:
          uVar7 = (uint)*(byte *)(**(int **)(DAT_00166d94 + 0x166c88) + 0x130);
          goto LAB_00166cb8;
        case 0x2a:
          uVar7 = (uint)*(byte *)(**(int **)(DAT_00166d98 + 0x166c96) + 0x138);
          goto LAB_00166cb8;
        case 0x2b:
          uVar7 = (uint)*(byte *)(**(int **)(DAT_00166d9c + 0x166ca4) + 0x140);
          goto LAB_00166cb8;
        case 0x2c:
          uVar7 = (uint)*(byte *)(**(int **)(DAT_00166da0 + 0x166cb2) + 0x148);
LAB_00166cb8:
          if (uVar7 != 0) goto LAB_00166cba;
        default:
          goto switchD_00166978_default;
        }
        if ((int)uVar7 < iVar5) {
LAB_00166cba:
          iVar10 = uVar6 + 1;
        }
switchD_00166978_default:
      }
      iVar5 = *(int *)(*(int *)this + uVar11 * 4);
      if ((iVar10 < iVar5) || (iVar5 == 0)) goto LAB_00166940;
    }
    uVar11 = uVar11 + 1;
  } while( true );
}

```

## target disasm
```
  00166914: push {r4,r5,r6,r7,lr}
  00166916: add r7,sp,#0xc
  00166918: push {r5,r6,r7,r8,r9,r10,r11}
  0016691c: str r1,[sp,#0x4]
  0016691e: mov r9,r0
  00166920: blx 0x0007765c
  00166924: ldr r6,[0x00166c50]
  00166926: mov.w r11,#0x0
  0016692a: add r6,pc
  0016692c: str r6,[sp,#0x8]
  0016692e: b 0x00166cfc
  00166930: ldr.w r0,[r9,#0x0]
  00166934: mov.w r10,#0x0
  00166938: ldr.w r0,[r0,r11,lsl #0x2]
  0016693c: cmp r0,#0x1
  0016693e: bne 0x0016694a
  00166940: ldr.w r0,[r9,#0x4]
  00166944: str.w r10,[r0,r11,lsl #0x2]
  00166948: b 0x00166cf8
  0016694a: movs r4,#0x0
  0016694c: cmp r4,#0x2
  0016694e: bhi.w 0x00166ce4
  00166952: add.w r0,r11,r11, lsl #0x1
  00166956: add.w r0,r6,r0, lsl #0x2
  0016695a: ldr.w r5,[r0,r4,lsl #0x2]
  0016695e: cmp r5,#0x0
  00166960: blt.w 0x00166cbe
  00166964: cmp.w r10,#0x0
  00166968: bgt.w 0x00166ce4
  0016696c: bic r0,r11,#0x80000000
  00166970: subs r1,r0,#0x1
  00166972: cmp r1,#0x2b
  00166974: bhi.w 0x00166cbe
  00166978: tbh [pc,r1]
  001669d4: ldr r1,[0x00166d10]
  001669d6: cmp r0,#0x3
  001669d8: add r1,pc
  001669da: ldr r1,[r1,#0x0]
  001669dc: ldr r1,[r1,#0x0]
  001669de: beq.w 0x00166cc2
  001669e2: cmp r0,#0x2
  001669e4: ite ne
  001669e6: add.ne.w r0,r1,#0xac
  001669ea: add.eq.w r0,r1,#0x94
  001669ee: b 0x00166cc6
  001669f0: ldr r0,[sp,#0x4]
  001669f2: blx 0x00077668
  001669f6: cmp r0,r5
  001669f8: ble.w 0x00166cba
  001669fc: b 0x00166cbe
  001669fe: ldr r0,[0x00166d1c]
  00166a00: add r0,pc
  00166a02: ldr r0,[r0,#0x0]
  00166a04: ldr r0,[r0,#0x0]
  00166a06: blx 0x00074a4c
  00166a0a: b 0x00166c6e
  00166a0c: ldr r0,[0x00166d20]
  00166a0e: add r0,pc
  00166a10: ldr r0,[r0,#0x0]
  00166a12: ldr r0,[r0,#0x0]
  00166a14: ldr.w r0,[r0,#0x9c]
  00166a18: b 0x00166c30
  00166a1a: ldr r0,[0x00166d24]
  00166a1c: add r0,pc
  00166a1e: ldr r0,[r0,#0x0]
  00166a20: ldr r0,[r0,#0x0]
  00166a22: ldr.w r0,[r0,#0xa0]
  00166a26: b 0x00166c30
  00166a28: ldr r0,[0x00166d28]
  00166a2a: add r0,pc
  00166a2c: ldr r0,[r0,#0x0]
  00166a2e: ldr r0,[r0,#0x0]
  00166a30: ldr.w r0,[r0,#0xa4]
  00166a34: b 0x00166c30
  00166a36: ldr r0,[0x00166d2c]
  00166a38: add r0,pc
  00166a3a: ldr r0,[r0,#0x0]
  00166a3c: ldr r0,[r0,#0x0]
  00166a3e: ldr.w r0,[r0,#0xa8]
  00166a42: b 0x00166c30
  00166a44: ldr r0,[0x00166d18]
  00166a46: add r0,pc
  00166a48: ldr r0,[r0,#0x0]
  00166a4a: ldr r0,[r0,#0x0]
  00166a4c: ldr.w r0,[r0,#0xb0]
  00166a50: b 0x00166c30
  00166a52: ldr r0,[0x00166d30]
  00166a54: add r0,pc
  00166a56: ldr r0,[r0,#0x0]
  00166a58: ldr r0,[r0,#0x0]
  00166a5a: blx 0x00074a7c
  00166a5e: b 0x00166c6e
  00166a60: ldr r0,[0x00166d14]
  00166a62: movs r1,#0x0
  00166a64: movs r2,#0x0
  00166a66: add r0,pc
  00166a68: ldr r0,[r0,#0x0]
  00166a6a: ldr r0,[r0,#0x0]
  00166a6c: ldr.w r0,[r0,#0xb4]
  00166a70: ldr r0,[r0,#0x4]
  00166a72: b 0x00166a7a
  00166a74: ldrb r3,[r0,r1]
  00166a76: adds r1,#0x1
  00166a78: add r2,r3
  00166a7a: cmp r1,#0x16
  00166a7c: bne 0x00166a74
  00166a7e: cmp r2,r5
  00166a80: bge.w 0x00166cba
  00166a84: b 0x00166cbe
  00166a86: movs r6,#0x0
  00166a88: mov.w r8,#0x0
  00166a8c: b 0x00166a9c
  00166a8e: ldr r0,[r0,#0x4]
  00166a90: ldr.w r0,[r0,r6,lsl #0x2]
  00166a94: blx 0x000739a8
  00166a98: add r8,r0
  00166a9a: adds r6,#0x1
  00166a9c: ldr r0,[0x00166d34]
  00166a9e: add r0,pc
  00166aa0: ldr r0,[r0,#0x0]
  00166aa2: ldr r0,[r0,#0x0]
  00166aa4: ldr r0,[r0,#0x18]
  00166aa6: ldr r1,[r0,#0x0]
  00166aa8: cmp r6,r1
  00166aaa: bcc 0x00166a8e
  00166aac: cmp r8,r5
  00166aae: b 0x00166cde
  00166ab0: ldr r0,[0x00166d38]
  00166ab2: movs r2,#0x0
  00166ab4: movs r3,#0x0
  00166ab6: add r0,pc
  00166ab8: ldr r0,[r0,#0x0]
  00166aba: ldr r0,[r0,#0x0]
  00166abc: ldr r0,[r0,#0x18]
  00166abe: ldr r1,[r0,#0x0]
  00166ac0: b 0x00166ad2
  00166ac2: ldr r6,[r0,#0x4]
  00166ac4: ldr.w r6,[r6,r2,lsl #0x2]
  00166ac8: adds r2,#0x1
  00166aca: ldr r6,[r6,#0xc]
  00166acc: cmp r6,#0x0
  00166ace: it gt
  00166ad0: add.gt r3,#0x1
  00166ad2: cmp r1,r2
  00166ad4: bne 0x00166ac2
  00166ad6: b 0x00166cdc
  00166ad8: ldr r1,[0x00166d3c]
  00166ada: ldr r0,[0x00166d0c]
  00166adc: add r1,pc
  00166ade: muls r5,r0
  00166ae0: ldr r0,[r1,#0x0]
  00166ae2: ldr r0,[r0,#0x0]
  00166ae4: asrs r6,r5,#0x1f
  00166ae6: blx 0x00071710
  00166aea: subs r0,r5,r0
  00166aec: sbcs.w r0,r6,r1
  00166af0: ldr r6,[sp,#0x8]
  00166af2: blt.w 0x00166cba
  00166af6: b 0x00166cbe
  00166af8: ldr r0,[0x00166d40]
  00166afa: add r0,pc
  00166afc: ldr r0,[r0,#0x0]
  00166afe: ldr r0,[r0,#0x0]
  00166b00: blx 0x00074a58
  00166b04: b 0x00166c30
  00166b06: ldr r0,[0x00166d44]
  00166b08: add r0,pc
  00166b0a: ldr r0,[r0,#0x0]
  00166b0c: ldr r0,[r0,#0x0]
  00166b0e: blx 0x00074a94
  00166b12: b 0x00166c6e
  00166b14: ldr r0,[0x00166d48]
  00166b16: add r0,pc
  00166b18: ldr r0,[r0,#0x0]
  00166b1a: ldr r0,[r0,#0x0]
  00166b1c: ldr.w r0,[r0,#0xb8]
  00166b20: b 0x00166c30
  00166b22: ldr r0,[0x00166d4c]
  00166b24: movw r2,#0xea60
  00166b28: movs r3,#0x0
  00166b2a: add r0,pc
  00166b2c: ldr r0,[r0,#0x0]
  00166b2e: ldr r0,[r0,#0x0]
  00166b30: ldrd r0,r1,[r0,#0xc0]
  00166b34: bl 0x001aae54
  00166b38: subs r0,r0,r5
  00166b3a: sbcs.w r0,r1,r5, asr #0x1f
  00166b3e: bge.w 0x00166cba
  00166b42: b 0x00166cbe
  00166b44: ldr r0,[0x00166d50]
  00166b46: add r0,pc
  00166b48: ldr r0,[r0,#0x0]
  00166b4a: ldr r0,[r0,#0x0]
  00166b4c: ldr.w r0,[r0,#0xc8]
  00166b50: b 0x00166c30
  00166b52: ldr r0,[0x00166d54]
  00166b54: add r0,pc
  00166b56: ldr r0,[r0,#0x0]
  00166b58: ldr r0,[r0,#0x0]
  00166b5a: ldr.w r0,[r0,#0xcc]
  00166b5e: b 0x00166c30
  00166b60: ldrb.w r0,[r9,#0x18]
  00166b64: cmp r0,#0x0
  00166b66: it eq
  00166b68: add.eq.w r10,r4,#0x1
  00166b6c: b 0x00166cbe
  00166b6e: ldr.w r0,[r9,#0x14]
  00166b72: b 0x00166c6e
  00166b74: ldr r0,[0x00166d58]
  00166b76: add r0,pc
  00166b78: ldr r0,[r0,#0x0]
  00166b7a: ldr r0,[r0,#0x0]
  00166b7c: blx 0x00074aa0
  00166b80: b 0x00166c6e
  00166b82: ldr.w r0,[r9,#0x1c]
  00166b86: b 0x00166c6e
  00166b88: ldr r0,[0x00166d5c]
  00166b8a: add r0,pc
  00166b8c: ldr r0,[r0,#0x0]
  00166b8e: ldr r0,[r0,#0x0]
  00166b90: ldr.w r0,[r0,#0xd0]
  00166b94: b 0x00166c30
  00166b96: ldr r0,[0x00166d60]
  00166b98: add r0,pc
  00166b9a: ldr r0,[r0,#0x0]
  00166b9c: ldr r0,[r0,#0x0]
  00166b9e: ldr.w r0,[r0,#0xd4]
  00166ba2: b 0x00166c30
  00166ba4: ldr r0,[0x00166d64]
  00166ba6: add r0,pc
  00166ba8: ldr r0,[r0,#0x0]
  00166baa: ldr r0,[r0,#0x0]
  00166bac: blx 0x00071b84
  00166bb0: blx 0x00077674
  00166bb4: b 0x00166cb8
  00166bb6: ldr r0,[0x00166d68]
  00166bb8: add r0,pc
  00166bba: ldr r0,[r0,#0x0]
  00166bbc: ldr r0,[r0,#0x0]
  00166bbe: ldr.w r0,[r0,#0xd8]
  00166bc2: b 0x00166c30
  00166bc4: ldr r0,[0x00166d6c]
  00166bc6: add r0,pc
  00166bc8: ldr r0,[r0,#0x0]
  00166bca: ldr r0,[r0,#0x0]
  00166bcc: blx 0x00073c48
  00166bd0: b 0x00166cb8
  00166bd2: ldr r0,[0x00166d70]
  00166bd4: add r0,pc
  00166bd6: ldr r0,[r0,#0x0]
  00166bd8: ldr r0,[r0,#0x0]
  00166bda: ldr.w r0,[r0,#0xdc]
  00166bde: b 0x00166c30
  00166be0: ldr r0,[0x00166d74]
  00166be2: add r0,pc
  00166be4: ldr r0,[r0,#0x0]
  00166be6: ldr r0,[r0,#0x0]
  00166be8: ldr.w r0,[r0,#0xe0]
  00166bec: b 0x00166c30
  00166bee: ldr r0,[0x00166d78]
  00166bf0: add r0,pc
  00166bf2: ldr r0,[r0,#0x0]
  00166bf4: ldr r0,[r0,#0x0]
  00166bf6: ldr.w r0,[r0,#0xe8]
  00166bfa: b 0x00166c30
  00166bfc: ldr r0,[0x00166d7c]
  00166bfe: add r0,pc
  00166c00: ldr r0,[r0,#0x0]
  00166c02: ldr r0,[r0,#0x0]
  00166c04: ldr.w r0,[r0,#0xec]
  00166c08: b 0x00166c30
  00166c0a: ldr.w r0,[r9,#0x0]
  00166c0e: movs r1,#0x0
  00166c10: cmp r1,#0x2b
  00166c12: bhi 0x00166cba
  00166c14: ldr.w r2,[r0,r1,lsl #0x2]
  00166c18: adds r1,#0x1
  00166c1a: cmp r2,#0x0
  00166c1c: bne 0x00166c10
  00166c1e: b 0x00166cbe
  00166c20: ldr r0,[0x00166d80]
  00166c22: add r0,pc
  00166c24: ldr r0,[r0,#0x0]
  00166c26: ldr r0,[r0,#0x0]
  00166c28: blx 0x00071a58
  00166c2c: blx 0x00072c04
  00166c30: cmp r0,r5
  00166c32: bgt 0x00166cba
  00166c34: b 0x00166cbe
  00166c36: ldr r0,[0x00166d84]
  00166c38: add r0,pc
  00166c3a: ldr r0,[r0,#0x0]
  00166c3c: ldr r0,[r0,#0x0]
  00166c3e: ldr.w r0,[r0,#0x14c]
  00166c42: blx 0x00073708
  00166c46: cbz r0,0x00166cbe
  00166c48: ldr r0,[r0,#0x0]
  00166c4a: cmp r0,r5
  00166c4c: bcs 0x00166cba
  00166c4e: b 0x00166cbe
  00166c54: ldr r0,[0x00166d88]
  00166c56: add r0,pc
  00166c58: ldr r0,[r0,#0x0]
  00166c5a: ldr r0,[r0,#0x0]
  00166c5c: ldrb.w r0,[r0,#0x128]
  00166c60: b 0x00166cb8
  00166c62: ldr r0,[0x00166d8c]
  00166c64: add r0,pc
  00166c66: ldr r0,[r0,#0x0]
  00166c68: ldr r0,[r0,#0x0]
  00166c6a: ldr.w r0,[r0,#0x118]
  00166c6e: cmp r0,r5
  00166c70: bge 0x00166cba
  00166c72: b 0x00166cbe
  00166c74: ldr r0,[0x00166d90]
  00166c76: add r0,pc
  00166c78: ldr r0,[r0,#0x0]
  00166c7a: ldr r0,[r0,#0x0]
  00166c7c: ldrb.w r0,[r0,#0x120]
  00166c80: b 0x00166cb8
  00166c82: ldr r0,[0x00166d94]
  00166c84: add r0,pc
  00166c86: ldr r0,[r0,#0x0]
  00166c88: ldr r0,[r0,#0x0]
  00166c8a: ldrb.w r0,[r0,#0x130]
  00166c8e: b 0x00166cb8
  00166c90: ldr r0,[0x00166d98]
  00166c92: add r0,pc
  00166c94: ldr r0,[r0,#0x0]
  00166c96: ldr r0,[r0,#0x0]
  00166c98: ldrb.w r0,[r0,#0x138]
  00166c9c: b 0x00166cb8
  00166c9e: ldr r0,[0x00166d9c]
  00166ca0: add r0,pc
  00166ca2: ldr r0,[r0,#0x0]
  00166ca4: ldr r0,[r0,#0x0]
  00166ca6: ldrb.w r0,[r0,#0x140]
  00166caa: b 0x00166cb8
  00166cac: ldr r0,[0x00166da0]
  00166cae: add r0,pc
  00166cb0: ldr r0,[r0,#0x0]
  00166cb2: ldr r0,[r0,#0x0]
  00166cb4: ldrb.w r0,[r0,#0x148]
  00166cb8: cbz r0,0x00166cbe
  00166cba: add.w r10,r4,#0x1
  00166cbe: adds r4,#0x1
  00166cc0: b 0x0016694c
  00166cc2: add.w r0,r1,#0x98
  00166cc6: ldr r0,[r0,#0x0]
  00166cc8: movs r2,#0x0
  00166cca: movs r3,#0x0
  00166ccc: ldr r1,[r0,#0x0]
  00166cce: b 0x00166cd8
  00166cd0: ldr r6,[r0,#0x4]
  00166cd2: ldrb r6,[r6,r2]
  00166cd4: adds r2,#0x1
  00166cd6: add r3,r6
  00166cd8: cmp r1,r2
  00166cda: bne 0x00166cd0
  00166cdc: cmp r3,r5
  00166cde: ldr r6,[sp,#0x8]
  00166ce0: bge 0x00166cba
  00166ce2: b 0x00166cbe
  00166ce4: ldr.w r0,[r9,#0x0]
  00166ce8: ldr.w r0,[r0,r11,lsl #0x2]
  00166cec: cmp r0,r10
  00166cee: bgt.w 0x00166940
  00166cf2: cmp r0,#0x0
  00166cf4: beq.w 0x00166940
  00166cf8: add.w r11,r11,#0x1
  00166cfc: cmp.w r11,#0x2d
  00166d00: bne.w 0x00166930
  00166d04: pop.w {r1,r2,r3,r8,r9,r10,r11}
  00166d08: pop {r4,r5,r6,r7,pc}
```
