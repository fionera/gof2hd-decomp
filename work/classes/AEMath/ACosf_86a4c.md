# AbyssEngine::AEMath::ACosf
elf 0x86a4c body 26
Sig: undefined __stdcall ACosf(float param_1)

## decompile
```c

/* AbyssEngine::AEMath::ACosf(float) */

void AbyssEngine::AEMath::ACosf(float param_1)

{
                    /* WARNING: Could not recover jumptable at 0x001ab720. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab724 + 0x1ab728))();
  return;
}

```
## target disasm
```
  00070840: adr r12,0x170848
  00070844: add r12,r12,#0xb0000
  00070848: ldr pc,[r12,#0x910]!
  00096a4c: b.w 0x001ab718
  001ab718: bx pc
  001ab71c: ldr r12,[0x1ab724]
  001ab720: add pc,r12,pc
```
