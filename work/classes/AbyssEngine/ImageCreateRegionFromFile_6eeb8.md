# AbyssEngine::ImageCreateRegionFromFile
elf 0x6eeb8 body 626
Sig: undefined __stdcall ImageCreateRegionFromFile(Engine * param_1, char * param_2, ushort param_3, Image2D * param_4)

## decompile
```c

/* AbyssEngine::ImageCreateRegionFromFile(AbyssEngine::Engine*, char const*, unsigned short,
   AbyssEngine::Image2D*) */

void AbyssEngine::ImageCreateRegionFromFile
               (Engine *param_1,char *param_2,ushort param_3,Image2D *param_4)

{
  char *pcVar1;
  char *pcVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  undefined4 *puVar6;
  float *pfVar7;
  int *piVar8;
  ushort uVar9;
  uint in_fpscr;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  ulonglong in_d5;
  float fVar15;
  undefined8 in_d6;
  uint in_s14;
  double dVar16;
  double dVar17;
  undefined8 local_48;
  undefined8 local_40;
  uint local_38;
  int local_34;
  
  piVar8 = *(int **)(DAT_0007f134 + 0x7eed0);
  local_34 = *piVar8;
  uVar3 = 0xfffffffc;
  local_48 = in_d5;
  local_40 = in_d6;
  local_38 = in_s14;
  if ((param_1 != (Engine *)0x0) && (param_2 != (char *)0x0)) {
    local_38 = 0;
    iVar4 = AEFile::OpenRead((AEFile *)param_2,(char *)&local_38,&local_38);
    if (iVar4 != 0) {
      local_40 = CONCAT44(DAT_0007f12c,0x2a2a2a2a);
      iVar4 = AEFile::Read(8,&local_40,local_38);
      if (iVar4 != 0) {
        uVar5 = 0;
        do {
          if (7 < uVar5) {
            iVar4 = AEFile::Skip(1,local_38);
            if (((iVar4 != 0) && (iVar4 = AEFile::Read(2,param_4 + 8,local_38), iVar4 != 0)) &&
               (iVar4 = AEFile::Read(2,param_4 + 10,local_38), iVar4 != 0)) {
              local_48 = local_48 & 0xffffffffffff;
              iVar4 = AEFile::Read(2,(void *)((int)&local_48 + 6),local_38);
              uVar3 = 0xffffffff;
              if ((iVar4 == 0) || (local_48._6_2_ <= param_3)) goto LAB_0007f110;
              iVar4 = MeshCreate(param_1,4,2,0x13,param_4);
              if (iVar4 == 1) {
                uVar9 = 0;
                local_48 = CONCAT44(local_48._4_4_,param_4 + 0xe);
                goto LAB_0007f0e6;
              }
              uVar3 = 0xfffffffe;
              goto LAB_0007f110;
            }
            break;
          }
          pcVar1 = (char *)((int)&local_40 + uVar5);
          pcVar2 = (char *)(DAT_0007f138 + 0x7ef20 + uVar5);
          uVar5 = uVar5 + 1;
        } while (*pcVar2 == *pcVar1);
      }
    }
LAB_0007f10c:
    uVar3 = 0xffffffff;
  }
LAB_0007f110:
  if (*piVar8 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
LAB_0007f0e6:
  if (local_48._6_2_ <= uVar9) goto code_r0x0007f0f4;
  if (uVar9 == param_3) {
    iVar4 = AEFile::Read(2,param_4 + 0xc,local_38);
    if (iVar4 != 0) {
      iVar4 = AEFile::Read(2,(void *)local_48,local_38);
      if (((iVar4 != 0) && (iVar4 = AEFile::Read(2,param_4 + 0x10,local_38), iVar4 != 0)) &&
         (iVar4 = AEFile::Read(2,param_4 + 0x12,local_38), iVar4 != 0)) {
        iVar4 = *(int *)param_4;
        puVar6 = *(undefined4 **)(iVar4 + 4);
        *puVar6 = 0;
        puVar6[1] = 0;
        puVar6[2] = 0;
        fVar10 = (float)VectorUnsignedToFloat
                                  ((uint)*(ushort *)(param_4 + 0x10),(byte)(in_fpscr >> 0x16) & 3);
        puVar6[4] = 0;
        puVar6[5] = 0;
        puVar6[3] = fVar10;
        puVar6[6] = fVar10;
        fVar11 = (float)VectorUnsignedToFloat
                                  ((uint)*(ushort *)(param_4 + 0x12),(byte)(in_fpscr >> 0x16) & 3);
        puVar6[8] = 0;
        puVar6[9] = 0;
        puVar6[0xb] = 0;
        puVar6[7] = fVar11;
        puVar6[10] = fVar11;
        dVar16 = (double)VectorUnsignedToFloat
                                   ((uint)*(ushort *)(param_4 + 10),(byte)(in_fpscr >> 0x16) & 3);
        dVar17 = (double)VectorUnsignedToFloat
                                   ((uint)*(ushort *)(param_4 + 8),(byte)(in_fpscr >> 0x16) & 3);
        fVar12 = (float)VectorSignedToFloat((int)(short)*(ushort *)(param_4 + 0xe),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar13 = (float)VectorSignedToFloat((int)(short)*(ushort *)(param_4 + 0xc),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar14 = (float)VectorUnsignedToFloat
                                  ((uint)*(ushort *)(param_4 + 0xe),(byte)(in_fpscr >> 0x16) & 3);
        fVar15 = (float)VectorUnsignedToFloat
                                  ((uint)*(ushort *)(param_4 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
        pfVar7 = *(float **)(iVar4 + 8);
        fVar15 = fVar15 * (float)(1.0 / dVar17);
        fVar14 = fVar14 * (float)(1.0 / dVar16);
        fVar11 = (fVar11 + fVar12) * (float)(1.0 / dVar16);
        fVar10 = (fVar10 + fVar13) * (float)(1.0 / dVar17);
        *pfVar7 = fVar15;
        pfVar7[1] = fVar14;
        pfVar7[2] = fVar10;
        pfVar7[3] = fVar14;
        pfVar7[4] = fVar10;
        pfVar7[5] = fVar11;
        pfVar7[6] = fVar15;
        pfVar7[7] = fVar11;
        puVar6 = *(undefined4 **)(iVar4 + 0x2c);
        *puVar6 = 0x20000;
        puVar6[1] = 1;
        puVar6[2] = DAT_0007f130;
        goto LAB_0007f0e2;
      }
    }
LAB_0007f104:
    MeshRelease(param_1,(Mesh **)param_4);
    goto LAB_0007f10c;
  }
  iVar4 = AEFile::Skip(8,local_38);
  if (iVar4 == 0) goto LAB_0007f104;
LAB_0007f0e2:
  uVar9 = uVar9 + 1;
  goto LAB_0007f0e6;
code_r0x0007f0f4:
  AEFile::Close(local_38);
  uVar3 = 1;
  goto LAB_0007f110;
}

```

