# BumpShaderCloak::~BumpShaderCloak
elf 0x8b200 body 16
Sig: undefined __thiscall ~BumpShaderCloak(BumpShaderCloak * this)

## decompile
```c

/* AbyssEngine::BumpShaderCloak::~BumpShaderCloak() */

void __thiscall AbyssEngine::BumpShaderCloak::~BumpShaderCloak(BumpShaderCloak *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009b200: push {r7,lr}
  0009b202: mov r7,sp
  0009b204: blx 0x0007093c
  0009b208: pop.w {r7,lr}
  0009b20c: b.w 0x001ab098
```
