# BumpMapping::~BumpMapping
elf 0x914d0 body 16
Sig: undefined __thiscall ~BumpMapping(BumpMapping * this)

## decompile
```c

/* AbyssEngine::BumpMapping::~BumpMapping() */

void __thiscall AbyssEngine::BumpMapping::~BumpMapping(BumpMapping *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a14d0: push {r7,lr}
  000a14d2: mov r7,sp
  000a14d4: blx 0x0007093c
  000a14d8: pop.w {r7,lr}
  000a14dc: b.w 0x001ab098
```
