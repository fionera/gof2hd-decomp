# AbyssEngine::AEMath::Sinf
elf 0x86a44 body 14
Sig: undefined __stdcall Sinf(float param_1)

## decompile
```c

/* AbyssEngine::AEMath::Sinf(float) */

void AbyssEngine::AEMath::Sinf(float param_1)

{
  (*(code *)(DAT_001ab704 + 0x1ab708))();
  return;
}

```
## target disasm
```
  00096a44: b.w 0x001ab6f8
  001ab6f8: bx pc
  001ab6fc: ldr r12,[0x1ab704]
  001ab700: add pc,r12,pc
```
