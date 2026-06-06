# AbyssEngine::AEMath::MatrixGetPosition
elf 0x8072e body 135
Sig: undefined __stdcall MatrixGetPosition(Matrix * param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0014bf32) overlaps instruction at (ram,0x0014bf30)
    */
/* WARNING: Removing unreachable block (ram,0x001cbe2a) */
/* WARNING: Removing unreachable block (ram,0x0014be32) */
/* WARNING: Removing unreachable block (ram,0x000907d6) */
/* WARNING: Removing unreachable block (ram,0x000907da) */
/* WARNING: Removing unreachable block (ram,0x001cbe26) */
/* WARNING: Removing unreachable block (ram,0x001cbe3a) */
/* WARNING: Removing unreachable block (ram,0x001cbe3e) */
/* WARNING: Removing unreachable block (ram,0x001cbe42) */
/* WARNING: Removing unreachable block (ram,0x001cbe46) */
/* WARNING: Removing unreachable block (ram,0x000907de) */
/* WARNING: Removing unreachable block (ram,0x000907e2) */
/* WARNING: Removing unreachable block (ram,0x000907e6) */
/* WARNING: Removing unreachable block (ram,0x0028bfee) */
/* WARNING: Removing unreachable block (ram,0x001cc016) */
/* WARNING: Removing unreachable block (ram,0x001cc006) */
/* WARNING: Removing unreachable block (ram,0x001cbff6) */
/* WARNING: Removing unreachable block (ram,0x000907ea) */
/* WARNING: Removing unreachable block (ram,0x0014be36) */
/* WARNING: Removing unreachable block (ram,0x000907ee) */
/* WARNING: Removing unreachable block (ram,0x001cbffa) */
/* WARNING: Removing unreachable block (ram,0x001cbffe) */
/* WARNING: Removing unreachable block (ram,0x001cc002) */
/* WARNING: Removing unreachable block (ram,0x001cc01a) */
/* WARNING: Removing unreachable block (ram,0x0020bf0a) */
/* WARNING: Removing unreachable block (ram,0x0020bf0e) */
/* WARNING: Removing unreachable block (ram,0x0020bf12) */
/* WARNING: Removing unreachable block (ram,0x0020bf16) */
/* WARNING: Removing unreachable block (ram,0x0020bf1a) */
/* WARNING: Removing unreachable block (ram,0x0020bf1e) */
/* WARNING: Removing unreachable block (ram,0x0020bf22) */
/* WARNING: Removing unreachable block (ram,0x0020bf26) */
/* WARNING: Removing unreachable block (ram,0x0020bf2e) */
/* WARNING: Removing unreachable block (ram,0x0020bf32) */
/* WARNING: Removing unreachable block (ram,0x000908be) */
/* WARNING: Removing unreachable block (ram,0x000908c2) */
/* WARNING: Removing unreachable block (ram,0x0024bf0e) */
/* WARNING: Removing unreachable block (ram,0x000908ba) */
/* WARNING: Removing unreachable block (ram,0x001cbf06) */
/* WARNING: Removing unreachable block (ram,0x001cbf0a) */
/* WARNING: Removing unreachable block (ram,0x001cbf1a) */
/* WARNING: Removing unreachable block (ram,0x001cbf1e) */
/* WARNING: Removing unreachable block (ram,0x001cbf22) */
/* WARNING: Removing unreachable block (ram,0x0014c02a) */
/* WARNING: Removing unreachable block (ram,0x0014c02e) */
/* WARNING: Removing unreachable block (ram,0xfe5aa61a) */
/* WARNING: Removing unreachable block (ram,0x000908ea) */
/* WARNING: Removing unreachable block (ram,0x0030bf36) */
/* WARNING: Removing unreachable block (ram,0x000908de) */
/* WARNING: Removing unreachable block (ram,0x000908e2) */
/* WARNING: Removing unreachable block (ram,0x0014bf32) */
/* WARNING: Removing unreachable block (ram,0x0014bf36) */
/* WARNING: Removing unreachable block (ram,0x000908e6) */
/* WARNING: Removing unreachable block (ram,0x0024c0ee) */
/* WARNING: Removing unreachable block (ram,0x002cbf46) */
/* WARNING: Removing unreachable block (ram,0x0020c112) */
/* WARNING: Removing unreachable block (ram,0x000908fa) */
/* WARNING: Removing unreachable block (ram,0x000908fe) */
/* WARNING: Removing unreachable block (ram,0x000cc10e) */
/* WARNING: Removing unreachable block (ram,0x000cc170) */
/* WARNING: Removing unreachable block (ram,0x000cc178) */
/* WARNING: Removing unreachable block (ram,0x000cc18e) */
/* WARNING: Removing unreachable block (ram,0x000cc18a) */
/* WARNING: Removing unreachable block (ram,0x000cc1a2) */
/* WARNING: Removing unreachable block (ram,0x000cc1b8) */
/* WARNING: Removing unreachable block (ram,0x000cc1b0) */
/* WARNING: Removing unreachable block (ram,0x00090902) */
/* WARNING: Removing unreachable block (ram,0x00090906) */
/* WARNING: Removing unreachable block (ram,0x0024c116) */
/* WARNING: Removing unreachable block (ram,0x000908ee) */
/* WARNING: Removing unreachable block (ram,0x000908f2) */
/* WARNING: Removing unreachable block (ram,0x000908f6) */
/* WARNING: Removing unreachable block (ram,0x008cc0fe) */
/* WARNING: Removing unreachable block (ram,0x0009090a) */
/* WARNING: Removing unreachable block (ram,0x0009090e) */
/* WARNING: Removing unreachable block (ram,0x00090912) */
/* WARNING: Removing unreachable block (ram,0x00090916) */
/* WARNING: Removing unreachable block (ram,0x0009091a) */
/* WARNING: Removing unreachable block (ram,0x00090922) */
/* WARNING: Removing unreachable block (ram,0x00090926) */
/* WARNING: Removing unreachable block (ram,0x0014bf1a) */
/* WARNING: Removing unreachable block (ram,0x0014bf34) */
/* WARNING: Removing unreachable block (ram,0x0014bf30) */
/* WARNING: Removing unreachable block (ram,0x000cbf12) */
/* WARNING: Removing unreachable block (ram,0x0009092a) */
/* WARNING: Removing unreachable block (ram,0x0028c0f6) */
/* WARNING: Removing unreachable block (ram,0x0014bf3a) */
/* WARNING: Removing unreachable block (ram,0x0014c032) */
/* WARNING: Removing unreachable block (ram,0x0014c036) */
/* WARNING: Removing unreachable block (ram,0x0014c03a) */
/* WARNING: Removing unreachable block (ram,0x0014c046) */
/* WARNING: Removing unreachable block (ram,0x000908d6) */
/* WARNING: Removing unreachable block (ram,0x000908da) */
/* WARNING: Removing unreachable block (ram,0x0010bf26) */
/* WARNING: Removing unreachable block (ram,0x000908c6) */
/* WARNING: Removing unreachable block (ram,0x0018c152) */
/* WARNING: Removing unreachable block (ram,0x0018c1b8) */
/* WARNING: Removing unreachable block (ram,0x0018c1a4) */
/* WARNING: Removing unreachable block (ram,0x0018c1b0) */
/* WARNING: Removing unreachable block (ram,0x0018c1b2) */
/* WARNING: Removing unreachable block (ram,0x0018c1bc) */
/* WARNING: Removing unreachable block (ram,0x0018c352) */
/* WARNING: Removing unreachable block (ram,0x0018c218) */
/* WARNING: Removing unreachable block (ram,0x0018c3d4) */
/* WARNING: Removing unreachable block (ram,0x0018c3fe) */
/* WARNING: Removing unreachable block (ram,0x0018c3f2) */
/* WARNING: Removing unreachable block (ram,0x000908ca) */
/* WARNING: Removing unreachable block (ram,0x000908ce) */
/* WARNING: Removing unreachable block (ram,0x000908d2) */
/* WARNING: Removing unreachable block (ram,0x0020c15e) */
/* WARNING: Removing unreachable block (ram,0x000cbf1e) */
/* WARNING: Removing unreachable block (ram,0x000cbf22) */
/* WARNING: Removing unreachable block (ram,0x000cbf4e) */
/* WARNING: Removing unreachable block (ram,0x000cbf56) */
/* WARNING: Removing unreachable block (ram,0x000cbf5a) */
/* WARNING: Removing unreachable block (ram,0x000cbf5e) */
/* WARNING: Removing unreachable block (ram,0x000cbf7e) */
/* WARNING: Removing unreachable block (ram,0x000cbf82) */
/* WARNING: Removing unreachable block (ram,0x000cbf8e) */
/* WARNING: Removing unreachable block (ram,0x000cbf92) */
/* WARNING: Removing unreachable block (ram,0x001cc156) */
/* WARNING: Removing unreachable block (ram,0x001cbf26) */
/* WARNING: Removing unreachable block (ram,0x001cbf36) */
/* WARNING: Removing unreachable block (ram,0x001cbf3a) */
/* WARNING: Removing unreachable block (ram,0x00090816) */
/* WARNING: Removing unreachable block (ram,0x000908b2) */
/* WARNING: Removing unreachable block (ram,0x000908b6) */
/* WARNING: Removing unreachable block (ram,0x0010bf06) */
/* WARNING: Removing unreachable block (ram,0x0010bf0a) */
/* WARNING: Removing unreachable block (ram,0x0010bf0e) */
/* WARNING: Removing unreachable block (ram,0x0010bf12) */
/* WARNING: Removing unreachable block (ram,0x0010bf16) */
/* WARNING: Removing unreachable block (ram,0x0010bf1a) */
/* WARNING: Removing unreachable block (ram,0x0010bf1e) */
/* WARNING: Removing unreachable block (ram,0x00090812) */
/* WARNING: Removing unreachable block (ram,0x0014be1a) */
/* WARNING: Removing unreachable block (ram,0x0014be30) */
/* WARNING: Removing unreachable block (ram,0x0014be34) */
/* WARNING: Removing unreachable block (ram,0x0014be38) */
/* WARNING: Removing unreachable block (ram,0x008872c0) */
/* WARNING: Removing unreachable block (ram,0x0014be40) */
/* WARNING: Removing unreachable block (ram,0x000907f2) */
/* WARNING: Removing unreachable block (ram,0x0034bf3e) */
/* WARNING: Removing unreachable block (ram,0x000907f6) */
/* WARNING: Removing unreachable block (ram,0x008cbffe) */
/* WARNING: Removing unreachable block (ram,0x000907fa) */
/* WARNING: Removing unreachable block (ram,0x0024be46) */
/* WARNING: Removing unreachable block (ram,0x000907fe) */
/* WARNING: Removing unreachable block (ram,0x000cc00e) */
/* WARNING: Removing unreachable block (ram,0x000cc012) */
/* WARNING: Removing unreachable block (ram,0x00090802) */
/* WARNING: Removing unreachable block (ram,0x0024c012) */
/* WARNING: Removing unreachable block (ram,0x00090806) */
/* WARNING: Removing unreachable block (ram,0x0094c016) */
/* WARNING: Removing unreachable block (ram,0x0009080a) */
/* WARNING: Removing unreachable block (ram,0x0009080e) */
/* WARNING: Removing unreachable block (ram,0x000cbe12) */
/* WARNING: Removing unreachable block (ram,0x002cbf2a) */
/* WARNING: Removing unreachable block (ram,0x001cbe56) */
/* WARNING: Removing unreachable block (ram,0x001cbe5a) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* AbyssEngine::AEMath::MatrixGetPosition(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixGetPosition(Matrix *param_1)

{
  uint in_r1;
  undefined4 *in_r2;
  int in_r3;
  int unaff_r4;
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *unaff_r5;
  undefined4 *unaff_r6;
  int unaff_r7;
  undefined4 unaff_r8;
  int *unaff_r9;
  int *piVar3;
  undefined4 *unaff_r10;
  int unaff_r11;
  byte *pbVar4;
  int *in_r12;
  undefined4 unaff_lr;
  int iVar5;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  bool in_OV;
  bool bVar6;
  undefined4 in_cr2;
  undefined4 in_cr4;
  undefined4 in_cr10;
  undefined4 in_cr11;
  undefined4 in_cr12;
  undefined4 in_cr15;
  float fVar7;
  float fVar8;
  float in_s4;
  float fVar9;
  float fVar10;
  undefined8 in_d5;
  float in_s12;
  float fVar11;
  float unaff_s22;
  
  if (in_OV) {
    *unaff_r10 = param_1;
    unaff_r10[1] = in_r1;
    unaff_r10[2] = in_r3;
    unaff_r10[3] = unaff_r6;
    unaff_r10[4] = unaff_r7;
    unaff_r10[5] = unaff_r8;
    unaff_r10[6] = unaff_r11;
    unaff_r10[7] = register0x00000054;
    unaff_r10[8] = unaff_lr;
  }
  *(Matrix **)(param_1 + 4) = param_1;
  *(int *)(param_1 + 8) = in_r3;
  *(undefined4 **)(param_1 + 0xc) = unaff_r6;
  *(int *)(param_1 + 0x10) = unaff_r7;
  *(int **)(param_1 + 0x14) = unaff_r9;
  *(int *)(param_1 + 0x18) = unaff_r11;
  *(BADSPACEBASE **)(param_1 + 0x1c) = register0x00000054;
  *(undefined4 *)(param_1 + 0x20) = unaff_lr;
  if (in_OV) {
    in_r2 = (undefined4 *)(in_r1 + (int)param_1 * 0x40);
  }
  coprocessor_load(7,in_cr4,in_r2 + 0x70);
  fVar7 = *(float *)(in_r1 + 4) * (float)in_r2[0x20];
  fVar9 = in_s4 + fVar7 * in_s4;
  fVar10 = *(float *)(in_r1 + 0xc);
  fVar8 = *(float *)(in_r1 + 0x1c);
  fVar11 = *(float *)(in_r1 + 0xc);
  *(float *)(param_1 + 0x204) =
       in_s12 + fVar9 * unaff_s22 + (float)((ulonglong)in_d5 >> 0x20) + fVar7 * fVar9;
  *(float *)param_1 = fVar9 + fVar10;
  *(float *)(param_1 + 4) = fVar11 + fVar8;
  if (in_NG) {
    param_1 = param_1 + (int)in_r2 * -0x100000;
    in_r1 = (uint)(byte)*param_1;
  }
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_CY) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_NG) {
    if (in_CY) {
      coprocessor_loadlong(0xf,in_cr11,param_1);
    }
    if (in_NG) {
      unaff_r9[-1] = (int)unaff_r5;
      unaff_r9[-2] = in_r3;
      unaff_r9[-3] = (int)param_1;
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_CY) {
    pbVar4 = (byte *)(unaff_r11 + unaff_r4 * 0x1000);
    puVar1 = (undefined4 *)(uint)*pbVar4;
    if (in_NG == in_OV) {
      *unaff_r6 = unaff_lr;
      unaff_r6[-1] = unaff_r10;
      unaff_r6[-2] = unaff_r9;
      unaff_r6[-3] = in_r2;
    }
    coprocessor_function(6,0xe,7,in_cr15,in_cr10,in_cr2);
    if (in_NG) {
      puVar2 = (undefined4 *)((int)puVar1 - (int)unaff_r9);
      *puVar1 = unaff_lr;
      puVar1 = (undefined4 *)((int)puVar2 - (int)unaff_r10);
      *puVar2 = unaff_lr;
      *puVar1 = unaff_lr;
      *(undefined4 *)((int)puVar1 - (int)pbVar4) = unaff_lr;
    }
    if ((int)(in_r1 & (uint)&stack0x00000000) < 0 == in_OV) {
      *(uint *)param_1 = in_r1 & (uint)&stack0x00000000;
      *(undefined4 **)(param_1 + -4) = unaff_r10;
      *(int **)(param_1 + -8) = unaff_r9;
      *(undefined4 **)(param_1 + -0xc) = in_r2;
    }
    coprocessor_function(6,0xd,7,in_cr15,in_cr12,in_cr2);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_NG) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_ZR) {
    if (in_NG == in_OV) {
      unaff_r10 = (undefined4 *)unaff_r6[-1];
      unaff_r9 = (int *)unaff_r6[-2];
      unaff_r7 = unaff_r6[-3];
      in_r1 = unaff_r6[-4];
      param_1 = (Matrix *)unaff_r6[-5];
    }
    iVar5 = (int)unaff_r5 - (int)in_r2;
    bVar6 = iVar5 == 0;
    piVar3 = unaff_r9;
    if ((int)in_r2 <= (int)unaff_r5) {
      iVar5 = *unaff_r9;
      unaff_r10 = (undefined4 *)unaff_r9[-1];
      piVar3 = (int *)unaff_r9[-2];
      unaff_r7 = unaff_r9[-3];
      in_r1 = unaff_r9[-4];
      param_1 = (Matrix *)unaff_r9[-5];
    }
    *in_r2 = 0xcbe32;
    in_r2[-1] = iVar5;
    in_r2[-2] = register0x00000054;
    in_r2[-3] = in_r12;
    in_r2[-4] = unaff_r10;
    in_r2[-5] = piVar3;
    in_r2[-6] = unaff_r8;
    in_r2[-7] = unaff_r7;
    in_r2[-8] = unaff_r5;
    in_r2[-9] = in_r1;
    in_r2[-10] = param_1;
    if (unaff_r5 < in_r2 || bVar6) {
      *in_r12 = 0xcbe36 - ((int)in_r1 >> ((uint)param_1 & 0xff));
      in_r12[-1] = (int)unaff_r10;
      in_r12[-2] = (int)piVar3;
      in_r12[-3] = unaff_r7;
      in_r12[-4] = in_r1;
      in_r12[-5] = (int)param_1;
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  0009072e: stmiavs r10,{r0,r1,r3,r6,r7,r8,r11,sp,lr}^
  00090732: stmib r0,{r0,r3,r6,r7,r9,r11,sp,lr}^
  00090736: addvs r2,r1,r0, lsl #0x6
  0009073a: ldc p7,cr4,[r2,#0x1c0]
  0009073e: vldr.32 s2,[r1,#0x4]
  00090742: vldr.32 s9,[r1,#0x4]
  00090746: vldr.32 s1,[r1,#0x14]
  0009074a: vmul.f32 s8,s2,s18
  0009074e: vmul.f32 s9,s2,s8
  00090752: vldr.32 s0,[r2,#0x80]
  00090756: vmul.f32 s0,s2,s0
  0009075a: vldr.32 s2,[r1,#0x90]
  0009075e: vldr.32 s6,[r1]
  00090762: vldr.32 s15,[r1,#0x10]
  00090766: vldr.32 s6,[r2,#0x20]
  0009076a: vmla.f32 s4,s0,s4
  0009076e: vldr.32 s8,[r1,#0xc]
  00090772: vmla.f32 s11,s0,s4
  00090776: vldr.32 s1,[r1,#0x1c]
  0009077a: vmla.f32 s2,s0,s12
  0009077e: vldr.32 s3,[r1,#0x8c]
  00090782: vldr.32 s10,[r1,#0x28]
  00090786: vldr.32 s13,[r1,#0xc]
  0009078a: vldr.32 s5,[r1,#0x1c]
  0009078e: vmla.f32 s12,s4,s22
  00090792: vmla.f32 s9,s4,s10
  00090796: vmla.f32 s0,s4,s3
  0009079a: vadd.f32 s2,s12,s11
  0009079e: vadd.f32 s4,s4,s8
  000907a2: vadd.f32 s0,s13,s1
  000907a6: vstr.32 s2,[r0,#0x204]
  000907aa: vstr.32 s4,[r0]
  000907ae: vstr.32 s0,[r0,#0x4]
  000907b2: ldrbmi r1,[r0,-r2,lsl #0x14]!
```
