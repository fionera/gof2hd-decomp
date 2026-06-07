# ObjectGun::~ObjectGun
elf 0x15f94c body 16
Sig: undefined __thiscall ~ObjectGun(ObjectGun * this)

## decompile
```c

/* ObjectGun::~ObjectGun() */

void __thiscall ObjectGun::~ObjectGun(ObjectGun *this)

{
  ~ObjectGun(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0016f94c: push {r7,lr}
  0016f94e: mov r7,sp
  0016f950: blx 0x000775c0
  0016f954: pop.w {r7,lr}
  0016f958: b.w 0x001ab098
```
