# ParticleSystemManager::resetSystem
elf 0x183bb4 body 36
Sig: undefined __thiscall resetSystem(ParticleSystemManager * this, int param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x00193bc4) */
/* WARNING: Removing unreachable block (ram,0x00193bd0) */
/* ParticleSystemManager::resetSystem(int) */

void __thiscall ParticleSystemManager::resetSystem(ParticleSystemManager *this,int param_1)

{
  undefined1 unaff_r4;
  undefined1 *unaff_r10;
  undefined1 *puVar1;
  undefined4 unaff_r11;
  undefined4 in_r12;
  int unaff_lr;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  bool in_OV;
  
  if (in_NG != in_OV) {
    software_interrupt(0x81c4a);
  }
  puVar1 = unaff_r10;
  if (in_ZR) {
    puVar1 = unaff_r10 + -0x770;
    *unaff_r10 = unaff_r4;
  }
  if (in_OV) {
    *(ParticleSystemManager **)(this + 4) = this;
    *(undefined1 **)(this + 8) = puVar1;
    *(undefined4 *)(this + 0xc) = in_r12;
    *(int *)(this + 0x10) = unaff_lr;
    *(undefined4 *)(this + 0x14) = 0x193bc4;
  }
  if (!in_NG) {
    if (in_ZR) {
      unaff_lr = param_1 + (int)this * 0x400000;
    }
    if (in_OV) {
      *(int *)param_1 = unaff_lr;
      *(BADSPACEBASE **)(param_1 + -4) = register0x00000054;
      *(undefined4 *)(param_1 + -8) = unaff_r11;
    }
    if (!in_CY) {
      software_interrupt(0xffbfff);
    }
    if (in_OV == false) {
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
  00193bb4: swilt 0x81c4a
  00193bb8: strbeq r4,[r10],#-0x770
  00193bbc: stmibvs r0,{r0,r10,r12,lr,pc}^
  00193bc0: bmi 0x002cbbd0
  00193bc4: andmis r6,r1,r0, lsl #0x18
  00193bc8: addeq lr,r1,r0, lsl #0x16
  00193bcc: stmdavs r1,{r11,sp,lr}
  00193bd0: strmi r6,[r8,-r9,lsl #0x11]
  00193bd4: swicc 0xffbfff
```
