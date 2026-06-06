# AEFile::ReadSwitched
elf 0x70d06 body 28
Sig: undefined __stdcall ReadSwitched(ushort * param_1, uint param_2)

## decompile
```c

/* AEFile::ReadSwitched(unsigned short&, unsigned int) */

void AEFile::ReadSwitched(ushort *param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = Read(2,param_1,param_2);
  if (iVar1 != 0) {
    *param_1 = (ushort)(((uint)*param_1 << 0x18) >> 0x10) | *param_1 >> 8;
  }
  return;
}

```
## target disasm
```
  00080d06: push {r4,r6,r7,lr}
  00080d08: add r7,sp,#0x8
  00080d0a: mov r4,r0
  00080d0c: mov r2,r1
  00080d0e: movs r0,#0x2
  00080d10: mov r1,r4
  00080d12: blx 0x0006eb90
  00080d16: cbz r0,0x00080d20
  00080d18: ldrh r1,[r4,#0x0]
  00080d1a: rev r1,r1
  00080d1c: lsrs r1,r1,#0x10
  00080d1e: strh r1,[r4,#0x0]
  00080d20: pop {r4,r6,r7,pc}
```
