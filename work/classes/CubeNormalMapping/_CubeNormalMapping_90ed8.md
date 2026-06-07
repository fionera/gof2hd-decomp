# CubeNormalMapping::~CubeNormalMapping
elf 0x90ed8 body 16
Sig: undefined __thiscall ~CubeNormalMapping(CubeNormalMapping * this)

## decompile
```c

/* AbyssEngine::CubeNormalMapping::~CubeNormalMapping() */

void __thiscall AbyssEngine::CubeNormalMapping::~CubeNormalMapping(CubeNormalMapping *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a0ed8: push {r7,lr}
  000a0eda: mov r7,sp
  000a0edc: blx 0x0007093c
  000a0ee0: pop.w {r7,lr}
  000a0ee4: b.w 0x001ab098
```
