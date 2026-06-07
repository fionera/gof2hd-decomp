# BumpRimCubeShader::~BumpRimCubeShader
elf 0x90160 body 16
Sig: undefined __thiscall ~BumpRimCubeShader(BumpRimCubeShader * this)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader::~BumpRimCubeShader() */

void __thiscall AbyssEngine::BumpRimCubeShader::~BumpRimCubeShader(BumpRimCubeShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a0160: push {r7,lr}
  000a0162: mov r7,sp
  000a0164: blx 0x0007093c
  000a0168: pop.w {r7,lr}
  000a016c: b.w 0x001ab098
```
