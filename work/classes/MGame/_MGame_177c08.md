# MGame::~MGame
elf 0x177c08 body 34
Sig: undefined __thiscall ~MGame(MGame * this)

## decompile
```c

/* MGame::~MGame() */

MGame * __thiscall MGame::~MGame(MGame *this)

{
  this->field_0 = (int *)(*(int *)(DAT_00187c3c + 0x187c14) + 8);
  OnRelease(this);
  AbyssEngine::String::~String((String *)&this->field_64);
  return this;
}

```

## target disasm
```
  00187c08: push {r4,r5,r7,lr}
  00187c0a: add r7,sp,#0x8
  00187c0c: mov r4,r0
  00187c0e: ldr r0,[0x00187c3c]
  00187c10: add r0,pc
  00187c12: ldr r0,[r0,#0x0]
  00187c14: adds r0,#0x8
  00187c16: str r0,[r4,#0x0]
  00187c18: mov r0,r4
  00187c1a: blx 0x000780a0
  00187c1e: add.w r0,r4,#0x64
  00187c22: blx 0x0006ee30
  00187c26: mov r0,r4
  00187c28: pop {r4,r5,r7,pc}
```
