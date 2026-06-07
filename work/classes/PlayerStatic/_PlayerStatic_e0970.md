# PlayerStatic::~PlayerStatic
elf 0xe0970 body 16
Sig: undefined __thiscall ~PlayerStatic(PlayerStatic * this)

## decompile
```c

/* PlayerStatic::~PlayerStatic() */

void __thiscall PlayerStatic::~PlayerStatic(PlayerStatic *this)

{
  KIPlayer::~KIPlayer((KIPlayer *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000f0970: push {r7,lr}
  000f0972: mov r7,sp
  000f0974: blx 0x000732b8
  000f0978: pop.w {r7,lr}
  000f097c: b.w 0x001ab098
```
