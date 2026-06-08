# Material::~Material
elf 0x7c8b8 body 50
Sig: undefined __thiscall ~Material(Material * this)

## decompile
```c

/* AbyssEngine::Material::~Material() */

Material * __thiscall AbyssEngine::Material::~Material(Material *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_0008c8ec + 0x8c8c4);
  (*pcVar1)(this + 0x5c);
  Array<unsigned_int>::~Array((Array<unsigned_int> *)(this + 0x50));
  Array<AbyssEngine::Mesh*>::~Array((Array<AbyssEngine::Mesh*> *)(this + 0x44));
  (*pcVar1)(this + 0x38);
  (*pcVar1)(this + 0x2c);
  return this;
}

```

## target disasm
```
  0008c8b8: push {r4,r5,r7,lr}
  0008c8ba: add r7,sp,#0x8
  0008c8bc: mov r4,r0
  0008c8be: ldr r0,[0x0008c8ec]
  0008c8c0: add r0,pc
  0008c8c2: ldr r5,[r0,#0x0]
  0008c8c4: add.w r0,r4,#0x5c
  0008c8c8: blx r5
  0008c8ca: add.w r0,r4,#0x50
  0008c8ce: blx 0x0006f904
  0008c8d2: add.w r0,r4,#0x44
  0008c8d6: blx 0x0006f760
  0008c8da: add.w r0,r4,#0x38
  0008c8de: blx r5
  0008c8e0: add.w r0,r4,#0x2c
  0008c8e4: blx r5
  0008c8e6: mov r0,r4
  0008c8e8: pop {r4,r5,r7,pc}
```
