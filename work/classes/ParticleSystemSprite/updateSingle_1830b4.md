# ParticleSystemSprite::updateSingle
elf 0x1830b4 body 696
Sig: undefined __stdcall updateSingle(int param_1, float param_2)

## decompile
```c

/* ParticleSystemSprite::updateSingle(int, float) */

void ParticleSystemSprite::updateSingle(int param_1,float param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  float *pfVar4;
  float *in_r1;
  float in_r2;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  uint in_fpscr;
  float fVar9;
  float extraout_s0;
  float in_s1;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float in_s2;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s3_01;
  float fVar10;
  float fVar11;
  float local_60;
  float local_5c;
  float local_58;
  Vector aVStack_54 [16];
  float local_44;
  float local_40;
  float local_3c;
  float local_38;
  int local_34;
  
  piVar6 = *(int **)(DAT_00193354 + 0x1930d0);
  local_34 = *piVar6;
  if ((int)((uint)*(byte *)(param_1 + 0x34) << 0x18) < 0) {
    if (*piVar6 - local_34 == 0) {
                    /* WARNING: Could not recover jumptable at 0x001ac8d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(DAT_001ac8d4 + 0x1ac8d8))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*piVar6 - local_34);
  }
  iVar1 = *(int *)(param_1 + 0x68);
  fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(iVar1 + (int)in_r1 * 4),
                                     (byte)(in_fpscr >> 0x16) & 3);
  iVar5 = *(int *)(DAT_00193358 + 0x1930fa) +
          *(char *)(*(int *)(param_1 + 0x6c) + (int)in_r1) * 0xa0;
  fVar9 = (float)(int)(fVar9 + in_r2);
  *(float *)(iVar1 + (int)in_r1 * 4) = fVar9;
  if (*(int *)(iVar5 + 0x28) < (int)fVar9) {
    *(undefined4 *)(iVar1 + (int)in_r1 * 4) = 0xffffffff;
    AbyssEngine::PaintCanvas::SpriteSystemSetPosition
              (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),fVar9,in_s1,in_s2);
    if (*piVar6 - local_34 == 0) {
      (*(code *)(DAT_001ac8c4 + 0x1ac8c8))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),
                     *(undefined4 *)(param_1 + 0x58),*piVar6 - local_34);
  }
  VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x44),(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::PaintCanvas::SpriteSystemAddSize
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),
             (short)*(undefined4 *)(param_1 + 0x58) + (short)in_r1);
  IParticleSystem::interpolateColor(param_1,in_r1,&local_58,&local_5c,&local_60);
  fVar9 = (float)AbyssEngine::PaintCanvas::SpriteSystemSetRGBA
                           (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),local_5c,
                            extraout_s1,local_60,extraout_s3);
  iVar1 = *(int *)(iVar5 + 0x9c);
  if (iVar1 != 0) {
    iVar8 = *(int *)(iVar5 + 0x28);
    iVar7 = *(int *)(*(int *)(param_1 + 0x68) + (int)in_r1 * 4) + -1;
    uVar2 = __aeabi_idiv(iVar7 * iVar1,iVar8);
    uVar3 = __aeabi_idiv((iVar7 - (int)in_r2) * iVar1,iVar8);
    fVar9 = extraout_s0;
    if (uVar2 != (uVar3 & ~((int)uVar3 >> 0x1f))) {
      fVar11 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
      fVar9 = *(float *)(iVar5 + 0x90) - *(float *)(iVar5 + 0x88);
      fVar10 = *(float *)(iVar5 + 0x94) - *(float *)(iVar5 + 0x8c);
      local_44 = *(float *)(iVar5 + 0x88) + fVar9 * fVar11;
      fVar11 = (float)VectorSignedToFloat((int)(local_44 + DAT_00193350),
                                          (byte)(in_fpscr >> 0x16) & 3);
      local_44 = local_44 - fVar11;
      local_3c = *(float *)(iVar5 + 0x8c) + fVar10 * fVar11;
      local_40 = fVar9 + local_44;
      local_38 = fVar10 + local_3c;
      if ((int)((uint)*(byte *)(param_1 + 0x37) << 0x1e) < 0) {
        pfVar4 = (float *)IParticleSystem::rotateUVs((float *)param_1,(int)&local_44,in_r1);
        fVar9 = extraout_s1_01;
        fVar10 = extraout_s3_01;
      }
      else {
        pfVar4 = &local_44;
        fVar9 = extraout_s1_00;
        fVar10 = extraout_s3_00;
      }
      fVar9 = (float)AbyssEngine::PaintCanvas::SpriteSystemSetUv
                               (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),
                                pfVar4[1],fVar9,pfVar4[2],fVar10);
    }
  }
  fVar9 = (float)AbyssEngine::AEMath::operator*(aVStack_54,fVar9);
  AbyssEngine::AEMath::operator*((Vector *)&local_44,fVar9);
  AbyssEngine::PaintCanvas::SpriteSystemAddPosition
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),local_40,extraout_s1_02,
             local_3c);
  if (*piVar6 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000788f8: adr r12,0x178900
  000788fc: add r12,r12,#0xab000
  00078900: ldr pc,[r12,#0x340]!
  001930b4: push {r4,r5,r6,r7,lr}
  001930b6: add r7,sp,#0xc
  001930b8: push {r7,r8,r9,r10,r11}
  001930bc: vpush {d8}
  001930c0: sub sp,#0x48
  001930c2: mov r5,r0
  001930c4: ldr r0,[0x00193354]
  001930c6: vmov s16,r2
  001930ca: mov r11,r2
  001930cc: add r0,pc
  001930ce: mov r4,r1
  001930d0: ldr.w r9,[r0,#0x0]
  001930d4: ldr.w r0,[r9,#0x0]
  001930d8: str r0,[sp,#0x44]
  001930da: ldrb.w r0,[r5,#0x34]
  001930de: lsls r0,r0,#0x18
  001930e0: bmi 0x00193170
  001930e2: ldrd r0,r1,[r5,#0x68]
  001930e6: add.w r2,r0,r4, lsl #0x2
  001930ea: vldr.32 s0,[r2]
  001930ee: vcvt.f32.s32 s0,s0
  001930f2: ldr r2,[0x00193358]
  001930f4: ldrsb r1,[r1,r4]
  001930f6: add r2,pc
  001930f8: ldr r2,[r2,#0x0]
  001930fa: add.w r1,r1,r1, lsl #0x2
  001930fe: vadd.f32 s0,s0,s16
  00193102: add.w r8,r2,r1, lsl #0x5
  00193106: mov r6,r8
  00193108: vcvt.s32.f32 s0,s0
  0019310c: vmov r3,s0
  00193110: str.w r3,[r0,r4,lsl #0x2]
  00193114: ldr.w r1,[r6,#0x28]!
  00193118: cmp r1,r3
  0019311a: bge 0x0019319c
  0019311c: mov.w r1,#0xffffffff
  00193120: mov.w r3,#0x4f800000
  00193124: str.w r1,[r0,r4,lsl #0x2]
  00193128: ldr r0,[r5,#0x8]
  0019312a: ldrd r1,r2,[r5,#0x54]
  0019312e: add r2,r4
  00193130: strd r3,r3,[sp,#0x0]
  00193134: mov.w r3,#0x4f800000
  00193138: uxth r2,r2
  0019313a: blx 0x000778d8
  0019313e: ldr r0,[r5,#0x8]
  00193140: ldrd r1,r2,[r5,#0x54]
  00193144: ldr r3,[sp,#0x44]
  00193146: ldr.w r6,[r9,#0x0]
  0019314a: subs r3,r6,r3
  0019314c: itttt eq
  0019314e: add.eq r2,r4
  00193150: uxth.eq r2,r2
  00193152: mov.eq r3,#0x0
  00193154: add.eq sp,#0x48
  00193156: itttt eq
  00193158: vpop.eq {d8}
  0019315c: add.eq sp,#0x4
  0019315e: pop.eq.w {r8,r9,r10,r11}
  00193162: pop.eq.w {r4,r5,r6,r7,lr}
  00193166: it eq
  00193168: b.eq.w 0x001ac8b8
  0019316c: blx 0x0006e824
  00193170: ldr r0,[sp,#0x44]
  00193172: ldr.w r1,[r9,#0x0]
  00193176: subs r0,r1,r0
  00193178: itttt eq
  0019317a: mov.eq r0,r5
  0019317c: mov.eq r1,r4
  0019317e: mov.eq r2,r11
  00193180: add.eq sp,#0x48
  00193182: itttt eq
  00193184: vpop.eq {d8}
  00193188: add.eq sp,#0x4
  0019318a: pop.eq.w {r8,r9,r10,r11}
  0019318e: pop.eq.w {r4,r5,r6,r7,lr}
  00193192: it eq
  00193194: b.eq.w 0x001ac8c8
  00193198: blx 0x0006e824
  0019319c: vldr.32 s0,[r8,#0x44]
  001931a0: vldr.32 s2,[pc,#0x1a8]
  001931a4: vcvt.f32.s32 s0,s0
  001931a8: ldr r0,[r5,#0x8]
  001931aa: ldrd r1,r2,[r5,#0x54]
  001931ae: add r2,r4
  001931b0: uxth r2,r2
  001931b2: vmul.f32 s0,s0,s16
  001931b6: vmul.f32 s0,s0,s2
  001931ba: vcvt.s32.f32 s0,s0
  001931be: vmov r3,s0
  001931c2: blx 0x00078904
  001931c6: add r0,sp,#0x14
  001931c8: add r1,sp,#0x18
  001931ca: add r2,sp,#0x20
  001931cc: add r3,sp,#0x1c
  001931ce: strd r1,r0,[sp,#0x0]
  001931d2: mov r0,r5
  001931d4: mov r1,r4
  001931d6: blx 0x00078910
  001931da: ldr r0,[r5,#0x8]
  001931dc: ldrd r1,r2,[r5,#0x54]
  001931e0: add r2,r4
  001931e2: ldr r3,[sp,#0x20]
  001931e4: vldr.32 s0,[sp,#0x1c]
  001931e8: vldr.32 s2,[sp,#0x18]
  001931ec: uxth r2,r2
  001931ee: vldr.32 s4,[sp,#0x14]
  001931f2: vstr.32 s0,[sp]
  001931f6: vstr.32 s2,[sp,#0x4]
  001931fa: vstr.32 s4,[sp,#0x8]
  001931fe: blx 0x000788d4
  00193202: ldr.w r10,[r8,#0x9c]
  00193206: cmp.w r10,#0x0
  0019320a: beq 0x001932f0
  0019320c: ldr r0,[r5,#0x68]
  0019320e: str.w r9,[sp,#0x10]
  00193212: str.w r11,[sp,#0xc]
  00193216: ldr.w r0,[r0,r4,lsl #0x2]
  0019321a: ldr.w r11,[r6,#0x0]
  0019321e: sub.w r9,r0,#0x1
  00193222: mov r1,r11
  00193224: mul r0,r9,r10
  00193228: blx 0x0007198c
  0019322c: vcvt.s32.f32 s0,s16
  00193230: mov r6,r0
  00193232: mov r1,r11
  00193234: ldr.w r11,[sp,#0xc]
  00193238: vmov r0,s0
  0019323c: sub.w r0,r9,r0
  00193240: ldr.w r9,[sp,#0x10]
  00193244: mul r0,r0,r10
  00193248: blx 0x0007198c
  0019324c: bic.w r0,r0,r0, asr #0x1f
  00193250: cmp r6,r0
  00193252: beq 0x001932f0
  00193254: vldr.32 s0,[r8,#0x88]
  00193258: vmov s8,r6
  0019325c: vldr.32 s4,[r8,#0x90]
  00193260: vldr.32 s2,[r8,#0x8c]
  00193264: vldr.32 s6,[r8,#0x94]
  00193268: vcvt.f32.s32 s8,s8
  0019326c: vsub.f32 s4,s4,s0
  00193270: vsub.f32 s6,s6,s2
  00193274: vmla.f32 s0,s4,s8
  00193278: vldr.32 s8,[pc,#0xd4]
  0019327c: vadd.f32 s8,s0,s8
  00193280: vcvt.s32.f32 s8,s8
  00193284: vcvt.f32.s32 s8,s8
  00193288: vsub.f32 s0,s0,s8
  0019328c: vmla.f32 s2,s6,s8
  00193290: vstr.32 s0,[sp,#0x34]
  00193294: vadd.f32 s4,s4,s0
  00193298: vadd.f32 s6,s6,s2
  0019329c: vstr.32 s2,[sp,#0x3c]
  001932a0: vstr.32 s4,[sp,#0x38]
  001932a4: vstr.32 s6,[sp,#0x40]
  001932a8: ldrb.w r0,[r5,#0x37]
  001932ac: lsls r0,r0,#0x1e
  001932ae: bmi 0x001932b4
  001932b0: add r0,sp,#0x34
  001932b2: b 0x001932c4
  001932b4: add r1,sp,#0x34
  001932b6: add r3,sp,#0x24
  001932b8: mov r0,r5
  001932ba: mov r2,r4
  001932bc: blx 0x00078874
  001932c0: vldr.32 s0,[r0]
  001932c4: vmov r3,s0
  001932c8: ldr r6,[r5,#0x8]
  001932ca: ldrd r1,r2,[r5,#0x54]
  001932ce: vldr.32 s0,[r0,#0x4]
  001932d2: vldr.32 s2,[r0,#0x8]
  001932d6: vldr.32 s4,[r0,#0xc]
  001932da: adds r0,r2,r4
  001932dc: vstr.32 s2,[sp]
  001932e0: uxth r2,r0
  001932e2: mov r0,r6
  001932e4: vstr.32 s0,[sp,#0x4]
  001932e8: vstr.32 s4,[sp,#0x8]
  001932ec: blx 0x000788c8
  001932f0: ldr r0,[r5,#0x64]
  001932f2: add.w r1,r4,r4, lsl #0x1
  001932f6: add r6,sp,#0x24
  001932f8: mov r2,r11
  001932fa: add.w r1,r0,r1, lsl #0x2
  001932fe: mov r0,r6
  00193300: blx 0x0006ec74
  00193304: ldr r2,[0x0019334c]
  00193306: add r0,sp,#0x34
  00193308: mov r1,r6
  0019330a: blx 0x0006ec74
  0019330e: ldr r0,[r5,#0x8]
  00193310: ldrd r1,r2,[r5,#0x54]
  00193314: add r2,r4
  00193316: ldr r3,[sp,#0x34]
  00193318: vldr.32 s0,[sp,#0x38]
  0019331c: vldr.32 s2,[sp,#0x3c]
  00193320: uxth r2,r2
  00193322: vstr.32 s0,[sp]
  00193326: vstr.32 s2,[sp,#0x4]
  0019332a: blx 0x000778fc
  0019332e: ldr r0,[sp,#0x44]
  00193330: ldr.w r1,[r9,#0x0]
  00193334: subs r0,r1,r0
  00193336: itttt eq
  00193338: add.eq sp,#0x48
  0019333a: vpop.eq {d8}
  0019333e: add.eq sp,#0x4
  00193340: pop.eq.w {r8,r9,r10,r11}
  00193344: it eq
  00193346: pop.eq {r4,r5,r6,r7,pc}
  00193348: blx 0x0006e824
  001ac8b8: bx pc
  001ac8bc: ldr r12,[0x1ac8c4]
  001ac8c0: add pc,r12,pc
  001ac8c8: bx pc
  001ac8cc: ldr r12,[0x1ac8d4]
  001ac8d0: add pc,r12,pc
```
