# PlayerWormHole::~PlayerWormHole
elf 0xa5302 body 16
Sig: undefined __thiscall ~PlayerWormHole(PlayerWormHole * this)

## decompile
```c

/* PlayerWormHole::~PlayerWormHole() */

void __thiscall PlayerWormHole::~PlayerWormHole(PlayerWormHole *this)

{
  PlayerStaticFar::~PlayerStaticFar((PlayerStaticFar *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000b5302: push {r7,lr}
  000b5304: mov r7,sp
  000b5306: blx 0x000730f0
  000b530a: pop.w {r7,lr}
  000b530e: b.w 0x001ab098
```
