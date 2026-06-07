# TextureConference::~TextureConference
elf 0x8cd0a body 16
Sig: undefined __thiscall ~TextureConference(TextureConference * this)

## decompile
```c

/* AbyssEngine::TextureConference::~TextureConference() */

void __thiscall AbyssEngine::TextureConference::~TextureConference(TextureConference *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009cd0a: push {r7,lr}
  0009cd0c: mov r7,sp
  0009cd0e: blx 0x0007093c
  0009cd12: pop.w {r7,lr}
  0009cd16: b.w 0x001ab098
```
