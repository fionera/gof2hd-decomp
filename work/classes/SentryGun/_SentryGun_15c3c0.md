# SentryGun::~SentryGun
elf 0x15c3c0 body 16
Sig: undefined __thiscall ~SentryGun(SentryGun * this)

## decompile
```c

/* SentryGun::~SentryGun() */

void __thiscall SentryGun::~SentryGun(SentryGun *this)

{
  ObjectGun::~ObjectGun((ObjectGun *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0016c3c0: push {r7,lr}
  0016c3c2: mov r7,sp
  0016c3c4: blx 0x000775c0
  0016c3c8: pop.w {r7,lr}
  0016c3cc: b.w 0x001ab098
```
