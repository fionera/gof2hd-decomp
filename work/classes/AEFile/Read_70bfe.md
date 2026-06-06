# AEFile::Read
elf 0x70bfe body 10
Sig: undefined __stdcall Read(ushort * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(unsigned short&, unsigned int) */

void AEFile::Read(ushort *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(2,param_1,param_2);
  return;
}

```
## target disasm
```
  00080bfe: mov r2,r1
  00080c00: mov r1,r0
  00080c02: movs r0,#0x2
  00080c04: b.w 0x001ab0d8
```
