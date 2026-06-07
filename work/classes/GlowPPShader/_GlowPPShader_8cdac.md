# GlowPPShader::~GlowPPShader
elf 0x8cdac body 16
Sig: undefined __thiscall ~GlowPPShader(GlowPPShader * this)

## decompile
```c

/* AbyssEngine::GlowPPShader::~GlowPPShader() */

void __thiscall AbyssEngine::GlowPPShader::~GlowPPShader(GlowPPShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009cdac: push {r7,lr}
  0009cdae: mov r7,sp
  0009cdb0: blx 0x0007093c
  0009cdb4: pop.w {r7,lr}
  0009cdb8: b.w 0x001ab098
```
