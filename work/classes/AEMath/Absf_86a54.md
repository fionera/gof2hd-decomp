# AbyssEngine::AEMath::Absf
elf 0x86a54 body 24
Sig: undefined __stdcall Absf(float param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0xff0d2534) */
/* AbyssEngine::AEMath::Absf(float) */

void AbyssEngine::AEMath::Absf(float param_1)

{
  bool in_ZR;
  undefined4 in_cr0;
  undefined4 in_cr8;
  
  if (in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  func_0x004d2628();
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  coprocessor_movefromRt(0xf,5,5,in_cr0,in_cr8);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  00096a54: beq 0x004d225c
  00096a58: beq 0xff0d2534
  00096a5c: blx 0x004d2628
  00096a60: bne 0x010d252c
  00096a64: mrc p15,0x5,r11,cr0,cr8,0x5
  00096a68: vnmla.f32 s0,s0,s2
```
