# BumpShaderV4::~BumpShaderV4
elf 0x8b624 body 16
Sig: undefined __thiscall ~BumpShaderV4(BumpShaderV4 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV4::~BumpShaderV4() */

void __thiscall AbyssEngine::BumpShaderV4::~BumpShaderV4(BumpShaderV4 *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009b624: push {r7,lr}
  0009b626: mov r7,sp
  0009b628: blx 0x0007093c
  0009b62c: pop.w {r7,lr}
  0009b630: b.w 0x001ab098
```
