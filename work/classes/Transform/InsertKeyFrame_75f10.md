# Transform::InsertKeyFrame
elf 0x75f10 body 1178
Sig: undefined __stdcall InsertKeyFrame(float * param_1, longlong param_2, int param_3)

## decompile
```c

/* AbyssEngine::Transform::InsertKeyFrame(float const*, long long, int) */

void AbyssEngine::Transform::InsertKeyFrame(float *param_1,longlong param_2,int param_3)

{
  KeyFrame *this;
  uint uVar1;
  float fVar2;
  float *in_r1;
  int iVar3;
  undefined1 *puVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  code *pcVar8;
  int *piVar9;
  float fVar10;
  int iVar11;
  code *pcVar12;
  float fVar13;
  int iVar14;
  bool bVar15;
  undefined1 auStack_40 [12];
  int local_34;
  
  fVar13 = 0.0;
  iVar14 = -4;
  piVar9 = *(int **)(DAT_000862cc + 0x85f2e);
  local_34 = *piVar9;
  **(int **)(DAT_000862c8 + 0x85f28) = **(int **)(DAT_000862c8 + 0x85f28) + 1;
  fVar10 = param_1[0x3f];
  bVar15 = (uint)param_1[0x3e] < (uint)param_3;
  fVar2 = (float)(param_3 >> 0x1f);
  if ((int)(((int)fVar10 - (int)fVar2) - (uint)bVar15) < 0 !=
      (SBORROW4((int)fVar10,(int)fVar2) != SBORROW4((int)fVar10 - (int)fVar2,(uint)bVar15))) {
    param_1[0x3e] = (float)param_3;
    param_1[0x3f] = fVar2;
  }
  for (; (uint)fVar13 < (uint)param_1[0x47]; fVar13 = (float)((int)fVar13 + 1)) {
    this = *(KeyFrame **)((int)param_1[0x48] + iVar14 + 4);
    uVar1 = *(uint *)(this + 0x50);
    fVar10 = *(float *)(this + 0x54);
    if ((int)(((int)fVar10 - (int)fVar2) - (uint)(uVar1 < (uint)param_3)) < 0 ==
        (SBORROW4((int)fVar10,(int)fVar2) !=
        SBORROW4((int)fVar10 - (int)fVar2,(uint)(uVar1 < (uint)param_3)))) {
      if (uVar1 == param_3 && fVar10 == fVar2) goto LAB_0008617c;
      break;
    }
    iVar14 = iVar14 + 4;
  }
  this = operator_new(0x60);
  KeyFrame::KeyFrame(this);
  *(int *)(this + 0x50) = param_3;
  *(float *)(this + 0x54) = fVar2;
  InsertKeyFrame((Transform *)param_1,this,(int)fVar13);
  if (fVar13 == 0.0) {
    fVar13 = 0.0;
  }
  else {
    fVar10 = param_1[0x48];
    iVar3 = *(int *)((int)fVar10 + iVar14);
    if ((uint)fVar13 < (uint)((int)param_1[0x47] + -1)) {
      uVar1 = *(uint *)(iVar3 + 0x50);
      iVar5 = *(int *)(iVar3 + 0x54);
      fVar2 = (float)__aeabi_l2f(param_3 - uVar1,
                                 ((int)fVar2 - iVar5) - (uint)((uint)param_3 < uVar1));
      iVar11 = *(int *)((int)fVar10 + iVar14 + 8);
      fVar10 = (float)__aeabi_l2f(*(uint *)(iVar11 + 0x50) - uVar1,
                                  (*(int *)(iVar11 + 0x54) - iVar5) -
                                  (uint)(*(uint *)(iVar11 + 0x50) < uVar1));
      fVar2 = fVar2 / fVar10;
      pcVar8 = *(code **)(DAT_000862d0 + 0x86016);
      (*pcVar8)(auStack_40,iVar3,iVar11,fVar2);
      pcVar12 = *(code **)(DAT_000862d4 + 0x86028);
      (*pcVar12)(this,auStack_40);
      (*pcVar8)(auStack_40,*(int *)((int)param_1[0x48] + iVar14) + 0xc,
                *(int *)((int)param_1[0x48] + iVar14 + 8) + 0xc,fVar2);
      (*pcVar12)(this + 0xc,auStack_40);
      (*pcVar8)(auStack_40,*(int *)((int)param_1[0x48] + iVar14) + 0x18,
                *(int *)((int)param_1[0x48] + iVar14 + 8) + 0x18,fVar2);
      (*pcVar12)(this + 0x18,auStack_40);
      iVar5 = *(int *)((int)param_1[0x48] + iVar14);
      iVar3 = *(int *)((int)param_1[0x48] + iVar14 + 8);
      fVar10 = *(float *)(iVar5 + 0x48);
      *(float *)(this + 0x48) = fVar10 + fVar2 * (*(float *)(iVar3 + 0x48) - fVar10);
      (*pcVar8)(auStack_40,iVar5 + 0x24,iVar3 + 0x24,fVar2);
      (*pcVar12)(this + 0x24,auStack_40);
      (*pcVar8)(auStack_40,*(int *)((int)param_1[0x48] + iVar14) + 0x30,
                *(int *)((int)param_1[0x48] + iVar14 + 8) + 0x30,fVar2);
      (*pcVar12)(this + 0x30,auStack_40);
      (*pcVar8)(auStack_40,*(int *)((int)param_1[0x48] + iVar14) + 0x3c,
                *(int *)((int)param_1[0x48] + iVar14 + 8) + 0x3c,fVar2);
      puVar4 = auStack_40;
    }
    else {
      pcVar12 = *(code **)(DAT_00086408 + 0x860f8);
      (*pcVar12)(this);
      (*pcVar12)(this + 0xc,*(int *)((int)param_1[0x48] + iVar14) + 0xc);
      (*pcVar12)(this + 0x18,*(int *)((int)param_1[0x48] + iVar14) + 0x18);
      iVar3 = *(int *)((int)param_1[0x48] + iVar14);
      *(undefined4 *)(this + 0x48) = *(undefined4 *)(iVar3 + 0x48);
      (*pcVar12)(this + 0x24,iVar3 + 0x24);
      (*pcVar12)(this + 0x30,*(int *)((int)param_1[0x48] + iVar14) + 0x30);
      puVar4 = (undefined1 *)(*(int *)((int)param_1[0x48] + iVar14) + 0x3c);
    }
    (*pcVar12)(this + 0x3c,puVar4);
    uVar1 = *(uint *)(*(int *)((int)param_1[0x48] + iVar14) + 0x5c);
    *(uint *)(this + 0x58) =
         *(uint *)(*(int *)((int)param_1[0x48] + iVar14) + 0x58) | *(uint *)(this + 0x58);
    *(uint *)(this + 0x5c) = *(uint *)(this + 0x5c) | uVar1;
  }
LAB_0008617c:
  uVar7 = (uint)((ulonglong)param_2 >> 0x20);
  uVar6 = (uint)param_2;
  *(uint *)(this + 0x58) = *(uint *)(this + 0x58) | uVar6;
  *(uint *)(this + 0x5c) = *(uint *)(this + 0x5c) | uVar7;
  uVar1 = uVar6 - 1;
  iVar14 = uVar7 - (uVar6 == 0);
  if ((uint)-iVar14 < (uint)(uVar1 < 8)) {
                    /* WARNING: Could not recover jumptable at 0x00086274. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&UNK_00086278 + (uint)*(byte *)(uVar6 + 0x86277) * 2))
              (uVar1,-(uint)(uVar1 >= 8) - iVar14);
    return;
  }
  if (param_2 == 0x10) {
    fVar2 = *in_r1;
    *(float *)(this + 0x10) = fVar2;
  }
  else if (param_2 == 0x20) {
    fVar2 = *in_r1;
    *(float *)(this + 0x14) = fVar2;
  }
  else {
    if (param_2 != 0x38) {
      if (param_2 == 0x40) {
        *(float *)(this + 0x18) = *in_r1;
      }
      else if (param_2 == 0x80) {
        *(float *)(this + 0x1c) = *in_r1;
      }
      else {
        if (param_2 == 0x100) {
          fVar2 = *in_r1;
        }
        else {
          if (param_2 != 0x1c0) {
            if (param_2 == 0x200) {
              *(float *)(this + 0x48) = *in_r1 / DAT_00086404;
            }
            else if (param_2 == 0x400) {
              *(float *)(this + 0x24) = -*in_r1;
            }
            else if (param_2 == 0x800) {
              *(float *)(this + 0x28) = *in_r1;
            }
            else {
              if (param_2 == 0x1000) {
                fVar2 = *in_r1;
              }
              else {
                if (param_2 != 0x1c00) {
                  if (param_2 == 0x2000) {
                    *(float *)(this + 0x30) = *in_r1;
                  }
                  else if (param_2 == 0x4000) {
                    *(float *)(this + 0x34) = *in_r1;
                  }
                  else {
                    if (param_2 == 0x8000) {
                      fVar2 = *in_r1;
                    }
                    else {
                      if (param_2 != 0xe000) {
                        if (param_2 == 0x10000) {
                          *(float *)(this + 0x3c) = *in_r1;
                        }
                        else if (param_2 == 0x20000) {
                          *(float *)(this + 0x40) = *in_r1;
                        }
                        else {
                          if (param_2 == 0x40000) {
                            fVar2 = *in_r1;
                          }
                          else {
                            if (param_2 != 0x70000) goto LAB_000863d2;
                            *(float *)(this + 0x3c) = *in_r1;
                            *(float *)(this + 0x40) = in_r1[1];
                            fVar2 = in_r1[2];
                          }
                          *(float *)(this + 0x44) = fVar2;
                        }
                        goto LAB_000863d2;
                      }
                      *(float *)(this + 0x30) = *in_r1;
                      *(float *)(this + 0x34) = in_r1[1];
                      fVar2 = in_r1[2];
                    }
                    *(float *)(this + 0x38) = fVar2;
                  }
                  goto LAB_000863d2;
                }
                *(float *)(this + 0x24) = *in_r1;
                *(float *)(this + 0x28) = in_r1[1];
                fVar2 = in_r1[2];
              }
              *(float *)(this + 0x2c) = fVar2;
            }
            goto LAB_000863d2;
          }
          *(float *)(this + 0x18) = *in_r1;
          *(float *)(this + 0x1c) = in_r1[1];
          fVar2 = in_r1[2];
        }
        *(float *)(this + 0x20) = fVar2;
      }
      goto LAB_000863d2;
    }
    *(float *)(this + 0xc) = *in_r1;
    *(float *)(this + 0x10) = in_r1[1];
    *(float *)(this + 0x14) = in_r1[2];
    fVar10 = in_r1[1];
    if (in_r1[1] < *in_r1) {
      fVar10 = *in_r1;
    }
    fVar2 = in_r1[2];
    if (in_r1[2] < fVar10) {
      fVar2 = fVar10;
    }
  }
  if (param_1[0x39] < fVar2) {
    param_1[0x39] = fVar2;
  }
LAB_000863d2:
  UpdateKeyFrames((Transform *)param_1,this,(int)fVar13);
  if (*piVar9 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00085f10: push {r4,r5,r6,r7,lr}
  00085f12: add r7,sp,#0xc
  00085f14: push {r7,r8,r9,r10,r11}
  00085f18: vpush {d3,d4,d5,d6,d7,d8}
  00085f1c: mov r10,r0
  00085f1e: ldr r0,[0x000862c8]
  00085f20: mov r12,r1
  00085f22: ldr r1,[0x000862cc]
  00085f24: add r0,pc
  00085f26: ldr.w r8,[r7,#0x8]
  00085f2a: add r1,pc
  00085f2c: mov.w r9,#0x0
  00085f30: ldr r0,[r0,#0x0]
  00085f32: mvn r11,#0x3
  00085f36: ldr r6,[r1,#0x0]
  00085f38: str r6,[sp,#0x14]
  00085f3a: ldr r1,[r0,#0x0]
  00085f3c: ldr r6,[r6,#0x0]
  00085f3e: adds r1,#0x1
  00085f40: str r1,[r0,#0x0]
  00085f42: str r6,[sp,#0x24]
  00085f44: mov r0,r10
  00085f46: ldr.w r1,[r0,#0xf8]!
  00085f4a: ldr r6,[r0,#0x4]
  00085f4c: subs.w r1,r1,r8
  00085f50: sbcs.w r1,r6,r8, asr #0x1f
  00085f54: itt lt
  00085f56: asr.lt.w r1,r8, asr #0x1f
  00085f5a: strd.lt r8,r1,[r0,#0x0]
  00085f5e: ldr.w r0,[r10,#0x11c]
  00085f62: b 0x00085f82
  00085f64: ldr.w r1,[r10,#0x120]
  00085f68: add r1,r11
  00085f6a: ldr r6,[r1,#0x4]
  00085f6c: ldrd r5,r1,[r6,#0x50]
  00085f70: subs.w r4,r5,r8
  00085f74: sbcs.w r4,r1,r8, asr #0x1f
  00085f78: bge 0x00085f88
  00085f7a: add.w r11,r11,#0x4
  00085f7e: add.w r9,r9,#0x1
  00085f82: cmp r9,r0
  00085f84: bcc 0x00085f64
  00085f86: b 0x00085f96
  00085f88: eor.w r0,r5,r8
  00085f8c: eor.w r1,r1,r8, asr #0x1f
  00085f90: orrs r0,r1
  00085f92: beq.w 0x0008617c
  00085f96: add r0,sp,#0x8
  00085f98: stm r0,{r2,r3,r12}
  00085f9c: movs r0,#0x60
  00085f9e: blx 0x0006eb24
  00085fa2: mov r6,r0
  00085fa4: blx 0x0006f838
  00085fa8: asr.w r5,r8, asr #0x1f
  00085fac: mov r0,r10
  00085fae: mov r1,r6
  00085fb0: mov r2,r9
  00085fb2: strd r8,r5,[r6,#0x50]
  00085fb6: blx 0x0006f850
  00085fba: cmp.w r9,#0x0
  00085fbe: beq.w 0x000860e4
  00085fc2: ldrd r0,r4,[r10,#0x11c]
  00085fc6: ldr.w r1,[r4,r11]
  00085fca: subs r0,#0x1
  00085fcc: cmp r9,r0
  00085fce: bcs.w 0x000860f2
  00085fd2: ldrd r0,r2,[r1,#0x50]
  00085fd6: str r0,[sp,#0x0]
  00085fd8: subs.w r0,r8,r0
  00085fdc: str r1,[sp,#0x4]
  00085fde: sbc.w r1,r5,r2
  00085fe2: mov r5,r2
  00085fe4: blx 0x0006f7d8
  00085fe8: add.w r1,r4,r11
  00085fec: vmov s16,r0
  00085ff0: ldr r0,[sp,#0x0]
  00085ff2: ldr.w r8,[r1,#0x8]
  00085ff6: ldrd r1,r2,[r8,#0x50]
  00085ffa: subs r0,r1,r0
  00085ffc: sbc.w r1,r2,r5
  00086000: blx 0x0006f7d8
  00086004: vmov s0,r0
  00086008: ldr r0,[0x000862d0]
  0008600a: ldr r1,[sp,#0x4]
  0008600c: mov r2,r8
  0008600e: vdiv.f32 s16,s16,s0
  00086012: add r0,pc
  00086014: ldr r5,[r0,#0x0]
  00086016: add r0,sp,#0x18
  00086018: vmov r4,s16
  0008601c: mov r3,r4
  0008601e: blx r5
  00086020: ldr r0,[0x000862d4]
  00086022: add r1,sp,#0x18
  00086024: add r0,pc
  00086026: ldr.w r8,[r0,#0x0]
  0008602a: mov r0,r6
  0008602c: blx r8
  0008602e: ldr.w r0,[r10,#0x120]
  00086032: mov r3,r4
  00086034: ldr.w r1,[r0,r11]
  00086038: add r0,r11
  0008603a: ldr r0,[r0,#0x8]
  0008603c: adds r1,#0xc
  0008603e: add.w r2,r0,#0xc
  00086042: add r0,sp,#0x18
  00086044: blx r5
  00086046: add.w r0,r6,#0xc
  0008604a: add r1,sp,#0x18
  0008604c: blx r8
  0008604e: ldr.w r0,[r10,#0x120]
  00086052: mov r3,r4
  00086054: ldr.w r1,[r0,r11]
  00086058: add r0,r11
  0008605a: ldr r0,[r0,#0x8]
  0008605c: adds r1,#0x18
  0008605e: add.w r2,r0,#0x18
  00086062: add r0,sp,#0x18
  00086064: blx r5
  00086066: add.w r0,r6,#0x18
  0008606a: add r1,sp,#0x18
  0008606c: blx r8
  0008606e: ldr.w r0,[r10,#0x120]
  00086072: mov r3,r4
  00086074: ldr.w r2,[r0,r11]
  00086078: add r0,r11
  0008607a: ldr r0,[r0,#0x8]
  0008607c: add.w r1,r2,#0x24
  00086080: vldr.32 s0,[r2,#0x48]
  00086084: add.w r2,r0,#0x24
  00086088: vldr.32 s2,[r0,#0x48]
  0008608c: add r0,sp,#0x18
  0008608e: vsub.f32 s2,s2,s0
  00086092: vmla.f32 s0,s16,s2
  00086096: vstr.32 s0,[r6,#0x48]
  0008609a: blx r5
  0008609c: add.w r0,r6,#0x24
  000860a0: add r1,sp,#0x18
  000860a2: blx r8
  000860a4: ldr.w r0,[r10,#0x120]
  000860a8: mov r3,r4
  000860aa: ldr.w r1,[r0,r11]
  000860ae: add r0,r11
  000860b0: ldr r0,[r0,#0x8]
  000860b2: adds r1,#0x30
  000860b4: add.w r2,r0,#0x30
  000860b8: add r0,sp,#0x18
  000860ba: blx r5
  000860bc: add.w r0,r6,#0x30
  000860c0: add r1,sp,#0x18
  000860c2: blx r8
  000860c4: ldr.w r0,[r10,#0x120]
  000860c8: mov r3,r4
  000860ca: ldr.w r1,[r0,r11]
  000860ce: add r0,r11
  000860d0: ldr r0,[r0,#0x8]
  000860d2: adds r1,#0x3c
  000860d4: add.w r2,r0,#0x3c
  000860d8: add r0,sp,#0x18
  000860da: blx r5
  000860dc: add.w r0,r6,#0x3c
  000860e0: add r1,sp,#0x18
  000860e2: b 0x0008615a
  000860e4: add.w r12,sp,#0x8
  000860e8: mov.w r9,#0x0
  000860ec: ldm.w r12,{r2,r3,r12}
  000860f0: b 0x0008617c
  000860f2: ldr r0,[0x00086408]
  000860f4: add r0,pc
  000860f6: ldr.w r8,[r0,#0x0]
  000860fa: mov r0,r6
  000860fc: blx r8
  000860fe: ldr.w r0,[r10,#0x120]
  00086102: ldr.w r0,[r0,r11]
  00086106: add.w r1,r0,#0xc
  0008610a: add.w r0,r6,#0xc
  0008610e: blx r8
  00086110: ldr.w r0,[r10,#0x120]
  00086114: ldr.w r0,[r0,r11]
  00086118: add.w r1,r0,#0x18
  0008611c: add.w r0,r6,#0x18
  00086120: blx r8
  00086122: ldr.w r0,[r10,#0x120]
  00086126: ldr.w r0,[r0,r11]
  0008612a: ldr r1,[r0,#0x48]
  0008612c: str r1,[r6,#0x48]
  0008612e: add.w r1,r0,#0x24
  00086132: add.w r0,r6,#0x24
  00086136: blx r8
  00086138: ldr.w r0,[r10,#0x120]
  0008613c: ldr.w r0,[r0,r11]
  00086140: add.w r1,r0,#0x30
  00086144: add.w r0,r6,#0x30
  00086148: blx r8
  0008614a: ldr.w r0,[r10,#0x120]
  0008614e: ldr.w r0,[r0,r11]
  00086152: add.w r1,r0,#0x3c
  00086156: add.w r0,r6,#0x3c
  0008615a: blx r8
  0008615c: add.w r12,sp,#0x8
  00086160: ldm.w r12,{r2,r3,r12}
  00086164: ldr.w r0,[r10,#0x120]
  00086168: ldrd r1,r5,[r6,#0x58]
  0008616c: ldr.w r0,[r0,r11]
  00086170: ldrd r0,r4,[r0,#0x58]
  00086174: orrs r5,r4
  00086176: orrs r0,r1
  00086178: strd r0,r5,[r6,#0x58]
  0008617c: ldrd r0,r1,[r6,#0x58]
  00086180: movs r5,#0x0
  00086182: orrs r1,r3
  00086184: orrs r0,r2
  00086186: strd r0,r1,[r6,#0x58]
  0008618a: subs r0,r2,#0x1
  0008618c: sbc r1,r3,#0x0
  00086190: rsbs.w r4,r0,#0x7
  00086194: sbcs.w r1,r5,r1
  00086198: bcs 0x00086274
  0008619a: eor r0,r2,#0x10
  0008619e: orrs r0,r3
  000861a0: beq.w 0x000862b4
  000861a4: eor r0,r2,#0x20
  000861a8: orrs r0,r3
  000861aa: beq.w 0x000862be
  000861ae: eor r0,r2,#0x38
  000861b2: orrs r0,r3
  000861b4: beq.w 0x000862d8
  000861b8: eor r0,r2,#0x40
  000861bc: orrs r0,r3
  000861be: beq.w 0x00086326
  000861c2: eor r0,r2,#0x80
  000861c6: orrs r0,r3
  000861c8: beq.w 0x0008632e
  000861cc: eor r0,r2,#0x100
  000861d0: orrs r0,r3
  000861d2: beq.w 0x00086336
  000861d6: eor r0,r2,#0x1c0
  000861da: orrs r0,r3
  000861dc: beq.w 0x0008633c
  000861e0: eor r0,r2,#0x200
  000861e4: orrs r0,r3
  000861e6: beq.w 0x00086350
  000861ea: eor r0,r2,#0x400
  000861ee: orrs r0,r3
  000861f0: beq.w 0x00086362
  000861f4: eor r0,r2,#0x800
  000861f8: orrs r0,r3
  000861fa: beq.w 0x00086370
  000861fe: eor r0,r2,#0x1000
  00086202: orrs r0,r3
  00086204: beq.w 0x00086378
  00086208: eor r0,r2,#0x1c00
  0008620c: orrs r0,r3
  0008620e: beq.w 0x0008637e
  00086212: eor r0,r2,#0x2000
  00086216: orrs r0,r3
  00086218: beq.w 0x00086392
  0008621c: eor r0,r2,#0x4000
  00086220: orrs r0,r3
  00086222: beq.w 0x0008639a
  00086226: eor r0,r2,#0x8000
  0008622a: orrs r0,r3
  0008622c: beq.w 0x000863a2
  00086230: eor r0,r2,#0xe000
  00086234: orrs r0,r3
  00086236: beq.w 0x000863a8
  0008623a: eor r0,r2,#0x10000
  0008623e: orrs r0,r3
  00086240: beq.w 0x000863bc
  00086244: eor r0,r2,#0x20000
  00086248: orrs r0,r3
  0008624a: beq.w 0x000863c4
  0008624e: eor r0,r2,#0x40000
  00086252: orrs r0,r3
  00086254: beq.w 0x000863cc
  00086258: eor r0,r2,#0x70000
  0008625c: orrs r0,r3
  0008625e: bne.w 0x000863d2
  00086262: ldr.w r0,[r12,#0x0]
  00086266: str r0,[r6,#0x3c]
  00086268: ldr.w r0,[r12,#0x4]
  0008626c: str r0,[r6,#0x40]
  0008626e: ldr.w r0,[r12,#0x8]
  00086272: b 0x000863d0
  00086274: tbb [pc,r0]
  000862b4: vldr.32 s0,[r12]
  000862b8: vstr.32 s0,[r6,#0x10]
  000862bc: b 0x00086312
  000862be: vldr.32 s0,[r12]
  000862c2: vstr.32 s0,[r6,#0x14]
  000862c6: b 0x00086312
  000862d8: ldr.w r0,[r12,#0x0]
  000862dc: str r0,[r6,#0xc]
  000862de: ldr.w r0,[r12,#0x4]
  000862e2: str r0,[r6,#0x10]
  000862e4: ldr.w r0,[r12,#0x8]
  000862e8: str r0,[r6,#0x14]
  000862ea: vldr.32 s2,[r12]
  000862ee: vldr.32 s4,[r12,#0x4]
  000862f2: vldr.32 s0,[r12,#0x8]
  000862f6: vcmpe.f32 s2,s4
  000862fa: vmrs apsr,fpscr
  000862fe: it gt
  00086300: vmov.gt.f32 s4,s2
  00086304: vcmpe.f32 s4,s0
  00086308: vmrs apsr,fpscr
  0008630c: it gt
  0008630e: vmov.gt.f32 s0,s4
  00086312: vldr.32 s2,[r10,#0xe4]
  00086316: vcmpe.f32 s0,s2
  0008631a: vmrs apsr,fpscr
  0008631e: it gt
  00086320: vstr.gt.32 s0,[r10,#0xe4]
  00086324: b 0x000863d2
  00086326: ldr.w r0,[r12,#0x0]
  0008632a: str r0,[r6,#0x18]
  0008632c: b 0x000863d2
  0008632e: ldr.w r0,[r12,#0x0]
  00086332: str r0,[r6,#0x1c]
  00086334: b 0x000863d2
  00086336: ldr.w r0,[r12,#0x0]
  0008633a: b 0x0008634c
  0008633c: ldr.w r0,[r12,#0x0]
  00086340: str r0,[r6,#0x18]
  00086342: ldr.w r0,[r12,#0x4]
  00086346: str r0,[r6,#0x1c]
  00086348: ldr.w r0,[r12,#0x8]
  0008634c: str r0,[r6,#0x20]
  0008634e: b 0x000863d2
  00086350: vldr.32 s0,[pc,#0xb0]
  00086354: vldr.32 s2,[r12]
  00086358: vdiv.f32 s0,s2,s0
  0008635c: vstr.32 s0,[r6,#0x48]
  00086360: b 0x000863d2
  00086362: vldr.32 s0,[r12]
  00086366: vneg.f32 s0,s0
  0008636a: vstr.32 s0,[r6,#0x24]
  0008636e: b 0x000863d2
  00086370: ldr.w r0,[r12,#0x0]
  00086374: str r0,[r6,#0x28]
  00086376: b 0x000863d2
  00086378: ldr.w r0,[r12,#0x0]
  0008637c: b 0x0008638e
  0008637e: ldr.w r0,[r12,#0x0]
  00086382: str r0,[r6,#0x24]
  00086384: ldr.w r0,[r12,#0x4]
  00086388: str r0,[r6,#0x28]
  0008638a: ldr.w r0,[r12,#0x8]
  0008638e: str r0,[r6,#0x2c]
  00086390: b 0x000863d2
  00086392: ldr.w r0,[r12,#0x0]
  00086396: str r0,[r6,#0x30]
  00086398: b 0x000863d2
  0008639a: ldr.w r0,[r12,#0x0]
  0008639e: str r0,[r6,#0x34]
  000863a0: b 0x000863d2
  000863a2: ldr.w r0,[r12,#0x0]
  000863a6: b 0x000863b8
  000863a8: ldr.w r0,[r12,#0x0]
  000863ac: str r0,[r6,#0x30]
  000863ae: ldr.w r0,[r12,#0x4]
  000863b2: str r0,[r6,#0x34]
  000863b4: ldr.w r0,[r12,#0x8]
  000863b8: str r0,[r6,#0x38]
  000863ba: b 0x000863d2
  000863bc: ldr.w r0,[r12,#0x0]
  000863c0: str r0,[r6,#0x3c]
  000863c2: b 0x000863d2
  000863c4: ldr.w r0,[r12,#0x0]
  000863c8: str r0,[r6,#0x40]
  000863ca: b 0x000863d2
  000863cc: ldr.w r0,[r12,#0x0]
  000863d0: str r0,[r6,#0x44]
  000863d2: mov r0,r10
  000863d4: mov r1,r6
  000863d6: mov r2,r9
  000863d8: blx 0x0006f85c
  000863dc: ldr r0,[sp,#0x24]
  000863de: ldr r1,[sp,#0x14]
  000863e0: ldr r1,[r1,#0x0]
  000863e2: subs r0,r1,r0
  000863e4: itttt eq
  000863e6: vpop.eq {d3,d4,d5,d6,d7,d8}
  000863ea: add.eq sp,#0x4
  000863ec: pop.eq.w {r8,r9,r10,r11}
  000863f0: pop.eq {r4,r5,r6,r7,pc}
  000863f2: blx 0x0006e824
```
