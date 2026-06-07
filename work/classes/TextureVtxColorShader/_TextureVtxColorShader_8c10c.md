# TextureVtxColorShader::~TextureVtxColorShader
elf 0x8c10c body 16
Sig: undefined __thiscall ~TextureVtxColorShader(TextureVtxColorShader * this)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::~TextureVtxColorShader() */

void __thiscall
AbyssEngine::TextureVtxColorShader::~TextureVtxColorShader(TextureVtxColorShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009c10c: push {r7,lr}
  0009c10e: mov r7,sp
  0009c110: blx 0x0007093c
  0009c114: pop.w {r7,lr}
  0009c118: b.w 0x001ab098
```
