# AbyssEngine::AEMath::Pow
elf 0x86aa8 body 26
Sig: undefined __stdcall Pow(float param_1, float param_2)

## decompile
```c

/* AbyssEngine::AEMath::Pow(float, float) */

void AbyssEngine::AEMath::Pow(float param_1,float param_2)

{
                    /* WARNING: Could not recover jumptable at 0x001ab740. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab744 + 0x1ab748))();
  return;
}

```
## target disasm
```
  00070858: adr r12,0x170860
  0007085c: add r12,r12,#0xb0000
  00070860: ldr pc,[r12,#0x900]!
  00096aa8: b.w 0x001ab738
  001ab738: bx pc
  001ab73c: ldr r12,[0x1ab744]
  001ab740: add pc,r12,pc
```
