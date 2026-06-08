# GameData::~GameData
elf 0xd06f4 body 64
Sig: undefined __thiscall ~GameData(GameData * this)

## decompile
```c

/* GameData::~GameData() */

GameData * __thiscall GameData::~GameData(GameData *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000e0734 + 0xe0700);
  (*pcVar1)(this + 0xb8);
  (*pcVar1)(this + 0xa8);
  (*pcVar1)(this + 0x94);
  (*pcVar1)(this + 0x88);
  (*pcVar1)(this + 0x7c);
  (*pcVar1)(this + 0x30);
  (*pcVar1)(this + 0x24);
  (*pcVar1)(this + 0x18);
  return this;
}

```

## target disasm
```
  000e06f4: push {r4,r5,r7,lr}
  000e06f6: add r7,sp,#0x8
  000e06f8: mov r4,r0
  000e06fa: ldr r0,[0x000e0734]
  000e06fc: add r0,pc
  000e06fe: ldr r5,[r0,#0x0]
  000e0700: add.w r0,r4,#0xb8
  000e0704: blx r5
  000e0706: add.w r0,r4,#0xa8
  000e070a: blx r5
  000e070c: add.w r0,r4,#0x94
  000e0710: blx r5
  000e0712: add.w r0,r4,#0x88
  000e0716: blx r5
  000e0718: add.w r0,r4,#0x7c
  000e071c: blx r5
  000e071e: add.w r0,r4,#0x30
  000e0722: blx r5
  000e0724: add.w r0,r4,#0x24
  000e0728: blx r5
  000e072a: add.w r0,r4,#0x18
  000e072e: blx r5
  000e0730: mov r0,r4
  000e0732: pop {r4,r5,r7,pc}
```
