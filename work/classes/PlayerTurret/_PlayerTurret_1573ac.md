# PlayerTurret::~PlayerTurret
elf 0x1573ac body 16
Sig: undefined __thiscall ~PlayerTurret(PlayerTurret * this)

## decompile
```c

/* PlayerTurret::~PlayerTurret() */

void __thiscall PlayerTurret::~PlayerTurret(PlayerTurret *this)

{
  ~PlayerTurret(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001673ac: push {r7,lr}
  001673ae: mov r7,sp
  001673b0: blx 0x0007768c
  001673b4: pop.w {r7,lr}
  001673b8: b.w 0x001ab098
```
