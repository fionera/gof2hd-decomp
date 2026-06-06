# Player::update
elf 0xa4110 body 422
Sig: undefined __stdcall update(int param_1, bool param_2)

## decompile
```c

/* Player::update(int, bool) */

Vector * Player::update(int param_1,bool param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int in_r2;
  int *piVar5;
  code *pcVar6;
  uint in_fpscr;
  Vector *in_s0;
  float fVar7;
  undefined8 uVar8;
  float fVar9;
  float fVar10;
  undefined8 in_d1;
  undefined8 in_d2;
  undefined4 extraout_s5;
  undefined4 extraout_s5_00;
  undefined4 extraout_s5_01;
  undefined4 uVar11;
  undefined8 in_d3;
  undefined4 extraout_s7;
  undefined4 extraout_s7_00;
  undefined4 extraout_s7_01;
  undefined4 in_s14;
  undefined8 local_68;
  Vector aVStack_58 [12];
  Vector aVStack_4c [12];
  Vector aVStack_40 [8];
  undefined4 uStack_38;
  int local_34;
  
  uVar11 = (undefined4)((ulonglong)in_d2 >> 0x20);
  uVar3 = (undefined4)((ulonglong)in_d3 >> 0x20);
  uVar4 = (uint)param_2;
  piVar5 = *(int **)(DAT_000b42c4 + 0xb4128);
  _uStack_38 = CONCAT44(*piVar5,in_s14);
  iVar1 = *(int *)(param_1 + 0xb4) + uVar4;
  *(int *)(param_1 + 0xb4) = iVar1;
  if (3000 < iVar1) {
    *(undefined1 *)(param_1 + 0xc1) = 0;
    *(undefined4 *)(param_1 + 0xb4) = 0;
  }
  local_68 = in_d1;
  if (*(char *)(param_1 + 0x68) != '\0') {
    iVar1 = *(int *)(param_1 + 0xe8) + uVar4;
    fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0xe4),(byte)(in_fpscr >> 0x16) & 3)
    ;
    fVar9 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    iVar2 = *(int *)(param_1 + 0x80);
    fVar10 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    fVar10 = (fVar9 / fVar7) * fVar10;
    *(int *)(param_1 + 0xe8) = iVar1;
    *(int *)(param_1 + 0x7c) = (int)fVar10;
    iVar1 = DAT_000b42c8;
    if (iVar2 < (int)fVar10) {
      *(undefined1 *)(param_1 + 0x68) = 0;
      *(int *)(param_1 + 0x7c) = iVar2;
      *(int *)(**(int **)(iVar1 + 0xb41a0) + 0x134) =
           *(int *)(**(int **)(iVar1 + 0xb41a0) + 0x134) + -1;
      *(undefined4 *)(param_1 + 0xe8) = 0;
    }
    updateDamageRate((Player *)param_1);
    uVar3 = extraout_s7;
    uVar11 = extraout_s5;
  }
  if (((*(char *)(*(int *)(DAT_000b42cc + 0xb41be) + 0xf) == '\0') || (in_r2 == 0)) ||
     (*(int *)(param_1 + 0xf4) == -1)) {
    if (*(char *)(param_1 + 0x108) != '\0') {
      StopEngineSound();
      uVar3 = extraout_s7_01;
      uVar11 = extraout_s5_01;
    }
  }
  else {
    iVar1 = param_1 + 4;
    pcVar6 = *(code **)(DAT_000b42d4 + 0xb41e2);
    VectorSignedToFloat(**(undefined4 **)(DAT_000b42d0 + 0xb41e0),(byte)(in_fpscr >> 0x16) & 3);
    (*pcVar6)(**(undefined4 **)(DAT_000b42d0 + 0xb41e0),(Vector *)((int)&local_68 + 4),iVar1);
    local_68 = CONCAT44(local_68._4_4_,param_1 + 0xfc);
    fVar7 = (float)AbyssEngine::AEMath::operator-(aVStack_58,(Vector *)((int)&local_68 + 4));
    AbyssEngine::AEMath::operator*(fVar7,aVStack_4c);
    in_s0 = aVStack_40;
    fVar7 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::operator/(in_s0,fVar7);
    (*pcVar6)(aVStack_4c,iVar1);
    uVar3 = FModSound::updateEvent3DAttributes
                      ((FModSound *)**(undefined4 **)(DAT_000b42d8 + 0xb423c),
                       *(Event **)(param_1 + 0xf0),*(int *)(param_1 + 0xf4),aVStack_4c,in_s0,false);
    *(undefined4 *)(param_1 + 0xf0) = uVar3;
    *(undefined1 *)(param_1 + 0x108) = 1;
    (*pcVar6)(aVStack_58,iVar1);
    AbyssEngine::AEMath::Vector::operator=((Vector *)local_68,aVStack_58);
    uVar3 = extraout_s7_00;
    uVar11 = extraout_s5_00;
  }
  fVar7 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
  uVar8 = FloatVectorMax(CONCAT44(uVar11,*(float *)(param_1 + 0x60) +
                                         fVar7 * DAT_000b42b8 * DAT_000b42bc),
                         CONCAT44(uVar3,DAT_000b42c0),2,0x20);
  *(int *)(param_1 + 0x60) = (int)uVar8;
  if (*piVar5 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return in_s0;
}

```
## target disasm
```
  000b4110: push {r4,r5,r6,r7,lr}
  000b4112: add r7,sp,#0xc
  000b4114: push {r7,r8,r9,r10,r11}
  000b4118: vpush {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  000b411c: mov r4,r0
  000b411e: ldr r0,[0x000b42c4]
  000b4120: mov r5,r1
  000b4122: mov r6,r2
  000b4124: add r0,pc
  000b4126: ldr.w r8,[r0,#0x0]
  000b412a: ldr.w r0,[r8,#0x0]
  000b412e: str r0,[sp,#0x3c]
  000b4130: ldr.w r0,[r4,#0xb4]
  000b4134: add r0,r1
  000b4136: movw r1,#0xbb9
  000b413a: str.w r0,[r4,#0xb4]
  000b413e: cmp r0,r1
  000b4140: ittt ge
  000b4142: mov.ge r0,#0x0
  000b4144: strb.ge.w r0,[r4,#0xc1]
  000b4148: str.w.ge r0,[r4,#0xb4]
  000b414c: ldrb.w r0,[r4,#0x68]
  000b4150: cbz r0,0x000b41b8
  000b4152: ldr.w r0,[r4,#0xe8]
  000b4156: vldr.32 s0,[r4,#0xe4]
  000b415a: adds r1,r0,r5
  000b415c: vcvt.f32.s32 s0,s0
  000b4160: vmov s2,r1
  000b4164: vcvt.f32.s32 s2,s2
  000b4168: ldr.w r0,[r4,#0x80]
  000b416c: vdiv.f32 s0,s2,s0
  000b4170: vmov s2,r0
  000b4174: vcvt.f32.s32 s2,s2
  000b4178: vmul.f32 s0,s0,s2
  000b417c: vcvt.s32.f32 s2,s0
  000b4180: vcvt.s32.f32 s0,s0
  000b4184: str.w r1,[r4,#0xe8]
  000b4188: vstr.32 s2,[r4,#0x7c]
  000b418c: vmov r1,s0
  000b4190: cmp r0,r1
  000b4192: bge 0x000b41b2
  000b4194: ldr r1,[0x000b42c8]
  000b4196: movs r2,#0x0
  000b4198: strb.w r2,[r4,#0x68]
  000b419c: add r1,pc
  000b419e: str r0,[r4,#0x7c]
  000b41a0: ldr r1,[r1,#0x0]
  000b41a2: ldr r0,[r1,#0x0]
  000b41a4: ldr.w r1,[r0,#0x134]
  000b41a8: subs r1,#0x1
  000b41aa: str.w r1,[r0,#0x134]
  000b41ae: str.w r2,[r4,#0xe8]
  000b41b2: mov r0,r4
  000b41b4: blx 0x00072df0
  000b41b8: ldr r0,[0x000b42cc]
  000b41ba: add r0,pc
  000b41bc: ldr r0,[r0,#0x0]
  000b41be: ldrb r0,[r0,#0xf]
  000b41c0: cmp r0,#0x0
  000b41c2: beq 0x000b4268
  000b41c4: cmp r6,#0x0
  000b41c6: beq 0x000b4268
  000b41c8: ldr.w r0,[r4,#0xf4]
  000b41cc: adds r0,#0x1
  000b41ce: beq 0x000b4268
  000b41d0: ldr r0,[0x000b42d0]
  000b41d2: add.w r11,r4,#0x4
  000b41d6: ldr r1,[0x000b42d4]
  000b41d8: add.w r9,sp,#0xc
  000b41dc: add r0,pc
  000b41de: add r1,pc
  000b41e0: ldr r0,[r0,#0x0]
  000b41e2: ldr.w r10,[r1,#0x0]
  000b41e6: mov r1,r11
  000b41e8: vldr.32 s0,[r0]
  000b41ec: mov r0,r9
  000b41ee: vcvt.f32.s32 s16,s0
  000b41f2: blx r10
  000b41f4: add r6,sp,#0x18
  000b41f6: add.w r2,r4,#0xfc
  000b41fa: mov r1,r9
  000b41fc: str r2,[sp,#0x8]
  000b41fe: mov r0,r6
  000b4200: blx 0x0006ec38
  000b4204: vmov r1,s16
  000b4208: add.w r9,sp,#0x24
  000b420c: mov r2,r6
  000b420e: mov r0,r9
  000b4210: blx 0x00072658
  000b4214: vmov s0,r5
  000b4218: add r6,sp,#0x30
  000b421a: mov r1,r9
  000b421c: vcvt.f32.s32 s0,s0
  000b4220: mov r0,r6
  000b4222: vmov r2,s0
  000b4226: blx 0x00073024
  000b422a: add.w r9,sp,#0x24
  000b422e: mov r1,r11
  000b4230: mov r0,r9
  000b4232: blx r10
  000b4234: ldr r0,[0x000b42d8]
  000b4236: movs r3,#0x0
  000b4238: add r0,pc
  000b423a: ldr r0,[r0,#0x0]
  000b423c: ldr r0,[r0,#0x0]
  000b423e: ldrd r1,r2,[r4,#0xf0]
  000b4242: strd r6,r3,[sp,#0x0]
  000b4246: mov r3,r9
  000b4248: blx 0x000715b4
  000b424c: add r6,sp,#0x18
  000b424e: str.w r0,[r4,#0xf0]
  000b4252: movs r0,#0x1
  000b4254: mov r1,r11
  000b4256: strb.w r0,[r4,#0x108]
  000b425a: mov r0,r6
  000b425c: blx r10
  000b425e: ldr r0,[sp,#0x8]
  000b4260: mov r1,r6
  000b4262: blx 0x0006eb3c
  000b4266: b 0x000b4274
  000b4268: ldrb.w r0,[r4,#0x108]
  000b426c: cbz r0,0x000b4274
  000b426e: mov r0,r4
  000b4270: blx 0x0007240c
  000b4274: vmov s0,r5
  000b4278: vldr.32 s2,[pc,#0x3c]
  000b427c: vldr.32 s6,[pc,#0x40]
  000b4280: vcvt.f32.s32 s0,s0
  000b4284: vldr.32 s4,[r4,#0x60]
  000b4288: vmul.f32 s0,s0,s2
  000b428c: vldr.32 s2,[pc,#0x2c]
  000b4290: vmla.f32 s4,s0,s2
  000b4294: vmax.f32 d0,d2,d3
  000b4298: vstr.32 s0,[r4,#0x60]
  000b429c: ldr r0,[sp,#0x3c]
  000b429e: ldr.w r1,[r8,#0x0]
  000b42a2: subs r0,r1,r0
  000b42a4: itttt eq
  000b42a6: vpop.eq {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  000b42aa: add.eq sp,#0x4
  000b42ac: pop.eq.w {r8,r9,r10,r11}
  000b42b0: pop.eq {r4,r5,r6,r7,pc}
  000b42b2: blx 0x0006e824
```
