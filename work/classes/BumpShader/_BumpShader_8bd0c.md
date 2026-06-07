# BumpShader::~BumpShader
elf 0x8bd0c body 16
Sig: undefined __thiscall ~BumpShader(BumpShader * this)

## decompile
```c

/* AbyssEngine::BumpShader::~BumpShader() */

void __thiscall AbyssEngine::BumpShader::~BumpShader(BumpShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009bd0c: push {r7,lr}
  0009bd0e: mov r7,sp
  0009bd10: blx 0x0007093c
  0009bd14: pop.w {r7,lr}
  0009bd18: b.w 0x001ab098
```
