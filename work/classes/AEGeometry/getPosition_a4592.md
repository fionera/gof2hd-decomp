# AEGeometry::getPosition
elf 0xa4592 body 35
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AEGeometry::getPosition() */

void AEGeometry::getPosition(void)

{
  int *in_r0;
  int *piVar1;
  int in_r1;
  int in_r2;
  undefined4 *unaff_r4;
  undefined4 *puVar2;
  undefined4 unaff_r6;
  int unaff_r8;
  int unaff_r9;
  int *unaff_r10;
  uint unaff_r11;
  bool in_NG;
  bool in_OV;
  undefined4 in_cr15;
  
  if (in_NG == in_OV) {
    software_interrupt(0x2b5d0);
  }
  puVar2 = unaff_r4;
  if (in_NG) {
    puVar2 = (undefined4 *)((int)unaff_r4 - ((int)unaff_r10 >> 0x11));
    *unaff_r4 = unaff_r6;
    in_r1 = in_r1 - (unaff_r8 >> 0x15);
  }
  coprocessor_load(7,in_cr15,in_r2 + 0x2f4);
  piVar1 = in_r0;
  if (in_NG) {
    piVar1 = in_r0 + in_r1 * -0x400;
    *in_r0 = (int)puVar2;
  }
  coprocessor_storelong(7,in_cr15,in_r2 + 0x2e8);
  if (in_NG != in_OV) {
    unaff_r11 = (uint)*(byte *)(piVar1 + 0x374);
  }
  if (!in_OV) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *unaff_r10 = in_r1;
  unaff_r10[1] = unaff_r8;
  unaff_r10[2] = unaff_r9;
  unaff_r10[3] = (int)unaff_r10;
  unaff_r10[4] = unaff_r11;
  unaff_r10[5] = (int)register0x00000054;
  unaff_r10[6] = (int)&UNK_000b45b6;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  000b4592: swige 0x2b5d0
  000b4596: strmi r6,[r4],-r10,asr #0x11
  000b459a: ldrmi r6,[r1],-r8,asr #0x15
  000b459e: ldc p7,cr15,[r2,#0x2f4]
  000b45a2: strtmi r4,[r0],-r1,lsl #0xc
  000b45a6: stcl p7,cr15,[r2,#0x2e8]!
  000b45aa: ldrblt r11,[r0,#0xdd0]
  000b45ae: stmiavs r10,{r1,r8,r9,r10,r11,sp,pc}^
  000b45b2: bvs 0xff2c5dca
```
