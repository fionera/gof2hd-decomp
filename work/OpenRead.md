# AEFile::OpenRead

ELF vaddr 0x70530, body 8 bytes
Signature: undefined __stdcall OpenRead(String * param_1, uint * param_2)

## Ghidra decompile
```c

/* AEFile::OpenRead(AbyssEngine::String&, unsigned int*) */

void AEFile::OpenRead(String *param_1,uint *param_2)

{
  (*(code *)(DAT_001ab0c4 + 0x1ab0c8))(param_1,1,param_2);
  return;
}

```

## Target disassembly (must match)
```
  00080530: mov r2,r1
  00080532: movs r1,#0x1
  00080534: b.w 0x001ab0b8
```

Verify with: tools/try.sh <yourfile.cpp> OpenRead 70530 32
