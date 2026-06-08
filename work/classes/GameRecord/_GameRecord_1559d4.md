# GameRecord::~GameRecord
elf 0x1559d4 body 26
Sig: undefined __thiscall ~GameRecord(GameRecord * this)

## decompile
```c

/* GameRecord::~GameRecord() */

GameRecord * __thiscall GameRecord::~GameRecord(GameRecord *this)

{
  AbyssEngine::String::~String((String *)(this + 0x194));
  AbyssEngine::String::~String((String *)(this + 0x188));
  return this;
}

```

## target disasm
```
  001659d4: push {r4,r6,r7,lr}
  001659d6: add r7,sp,#0x8
  001659d8: mov r4,r0
  001659da: add.w r0,r0,#0x194
  001659de: blx 0x0006ee30
  001659e2: add.w r0,r4,#0x188
  001659e6: blx 0x0006ee30
  001659ea: mov r0,r4
  001659ec: pop {r4,r6,r7,pc}
```
