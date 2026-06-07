# AEGeometry::getMatrix
elf 0xa4616 body 23
Sig: undefined __stdcall getMatrix(void)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0xff0ce932) */
/* AEGeometry::getMatrix() */

void AEGeometry::getMatrix(void)

{
  undefined1 *in_r1;
  undefined1 *extraout_r1;
  int in_r2;
  undefined1 unaff_r6;
  undefined4 *unaff_r10;
  undefined4 unaff_r11;
  undefined1 in_NG;
  char in_OV;
  undefined4 in_cr15;
  
  if ((bool)in_NG) {
    uRam000b461e = (undefined1)unaff_r11;
  }
  if (!(bool)in_OV) {
    coprocessor_loadlong(7,in_cr15,in_r2 + -0x2f4);
    if ((bool)in_NG) {
      *unaff_r10 = unaff_r11;
    }
    if ((bool)in_NG != false) {
      func_0xfe630a0e();
      in_r1 = extraout_r1;
    }
    if ((bool)in_NG) {
      *in_r1 = unaff_r6;
    }
    if (in_NG != in_OV) {
      func_0xfe630a1e();
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
  000b4616: strbtmi r11,[pc],-r0,lsl #0xb
  000b461a: bvs 0xff0ce926
  000b461e: ldcl p7,cr15,[r2,#-0x2f4]
  000b4622: strmi r11,[r10],-r0,lsl #0x1b
  000b4626: bvs 0xff0ce932
  000b462a: bllt 0xfe630a0e
```
