# AbyssEngine::AEMath::ATanf
elf 0x86a50 body 26
Sig: undefined __stdcall ATanf(float param_1)

## decompile
```c

/* AbyssEngine::AEMath::ATanf(float) */

void AbyssEngine::AEMath::ATanf(float param_1)

{
                    /* WARNING: Could not recover jumptable at 0x001ab730. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab734 + 0x1ab738))();
  return;
}

```
## target disasm
```
  0007084c: adr r12,0x170854
  00070850: add r12,r12,#0xb0000
  00070854: ldr pc,[r12,#0x908]!
  00096a50: b.w 0x001ab728
  001ab728: bx pc
  001ab72c: ldr r12,[0x1ab734]
  001ab730: add pc,r12,pc
```
