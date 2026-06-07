# GenericShader2::~GenericShader2
elf 0x8e412 body 16
Sig: undefined __thiscall ~GenericShader2(GenericShader2 * this)

## decompile
```c

/* AbyssEngine::GenericShader2::~GenericShader2() */

void __thiscall AbyssEngine::GenericShader2::~GenericShader2(GenericShader2 *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009e412: push {r7,lr}
  0009e414: mov r7,sp
  0009e416: blx 0x0007093c
  0009e41a: pop.w {r7,lr}
  0009e41e: b.w 0x001ab098
```
