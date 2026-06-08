# GameData::GameData
elf 0xd0c6c body 94
Sig: undefined __thiscall GameData(GameData * this)

## decompile
```c

/* GameData::GameData() */

GameData * __thiscall GameData::GameData(GameData *this)

{
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x24));
  AbyssEngine::String::String((String *)(this + 0x30));
  AbyssEngine::String::String((String *)(this + 0x7c));
  AbyssEngine::String::String((String *)(this + 0x88));
  AbyssEngine::String::String((String *)(this + 0x94));
  AbyssEngine::String::String((String *)(this + 0xa8));
  AbyssEngine::String::String((String *)(this + 0xb8));
  return this;
}

```

## target disasm
```
  000e0c6c: push {r4,r5,r6,r7,lr}
  000e0c6e: add r7,sp,#0xc
  000e0c70: push {r7,r8,r9,r10,r11}
  000e0c74: mov r5,r0
  000e0c76: adds r0,#0x18
  000e0c78: str r0,[sp,#0x0]
  000e0c7a: blx 0x0006efbc
  000e0c7e: add.w r10,r5,#0x24
  000e0c82: mov r0,r10
  000e0c84: blx 0x0006efbc
  000e0c88: add.w r11,r5,#0x30
  000e0c8c: mov r0,r11
  000e0c8e: blx 0x0006efbc
  000e0c92: add.w r4,r5,#0x7c
  000e0c96: mov r0,r4
  000e0c98: blx 0x0006efbc
  000e0c9c: add.w r6,r5,#0x88
  000e0ca0: mov r0,r6
  000e0ca2: blx 0x0006efbc
  000e0ca6: add.w r9,r5,#0x94
  000e0caa: mov r0,r9
  000e0cac: blx 0x0006efbc
  000e0cb0: add.w r8,r5,#0xa8
  000e0cb4: mov r0,r8
  000e0cb6: blx 0x0006efbc
  000e0cba: add.w r0,r5,#0xb8
  000e0cbe: blx 0x0006efbc
  000e0cc2: mov r0,r5
  000e0cc4: pop.w {r3,r8,r9,r10,r11}
  000e0cc8: pop {r4,r5,r6,r7,pc}
```
