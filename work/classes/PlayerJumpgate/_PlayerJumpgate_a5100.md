# PlayerJumpgate::~PlayerJumpgate
elf 0xa5100 body 16
Sig: undefined __thiscall ~PlayerJumpgate(PlayerJumpgate * this)

## decompile
```c

/* PlayerJumpgate::~PlayerJumpgate() */

void __thiscall PlayerJumpgate::~PlayerJumpgate(PlayerJumpgate *this)

{
  PlayerStaticFar::~PlayerStaticFar((PlayerStaticFar *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000b5100: push {r7,lr}
  000b5102: mov r7,sp
  000b5104: blx 0x000730f0
  000b5108: pop.w {r7,lr}
  000b510c: b.w 0x001ab098
```
