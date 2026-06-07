# MineGun::~MineGun
elf 0x1566a6 body 16
Sig: undefined __thiscall ~MineGun(MineGun * this)

## decompile
```c

/* MineGun::~MineGun() */

void __thiscall MineGun::~MineGun(MineGun *this)

{
  ~MineGun(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001666a6: push {r7,lr}
  001666a8: mov r7,sp
  001666aa: blx 0x000775e4
  001666ae: pop.w {r7,lr}
  001666b2: b.w 0x001ab098
```
