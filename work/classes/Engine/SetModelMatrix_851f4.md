# Engine::SetModelMatrix
elf 0x851f4 body 400
Sig: undefined __stdcall SetModelMatrix(Matrix * param_1)

## decompile
```c

/* AbyssEngine::Engine::SetModelMatrix(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::Engine::SetModelMatrix(Matrix *param_1)

{
  Vector *in_r1;
  int iVar1;
  AEMath *pAVar2;
  int *piVar3;
  Matrix aMStack_3c [12];
  
  piVar3 = *(int **)(DAT_00095388 + 0x9520a);
  iVar1 = *piVar3;
  if (**(char **)(DAT_00095384 + 0x95208) != '\0') {
    *(undefined4 *)(param_1 + 0x204) = *(undefined4 *)in_r1;
    *(undefined4 *)(param_1 + 0x208) = *(undefined4 *)(in_r1 + 0x10);
    *(undefined4 *)(param_1 + 0x20c) = *(undefined4 *)(in_r1 + 0x20);
    *(undefined4 *)(param_1 + 0x210) = *(undefined4 *)(in_r1 + 4);
    *(undefined4 *)(param_1 + 0x214) = *(undefined4 *)(in_r1 + 0x14);
    *(undefined4 *)(param_1 + 0x218) = *(undefined4 *)(in_r1 + 0x24);
    *(undefined4 *)(param_1 + 0x21c) = *(undefined4 *)(in_r1 + 8);
    *(undefined4 *)(param_1 + 0x220) = *(undefined4 *)(in_r1 + 0x18);
    *(undefined4 *)(param_1 + 0x224) = *(undefined4 *)(in_r1 + 0x28);
    *(undefined4 *)(param_1 + 0x144) = *(undefined4 *)in_r1;
    *(undefined4 *)(param_1 + 0x148) = *(undefined4 *)(in_r1 + 0x10);
    *(undefined4 *)(param_1 + 0x14c) = *(undefined4 *)(in_r1 + 0x20);
    *(undefined4 *)(param_1 + 0x150) = 0;
    *(undefined4 *)(param_1 + 0x154) = *(undefined4 *)(in_r1 + 4);
    *(undefined4 *)(param_1 + 0x158) = *(undefined4 *)(in_r1 + 0x14);
    *(undefined4 *)(param_1 + 0x15c) = *(undefined4 *)(in_r1 + 0x24);
    *(undefined4 *)(param_1 + 0x160) = 0;
    *(undefined4 *)(param_1 + 0x164) = *(undefined4 *)(in_r1 + 8);
    *(undefined4 *)(param_1 + 0x168) = *(undefined4 *)(in_r1 + 0x18);
    *(undefined4 *)(param_1 + 0x16c) = *(undefined4 *)(in_r1 + 0x28);
    *(undefined4 *)(param_1 + 0x170) = 0;
    *(undefined4 *)(param_1 + 0x174) = *(undefined4 *)(in_r1 + 0xc);
    *(undefined4 *)(param_1 + 0x178) = *(undefined4 *)(in_r1 + 0x1c);
    *(undefined4 *)(param_1 + 0x17c) = *(undefined4 *)(in_r1 + 0x2c);
    *(undefined4 *)(param_1 + 0x180) = 0x3f800000;
    if (*(float *)(param_1 + 0x378) == 0.0) {
      AEMath::MatrixInverseRotateVector(aMStack_3c,in_r1);
      pAVar2 = (AEMath *)&stack0xffffffd0;
      AEMath::VectorNormalize(pAVar2,(Vector *)aMStack_3c);
    }
    else {
      pAVar2 = (AEMath *)(param_1 + 0x468);
    }
    AEMath::Vector::operator=((Vector *)(param_1 + 0x330),(Vector *)pAVar2);
    if (1 < *(int *)(param_1 + 0x32c)) {
      if (*(float *)(param_1 + 0x37c) == 0.0) {
        AEMath::MatrixInverseRotateVector(aMStack_3c,in_r1);
        pAVar2 = (AEMath *)&stack0xffffffd0;
        AEMath::VectorNormalize(pAVar2,(Vector *)aMStack_3c);
      }
      else {
        pAVar2 = (AEMath *)(param_1 + 0x474);
      }
      AEMath::Vector::operator=((Vector *)(param_1 + 0x33c),(Vector *)pAVar2);
    }
    ShaderUpdate((Engine *)param_1);
    AEMath::MatrixInverseTransformVector((Matrix *)&stack0xffffffd0,in_r1);
    AEMath::Vector::operator=((Vector *)(param_1 + 0x34c),(Vector *)&stack0xffffffd0);
    *(float *)(param_1 + 0x34c) = *(float *)(param_1 + 0x34c) / *(float *)(in_r1 + 0x30);
    *(float *)(param_1 + 0x350) = *(float *)(param_1 + 0x350) / *(float *)(in_r1 + 0x34);
    *(float *)(param_1 + 0x354) = *(float *)(param_1 + 0x354) / *(float *)(in_r1 + 0x38);
  }
  if (*piVar3 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000951f4: push {r4,r5,r6,r7,lr}
  000951f6: add r7,sp,#0xc
  000951f8: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000951fc: mov r4,r0
  000951fe: ldr r0,[0x00095384]
  00095200: mov r5,r1
  00095202: ldr r1,[0x00095388]
  00095204: add r0,pc
  00095206: add r1,pc
  00095208: ldr r0,[r0,#0x0]
  0009520a: ldr.w r9,[r1,#0x0]
  0009520e: ldrb r0,[r0,#0x0]
  00095210: ldr.w r1,[r9,#0x0]
  00095214: cmp r0,#0x0
  00095216: str r1,[sp,#0x1c]
  00095218: beq.w 0x0009536e
  0009521c: ldr r0,[r5,#0x0]
  0009521e: movs r1,#0x0
  00095220: str.w r0,[r4,#0x204]
  00095224: ldr r0,[r5,#0x10]
  00095226: str.w r0,[r4,#0x208]
  0009522a: ldr r0,[r5,#0x20]
  0009522c: str.w r0,[r4,#0x20c]
  00095230: ldr r0,[r5,#0x4]
  00095232: str.w r0,[r4,#0x210]
  00095236: ldr r0,[r5,#0x14]
  00095238: str.w r0,[r4,#0x214]
  0009523c: ldr r0,[r5,#0x24]
  0009523e: str.w r0,[r4,#0x218]
  00095242: ldr r0,[r5,#0x8]
  00095244: str.w r0,[r4,#0x21c]
  00095248: ldr r0,[r5,#0x18]
  0009524a: str.w r0,[r4,#0x220]
  0009524e: ldr r0,[r5,#0x28]
  00095250: str.w r0,[r4,#0x224]
  00095254: ldr r0,[r5,#0x0]
  00095256: str.w r0,[r4,#0x144]
  0009525a: ldr r0,[r5,#0x10]
  0009525c: str.w r0,[r4,#0x148]
  00095260: ldr r0,[r5,#0x20]
  00095262: strd r0,r1,[r4,#0x14c]
  00095266: ldr r0,[r5,#0x4]
  00095268: str.w r0,[r4,#0x154]
  0009526c: ldr r0,[r5,#0x14]
  0009526e: str.w r0,[r4,#0x158]
  00095272: ldr r0,[r5,#0x24]
  00095274: strd r0,r1,[r4,#0x15c]
  00095278: ldr r0,[r5,#0x8]
  0009527a: str.w r0,[r4,#0x164]
  0009527e: ldr r0,[r5,#0x18]
  00095280: str.w r0,[r4,#0x168]
  00095284: ldr r0,[r5,#0x28]
  00095286: strd r0,r1,[r4,#0x16c]
  0009528a: mov.w r1,#0x3f800000
  0009528e: ldr r0,[r5,#0xc]
  00095290: str.w r0,[r4,#0x174]
  00095294: ldr r0,[r5,#0x1c]
  00095296: str.w r0,[r4,#0x178]
  0009529a: ldr r0,[r5,#0x2c]
  0009529c: strd r0,r1,[r4,#0x17c]
  000952a0: vldr.32 s0,[r4,#0x378]
  000952a4: vcmpe.f32 s0,#0
  000952a8: vmrs apsr,fpscr
  000952ac: bne 0x000952d0
  000952ae: add.w r8,sp,#0x4
  000952b2: add.w r2,r4,#0x468
  000952b6: mov r1,r5
  000952b8: mov r0,r8
  000952ba: blx 0x0007075c
  000952be: add r6,sp,#0x10
  000952c0: mov r1,r8
  000952c2: mov r0,r6
  000952c4: blx 0x0006ec80
  000952c8: add.w r0,r4,#0x330
  000952cc: mov r1,r6
  000952ce: b 0x000952d8
  000952d0: add.w r0,r4,#0x330
  000952d4: add.w r1,r4,#0x468
  000952d8: blx 0x0006eb3c
  000952dc: ldr.w r0,[r4,#0x32c]
  000952e0: cmp r0,#0x2
  000952e2: blt 0x00095320
  000952e4: vldr.32 s0,[r4,#0x37c]
  000952e8: vcmpe.f32 s0,#0
  000952ec: vmrs apsr,fpscr
  000952f0: bne 0x00095314
  000952f2: add.w r8,sp,#0x4
  000952f6: addw r2,r4,#0x474
  000952fa: mov r1,r5
  000952fc: mov r0,r8
  000952fe: blx 0x0007075c
  00095302: add r6,sp,#0x10
  00095304: mov r1,r8
  00095306: mov r0,r6
  00095308: blx 0x0006ec80
  0009530c: add.w r0,r4,#0x33c
  00095310: mov r1,r6
  00095312: b 0x0009531c
  00095314: add.w r0,r4,#0x33c
  00095318: addw r1,r4,#0x474
  0009531c: blx 0x0006eb3c
  00095320: mov r0,r4
  00095322: blx 0x00070768
  00095326: add r6,sp,#0x10
  00095328: add.w r2,r4,#0x3fc
  0009532c: mov r1,r5
  0009532e: mov r0,r6
  00095330: blx 0x0006f4b4
  00095334: add.w r0,r4,#0x34c
  00095338: mov r1,r6
  0009533a: blx 0x0006eb3c
  0009533e: vldr.32 s0,[r4,#0x34c]
  00095342: vldr.32 s6,[r5,#0x30]
  00095346: vldr.32 s2,[r4,#0x350]
  0009534a: vdiv.f32 s0,s0,s6
  0009534e: vldr.32 s4,[r4,#0x354]
  00095352: vstr.32 s0,[r4,#0x34c]
  00095356: vldr.32 s0,[r5,#0x34]
  0009535a: vdiv.f32 s0,s2,s0
  0009535e: vstr.32 s0,[r4,#0x350]
  00095362: vldr.32 s0,[r5,#0x38]
  00095366: vdiv.f32 s0,s4,s0
  0009536a: vstr.32 s0,[r4,#0x354]
  0009536e: ldr r0,[sp,#0x1c]
  00095370: ldr.w r1,[r9,#0x0]
  00095374: subs r0,r1,r0
  00095376: ittt eq
  00095378: add.eq sp,#0x20
  0009537a: pop.eq.w {r8,r9,r11}
  0009537e: pop.eq {r4,r5,r6,r7,pc}
  00095380: blx 0x0006e824
```
