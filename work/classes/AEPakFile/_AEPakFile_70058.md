# AEPakFile::~AEPakFile
elf 0x70058 body 16
Sig: undefined __thiscall ~AEPakFile(AEPakFile * this)

## decompile
```c

/* AEPakFile::~AEPakFile() */

void __thiscall AEPakFile::~AEPakFile(AEPakFile *this)

{
  ~AEPakFile(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00080058: push {r7,lr}
  0008005a: mov r7,sp
  0008005c: blx 0x0006f2ec
  00080060: pop.w {r7,lr}
  00080064: b.w 0x001ab098
```
