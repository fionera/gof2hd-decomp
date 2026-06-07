# Explosion::start
elf 0xa853c body 20
Sig: undefined __thiscall start(Explosion * this, Matrix * param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* Explosion::start(AbyssEngine::AEMath::Matrix const&) */

void __thiscall Explosion::start(Explosion *this,Matrix *param_1)

{
  undefined4 *unaff_r5;
  undefined4 unaff_r7;
  undefined4 *unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined4 unaff_lr;
  bool in_NG;
  bool in_ZR;
  bool in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0xf8e92d);
  }
  if (in_NG) {
    *unaff_r5 = unaff_lr;
    unaff_r5[-1] = unaff_r10;
    unaff_r5[-2] = unaff_r9;
    unaff_r5[-3] = unaff_r7;
    unaff_r5[-4] = param_1;
    unaff_r5[-5] = this;
    unaff_r8 = (undefined4 *)((int)unaff_r8 + -0x689);
  }
  if (in_OV) {
    *unaff_r8 = unaff_lr;
    unaff_r8[-1] = register0x00000054;
    unaff_r8[-2] = unaff_r11;
    unaff_r8[-3] = this;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  000b853c: swige 0x3b5f0
  000b8540: swieq 0xf8e92d
  000b8544: stmdami r5,{r0,r1,r7,r9,r10,lr}^
  000b8548: ldrbtmi r4,[r8],#-0x689
  000b854c: stmdavs r8,{r0,r11,sp,lr}
```
