# ParticleSystemManager::enableSystemEmit
elf 0x183b6c body 36
Sig: undefined __stdcall enableSystemEmit(int param_1, bool param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x00193bc4) */
/* WARNING: Removing unreachable block (ram,0x00193bd0) */
/* ParticleSystemManager::enableSystemEmit(int, bool) */

void ParticleSystemManager::enableSystemEmit(int param_1,bool param_2)

{
  undefined1 uVar1;
  int *piVar2;
  int *piVar3;
  int unaff_r4;
  uint uVar4;
  uint uVar5;
  uint unaff_r9;
  undefined1 *unaff_r10;
  undefined1 *unaff_r11;
  undefined1 *puVar6;
  undefined1 *puVar7;
  int in_r12;
  int extraout_r12;
  int extraout_r12_00;
  int *unaff_lr;
  undefined4 unaff_pc;
  undefined1 in_NG;
  undefined1 in_ZR;
  undefined1 in_CY;
  undefined1 in_OV;
  undefined1 uVar8;
  bool bVar9;
  undefined4 in_cr6;
  undefined4 in_cr8;
  undefined4 in_cr12;
  undefined8 uVar10;
  
  uVar5 = (uint)param_2;
  if (in_NG != in_OV) {
    software_interrupt(0x81c4b);
  }
  uVar1 = (undefined1)unaff_r4;
  puVar6 = unaff_r11;
  if ((bool)in_ZR) {
    puVar6 = unaff_r11 + -0x770;
    *unaff_r11 = uVar1;
  }
  if ((bool)in_OV) {
    *(int *)(param_1 + 4) = param_1;
    *(undefined1 **)(param_1 + 8) = unaff_r10;
    *(int *)(param_1 + 0xc) = in_r12;
    *(int **)(param_1 + 0x10) = unaff_lr;
    *(undefined4 *)(param_1 + 0x14) = 0x193b7c;
  }
  if ((bool)in_NG) {
    unaff_lr = (int *)0x193b7c;
    uVar10 = func_0x002cbb88();
    uVar5 = (uint)((ulonglong)uVar10 >> 0x20);
    param_1 = (int)uVar10;
    in_r12 = extraout_r12;
  }
  uVar10 = CONCAT44(uVar5,param_1);
  uVar8 = false;
  if ((bool)in_NG) {
    in_CY = (byte)((uint)param_1 >> 8) & 1;
    uVar4 = unaff_r9 & param_1 << 0x18;
    uVar8 = (int)uVar4 < 0;
    in_ZR = uVar4 == 0;
  }
  if ((bool)in_ZR) {
    unaff_lr = (int *)(uVar5 + param_1 * 0x400000);
  }
  if ((bool)in_OV) {
    *(int **)param_1 = unaff_lr;
    *(undefined1 **)(param_1 + -4) = unaff_r10;
    *(uint *)(param_1 + -8) = unaff_r9;
    *(int *)(param_1 + -0xc) = unaff_r4;
    *(int *)(param_1 + -0x10) = param_1;
  }
  if (uVar8 != in_OV) {
    coprocessor_moveto(0,6,0,unaff_pc,in_cr6,in_cr8);
  }
  if (!(bool)in_CY) {
    software_interrupt(0xffbfff);
  }
  if (uVar8 != in_OV) {
    software_interrupt(0x81c4b);
  }
  puVar7 = puVar6;
  if ((bool)in_ZR) {
    puVar7 = puVar6 + -0x770;
    *puVar6 = uVar1;
  }
  if ((bool)in_OV) {
    *(int *)(param_1 + 4) = param_1;
    *(undefined1 **)(param_1 + 8) = unaff_r10;
    *(int *)(param_1 + 0xc) = in_r12;
    *(int **)(param_1 + 0x10) = unaff_lr;
    *(int *)(param_1 + 0x14) = 0x193ba0;
  }
  if ((bool)uVar8) {
    unaff_lr = (int *)0x193ba0;
    uVar10 = func_0x002cbbac();
    in_r12 = extraout_r12_00;
  }
  piVar3 = (int *)((ulonglong)uVar10 >> 0x20);
  piVar2 = (int *)uVar10;
  bVar9 = false;
  if ((bool)uVar8) {
    in_CY = (byte)((ulonglong)uVar10 >> 8) & 1;
    uVar5 = unaff_r9 & (int)piVar2 << 0x18;
    bVar9 = (int)uVar5 < 0;
    in_ZR = uVar5 == 0;
  }
  if ((bool)in_ZR) {
    unaff_lr = piVar3 + (int)piVar2 * 0x100000;
  }
  if ((bool)in_OV) {
    *piVar2 = (int)unaff_lr;
    piVar2[-1] = (int)unaff_r10;
    piVar2[-2] = unaff_r9;
    piVar2[-3] = unaff_r4;
    piVar2[-4] = (int)piVar2;
  }
  if (bVar9 != (bool)in_OV) {
    coprocessor_movefromRt(0,5,0,in_cr12,in_cr8);
  }
  if (!(bool)in_CY) {
    software_interrupt(0xffbfff);
  }
  if (bVar9 != (bool)in_OV) {
    software_interrupt(0x81c4a);
  }
  puVar6 = unaff_r10;
  if ((bool)in_ZR) {
    puVar6 = unaff_r10 + -0x770;
    *unaff_r10 = uVar1;
  }
  if ((bool)in_OV) {
    piVar2[1] = (int)piVar2;
    piVar2[2] = (int)puVar6;
    piVar2[3] = in_r12;
    piVar2[4] = (int)unaff_lr;
    piVar2[5] = 0x193bc4;
  }
  if (!bVar9) {
    if ((bool)in_ZR) {
      unaff_lr = piVar3 + (int)piVar2 * 0x100000;
    }
    if ((bool)in_OV) {
      *piVar3 = (int)unaff_lr;
      piVar3[-1] = (int)register0x00000054;
      piVar3[-2] = (int)puVar7;
    }
    if (!(bool)in_CY) {
      software_interrupt(0xffbfff);
    }
    if ((bool)in_OV == false) {
      software_interrupt(0x2b5b0);
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
  00193b6c: swilt 0x81c4b
  00193b70: strbeq r4,[r11],#-0x770
  00193b74: stmibvs r0,{r0,r10,r12,lr,pc}^
  00193b78: blmi 0x002cbb88
  00193b7c: andmis r6,r9,r0, lsl #0x18
  00193b80: addeq lr,r1,r0, lsl #0x16
  00193b84: stmdavs r0,{r0,r4,r9,r10,lr}
  00193b88: mcrlt p0,0x6,pc,cr6,cr8,0x0
  00193b8c: swicc 0xffbfff
```
