# BumpShaderParticle::~BumpShaderParticle
elf 0x88204 body 16
Sig: undefined __thiscall ~BumpShaderParticle(BumpShaderParticle * this)

## decompile
```c

/* AbyssEngine::BumpShaderParticle::~BumpShaderParticle() */

void __thiscall AbyssEngine::BumpShaderParticle::~BumpShaderParticle(BumpShaderParticle *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00098204: push {r7,lr}
  00098206: mov r7,sp
  00098208: blx 0x0007093c
  0009820c: pop.w {r7,lr}
  00098210: b.w 0x001ab098
```
