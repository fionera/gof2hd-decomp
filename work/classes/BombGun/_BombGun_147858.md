# BombGun::~BombGun
elf 0x147858 body 16
Sig: undefined __thiscall ~BombGun(BombGun * this)

## decompile
```c

/* BombGun::~BombGun() */

void __thiscall BombGun::~BombGun(BombGun *this)

{
  ~BombGun(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00157858: push {r7,lr}
  0015785a: mov r7,sp
  0015785c: blx 0x00076dec
  00157860: pop.w {r7,lr}
  00157864: b.w 0x001ab098
```
