# BeamGun::~BeamGun
elf 0x17786c body 16
Sig: undefined __thiscall ~BeamGun(BeamGun * this)

## decompile
```c

/* BeamGun::~BeamGun() */

void __thiscall BeamGun::~BeamGun(BeamGun *this)

{
  ~BeamGun(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0018786c: push {r7,lr}
  0018786e: mov r7,sp
  00187870: blx 0x00078040
  00187874: pop.w {r7,lr}
  00187878: b.w 0x001ab098
```
