# ShaderBaseStruct::DeleteShader
elf 0x8e4ea body 28
Sig: undefined __thiscall DeleteShader(ShaderBaseStruct * this)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::DeleteShader() */

void __thiscall AbyssEngine::ShaderBaseStruct::DeleteShader(ShaderBaseStruct *this)

{
                    /* WARNING: Could not recover jumptable at 0x001ab7d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab7d4 + 0x1ab7d8))(*(undefined4 *)(this + 4));
  return;
}

```

## target disasm
```
  00070fa8: adr r12,0x170fb0
  00070fac: add r12,r12,#0xb0000
  00070fb0: ldr pc,[r12,#0x420]!
  0009e4ea: ldr r0,[r0,#0x4]
  0009e4ec: b.w 0x001ab7c8
  001ab7c8: bx pc
  001ab7cc: ldr r12,[0x1ab7d4]
  001ab7d0: add pc,r12,pc
```
