# ModStation::~ModStation
elf 0xd56f8 body 16
Sig: undefined __thiscall ~ModStation(ModStation * this)

## decompile
```c

/* ModStation::~ModStation() */

void __thiscall ModStation::~ModStation(ModStation *this)

{
  ~ModStation(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000e56f8: push {r7,lr}
  000e56fa: mov r7,sp
  000e56fc: blx 0x00074ee4
  000e5700: pop.w {r7,lr}
  000e5704: b.w 0x001ab098
```
