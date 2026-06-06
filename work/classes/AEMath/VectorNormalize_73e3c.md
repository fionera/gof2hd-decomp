# AbyssEngine::AEMath::VectorNormalize
elf 0x73e3c body 60
Sig: undefined __thiscall VectorNormalize(AEMath * this, Vector * param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0013f694) */
/* WARNING: Removing unreachable block (ram,0x0013f698) */
/* WARNING: Removing unreachable block (ram,0x0013f69c) */
/* WARNING: Removing unreachable block (ram,0x0013f6a8) */
/* WARNING: Removing unreachable block (ram,0x0013f6ac) */
/* WARNING: Removing unreachable block (ram,0xff37b170) */
/* WARNING: Removing unreachable block (ram,0x011baf5c) */
/* WARNING: Removing unreachable block (ram,0x0013f674) */
/* WARNING: Removing unreachable block (ram,0x0013f678) */
/* WARNING: Removing unreachable block (ram,0x0013f67c) */
/* WARNING: Removing unreachable block (ram,0x0013f688) */
/* WARNING: Removing unreachable block (ram,0x0013f68c) */
/* WARNING: Removing unreachable block (ram,0x018bace4) */
/* WARNING: Removing unreachable block (ram,0x0013f690) */
/* WARNING: Removing unreachable block (ram,0x0197ace8) */
/* WARNING: Removing unreachable block (ram,0xff37b150) */
/* WARNING: Removing unreachable block (ram,0x011baf3c) */
/* WARNING: Removing unreachable block (ram,0x00083e60) */
/* WARNING: Removing unreachable block (ram,0x00083e64) */
/* WARNING: Removing unreachable block (ram,0x0187acc4) */
/* WARNING: Removing unreachable block (ram,0xff0bf938) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* AbyssEngine::AEMath::VectorNormalize(AbyssEngine::AEMath::Vector const&) */

undefined8 __thiscall AbyssEngine::AEMath::VectorNormalize(AEMath *this,Vector *param_1)

{
  undefined4 *unaff_r4;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  char in_OV;
  undefined4 in_cr4;
  undefined8 uVar1;
  
  if (in_NG == (bool)in_OV) {
    software_interrupt(0x2b5b0);
  }
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  coprocessor_load(6,in_cr4,param_1 + 0x34);
  if (in_NG) {
    *unaff_r4 = this;
  }
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_CY) {
    return CONCAT44(*(undefined4 *)(this + -0x2c),this + -0x30);
  }
  if (!in_ZR) {
    FUN_00137668();
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Could not recover jumptable at 0x0013f668. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  if (in_NG == (bool)in_OV) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  uVar1 = (*_LAB_0013f670)();
  return uVar1;
}

```
## target disasm
```
  00083e3c: swige 0x2b5b0
  00083e40: bne 0x000ff48c
  00083e44: ldc p6,cr4,[r1,#0x34]
  00083e48: strmi r0,[r4],-r0,lsl #0x14
  00083e4c: bne 0x000ff6d8
  00083e50: bcs 0x0013f49c
  00083e54: bne 0x000bf65c
  00083e58: bne 0x0013f668
  00083e5c: beq 0xff0ff928
  00083e60: beq 0xff0bf938
  00083e64: blx 0x004bfa30
  00083e68: cdp p7,0x1,cr13,cr1,cr5,0x0
  000bf65c: bl 0x00137668
  000ff6d8: and lr,r1,r8, asr #0x17
  000ff6dc: andeq r3,lr,r10, lsl #0x6
```
