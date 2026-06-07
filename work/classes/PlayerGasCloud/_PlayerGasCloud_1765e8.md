# PlayerGasCloud::~PlayerGasCloud
elf 0x1765e8 body 16
Sig: undefined __thiscall ~PlayerGasCloud(PlayerGasCloud * this)

## decompile
```c

/* PlayerGasCloud::~PlayerGasCloud() */

void __thiscall PlayerGasCloud::~PlayerGasCloud(PlayerGasCloud *this)

{
  ~PlayerGasCloud(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001865e8: push {r7,lr}
  001865ea: mov r7,sp
  001865ec: blx 0x00077fb0
  001865f0: pop.w {r7,lr}
  001865f4: b.w 0x001ab098
```
