# AENormalFile::~AENormalFile
elf 0x70130 body 16
Sig: undefined __thiscall ~AENormalFile(AENormalFile * this)

## decompile
```c

/* AENormalFile::~AENormalFile() */

void __thiscall AENormalFile::~AENormalFile(AENormalFile *this)

{
  ~AENormalFile(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00080130: push {r7,lr}
  00080132: mov r7,sp
  00080134: blx 0x0006f304
  00080138: pop.w {r7,lr}
  0008013c: b.w 0x001ab098
```
