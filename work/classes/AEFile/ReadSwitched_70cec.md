# AEFile::ReadSwitched
elf 0x70cec body 26
Sig: undefined __stdcall ReadSwitched(int * param_1, uint param_2)

## decompile
```c

/* AEFile::ReadSwitched(int&, unsigned int) */

void AEFile::ReadSwitched(int *param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = Read(4,param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = *param_1;
    *param_1 = uVar2 << 0x18 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
               uVar2 >> 0x18;
  }
  return;
}

```
## target disasm
```
  00080cec: push {r4,r6,r7,lr}
  00080cee: add r7,sp,#0x8
  00080cf0: mov r4,r0
  00080cf2: mov r2,r1
  00080cf4: movs r0,#0x4
  00080cf6: mov r1,r4
  00080cf8: blx 0x0006eb90
  00080cfc: cbz r0,0x00080d04
  00080cfe: ldr r1,[r4,#0x0]
  00080d00: rev r1,r1
  00080d02: str r1,[r4,#0x0]
  00080d04: pop {r4,r6,r7,pc}
```
