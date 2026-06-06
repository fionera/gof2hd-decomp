# AbyssEngine::AEMath::Cosf
elf 0x86a48 body 14
Sig: undefined __stdcall Cosf(float param_1)

## decompile
```c

/* AbyssEngine::AEMath::Cosf(float) */

void AbyssEngine::AEMath::Cosf(float param_1)

{
  (*(code *)(DAT_001ab714 + 0x1ab718))();
  return;
}

```
## target disasm
```
  00096a48: b.w 0x001ab708
  001ab708: bx pc
  001ab70c: ldr r12,[0x1ab714]
  001ab710: add pc,r12,pc
```