## target disasm
```
  0007eeb8: push {r4,r5,r6,r7,lr}
  0007eeba: add r7,sp,#0xc
  0007eebc: push {r7,r8,r9,r10,r11}
  0007eec0: vpush {d3,d4,d5,d6,d7,d8}
  0007eec4: mov r5,r0
  0007eec6: ldr r0,[0x0007f134]
  0007eec8: mov r4,r3
  0007eeca: mov r8,r2
  0007eecc: add r0,pc
  0007eece: cmp r5,#0x0
  0007eed0: ldr.w r9,[r0,#0x0]
  0007eed4: ldr.w r0,[r9,#0x0]
  0007eed8: str r0,[sp,#0x24]
  0007eeda: mvn r0,#0x3
  0007eede: beq.w 0x0007f110
  0007eee2: cmp r1,#0x0
  0007eee4: beq.w 0x0007f110
  0007eee8: add r2,sp,#0x20
  0007eeea: movs r0,#0x0
  0007eeec: str r0,[sp,#0x20]
  0007eeee: mov r0,r1
  0007eef0: mov r1,r2
  0007eef2: blx 0x0006ecd4
  0007eef6: cmp r0,#0x0
  0007eef8: beq.w 0x0007f10c
  0007eefc: ldr r0,[0x0007f12c]
  0007eefe: add r6,sp,#0x18
  0007ef00: ldr r2,[sp,#0x20]
  0007ef02: str r0,[sp,#0x1c]
  0007ef04: mov.w r0,#0x2a2a2a2a
  0007ef08: str r0,[sp,#0x18]
  0007ef0a: movs r0,#0x8
  0007ef0c: mov r1,r6
  0007ef0e: blx 0x0006eb90
  0007ef12: cmp r0,#0x0
  0007ef14: beq.w 0x0007f10c
  0007ef18: ldr r1,[0x0007f138]
  0007ef1a: movs r0,#0x0
  0007ef1c: add r1,pc
  0007ef1e: cmp r0,#0x7
  0007ef20: bhi 0x0007ef2e
  0007ef22: ldrb r2,[r6,r0]
  0007ef24: ldrb r3,[r1,r0]
  0007ef26: adds r0,#0x1
  0007ef28: cmp r3,r2
  0007ef2a: beq 0x0007ef1e
  0007ef2c: b 0x0007f10c
  0007ef2e: ldr r1,[sp,#0x20]
  0007ef30: movs r0,#0x1
  0007ef32: blx 0x0006f1f0
  0007ef36: cmp r0,#0x0
  0007ef38: beq.w 0x0007f10c
  0007ef3c: ldr r2,[sp,#0x20]
  0007ef3e: add.w r1,r4,#0x8
  0007ef42: movs r0,#0x2
  0007ef44: blx 0x0006eb90
  0007ef48: cmp r0,#0x0
  0007ef4a: beq.w 0x0007f10c
  0007ef4e: ldr r2,[sp,#0x20]
  0007ef50: add.w r1,r4,#0xa
  0007ef54: movs r0,#0x2
  0007ef56: blx 0x0006eb90
  0007ef5a: cmp r0,#0x0
  0007ef5c: beq.w 0x0007f10c
  0007ef60: ldr r2,[sp,#0x20]
  0007ef62: movs r0,#0x0
  0007ef64: sub.w r1,r7,#0x3a
  0007ef68: strh r0,[r7,#-0x3a]
  0007ef6c: movs r0,#0x2
  0007ef6e: blx 0x0006eb90
  0007ef72: mov r1,r0
  0007ef74: mov.w r0,#0xffffffff
  0007ef78: cmp r1,#0x0
  0007ef7a: beq.w 0x0007f110
  0007ef7e: ldrh.w r1,[r7,#-0x3a]
  0007ef82: cmp r1,r8
  0007ef84: bls.w 0x0007f110
  0007ef88: mov r0,r5
  0007ef8a: movs r1,#0x4
  0007ef8c: movs r2,#0x2
  0007ef8e: movs r3,#0x13
  0007ef90: str r4,[sp,#0x0]
  0007ef92: blx 0x0006f1fc
  0007ef96: cmp r0,#0x1
  0007ef98: bne.w 0x0007f0fe
  0007ef9c: vmov.f64 d8,0x3ff0000000000000
  0007efa0: add.w r0,r4,#0x12
  0007efa4: add.w r11,r4,#0xc
  0007efa8: str r0,[sp,#0x8]
  0007efaa: add.w r0,r4,#0x10
  0007efae: movs r6,#0x0
  0007efb0: mov.w r10,#0x0
  0007efb4: str r0,[sp,#0xc]
  0007efb6: add.w r0,r4,#0xe
  0007efba: str r0,[sp,#0x10]
  0007efbc: b 0x0007f0e6
  0007efbe: ldr r2,[sp,#0x20]
  0007efc0: cmp r0,r8
  0007efc2: bne.w 0x0007f0d8
  0007efc6: movs r0,#0x2
  0007efc8: mov r1,r11
  0007efca: blx 0x0006eb90
  0007efce: cmp r0,#0x0
  0007efd0: beq.w 0x0007f104
  0007efd4: ldr r2,[sp,#0x20]
  0007efd6: movs r0,#0x2
  0007efd8: ldr r1,[sp,#0x10]
  0007efda: blx 0x0006eb90
  0007efde: cmp r0,#0x0
  0007efe0: beq.w 0x0007f104
  0007efe4: ldr r2,[sp,#0x20]
  0007efe6: movs r0,#0x2
  0007efe8: ldr r1,[sp,#0xc]
  0007efea: blx 0x0006eb90
  0007efee: cmp r0,#0x0
  0007eff0: beq.w 0x0007f104
  0007eff4: ldr r2,[sp,#0x20]
  0007eff6: movs r0,#0x2
  0007eff8: ldr r1,[sp,#0x8]
  0007effa: blx 0x0006eb90
  0007effe: cmp r0,#0x0
  0007f000: beq.w 0x0007f104
  0007f004: ldr r0,[r4,#0x0]
  0007f006: ldr r1,[r0,#0x4]
  0007f008: strd r6,r6,[r1,#0x0]
  0007f00c: str r6,[r1,#0x8]
  0007f00e: ldrh r2,[r4,#0x10]
  0007f010: vmov s0,r2
  0007f014: vcvt.f32.u32 s0,s0
  0007f018: strd r6,r6,[r1,#0x10]
  0007f01c: vstr.32 s0,[r1,#0xc]
  0007f020: vstr.32 s0,[r1,#0x18]
  0007f024: ldrh r2,[r4,#0x12]
  0007f026: vmov s2,r2
  0007f02a: vcvt.f32.u32 s2,s2
  0007f02e: strd r6,r6,[r1,#0x20]
  0007f032: str r6,[r1,#0x2c]
  0007f034: vstr.32 s2,[r1,#0x1c]
  0007f038: vstr.32 s2,[r1,#0x28]
  0007f03c: ldrh r1,[r4,#0xa]
  0007f03e: vmov s4,r1
  0007f042: vcvt.f64.u32 d16,s4
  0007f046: ldrh r1,[r4,#0x8]
  0007f048: vmov s4,r1
  0007f04c: vdiv.f64 d16,d8,d16
  0007f050: vcvt.f64.u32 d17,s4
  0007f054: ldrsh.w r1,[r4,#0xc]
  0007f058: ldrsh.w r2,[r4,#0xe]
  0007f05c: vdiv.f64 d17,d8,d17
  0007f060: vmov s6,r2
  0007f064: uxth r2,r2
  0007f066: vmov s8,r1
  0007f06a: uxth r1,r1
  0007f06c: vcvt.f32.s32 s6,s6
  0007f070: vcvt.f32.s32 s8,s8
  0007f074: vmov s10,r2
  0007f078: vmov s12,r1
  0007f07c: vcvt.f32.u32 s10,s10
  0007f080: vcvt.f32.u32 s12,s12
  0007f084: ldr r1,[r0,#0x8]
  0007f086: vcvt.f32.f64 s4,d16
  0007f08a: vadd.f32 s2,s2,s6
  0007f08e: vcvt.f32.f64 s14,d17
  0007f092: vadd.f32 s0,s0,s8
  0007f096: vmul.f32 s8,s12,s14
  0007f09a: vmul.f32 s6,s10,s4
  0007f09e: vmul.f32 s2,s2,s4
  0007f0a2: vmul.f32 s0,s0,s14
  0007f0a6: vstr.32 s8,[r1]
  0007f0aa: vstr.32 s6,[r1,#0x4]
  0007f0ae: vstr.32 s0,[r1,#0x8]
  0007f0b2: vstr.32 s6,[r1,#0xc]
  0007f0b6: vstr.32 s0,[r1,#0x10]
  0007f0ba: vstr.32 s2,[r1,#0x14]
  0007f0be: vstr.32 s8,[r1,#0x18]
  0007f0c2: vstr.32 s2,[r1,#0x1c]
  0007f0c6: mov.w r1,#0x20000
  0007f0ca: ldr r0,[r0,#0x2c]
  0007f0cc: str r1,[r0,#0x0]
  0007f0ce: movs r1,#0x1
  0007f0d0: str r1,[r0,#0x4]
  0007f0d2: ldr r1,[0x0007f130]
  0007f0d4: str r1,[r0,#0x8]
  0007f0d6: b 0x0007f0e2
  0007f0d8: movs r0,#0x8
  0007f0da: mov r1,r2
  0007f0dc: blx 0x0006f1f0
  0007f0e0: cbz r0,0x0007f104
  0007f0e2: add.w r10,r10,#0x1
  0007f0e6: ldrh.w r1,[r7,#-0x3a]
  0007f0ea: uxth.w r0,r10
  0007f0ee: cmp r0,r1
  0007f0f0: bcc.w 0x0007efbe
  0007f0f4: ldr r0,[sp,#0x20]
  0007f0f6: blx 0x0006ece0
  0007f0fa: movs r0,#0x1
  0007f0fc: b 0x0007f110
  0007f0fe: mvn r0,#0x1
  0007f102: b 0x0007f110
  0007f104: mov r0,r5
  0007f106: mov r1,r4
  0007f108: blx 0x0006ebd8
  0007f10c: mov.w r0,#0xffffffff
  0007f110: ldr r1,[sp,#0x24]
  0007f112: ldr.w r2,[r9,#0x0]
  0007f116: subs r1,r2,r1
  0007f118: itttt eq
  0007f11a: vpop.eq {d3,d4,d5,d6,d7,d8}
  0007f11e: add.eq sp,#0x4
  0007f120: pop.eq.w {r8,r9,r10,r11}
  0007f124: pop.eq {r4,r5,r6,r7,pc}
  0007f126: blx 0x0006e824
```
