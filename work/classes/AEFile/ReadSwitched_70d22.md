# AEFile::ReadSwitched
elf 0x70d22 body 28
Sig: undefined __stdcall ReadSwitched(short * param_1, uint param_2)

## decompile
```c

/* AEFile::ReadSwitched(short&, unsigned int) */

void AEFile::ReadSwitched(short *param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = Read(2,param_1,param_2);
  if (iVar1 != 0) {
    *param_1 = (ushort)(((uint)(ushort)*param_1 << 0x18) >> 0x10) | (ushort)*param_1 >> 8;
  }
  return;
}

```
## target disasm
```
  00080d22: push {r4,r6,r7,lr}
  00080d24: add r7,sp,#0x8
  00080d26: mov r4,r0
  00080d28: mov r2,r1
  00080d2a: movs r0,#0x2
  00080d2c: mov r1,r4
  00080d2e: blx 0x0006eb90
  00080d32: cbz r0,0x00080d3c
  00080d34: ldrh r1,[r4,#0x0]
  00080d36: rev r1,r1
  00080d38: lsrs r1,r1,#0x10
  00080d3a: strh r1,[r4,#0x0]
  00080d3c: pop {r4,r6,r7,pc}
```
