# BumpShaderV3::~BumpShaderV3
elf 0x8c6b0 body 16
Sig: undefined __thiscall ~BumpShaderV3(BumpShaderV3 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV3::~BumpShaderV3() */

void __thiscall AbyssEngine::BumpShaderV3::~BumpShaderV3(BumpShaderV3 *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009c6b0: push {r7,lr}
  0009c6b2: mov r7,sp
  0009c6b4: blx 0x0007093c
  0009c6b8: pop.w {r7,lr}
  0009c6bc: b.w 0x001ab098
```
