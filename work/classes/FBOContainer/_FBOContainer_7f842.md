# FBOContainer::~FBOContainer
elf 0x7f842 body 22
Sig: undefined __thiscall ~FBOContainer(FBOContainer * this)

## decompile
```c

/* AbyssEngine::FBOContainer::~FBOContainer() */

FBOContainer * __thiscall AbyssEngine::FBOContainer::~FBOContainer(FBOContainer *this)

{
  Release(this);
  String::~String((String *)(this + 0x1c));
  return this;
}

```

## target disasm
```
  0008f842: push {r4,r5,r7,lr}
  0008f844: add r7,sp,#0x8
  0008f846: mov r4,r0
  0008f848: blx 0x00070018
  0008f84c: add.w r0,r4,#0x1c
  0008f850: blx 0x0006ee30
  0008f854: mov r0,r4
  0008f856: pop {r4,r5,r7,pc}
```
