# RecordHandler::notEnoughMemory
elf 0xcbb30 body 22
Sig: undefined __stdcall notEnoughMemory(void)

## decompile
```c

/* RecordHandler::notEnoughMemory() */

bool RecordHandler::notEnoughMemory(void)

{
  int iVar1;
  
  iVar1 = AEFile::GetDeviceFreeSpace();
  return iVar1 < 900;
}

```

## target disasm
```
  000dbb30: push {r7,lr}
  000dbb32: mov r7,sp
  000dbb34: blx 0x000747d0
  000dbb38: movs r1,#0x0
  000dbb3a: cmp.w r0,#0x384
  000dbb3e: it lt
  000dbb40: mov.lt r1,#0x1
  000dbb42: mov r0,r1
  000dbb44: pop {r7,pc}
```
