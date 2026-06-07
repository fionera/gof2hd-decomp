# AEPakFile::~AEPakFile
elf 0x70034 body 26
Sig: undefined __thiscall ~AEPakFile(AEPakFile * this)

## decompile
```c

/* AEPakFile::~AEPakFile() */

AEPakFile * __thiscall AEPakFile::~AEPakFile(AEPakFile *this)

{
  *(int *)this = *(int *)(DAT_00080054 + 0x80040) + 8;
  Release(this);
  return this;
}

```

## target disasm
```
  00080034: push {r4,r6,r7,lr}
  00080036: add r7,sp,#0x8
  00080038: mov r4,r0
  0008003a: ldr r0,[0x00080054]
  0008003c: add r0,pc
  0008003e: ldr r0,[r0,#0x0]
  00080040: adds r0,#0x8
  00080042: str r0,[r4,#0x0]
  00080044: mov r0,r4
  00080046: blx 0x0006f2e0
  0008004a: mov r0,r4
  0008004c: pop {r4,r6,r7,pc}
```
