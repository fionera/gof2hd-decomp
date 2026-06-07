# GreenShader::~GreenShader
elf 0x8847e body 16
Sig: undefined __thiscall ~GreenShader(GreenShader * this)

## decompile
```c

/* AbyssEngine::GreenShader::~GreenShader() */

void __thiscall AbyssEngine::GreenShader::~GreenShader(GreenShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009847e: push {r7,lr}
  00098480: mov r7,sp
  00098482: blx 0x0007093c
  00098486: pop.w {r7,lr}
  0009848a: b.w 0x001ab098
```
