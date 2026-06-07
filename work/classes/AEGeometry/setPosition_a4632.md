# AEGeometry::setPosition
elf 0xa4632 body 15
Sig: undefined __thiscall setPosition(AEGeometry * this, Vector * param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AEGeometry::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall AEGeometry::setPosition(AEGeometry *this,Vector *param_1)

{
  Vector *pVVar1;
  Vector unaff_r6;
  int unaff_r11;
  bool in_NG;
  char in_OV;
  
  pVVar1 = param_1;
  if (in_NG) {
    pVVar1 = param_1 + unaff_r11 * -0x20000;
    *param_1 = unaff_r6;
  }
  if (in_NG != (bool)in_OV) {
    func_0xfe630a1e(this,pVVar1);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  000b4632: andgt lr,r0,#0x344000
  000b4636: strbtmi r6,[r1],-r11,lsl #0x11
  000b463a: bllt 0xfe630a1e
  000b463e: ldrblt r0,[r0,#0x0]!
```
