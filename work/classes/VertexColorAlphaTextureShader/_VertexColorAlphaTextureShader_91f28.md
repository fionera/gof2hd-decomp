# VertexColorAlphaTextureShader::~VertexColorAlphaTextureShader
elf 0x91f28 body 16
Sig: undefined __thiscall ~VertexColorAlphaTextureShader(VertexColorAlphaTextureShader * this)

## decompile
```c

/* AbyssEngine::VertexColorAlphaTextureShader::~VertexColorAlphaTextureShader() */

void __thiscall
AbyssEngine::VertexColorAlphaTextureShader::~VertexColorAlphaTextureShader
          (VertexColorAlphaTextureShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a1f28: push {r7,lr}
  000a1f2a: mov r7,sp
  000a1f2c: blx 0x0007093c
  000a1f30: pop.w {r7,lr}
  000a1f34: b.w 0x001ab098
```
