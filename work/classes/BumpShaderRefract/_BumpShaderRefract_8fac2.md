# BumpShaderRefract::~BumpShaderRefract
elf 0x8fac2 body 16
Sig: undefined __thiscall ~BumpShaderRefract(BumpShaderRefract * this)

## decompile
```c

/* AbyssEngine::BumpShaderRefract::~BumpShaderRefract() */

void __thiscall AbyssEngine::BumpShaderRefract::~BumpShaderRefract(BumpShaderRefract *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009fac2: push {r7,lr}
  0009fac4: mov r7,sp
  0009fac6: blx 0x0007093c
  0009faca: pop.w {r7,lr}
  0009face: b.w 0x001ab098
```
