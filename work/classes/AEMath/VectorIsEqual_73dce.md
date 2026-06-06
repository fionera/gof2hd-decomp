# AbyssEngine::AEMath::VectorIsEqual
elf 0x73dce body 26
Sig: undefined __stdcall VectorIsEqual(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::VectorIsEqual(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::VectorIsEqual(Vector *param_1,Vector *param_2)

{
                    /* WARNING: Could not recover jumptable at 0x001ab130. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab134 + 0x1ab138))(param_1);
  return;
}

```
## target disasm
```
  0006f67c: adr r12,0x16f684
  0006f680: add r12,r12,#0xb1000
  0006f684: ldr pc,[r12,#0x4e8]!
  00083dce: b.w 0x001ab128
  001ab128: bx pc
  001ab12c: ldr r12,[0x1ab134]
  001ab130: add pc,r12,pc
```
