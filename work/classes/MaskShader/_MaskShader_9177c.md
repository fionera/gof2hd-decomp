# MaskShader::~MaskShader
elf 0x9177c body 16
Sig: undefined __thiscall ~MaskShader(MaskShader * this)

## decompile
```c

/* AbyssEngine::MaskShader::~MaskShader() */

void __thiscall AbyssEngine::MaskShader::~MaskShader(MaskShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a177c: push {r7,lr}
  000a177e: mov r7,sp
  000a1780: blx 0x0007093c
  000a1784: pop.w {r7,lr}
  000a1788: b.w 0x001ab098
```
