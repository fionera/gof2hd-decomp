# MenuTouchWindow::startSupernovaChallenge
elf 0x12d0cc body 16
Sig: undefined __stdcall startSupernovaChallenge(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* MenuTouchWindow::startSupernovaChallenge() */

void MenuTouchWindow::startSupernovaChallenge(void)

{
  undefined4 in_r0;
  undefined4 in_r3;
  undefined4 unaff_r7;
  undefined4 *in_r12;
  bool in_NG;
  bool in_ZR;
  char in_OV;
  
  if (in_NG == (bool)in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0xe92d);
  }
  if (in_NG) {
    *in_r12 = in_r0;
    in_r12[1] = in_r3;
    in_r12[2] = unaff_r7;
    in_r12[3] = in_r12;
    in_r12[4] = register0x00000054;
    in_r12[5] = &UNK_0013d0dc;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  0013d0cc: swige 0x3b5f0
  0013d0d0: swieq 0xe92d
  0013d0d4: stmiami r12!,{r0,r3,r7,r12,sp,pc}
  0013d0d8: ldrbtmi r4,[r8],#-0x9ac
```
