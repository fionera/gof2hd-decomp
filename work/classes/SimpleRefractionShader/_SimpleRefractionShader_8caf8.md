# SimpleRefractionShader::~SimpleRefractionShader
elf 0x8caf8 body 16
Sig: undefined __thiscall ~SimpleRefractionShader(SimpleRefractionShader * this)

## decompile
```c

/* AbyssEngine::SimpleRefractionShader::~SimpleRefractionShader() */

void __thiscall
AbyssEngine::SimpleRefractionShader::~SimpleRefractionShader(SimpleRefractionShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009caf8: push {r7,lr}
  0009cafa: mov r7,sp
  0009cafc: blx 0x0007093c
  0009cb00: pop.w {r7,lr}
  0009cb04: b.w 0x001ab098
```
