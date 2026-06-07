# GlowShader::~GlowShader
elf 0x9212a body 16
Sig: undefined __thiscall ~GlowShader(GlowShader * this)

## decompile
```c

/* AbyssEngine::GlowShader::~GlowShader() */

void __thiscall AbyssEngine::GlowShader::~GlowShader(GlowShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a212a: push {r7,lr}
  000a212c: mov r7,sp
  000a212e: blx 0x0007093c
  000a2132: pop.w {r7,lr}
  000a2136: b.w 0x001ab098
```
