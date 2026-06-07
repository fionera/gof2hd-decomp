# VertexColorShader::~VertexColorShader
elf 0x905b8 body 16
Sig: undefined __thiscall ~VertexColorShader(VertexColorShader * this)

## decompile
```c

/* AbyssEngine::VertexColorShader::~VertexColorShader() */

void __thiscall AbyssEngine::VertexColorShader::~VertexColorShader(VertexColorShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a05b8: push {r7,lr}
  000a05ba: mov r7,sp
  000a05bc: blx 0x0007093c
  000a05c0: pop.w {r7,lr}
  000a05c4: b.w 0x001ab098
```
