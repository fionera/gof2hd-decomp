# RecordHandler::~RecordHandler
elf 0xcbb08 body 34
Sig: undefined __thiscall ~RecordHandler(RecordHandler * this)

## decompile
```c

/* RecordHandler::~RecordHandler() */

RecordHandler * __thiscall RecordHandler::~RecordHandler(RecordHandler *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000dbb2c + 0xdbb14);
  (*pcVar1)(this + 0x20);
  (*pcVar1)(this + 0x14);
  (*pcVar1)(this + 8);
  return this;
}

```

## target disasm
```
  000dbb08: push {r4,r5,r7,lr}
  000dbb0a: add r7,sp,#0x8
  000dbb0c: mov r4,r0
  000dbb0e: ldr r0,[0x000dbb2c]
  000dbb10: add r0,pc
  000dbb12: ldr r5,[r0,#0x0]
  000dbb14: add.w r0,r4,#0x20
  000dbb18: blx r5
  000dbb1a: add.w r0,r4,#0x14
  000dbb1e: blx r5
  000dbb20: add.w r0,r4,#0x8
  000dbb24: blx r5
  000dbb26: mov r0,r4
  000dbb28: pop {r4,r5,r7,pc}
```
