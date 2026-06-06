# AEFile::Read
elf 0x70c08 body 10
Sig: undefined __stdcall Read(short * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(short&, unsigned int) */

void AEFile::Read(short *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(2,param_1,param_2);
  return;
}

```
## target disasm
```
  00080c08: mov r2,r1
  00080c0a: mov r1,r0
  00080c0c: movs r0,#0x2
  00080c0e: b.w 0x001ab0d8
```
