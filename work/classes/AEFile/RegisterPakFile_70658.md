# AEFile::RegisterPakFile
elf 0x70658 body 18
Sig: undefined __stdcall RegisterPakFile(String param_1)

## decompile
```c

/* AEFile::RegisterPakFile(AbyssEngine::String) */

void AEFile::RegisterPakFile(String *param_1)

{
  collectFilesInPakFiles(param_1);
  (*(code *)(DAT_001ab0d4 + 0x1ab0d8))();
  return;
}

```
## target disasm
```
  00080658: push {r7,lr}
  0008065a: mov r7,sp
  0008065c: blx 0x0006f388
  00080660: pop.w {r7,lr}
  00080664: b.w 0x001ab0c8
  001ab0c8: bx pc
```
