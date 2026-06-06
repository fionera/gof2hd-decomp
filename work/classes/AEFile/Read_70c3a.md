# AEFile::Read
elf 0x70c3a body 12
Sig: undefined __stdcall Read(float * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(float&, unsigned int) */

void AEFile::Read(float *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(4,param_1,param_2);
  return;
}

```
## target disasm
```
  00080c3a: mov r2,r1
  00080c3c: mov r1,r0
  00080c3e: movs r0,#0x4
  00080c40: b.w 0x001ab0d8
  001ab0d8: bx pc
```
