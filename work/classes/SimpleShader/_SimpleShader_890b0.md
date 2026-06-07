# SimpleShader::~SimpleShader
elf 0x890b0 body 16
Sig: undefined __thiscall ~SimpleShader(SimpleShader * this)

## decompile
```c

/* AbyssEngine::SimpleShader::~SimpleShader() */

void __thiscall AbyssEngine::SimpleShader::~SimpleShader(SimpleShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000990b0: push {r7,lr}
  000990b2: mov r7,sp
  000990b4: blx 0x0007093c
  000990b8: pop.w {r7,lr}
  000990bc: b.w 0x001ab098
```
