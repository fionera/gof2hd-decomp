# AbyssEngine::Quaternion::Lerp
elf 0x8145c body 114
Sig: undefined __thiscall Lerp(Quaternion * this, float * param_1, float * param_2, float param_3)

## decompile
```c

/* WARNING: Restarted to delay deadcode elimination for space: register */
/* AbyssEngine::Quaternion::Lerp(float const*, float const*, float) */

void __thiscall
AbyssEngine::Quaternion::Lerp(Quaternion *this,float *param_1,float *param_2,float param_3)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined4 in_r3;
  int iVar3;
  int *piVar4;
  undefined4 in_fpscr;
  float fVar5;
  float fVar6;
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  
  piVar4 = *(int **)(DAT_000914d0 + 0x91474);
  iVar3 = *piVar4;
  fVar5 = (float)((ulonglong)*(undefined8 *)(param_2 + 2) >> 0x20);
  auVar8 = *(undefined1 (*) [16])param_1;
  fVar6 = auVar8._4_4_ * (float)((ulonglong)*(undefined8 *)param_2 >> 0x20) +
          auVar8._0_4_ * (float)*(undefined8 *)param_2 +
          auVar8._8_4_ * (float)*(undefined8 *)(param_2 + 2) + auVar8._12_4_ * fVar5;
  if ((char)((byte)((uint)in_fpscr >> 0x18) & 0xf | (byte)(((uint)(fVar6 < 0.0) << 0x1f) >> 0x18) |
             (byte)(((uint)(fVar6 == 0.0) << 0x1e) >> 0x18) |
            (byte)(((uint)(0.0 <= fVar6) << 0x1d) >> 0x18)) < '\0') {
    auVar2._12_4_ = fVar5;
    auVar2._0_12_ = *(undefined1 (*) [12])param_2;
    auVar7 = FloatVectorNeg(auVar2,2,2);
    auVar8 = FloatVectorSub(auVar7,auVar8,2,0x20);
  }
  else {
    auVar7._12_4_ = fVar5;
    auVar7._0_12_ = *(undefined1 (*) [12])param_2;
    auVar8 = FloatVectorSub(auVar7,auVar8,2,0x20);
  }
  auVar1._8_8_ = CONCAT44(in_r3,in_r3);
  auVar1._0_8_ = CONCAT44(in_r3,in_r3);
  FloatVectorMultiplyAccumulate(auVar1,auVar8,2,0x10);
  Normalized();
  if (*piVar4 != iVar3) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  0009145c: push {r4,r5,r7,lr}
  0009145e: add r7,sp,#0x8
  00091460: sub sp,#0x20
  00091462: mov r4,sp
  00091464: bfc r4,#0x0,#0x4
  00091468: mov sp,r4
  0009146a: ldr r4,[0x000914d0]
  0009146c: vmov s8,r3
  00091470: add r4,pc
  00091472: ldr r5,[r4,#0x0]
  00091474: ldr r4,[r5,#0x0]
  00091476: str r4,[sp,#0x1c]
  00091478: vld1.32 {d2,d3},[r2]
  0009147c: vld1.32 {d0,d1},[r1]
  00091480: vmul.f32 s10,s1,s5
  00091484: vmla.f32 s10,s0,s4
  00091488: vmla.f32 s10,s2,s6
  0009148c: vmla.f32 s10,s3,s7
  00091490: vcmpe.f32 s10,#0
  00091494: vmrs apsr,fpscr
  00091498: bpl 0x000914a4
  0009149a: vneg.f32 q8,q1
  0009149e: vsub.f32 q8,q8,q0
  000914a2: b 0x000914a8
  000914a4: vsub.f32 q8,q1,q0
  000914a8: vdup.32 q9,d4[0x0]
  000914ac: mov r1,sp
  000914ae: vmla.f32 q0,q9,q8
  000914b2: vst1.64 {d0,d1},[r1:128]
  000914b6: blx 0x000701e0
  000914ba: ldr r0,[sp,#0x1c]
  000914bc: ldr r1,[r5,#0x0]
  000914be: subs r0,r1,r0
  000914c0: ittt eq
  000914c2: sub.eq.w r4,r7,#0x8
  000914c6: mov.eq sp,r4
  000914c8: pop.eq {r4,r5,r7,pc}
  000914ca: blx 0x0006e824
```
