# MiningGame::~MiningGame
elf 0x11eb74 body 30
Sig: undefined __thiscall ~MiningGame(MiningGame * this)

## decompile
```c

/* MiningGame::~MiningGame() */

MiningGame * __thiscall MiningGame::~MiningGame(MiningGame *this)

{
  void *pvVar1;
  
  if (*(Sprite **)(this + 0x94) != (Sprite *)0x0) {
    pvVar1 = (void *)Sprite::~Sprite(*(Sprite **)(this + 0x94));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x94) = 0;
  return this;
}

```

## target disasm
```
  0012eb74: push {r4,r6,r7,lr}
  0012eb76: add r7,sp,#0x8
  0012eb78: mov r4,r0
  0012eb7a: ldr.w r0,[r0,#0x94]
  0012eb7e: cbz r0,0x0012eb88
  0012eb80: blx 0x0007600c
  0012eb84: blx 0x0006eb48
  0012eb88: movs r0,#0x0
  0012eb8a: str.w r0,[r4,#0x94]
  0012eb8e: mov r0,r4
  0012eb90: pop {r4,r6,r7,pc}
```
