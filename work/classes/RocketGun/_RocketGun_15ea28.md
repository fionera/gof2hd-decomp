# RocketGun::~RocketGun
elf 0x15ea28 body 16
Sig: undefined __thiscall ~RocketGun(RocketGun * this)

## decompile
```c

/* RocketGun::~RocketGun() */

void __thiscall RocketGun::~RocketGun(RocketGun *this)

{
  ~RocketGun(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0016ea28: push {r7,lr}
  0016ea2a: mov r7,sp
  0016ea2c: blx 0x00076de0
  0016ea30: pop.w {r7,lr}
  0016ea34: b.w 0x001ab098
```
