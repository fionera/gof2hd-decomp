# Level::enemyDied
elf 0xc4230 body 488
Sig: undefined __stdcall enemyDied(int param_1, bool param_2)

## decompile
```c

/* Level::enemyDied(int, bool) */

void Level::enemyDied(int param_1,bool param_2)

{
  int iVar1;
  undefined4 uVar2;
  Hud *pHVar3;
  int extraout_r1;
  int in_r2;
  int iVar4;
  int *piVar5;
  undefined4 *puVar6;
  uint in_fpscr;
  float fVar7;
  float fVar8;
  float fVar9;
  
  *(int *)(param_1 + 0x118) = *(int *)(param_1 + 0x118) + -1;
  *(int *)(param_1 + 300) = *(int *)(param_1 + 300) + 1;
  if (in_r2 == 0) {
    piVar5 = *(int **)(DAT_000d441c + 0xd4268);
    Status::incKills((Status *)*piVar5);
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 1;
    if (*(int *)(param_1 + 0xf0) != 0) {
      iVar1 = Radar::hasScanner();
      if (iVar1 == 0) {
        puVar6 = *(undefined4 **)(DAT_000d4420 + 0xd4294);
        iVar1 = Achievements::hasMedal((Achievements *)*puVar6,0x28,1);
        if (iVar1 == 0) {
          iVar1 = *piVar5;
          iVar4 = *(int *)(iVar1 + 0x11c);
          if (*(char *)(iVar1 + 0x120) == '\0') {
            iVar4 = iVar4 + 1;
            *(int *)(iVar1 + 0x11c) = iVar4;
          }
          uVar2 = Achievements::getValue((Achievements *)*puVar6,0x28,1);
          fVar8 = DAT_000d4418;
          fVar7 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
          fVar9 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
          __aeabi_idivmod((int)((fVar7 / fVar9) * DAT_000d4418),10);
          if (extraout_r1 == 0) {
            pHVar3 = (Hud *)PlayerEgo::getHUD(*(PlayerEgo **)(param_1 + 0xf0));
            fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar5 + 0x11c),
                                               (byte)(in_fpscr >> 0x16) & 3);
            uVar2 = Achievements::getValue((Achievements *)*puVar6,0x28,1);
            fVar7 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
            Hud::hudEventMedal(pHVar3,0x28,(int)((fVar9 / fVar7) * fVar8));
          }
          iVar4 = *(int *)(*piVar5 + 0x11c);
          iVar1 = Achievements::getValue((Achievements *)*puVar6,0x28,1);
          if (iVar1 <= iVar4) {
            *(undefined1 *)(*piVar5 + 0x120) = 1;
          }
        }
      }
      if ((*(PlayerEgo **)(param_1 + 0xf0) != (PlayerEgo *)0x0) &&
         (iVar1 = PlayerEgo::emergencySystemActive(*(PlayerEgo **)(param_1 + 0xf0)), iVar1 != 0)) {
        puVar6 = *(undefined4 **)(DAT_000d4424 + 0xd4366);
        iVar1 = Achievements::hasMedal((Achievements *)*puVar6,0x2b,1);
        if (iVar1 == 0) {
          iVar1 = *(int *)(*piVar5 + 0x13c) + 1;
          *(int *)(*piVar5 + 0x13c) = iVar1;
          uVar2 = Achievements::getValue((Achievements *)*puVar6,0x2b,1);
          fVar8 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
          fVar7 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
          if (0 < (int)(fVar8 / fVar7)) {
            pHVar3 = (Hud *)PlayerEgo::getHUD(*(PlayerEgo **)(param_1 + 0xf0));
            fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar5 + 0x13c),
                                               (byte)(in_fpscr >> 0x16) & 3);
            uVar2 = Achievements::getValue((Achievements *)*puVar6,0x2b,1);
            fVar8 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
            Hud::hudEventMedal(pHVar3,0x2b,(int)((fVar7 / fVar8) * DAT_000d4418));
          }
          iVar4 = *(int *)(*piVar5 + 0x13c);
          iVar1 = Achievements::getValue((Achievements *)*puVar6,0x2b,1);
          if (iVar1 <= iVar4) {
            *(undefined1 *)(*piVar5 + 0x140) = 1;
          }
        }
      }
    }
  }
  else {
    *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + 1;
  }
  return;
}

```
## target disasm
```
  000d4230: push {r4,r5,r6,r7,lr}
  000d4232: add r7,sp,#0xc
  000d4234: push {r8,r9,r11}
  000d4238: vpush {d8,d9}
  000d423c: mov r4,r0
  000d423e: ldr.w r0,[r0,#0x118]
  000d4242: ldr.w r1,[r4,#0x12c]
  000d4246: cmp r2,#0x0
  000d4248: sub.w r0,r0,#0x1
  000d424c: str.w r0,[r4,#0x118]
  000d4250: add.w r0,r1,#0x1
  000d4254: str.w r0,[r4,#0x12c]
  000d4258: beq 0x000d4262
  000d425a: ldr r0,[r4,#0x20]
  000d425c: adds r0,#0x1
  000d425e: str r0,[r4,#0x20]
  000d4260: b 0x000d440e
  000d4262: ldr r0,[0x000d441c]
  000d4264: add r0,pc
  000d4266: ldr r6,[r0,#0x0]
  000d4268: ldr r0,[r6,#0x0]
  000d426a: blx 0x000743c8
  000d426e: ldr.w r0,[r4,#0xf0]
  000d4272: ldr r1,[r4,#0x24]
  000d4274: cmp r0,#0x0
  000d4276: add.w r1,r1,#0x1
  000d427a: str r1,[r4,#0x24]
  000d427c: beq.w 0x000d440e
  000d4280: ldr r0,[r0,#0x14]
  000d4282: blx 0x000743d4
  000d4286: cmp r0,#0x0
  000d4288: bne 0x000d434c
  000d428a: ldr r0,[0x000d4420]
  000d428c: movs r1,#0x28
  000d428e: movs r2,#0x1
  000d4290: add r0,pc
  000d4292: ldr.w r9,[r0,#0x0]
  000d4296: ldr.w r0,[r9,#0x0]
  000d429a: blx 0x00072ef8
  000d429e: cmp r0,#0x0
  000d42a0: bne 0x000d434c
  000d42a2: ldr r0,[r6,#0x0]
  000d42a4: ldrb.w r1,[r0,#0x120]
  000d42a8: ldr.w r5,[r0,#0x11c]
  000d42ac: cbnz r1,0x000d42b4
  000d42ae: adds r5,#0x1
  000d42b0: str.w r5,[r0,#0x11c]
  000d42b4: ldr.w r0,[r9,#0x0]
  000d42b8: movs r1,#0x28
  000d42ba: movs r2,#0x1
  000d42bc: blx 0x00072f04
  000d42c0: vmov s0,r5
  000d42c4: vldr.32 s16,[pc,#0x150]
  000d42c8: vmov s2,r0
  000d42cc: movs r1,#0xa
  000d42ce: vcvt.f32.s32 s0,s0
  000d42d2: vcvt.f32.s32 s2,s2
  000d42d6: vdiv.f32 s0,s0,s2
  000d42da: vmul.f32 s0,s0,s16
  000d42de: vcvt.s32.f32 s0,s0
  000d42e2: vmov r0,s0
  000d42e6: blx 0x0006f514
  000d42ea: cbnz r1,0x000d432c
  000d42ec: ldr.w r0,[r4,#0xf0]
  000d42f0: blx 0x00072f10
  000d42f4: mov r8,r0
  000d42f6: ldr r0,[r6,#0x0]
  000d42f8: movs r1,#0x28
  000d42fa: movs r2,#0x1
  000d42fc: vldr.32 s0,[r0,#0x11c]
  000d4300: ldr.w r0,[r9,#0x0]
  000d4304: vcvt.f32.s32 s18,s0
  000d4308: blx 0x00072f04
  000d430c: vmov s0,r0
  000d4310: mov r0,r8
  000d4312: movs r1,#0x28
  000d4314: vcvt.f32.s32 s0,s0
  000d4318: vdiv.f32 s0,s18,s0
  000d431c: vmul.f32 s0,s0,s16
  000d4320: vcvt.s32.f32 s0,s0
  000d4324: vmov r2,s0
  000d4328: blx 0x00072f1c
  000d432c: ldr r1,[r6,#0x0]
  000d432e: movs r2,#0x1
  000d4330: ldr.w r0,[r9,#0x0]
  000d4334: mov.w r8,#0x1
  000d4338: ldr.w r5,[r1,#0x11c]
  000d433c: movs r1,#0x28
  000d433e: blx 0x00072f04
  000d4342: cmp r5,r0
  000d4344: itt ge
  000d4346: ldr.ge r0,[r6,#0x0]
  000d4348: strb.ge.w r8,[r0,#0x120]
  000d434c: ldr.w r0,[r4,#0xf0]
  000d4350: cmp r0,#0x0
  000d4352: beq 0x000d440e
  000d4354: blx 0x000743e0
  000d4358: cmp r0,#0x0
  000d435a: beq 0x000d440e
  000d435c: ldr r0,[0x000d4424]
  000d435e: movs r1,#0x2b
  000d4360: movs r2,#0x1
  000d4362: add r0,pc
  000d4364: ldr.w r8,[r0,#0x0]
  000d4368: ldr.w r0,[r8,#0x0]
  000d436c: blx 0x00072ef8
  000d4370: cmp r0,#0x0
  000d4372: bne 0x000d440e
  000d4374: ldr r0,[r6,#0x0]
  000d4376: movs r2,#0x1
  000d4378: ldr.w r1,[r0,#0x13c]
  000d437c: adds r5,r1,#0x1
  000d437e: str.w r5,[r0,#0x13c]
  000d4382: ldr.w r0,[r8,#0x0]
  000d4386: movs r1,#0x2b
  000d4388: blx 0x00072f04
  000d438c: vmov s0,r5
  000d4390: vmov s2,r0
  000d4394: vcvt.f32.s32 s0,s0
  000d4398: vcvt.f32.s32 s2,s2
  000d439c: vdiv.f32 s0,s0,s2
  000d43a0: vcvt.s32.f32 s0,s0
  000d43a4: vmov r0,s0
  000d43a8: cmp r0,#0x1
  000d43aa: blt 0x000d43f0
  000d43ac: ldr.w r0,[r4,#0xf0]
  000d43b0: blx 0x00072f10
  000d43b4: mov r4,r0
  000d43b6: ldr r0,[r6,#0x0]
  000d43b8: movs r1,#0x2b
  000d43ba: movs r2,#0x1
  000d43bc: vldr.32 s0,[r0,#0x13c]
  000d43c0: ldr.w r0,[r8,#0x0]
  000d43c4: vcvt.f32.s32 s16,s0
  000d43c8: blx 0x00072f04
  000d43cc: vmov s0,r0
  000d43d0: vldr.32 s2,[pc,#0x44]
  000d43d4: mov r0,r4
  000d43d6: movs r1,#0x2b
  000d43d8: vcvt.f32.s32 s0,s0
  000d43dc: vdiv.f32 s0,s16,s0
  000d43e0: vmul.f32 s0,s0,s2
  000d43e4: vcvt.s32.f32 s0,s0
  000d43e8: vmov r2,s0
  000d43ec: blx 0x00072f1c
  000d43f0: ldr r1,[r6,#0x0]
  000d43f2: movs r2,#0x1
  000d43f4: ldr.w r0,[r8,#0x0]
  000d43f8: movs r4,#0x1
  000d43fa: ldr.w r5,[r1,#0x13c]
  000d43fe: movs r1,#0x2b
  000d4400: blx 0x00072f04
  000d4404: cmp r5,r0
  000d4406: itt ge
  000d4408: ldr.ge r0,[r6,#0x0]
  000d440a: strb.ge.w r4,[r0,#0x140]
  000d440e: vpop {d8,d9}
  000d4412: pop.w {r8,r9,r11}
  000d4416: pop {r4,r5,r6,r7,pc}
```
