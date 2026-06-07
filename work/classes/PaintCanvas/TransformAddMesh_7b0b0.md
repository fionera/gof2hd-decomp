# PaintCanvas::TransformAddMesh
elf 0x7b0b0 body 16
Sig: undefined __stdcall TransformAddMesh(uint param_1, ushort param_2, bool param_3)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AbyssEngine::PaintCanvas::TransformAddMesh(unsigned int, unsigned short, bool) */

void AbyssEngine::PaintCanvas::TransformAddMesh(uint param_1,ushort param_2,bool param_3)

{
  int unaff_r4;
  undefined4 *unaff_r5;
  undefined4 unaff_r11;
  int *in_r12;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  char in_OV;
  undefined4 in_cr15;
  
  if (in_NG == (bool)in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_CY && !in_ZR) {
    coprocessor_store(8,in_cr15,unaff_r4 + -0x134);
  }
  if (in_NG) {
    *unaff_r5 = unaff_r11;
    *in_r12 = unaff_r4;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  0008b0b0: swige 0x3b5f0
  0008b0b4: stchi p8,cr15,[r4,#-0x134]
  0008b0b8: strmi r11,[r5],-r2,lsl #0x1
  0008b0bc: strmi r4,[r12],-sp,lsr #0x10
```
