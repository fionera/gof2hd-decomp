# ModStation::leaveStation
elf 0xd8e58 body 16
Sig: undefined __stdcall leaveStation(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* ModStation::leaveStation() */

void ModStation::leaveStation(void)

{
  undefined4 *in_r3;
  int unaff_r4;
  undefined4 unaff_r5;
  undefined4 *unaff_r6;
  undefined4 unaff_r10;
  undefined4 unaff_lr;
  bool in_NG;
  bool in_OV;
  undefined4 in_cr15;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  else {
    coprocessor_store(8,in_cr15,unaff_r4 + -0x134);
  }
  if (in_NG) {
    unaff_lr = *in_r3;
    unaff_r10 = in_r3[-1];
    in_r3 = in_r3 + -4;
  }
  if (in_OV) {
    *unaff_r6 = unaff_lr;
    unaff_r6[-1] = unaff_r10;
    unaff_r6[-2] = unaff_r6;
    unaff_r6[-3] = unaff_r5;
    unaff_r6[-4] = unaff_r4;
    unaff_r6[-5] = in_r3;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  000e8e58: swige 0x3b5f0
  000e8e5c: stclt p8,cr15,[r4,#-0x134]
  000e8e60: ldmdami r3!,{r2,r9,r10,lr}^
  000e8e64: stmdavs r6,{r3,r4,r5,r6,r10,lr}
```
