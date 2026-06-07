# PlayerGasCloud::translate
elf 0x176658 body 16
Sig: undefined __stdcall translate(Vector * param_1)

## decompile
```c

/* PlayerGasCloud::translate(AbyssEngine::AEMath::Vector const&) */

void PlayerGasCloud::translate(Vector *param_1)

{
  (*(code *)(DAT_001abef4 + 0x1abef8))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00186658: ldr r0,[r0,#0x8]
  0018665a: b.w 0x001abee8
  001abee8: bx pc
  001abeec: ldr r12,[0x1abef4]
  001abef0: add pc,r12,pc
```
