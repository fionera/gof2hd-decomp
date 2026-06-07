# BumpRimCubeShader_new::~BumpRimCubeShader_new
elf 0x88f40 body 16
Sig: undefined __thiscall ~BumpRimCubeShader_new(BumpRimCubeShader_new * this)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader_new::~BumpRimCubeShader_new() */

void __thiscall
AbyssEngine::BumpRimCubeShader_new::~BumpRimCubeShader_new(BumpRimCubeShader_new *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00098f40: push {r7,lr}
  00098f42: mov r7,sp
  00098f44: blx 0x0007093c
  00098f48: pop.w {r7,lr}
  00098f4c: b.w 0x001ab098
```
