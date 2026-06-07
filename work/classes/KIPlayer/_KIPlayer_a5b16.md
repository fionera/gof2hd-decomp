# KIPlayer::~KIPlayer
elf 0xa5b16 body 16
Sig: undefined __thiscall ~KIPlayer(KIPlayer * this)

## decompile
```c

/* KIPlayer::~KIPlayer() */

void __thiscall KIPlayer::~KIPlayer(KIPlayer *this)

{
  ~KIPlayer(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000b5b16: push {r7,lr}
  000b5b18: mov r7,sp
  000b5b1a: blx 0x000732b8
  000b5b1e: pop.w {r7,lr}
  000b5b22: b.w 0x001ab098
```
