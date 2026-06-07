# RecordHandler::readRecord
elf 0xcd25c body 14
Sig: undefined __stdcall readRecord(int param_1)

## decompile
```c

/* RecordHandler::readRecord(int) */

void RecordHandler::readRecord(int param_1)

{
  (*(code *)(DAT_001ac064 + 0x1ac068))(param_1);
  return;
}

```

## target disasm
```
  000dd25c: b.w 0x001ac058
  001ac058: bx pc
  001ac05c: ldr r12,[0x1ac064]
  001ac060: add pc,r12,pc
```
