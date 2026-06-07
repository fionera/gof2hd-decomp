# PlayerTurret::setTurretRange
elf 0x157418 body 6
Sig: undefined __thiscall setTurretRange(PlayerTurret * this, int param_1)

## decompile
```c

/* PlayerTurret::setTurretRange(int) */

void __thiscall PlayerTurret::setTurretRange(PlayerTurret *this,int param_1)

{
  *(int *)(this + 0x164) = param_1;
  return;
}

```

## target disasm
```
  00167418: str.w r1,[r0,#0x164]
  0016741c: bx lr
```
