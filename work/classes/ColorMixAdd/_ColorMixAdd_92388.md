# ColorMixAdd::~ColorMixAdd
elf 0x92388 body 16
Sig: undefined __thiscall ~ColorMixAdd(ColorMixAdd * this)

## decompile
```c

/* AbyssEngine::ColorMixAdd::~ColorMixAdd() */

void __thiscall AbyssEngine::ColorMixAdd::~ColorMixAdd(ColorMixAdd *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a2388: push {r7,lr}
  000a238a: mov r7,sp
  000a238c: blx 0x0007093c
  000a2390: pop.w {r7,lr}
  000a2394: b.w 0x001ab098
```
