# MGame::~MGame
elf 0x177c40 body 16
Sig: undefined __thiscall ~MGame(MGame * this)

## decompile
```c

/* MGame::~MGame() */

void __thiscall MGame::~MGame(MGame *this)

{
  ~MGame(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00187c40: push {r7,lr}
  00187c42: mov r7,sp
  00187c44: blx 0x000780ac
  00187c48: pop.w {r7,lr}
  00187c4c: b.w 0x001ab098
```
