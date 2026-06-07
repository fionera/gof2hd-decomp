# ParticleSystemManager::enableSystemRender
elf 0x183b90 body 36
Sig: undefined __stdcall enableSystemRender(int param_1, bool param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x00193bc4) */
/* WARNING: Removing unreachable block (ram,0x00193bd0) */
/* ParticleSystemManager::enableSystemRender(int, bool) */

void ParticleSystemManager::enableSystemRender(int param_1,bool param_2)

{
  int *piVar1;
  int unaff_r4;
  uint uVar2;
  uint unaff_r9;
  undefined1 *unaff_r10;
  undefined1 *puVar3;
  undefined1 *unaff_r11;
  undefined1 *puVar4;
  int in_r12;
  int extraout_r12;
  int *unaff_lr;
  undefined1 in_NG;
  undefined1 in_ZR;
  undefined1 in_CY;
  undefined1 in_OV;
  bool bVar5;
  undefined4 in_cr8;
  undefined4 in_cr12;
  undefined8 uVar6;
  
  piVar1 = (int *)(uint)param_2;
  if (in_NG != in_OV) {
    software_interrupt(0x81c4b);
  }
  puVar4 = unaff_r11;
  if ((bool)in_ZR) {
    puVar4 = unaff_r11 + -0x770;
    *unaff_r11 = (char)unaff_r4;
  }
  if ((bool)in_OV) {
    *(int *)(param_1 + 4) = param_1;
    *(undefined1 **)(param_1 + 8) = unaff_r10;
    *(int *)(param_1 + 0xc) = in_r12;
    *(int **)(param_1 + 0x10) = unaff_lr;
    *(undefined4 *)(param_1 + 0x14) = 0x193ba0;
  }
  if ((bool)in_NG) {
    unaff_lr = (int *)0x193ba0;
    uVar6 = func_0x002cbbac();
    piVar1 = (int *)((ulonglong)uVar6 >> 0x20);
    param_1 = (int)uVar6;
    in_r12 = extraout_r12;
  }
  bVar5 = false;
  if ((bool)in_NG) {
    in_CY = (byte)((uint)param_1 >> 8) & 1;
    uVar2 = unaff_r9 & param_1 << 0x18;
    bVar5 = (int)uVar2 < 0;
    in_ZR = uVar2 == 0;
  }
  if ((bool)in_ZR) {
    unaff_lr = piVar1 + param_1 * 0x100000;
  }
  if ((bool)in_OV) {
    *(int **)param_1 = unaff_lr;
    *(undefined1 **)(param_1 + -4) = unaff_r10;
    *(uint *)(param_1 + -8) = unaff_r9;
    *(int *)(param_1 + -0xc) = unaff_r4;
    *(int *)(param_1 + -0x10) = param_1;
  }
  if (bVar5 != (bool)in_OV) {
    coprocessor_movefromRt(0,5,0,in_cr12,in_cr8);
  }
  if (!(bool)in_CY) {
    software_interrupt(0xffbfff);
  }
  if (bVar5 != (bool)in_OV) {
    software_interrupt(0x81c4a);
  }
  puVar3 = unaff_r10;
  if ((bool)in_ZR) {
    puVar3 = unaff_r10 + -0x770;
    *unaff_r10 = (char)unaff_r4;
  }
  if ((bool)in_OV) {
    *(int *)(param_1 + 4) = param_1;
    *(undefined1 **)(param_1 + 8) = puVar3;
    *(int *)(param_1 + 0xc) = in_r12;
    *(int **)(param_1 + 0x10) = unaff_lr;
    *(int *)(param_1 + 0x14) = 0x193bc4;
  }
  if (!bVar5) {
    if ((bool)in_ZR) {
      unaff_lr = piVar1 + param_1 * 0x100000;
    }
    if ((bool)in_OV) {
      *piVar1 = (int)unaff_lr;
      piVar1[-1] = (int)register0x00000054;
      piVar1[-2] = (int)puVar4;
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
  00193b90: swilt 0x81c4b
  00193b94: strbeq r4,[r11],#-0x770
  00193b98: stmibvs r0,{r0,r10,r12,lr,pc}^
  00193b9c: blmi 0x002cbbac
  00193ba0: andmis r6,r9,r0, lsl #0x18
  00193ba4: addeq lr,r1,r0, lsl #0x16
  00193ba8: stmdavs r0,{r0,r4,r9,r10,lr}
  00193bac: mrclt p0,0x5,pc,cr12,cr8,0x0
  00193bb0: swicc 0xffbfff
```
