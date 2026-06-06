# Level::updateOrbit
elf 0xc5208 body 940
Sig: undefined __thiscall updateOrbit(Level * this, int param_1)

## decompile
```c

/* Level::updateOrbit(int) */

void __thiscall Level::updateOrbit(Level *this,int param_1)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  SolarSystem *pSVar7;
  int iVar8;
  undefined4 uVar9;
  float unaff_r4;
  float unaff_r5;
  KIPlayer *this_00;
  float unaff_r6;
  uint uVar10;
  int *piVar11;
  int *piVar12;
  code *pcVar13;
  undefined4 uVar14;
  int iVar15;
  undefined4 *puVar16;
  uint in_fpscr;
  float fVar17;
  uint uVar18;
  
  piVar12 = *(int **)(DAT_000d5588 + 0xd5218);
  iVar3 = *piVar12;
  iVar4 = *(int *)(this + 0x178) + param_1;
  *(int *)(this + 0x174) = *(int *)(this + 0x174) + param_1;
  *(int *)(this + 0x178) = iVar4;
  if (this[0x18a] != (Level)0x0) {
    iVar4 = Status::getSystem();
    if ((iVar4 != 0) && (*(int *)(*(int *)(this + 0xf0) + 0x18) != 0)) {
      Status::getSystem();
      iVar4 = SolarSystem::getRace();
      alarmAllFriends(this,iVar4,false);
      Status::getSystem();
      SolarSystem::getRace();
      createRadioMessage((int)this,2);
      this[0x18a] = (Level)0x0;
    }
    iVar4 = *(int *)(this + 0x178);
  }
  if (10000 < iVar4) {
    puVar5 = *(uint **)(this + 0xf8);
    *(undefined4 *)(this + 0x178) = 0;
    if (puVar5 != (uint *)0x0) {
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        this_00 = *(KIPlayer **)(puVar5[1] + uVar10 * 4);
        iVar4 = KIPlayer::isJumper(this_00);
        if ((((iVar4 != 0) && (iVar4 = KIPlayer::isDead(), iVar4 != 0)) &&
            (iVar4 = Player::isActive(), iVar4 == 0)) && (this_00[0x42] == (KIPlayer)0x0)) {
          (**(code **)(*(int *)this_00 + 0x18))(this_00);
          if ((int)uVar10 < *(int *)(this + 0x160) + *(int *)(this + 0x164)) {
            iVar4 = *(int *)this_00;
            uVar14 = 0;
            uVar9 = 0;
          }
          else {
            iVar4 = *(int *)this_00;
            uVar14 = DAT_000d55b8;
            uVar9 = DAT_000d55bc;
          }
          (**(code **)(iVar4 + 0x48))(this_00,uVar14,0,uVar9);
          break;
        }
        puVar5 = *(uint **)(this + 0xf8);
      }
    }
  }
  if (45000 < *(int *)(this + 0x174)) {
    iVar4 = 0;
    puVar5 = *(uint **)(this + 0xf8);
    *(undefined4 *)(this + 0x174) = 0;
    if (0 < *(int *)(this + 0x16c)) {
      if (puVar5 == (uint *)0x0) goto LAB_000d5572;
      iVar4 = 0;
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        if (((*puVar5 - *(int *)(this + 0x16c) <= uVar10) &&
            (iVar6 = KIPlayer::isWingMan(), iVar6 == 0)) && (iVar6 = KIPlayer::isDead(), iVar6 != 0)
           ) {
          uVar18 = Player::isActive();
          iVar4 = iVar4 + (uVar18 ^ 1);
        }
        puVar5 = *(uint **)(this + 0xf8);
      }
    }
    if (puVar5 != (uint *)0x0) {
      uVar18 = 0;
      uVar14 = *(undefined4 *)(DAT_000d55c4 + 0xd5354);
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        piVar11 = *(int **)(puVar5[1] + uVar10 * 4);
        if (((0 < *(int *)(this + 0x160)) && ((int)uVar10 < *(int *)(this + 0x160))) &&
           ((iVar6 = KIPlayer::isDead(), iVar6 != 0 &&
            ((iVar6 = Player::isActive(), iVar6 == 0 && (*(char *)((int)piVar11 + 0x42) == '\0')))))
           ) {
          (**(code **)(*piVar11 + 0x18))(piVar11);
          (**(code **)(*piVar11 + 0x48))(piVar11,0,0,0,piVar12,uVar18,uVar14);
        }
        if ((((1 < iVar4) && (*(int *)(this + 0x184) < 2)) && (0 < *(int *)(this + 0x16c))) &&
           ((uint)(**(int **)(this + 0xf8) - *(int *)(this + 0x16c)) <= uVar10)) {
          iVar15 = piVar11[10];
          iVar6 = Status::inAlienOrbit();
          if (iVar6 == 0) {
            pSVar7 = (SolarSystem *)Status::getSystem();
            iVar6 = SolarSystem::getSecurityLevel(pSVar7);
            bVar1 = iVar6 == 0;
          }
          else {
            bVar1 = false;
          }
          iVar6 = Status::inAlienOrbit();
          if (iVar6 == 0) {
            pSVar7 = (SolarSystem *)Status::getSystem();
            iVar6 = SolarSystem::getSecurityLevel(pSVar7);
            bVar2 = iVar6 == 1;
          }
          else {
            bVar2 = false;
          }
          iVar6 = KIPlayer::isDead();
          if (((iVar6 != 0) && (iVar6 = Player::isActive(), iVar6 == 0)) &&
             (*(char *)((int)piVar11 + 0x42) == '\0')) {
            if ((bool)(iVar15 != 9 & (bVar1 ^ 1U))) {
              if ((!bVar2) || (iVar6 = *(int *)(this + 0x17c), 2 < iVar6)) goto LAB_000d542a;
            }
            else {
              iVar6 = *(int *)(this + 0x17c);
            }
            *(int *)(this + 0x17c) = iVar6 + 1;
            (**(code **)(*piVar11 + 0x18))(piVar11);
            PlayerEgo::getPosition();
            puVar16 = *(undefined4 **)(DAT_000d55c8 + 0xd546a);
            pcVar13 = *(code **)(DAT_000d55cc + 0xd546c);
            iVar6 = (*pcVar13)(*puVar16,60000);
            iVar15 = (*pcVar13)(*puVar16,2);
            iVar8 = -1;
            if (iVar15 == 0) {
              iVar8 = 1;
            }
            fVar17 = (float)VectorSignedToFloat(iVar8 * (iVar6 + 60000),(byte)(in_fpscr >> 0x16) & 3
                                               );
            unaff_r4 = unaff_r4 + fVar17;
            iVar6 = (*pcVar13)(*puVar16,5000);
            iVar15 = (*pcVar13)(*puVar16,2);
            iVar8 = -1;
            if (iVar15 == 0) {
              iVar8 = 1;
            }
            fVar17 = (float)VectorSignedToFloat(iVar8 * (iVar6 + 5000),(byte)(in_fpscr >> 0x16) & 3)
            ;
            unaff_r5 = unaff_r5 + fVar17;
            iVar6 = (*pcVar13)(*puVar16,60000);
            iVar15 = (*pcVar13)(*puVar16,2);
            iVar8 = -1;
            if (iVar15 == 0) {
              iVar8 = 1;
            }
            fVar17 = (float)VectorSignedToFloat(iVar8 * (iVar6 + 60000),(byte)(in_fpscr >> 0x16) & 3
                                               );
            unaff_r6 = unaff_r6 + fVar17;
            (**(code **)(*piVar11 + 0x48))(piVar11,unaff_r4,unaff_r5,unaff_r6);
            uVar18 = 1;
          }
        }
LAB_000d542a:
        puVar5 = *(uint **)(this + 0xf8);
      }
      if ((uVar18 & 1) != 0) {
        *(int *)(this + 0x184) = *(int *)(this + 0x184) + 1;
      }
    }
  }
LAB_000d5572:
  if (*piVar12 != iVar3) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000d5208: push {r4,r5,r6,r7,lr}
  000d520a: add r7,sp,#0xc
  000d520c: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000d5210: mov r4,r0
  000d5212: ldr r0,[0x000d5588]
  000d5214: add r0,pc
  000d5216: ldr.w r8,[r0,#0x0]
  000d521a: ldr.w r0,[r8,#0x0]
  000d521e: str r0,[sp,#0x18]
  000d5220: ldrd r0,r2,[r4,#0x174]
  000d5224: ldrb.w r3,[r4,#0x18a]
  000d5228: adds r6,r0,r1
  000d522a: adds r0,r2,r1
  000d522c: cmp r3,#0x0
  000d522e: strd r6,r0,[r4,#0x174]
  000d5232: beq 0x000d527c
  000d5234: ldr r0,[0x000d55c0]
  000d5236: add r0,pc
  000d5238: ldr r5,[r0,#0x0]
  000d523a: ldr r0,[r5,#0x0]
  000d523c: blx 0x00071a10
  000d5240: cbz r0,0x000d5278
  000d5242: ldr.w r0,[r4,#0xf0]
  000d5246: ldr r0,[r0,#0x18]
  000d5248: cbz r0,0x000d5278
  000d524a: ldr r0,[r5,#0x0]
  000d524c: blx 0x00071a10
  000d5250: blx 0x00071950
  000d5254: mov r1,r0
  000d5256: mov r0,r4
  000d5258: movs r2,#0x0
  000d525a: movs r6,#0x0
  000d525c: blx 0x00072eec
  000d5260: ldr r0,[r5,#0x0]
  000d5262: blx 0x00071a10
  000d5266: blx 0x00071950
  000d526a: mov r2,r0
  000d526c: mov r0,r4
  000d526e: movs r1,#0x2
  000d5270: blx 0x00072754
  000d5274: strb.w r6,[r4,#0x18a]
  000d5278: ldr.w r0,[r4,#0x178]
  000d527c: movw r1,#0x2711
  000d5280: cmp r0,r1
  000d5282: blt 0x000d52c8
  000d5284: ldr.w r0,[r4,#0xf8]
  000d5288: movs r1,#0x0
  000d528a: str.w r1,[r4,#0x178]
  000d528e: cbz r0,0x000d52c8
  000d5290: movs r6,#0x0
  000d5292: b 0x000d52c2
  000d5294: ldr r0,[r0,#0x4]
  000d5296: ldr.w r5,[r0,r6,lsl #0x2]
  000d529a: mov r0,r5
  000d529c: blx 0x00074464
  000d52a0: cbz r0,0x000d52bc
  000d52a2: mov r0,r5
  000d52a4: blx 0x00071ec0
  000d52a8: cbz r0,0x000d52bc
  000d52aa: ldr r0,[r5,#0x4]
  000d52ac: blx 0x00071f14
  000d52b0: cbnz r0,0x000d52bc
  000d52b2: ldrb.w r0,[r5,#0x42]
  000d52b6: cmp r0,#0x0
  000d52b8: beq.w 0x000d558c
  000d52bc: ldr.w r0,[r4,#0xf8]
  000d52c0: adds r6,#0x1
  000d52c2: ldr r1,[r0,#0x0]
  000d52c4: cmp r6,r1
  000d52c6: bcc 0x000d5294
  000d52c8: ldr.w r0,[r4,#0x174]
  000d52cc: movw r1,#0xafc9
  000d52d0: cmp r0,r1
  000d52d2: blt.w 0x000d5572
  000d52d6: ldr.w r1,[r4,#0x16c]
  000d52da: movs r5,#0x0
  000d52dc: ldr.w r0,[r4,#0xf8]
  000d52e0: cmp r1,#0x1
  000d52e2: str.w r5,[r4,#0x174]
  000d52e6: blt 0x000d533c
  000d52e8: cmp r0,#0x0
  000d52ea: beq.w 0x000d5572
  000d52ee: movs r6,#0x0
  000d52f0: movs r5,#0x0
  000d52f2: b 0x000d5336
  000d52f4: ldr.w r2,[r4,#0x16c]
  000d52f8: subs r1,r1,r2
  000d52fa: cmp r6,r1
  000d52fc: bcc 0x000d5330
  000d52fe: ldr r0,[r0,#0x4]
  000d5300: ldr.w r0,[r0,r6,lsl #0x2]
  000d5304: blx 0x00072ed4
  000d5308: cbnz r0,0x000d5330
  000d530a: ldr.w r0,[r4,#0xf8]
  000d530e: ldr r0,[r0,#0x4]
  000d5310: ldr.w r0,[r0,r6,lsl #0x2]
  000d5314: blx 0x00071ec0
  000d5318: cbz r0,0x000d5330
  000d531a: ldr.w r0,[r4,#0xf8]
  000d531e: ldr r0,[r0,#0x4]
  000d5320: ldr.w r0,[r0,r6,lsl #0x2]
  000d5324: ldr r0,[r0,#0x4]
  000d5326: blx 0x00071f14
  000d532a: eor r0,r0,#0x1
  000d532e: add r5,r0
  000d5330: ldr.w r0,[r4,#0xf8]
  000d5334: adds r6,#0x1
  000d5336: ldr r1,[r0,#0x0]
  000d5338: cmp r6,r1
  000d533a: bcc 0x000d52f4
  000d533c: cmp r0,#0x0
  000d533e: beq.w 0x000d5572
  000d5342: movs r1,#0x0
  000d5344: str.w r8,[sp,#0x0]
  000d5348: str r1,[sp,#0x4]
  000d534a: mov.w r9,#0x0
  000d534e: ldr r1,[0x000d55c4]
  000d5350: add r1,pc
  000d5352: ldr.w r10,[r1,#0x0]
  000d5356: str.w r10,[sp,#0x8]
  000d535a: ldr r1,[r0,#0x0]
  000d535c: cmp r9,r1
  000d535e: bcs.w 0x000d555e
  000d5362: ldr r1,[r0,#0x4]
  000d5364: ldr.w r0,[r4,#0x160]
  000d5368: ldr.w r6,[r1,r9,lsl #0x2]
  000d536c: cmp r0,#0x1
  000d536e: blt 0x000d53a2
  000d5370: cmp r9,r0
  000d5372: bge 0x000d53a2
  000d5374: mov r0,r6
  000d5376: blx 0x00071ec0
  000d537a: cbz r0,0x000d53a2
  000d537c: ldr r0,[r6,#0x4]
  000d537e: blx 0x00071f14
  000d5382: cbnz r0,0x000d53a2
  000d5384: ldrb.w r0,[r6,#0x42]
  000d5388: cbnz r0,0x000d53a2
  000d538a: ldr r0,[r6,#0x0]
  000d538c: ldr r1,[r0,#0x18]
  000d538e: mov r0,r6
  000d5390: blx r1
  000d5392: ldr r0,[r6,#0x0]
  000d5394: movs r1,#0x0
  000d5396: movs r2,#0x0
  000d5398: movs r3,#0x0
  000d539a: ldr.w r12,[r0,#0x48]
  000d539e: mov r0,r6
  000d53a0: blx r12
  000d53a2: cmp r5,#0x2
  000d53a4: blt 0x000d542a
  000d53a6: ldr.w r0,[r4,#0x184]
  000d53aa: cmp r0,#0x1
  000d53ac: bgt 0x000d542a
  000d53ae: ldr.w r0,[r4,#0x16c]
  000d53b2: cmp r0,#0x1
  000d53b4: blt 0x000d542a
  000d53b6: ldr.w r1,[r4,#0xf8]
  000d53ba: ldr r1,[r1,#0x0]
  000d53bc: subs r0,r1,r0
  000d53be: cmp r9,r0
  000d53c0: bcc 0x000d542a
  000d53c2: ldr.w r0,[r10,#0x0]
  000d53c6: ldr.w r11,[r6,#0x28]
  000d53ca: blx 0x000723d0
  000d53ce: cbz r0,0x000d53d6
  000d53d0: mov.w r8,#0x0
  000d53d4: b 0x000d53ea
  000d53d6: ldr.w r0,[r10,#0x0]
  000d53da: blx 0x00071a10
  000d53de: blx 0x00073ef4
  000d53e2: clz r0,r0
  000d53e6: lsr.w r8,r0,#0x5
  000d53ea: ldr.w r0,[r10,#0x0]
  000d53ee: blx 0x000723d0
  000d53f2: cbz r0,0x000d53fa
  000d53f4: mov.w r10,#0x0
  000d53f8: b 0x000d5410
  000d53fa: ldr.w r0,[r10,#0x0]
  000d53fe: blx 0x00071a10
  000d5402: blx 0x00073ef4
  000d5406: subs r0,#0x1
  000d5408: clz r0,r0
  000d540c: lsr.w r10,r0,#0x5
  000d5410: mov r0,r6
  000d5412: blx 0x00071ec0
  000d5416: cbz r0,0x000d5426
  000d5418: ldr r0,[r6,#0x4]
  000d541a: blx 0x00071f14
  000d541e: cbnz r0,0x000d5426
  000d5420: ldrb.w r0,[r6,#0x42]
  000d5424: cbz r0,0x000d5434
  000d5426: ldr.w r10,[sp,#0x8]
  000d542a: ldr.w r0,[r4,#0xf8]
  000d542e: add.w r9,r9,#0x1
  000d5432: b 0x000d535a
  000d5434: subs.w r0,r11,#0x9
  000d5438: eor r1,r8,#0x1
  000d543c: it ne
  000d543e: mov.ne r0,#0x1
  000d5440: tst r0,r1
  000d5442: bne.w 0x000d5546
  000d5446: ldr.w r0,[r4,#0x17c]
  000d544a: adds r0,#0x1
  000d544c: str.w r0,[r4,#0x17c]
  000d5450: ldr r0,[r6,#0x0]
  000d5452: ldr r1,[r0,#0x18]
  000d5454: mov r0,r6
  000d5456: blx r1
  000d5458: ldr.w r1,[r4,#0xf0]
  000d545c: add r0,sp,#0xc
  000d545e: blx 0x0007264c
  000d5462: ldr r0,[0x000d55c8]
  000d5464: ldr r1,[0x000d55cc]
  000d5466: add r0,pc
  000d5468: add r1,pc
  000d546a: ldr.w r11,[r0,#0x0]
  000d546e: ldr.w r8,[r1,#0x0]
  000d5472: movw r1,#0xea60
  000d5476: ldr.w r0,[r11,#0x0]
  000d547a: blx r8
  000d547c: ldr.w r1,[r11,#0x0]
  000d5480: movw r2,#0xea60
  000d5484: add.w r10,r0,r2
  000d5488: mov r0,r1
  000d548a: movs r1,#0x2
  000d548c: blx r8
  000d548e: cmp r0,#0x0
  000d5490: mov.w r0,#0xffffffff
  000d5494: it eq
  000d5496: mov.eq r0,#0x1
  000d5498: movw r1,#0x1388
  000d549c: mul r0,r0,r10
  000d54a0: vmov s0,r0
  000d54a4: vcvt.f32.s32 s0,s0
  000d54a8: vldr.32 s2,[sp,#0xc]
  000d54ac: ldr.w r0,[r11,#0x0]
  000d54b0: vadd.f32 s0,s2,s0
  000d54b4: vstr.32 s0,[sp,#0xc]
  000d54b8: blx r8
  000d54ba: movw r1,#0x1388
  000d54be: add.w r10,r0,r1
  000d54c2: ldr.w r0,[r11,#0x0]
  000d54c6: movs r1,#0x2
  000d54c8: blx r8
  000d54ca: cmp r0,#0x0
  000d54cc: mov.w r0,#0xffffffff
  000d54d0: it eq
  000d54d2: mov.eq r0,#0x1
  000d54d4: movw r1,#0xea60
  000d54d8: mul r0,r0,r10
  000d54dc: vmov s0,r0
  000d54e0: vcvt.f32.s32 s0,s0
  000d54e4: vldr.32 s2,[sp,#0x10]
  000d54e8: ldr.w r0,[r11,#0x0]
  000d54ec: vadd.f32 s0,s2,s0
  000d54f0: vstr.32 s0,[sp,#0x10]
  000d54f4: blx r8
  000d54f6: ldr.w r1,[r11,#0x0]
  000d54fa: movw r2,#0xea60
  000d54fe: add.w r10,r0,r2
  000d5502: mov r0,r1
  000d5504: movs r1,#0x2
  000d5506: blx r8
  000d5508: cmp r0,#0x0
  000d550a: mov.w r0,#0xffffffff
  000d550e: it eq
  000d5510: mov.eq r0,#0x1
  000d5512: mul r0,r0,r10
  000d5516: ldr.w r10,[sp,#0x8]
  000d551a: vmov s0,r0
  000d551e: vcvt.f32.s32 s0,s0
  000d5522: vldr.32 s2,[sp,#0x14]
  000d5526: vadd.f32 s0,s2,s0
  000d552a: vmov r3,s0
  000d552e: vstr.32 s0,[sp,#0x14]
  000d5532: ldr r0,[r6,#0x0]
  000d5534: ldrd r1,r2,[sp,#0xc]
  000d5538: ldr.w r12,[r0,#0x48]
  000d553c: mov r0,r6
  000d553e: blx r12
  000d5540: movs r0,#0x1
  000d5542: str r0,[sp,#0x4]
  000d5544: b 0x000d542a
  000d5546: cmp.w r10,#0x0
  000d554a: beq.w 0x000d5426
  000d554e: ldr.w r0,[r4,#0x17c]
  000d5552: ldr.w r10,[sp,#0x8]
  000d5556: cmp r0,#0x2
  000d5558: bgt.w 0x000d542a
  000d555c: b 0x000d544a
  000d555e: ldr r0,[sp,#0x4]
  000d5560: ldr.w r8,[sp,#0x0]
  000d5564: lsls r0,r0,#0x1f
  000d5566: ittt ne
  000d5568: ldr.ne.w r0,[r4,#0x184]
  000d556c: add.ne r0,#0x1
  000d556e: str.w.ne r0,[r4,#0x184]
  000d5572: ldr r0,[sp,#0x18]
  000d5574: ldr.w r1,[r8,#0x0]
  000d5578: subs r0,r1,r0
  000d557a: ittt eq
  000d557c: add.eq sp,#0x1c
  000d557e: pop.eq.w {r8,r9,r10,r11}
  000d5582: pop.eq {r4,r5,r6,r7,pc}
  000d5584: blx 0x0006e824
  000d558c: ldr r0,[r5,#0x0]
  000d558e: ldr r1,[r0,#0x18]
  000d5590: mov r0,r5
  000d5592: blx r1
  000d5594: ldrd r0,r1,[r4,#0x160]
  000d5598: add r0,r1
  000d559a: cmp r6,r0
  000d559c: bge 0x000d55a8
  000d559e: ldr r0,[r5,#0x0]
  000d55a0: movs r1,#0x0
  000d55a2: movs r2,#0x0
  000d55a4: movs r3,#0x0
  000d55a6: b 0x000d55b0
  000d55a8: ldr r0,[r5,#0x0]
  000d55aa: movs r2,#0x0
  000d55ac: ldr r1,[0x000d55b8]
  000d55ae: ldr r3,[0x000d55bc]
  000d55b0: ldr r6,[r0,#0x48]
  000d55b2: mov r0,r5
  000d55b4: blx r6
  000d55b6: b 0x000d52c8
```
