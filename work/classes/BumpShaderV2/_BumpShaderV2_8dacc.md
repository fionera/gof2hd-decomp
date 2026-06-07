# BumpShaderV2::~BumpShaderV2
elf 0x8dacc body 16
Sig: undefined __thiscall ~BumpShaderV2(BumpShaderV2 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV2::~BumpShaderV2() */

void __thiscall AbyssEngine::BumpShaderV2::~BumpShaderV2(BumpShaderV2 *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009dacc: push {r7,lr}
  0009dace: mov r7,sp
  0009dad0: blx 0x0007093c
  0009dad4: pop.w {r7,lr}
  0009dad8: b.w 0x001ab098
```
