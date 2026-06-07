# AENormalFile::~AENormalFile
elf 0x7010c body 26
Sig: undefined __thiscall ~AENormalFile(AENormalFile * this)

## decompile
```c

/* AENormalFile::~AENormalFile() */

AENormalFile * __thiscall AENormalFile::~AENormalFile(AENormalFile *this)

{
  *(int *)this = *(int *)(DAT_0008012c + 0x80118) + 8;
  Release(this);
  return this;
}

```

## target disasm
```
  0008010c: push {r4,r6,r7,lr}
  0008010e: add r7,sp,#0x8
  00080110: mov r4,r0
  00080112: ldr r0,[0x0008012c]
  00080114: add r0,pc
  00080116: ldr r0,[r0,#0x0]
  00080118: adds r0,#0x8
  0008011a: str r0,[r4,#0x0]
  0008011c: mov r0,r4
  0008011e: blx 0x0006f2f8
  00080122: mov r0,r4
  00080124: pop {r4,r6,r7,pc}
```
