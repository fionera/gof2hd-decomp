# PlayerJunk::~PlayerJunk
elf 0x15e76c body 16
Sig: undefined __thiscall ~PlayerJunk(PlayerJunk * this)

## decompile
```c

/* PlayerJunk::~PlayerJunk() */

void __thiscall PlayerJunk::~PlayerJunk(PlayerJunk *this)

{
  KIPlayer::~KIPlayer((KIPlayer *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0016e76c: push {r7,lr}
  0016e76e: mov r7,sp
  0016e770: blx 0x000732b8
  0016e774: pop.w {r7,lr}
  0016e778: b.w 0x001ab098
```
