# CubeMapping::~CubeMapping
elf 0x90ab0 body 16
Sig: undefined __thiscall ~CubeMapping(CubeMapping * this)

## decompile
```c

/* AbyssEngine::CubeMapping::~CubeMapping() */

void __thiscall AbyssEngine::CubeMapping::~CubeMapping(CubeMapping *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a0ab0: push {r7,lr}
  000a0ab2: mov r7,sp
  000a0ab4: blx 0x0007093c
  000a0ab8: pop.w {r7,lr}
  000a0abc: b.w 0x001ab098
```
