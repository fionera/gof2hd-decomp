# MTitle::~MTitle
elf 0x979bc body 16
Sig: undefined __thiscall ~MTitle(MTitle * this)

## decompile
```c

/* MTitle::~MTitle() */

void __thiscall MTitle::~MTitle(MTitle *this)

{
  ~MTitle(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a79bc: push {r7,lr}
  000a79be: mov r7,sp
  000a79c0: blx 0x00071cec
  000a79c4: pop.w {r7,lr}
  000a79c8: b.w 0x001ab098
```
