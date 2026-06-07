# ModStation::~ModStation
elf 0xd56c0 body 34
Sig: undefined __thiscall ~ModStation(ModStation * this)

## decompile
```c

/* ModStation::~ModStation() */

ModStation * __thiscall ModStation::~ModStation(ModStation *this)

{
  this->field_0 = *(int *)(DAT_000e56f4 + 0xe56cc) + 8;
  OnRelease(this);
  AbyssEngine::String::~String((String *)&this->field_38);
  return this;
}

```

## target disasm
```
  000e56c0: push {r4,r5,r7,lr}
  000e56c2: add r7,sp,#0x8
  000e56c4: mov r4,r0
  000e56c6: ldr r0,[0x000e56f4]
  000e56c8: add r0,pc
  000e56ca: ldr r0,[r0,#0x0]
  000e56cc: adds r0,#0x8
  000e56ce: str r0,[r4,#0x0]
  000e56d0: mov r0,r4
  000e56d2: blx 0x00074ed8
  000e56d6: add.w r0,r4,#0x38
  000e56da: blx 0x0006ee30
  000e56de: mov r0,r4
  000e56e0: pop {r4,r5,r7,pc}
```
